#ifndef STATSD_CORRELATOR_HH
#define STATSD_CORRELATOR_HH

#include <memory>
#include <chrono>
#include <map>
#include <mutex>
#include <tuple>
#include <statsdcpp/base.hh>
#include <statsdcpp/measure.hh>
#include <statsdcpp/counter.hh>

namespace statsdcpp {

template<typename TCheckpoint>
struct transition {
   TCheckpoint from;
   TCheckpoint to;
   std::string const name;
};

// Correlates different states of processing for one request.
// This is a higher level statistic object which itself uses counters
// for the things it measures.
// The TPID is the type how a processing step is identified.
// The TUID is the UID of the request.
template< typename TWriter, typename TUID, typename TCheckpoint >
class correlator : public base< TWriter > {
public:
   correlator(std::string const & name,
              std::vector< statsdcpp::transition< TCheckpoint > > const & iv);

   void in(TUID const & uid, TCheckpoint const & pid);
   void seen(TUID const & uid, TCheckpoint const & pid);
   void out(TUID const & uid, TCheckpoint const & pid);

   void serialize_debug(TWriter & writer);
   void serialize_carbon(TWriter & writer);

   void remove_old_uids(siunits::time ts);

private:
   void in_locked(TUID const & uid, TCheckpoint const & pid);
   void seen_locked(TUID const & uid, TCheckpoint const & pid);

   std::mutex _data_mutex;

   std::map< TUID, std::tuple<
                      TCheckpoint,
                      std::chrono::system_clock::time_point> > _uids;

   struct times_value {
      measure_sp< siunits::time, TWriter > duration;
      counter_sp< TWriter > count;
   };

   std::map< std::tuple< TCheckpoint, TCheckpoint >, times_value > _times;

   counter< TWriter > _seen_uid_not_found;
   counter< TWriter > _seen_checkpoint_not_found;
   counter< TWriter > _dropped_uids;
};

template< typename TWriter, typename TCheckpoint, typename TUID >
using correlator_sp = std::shared_ptr<
   correlator< TWriter, TCheckpoint, TUID > >;

// === Implementation

template< typename TWriter, typename TUID, typename TCheckpoint >
correlator< TWriter, TUID, TCheckpoint >::correlator(
   std::string const & name,
   std::vector< statsdcpp::transition< TCheckpoint > > const & iv)
   : base< TWriter >(name),
     _seen_uid_not_found(name+".seen_uid_not_found"),
     _seen_checkpoint_not_found(name+".seen_checkpoint_not_found"),
     _dropped_uids(name+".dropped_uids") {
   for(auto const iv_idx : iv) {
      auto const msr( std::make_shared< measure< siunits::time, TWriter > > (
                         this->_name + ".times." + iv_idx.name));
      auto const cntr( std::make_shared< counter< TWriter > > (
                          this->_name + ".count." + iv_idx.name));
      times_value const tv{ msr, cntr };
      _times.insert(std::make_pair(
                       std::make_tuple(iv_idx.from, iv_idx.to), tv));
   }
}


template< typename TWriter, typename TUID, typename TCheckpoint >
void
correlator< TWriter, TUID, TCheckpoint >::in(
   TUID const & uid, TCheckpoint const & pid) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   in_locked(uid, pid);
}

template< typename TWriter, typename TUID, typename TCheckpoint >
void
correlator< TWriter, TUID, TCheckpoint >::in_locked(
   TUID const & uid, TCheckpoint const & pid) {
   _uids.erase(uid);
   _uids.insert(
      std::make_pair(uid,
                     std::make_tuple(
                        pid, std::chrono::high_resolution_clock::now())) );
}

template< typename TWriter, typename TUID, typename TCheckpoint >
void
correlator< TWriter, TUID, TCheckpoint >::seen(
   TUID const & uid, TCheckpoint const & pid) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   seen_locked(uid, pid);
   in_locked(uid, pid);
}

template< typename TWriter, typename TUID, typename TCheckpoint >
void
correlator< TWriter, TUID, TCheckpoint >::seen_locked(
   TUID const & uid, TCheckpoint const & pid) {
   auto const now(std::chrono::high_resolution_clock::now());
   auto const fuid(_uids.find(uid));
   if(fuid==_uids.end()) {
      _seen_uid_not_found.inc();
      return;
   }

   std::chrono::nanoseconds const diff(now - std::get<1>(fuid->second));

   auto const ftimes(_times.find(std::make_tuple(std::get<0>(fuid->second),
                                                 pid)));
   if(ftimes==_times.end()) {
      _seen_checkpoint_not_found.inc();
   } else {
      ftimes->second.duration->add_locked(
         siunits::factor(std::chrono::duration_cast<std::chrono::nanoseconds>(
                            diff).count()) * siunits::nanoseconds);
      ftimes->second.count->inc();
   }
}

template< typename TWriter, typename TUID, typename TCheckpoint >
void
correlator< TWriter, TUID, TCheckpoint >::out(
   TUID const & uid, TCheckpoint const & pid) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   seen_locked(uid, pid);
   _uids.erase(uid);
}

template< typename TWriter, typename TUID, typename TCheckpoint >
void
correlator< TWriter, TUID, TCheckpoint >::serialize_debug(TWriter & writer) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   _seen_uid_not_found.serialize_debug(writer);
   for(auto const & timescur : _times) {
      timescur.second.duration->serialize_debug(writer);
      timescur.second.count->serialize_debug(writer);
   }
}

template< typename TWriter, typename TUID, typename TCheckpoint >
void
correlator< TWriter, TUID, TCheckpoint >::serialize_carbon(TWriter & writer) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   _seen_uid_not_found.serialize_carbon(writer);
   _seen_checkpoint_not_found.serialize_carbon(writer);
   for(auto const & timescur : _times) {
      timescur.second.duration->serialize_carbon(writer);
      timescur.second.count->serialize_carbon(writer);
   }
}

template< typename TWriter, typename TUID, typename TCheckpoint >
void
correlator< TWriter, TUID, TCheckpoint >::remove_old_uids(siunits::time ts) {
   auto const now(std::chrono::high_resolution_clock::now());
   std::chrono::system_clock::time_point too_old(
      now - std::chrono::seconds(ts.value()));
   std::vector< TUID > to_be_deleted;
   std::lock_guard< std::mutex > lock(_data_mutex);

   for(auto const uid: _uids) {
      if(std::get<1>(uid->second)<too_old) {
         to_be_deleted.push_back(uid);
      }
   }
   for(auto const tbd: to_be_deleted) {
      _uids.erase(tbd);
   }
}

}

#endif
