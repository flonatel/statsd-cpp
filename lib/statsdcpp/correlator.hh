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

// Correlates different states of processing for one request.
// This is a higher level statistic object which itself uses counters
// for the things it measures.
// The TPID is the type how a processing step is identified.
// The TUID is the UID of the request.
template< typename TWriter, typename TUID, typename TPID >
class correlator : public base< TWriter > {
public:
   correlator(std::string const & name);

   void in(TUID const & uid, TPID const & pid);
   void seen(TUID const & uid, TPID const & pid);
   void out(TUID const & uid, TPID const & pid);

   void serialize_debug(TWriter & writer);
   void serialize_carbon(TWriter & writer);

   void remove_old_uids(siunits::time ts);

private:
   void in_locked(TUID const & uid, TPID const & pid);
   void seen_locked(TUID const & uid, TPID const & pid);

   std::mutex _data_mutex;

   std::map< TUID, std::tuple<
                      TPID, std::chrono::system_clock::time_point> > _uids;

   struct times_value {
      measure_sp< siunits::time, TWriter >  duration;
      counter_sp< TWriter > count;
   };

   std::map< std::tuple< TPID, TPID >, times_value > _times;

   counter< TWriter > _seen_uid_not_found;
   counter< TWriter > _dropped_uids;
};

template< typename TWriter, typename TPID, typename TUID >
using correlator_sp = std::shared_ptr< correlator< TWriter, TPID, TUID > >;

// === Implementation

template< typename TWriter, typename TUID, typename TPID >
correlator< TWriter, TUID, TPID >::correlator(std::string const & name)
   : base< TWriter >(name),
     _seen_uid_not_found(name+".seen_uid_not_found"),
     _dropped_uids(name+".dropped_uids") {
}


template< typename TWriter, typename TUID, typename TPID >
void
correlator< TWriter, TUID, TPID >::in(TUID const & uid, TPID const & pid) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   in_locked(uid, pid);
}

template< typename TWriter, typename TUID, typename TPID >
void
correlator< TWriter, TUID, TPID >::in_locked(
   TUID const & uid, TPID const & pid) {
   _uids.erase(uid);
   _uids.insert(
      std::make_pair(uid,
                     std::make_tuple(
                        pid, std::chrono::high_resolution_clock::now())) );
}

template< typename TWriter, typename TUID, typename TPID >
void
correlator< TWriter, TUID, TPID >::seen(TUID const & uid, TPID const & pid) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   seen_locked(uid, pid);
   in_locked(uid, pid);
}

template< typename TWriter, typename TUID, typename TPID >
void
correlator< TWriter, TUID, TPID >::seen_locked(
   TUID const & uid, TPID const & pid) {
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
      std::string const pids("." + std::get<0>(fuid->second) + "." + pid);
      auto const msr( std::make_shared< measure< siunits::time, TWriter > > (
                         this->_name + ".times" + pids));
      auto const cntr( std::make_shared< counter< TWriter > > (
                          this->_name + ".count" + pids));
      msr->add(
         siunits::factor(std::chrono::duration_cast<std::chrono::nanoseconds>(
                            diff).count()) * siunits::nanoseconds);
      cntr->inc();

      times_value const tv{ msr, cntr };

      _times.insert(
         std::make_pair(
            std::make_tuple(std::get<0>(fuid->second), pid), tv));
   } else {
      ftimes->second.duration->add(
         siunits::factor(std::chrono::duration_cast<std::chrono::nanoseconds>(
                            diff).count()) * siunits::nanoseconds);
      ftimes->second.count->inc();
   }
}

template< typename TWriter, typename TUID, typename TPID >
void
correlator< TWriter, TUID, TPID >::out(TUID const & uid, TPID const & pid) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   seen_locked(uid, pid);
   _uids.erase(uid);
}

template< typename TWriter, typename TUID, typename TPID >
void
correlator< TWriter, TUID, TPID >::serialize_debug(TWriter & writer) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   _seen_uid_not_found.serialize_debug(writer);
   for(auto const & timescur : _times) {
      timescur.second.duration->serialize_debug(writer);
      timescur.second.count->serialize_debug(writer);
   }
}

template< typename TWriter, typename TUID, typename TPID >
void
correlator< TWriter, TUID, TPID >::serialize_carbon(TWriter & writer) {
   std::lock_guard< std::mutex > lock(_data_mutex);
   _seen_uid_not_found.serialize_carbon(writer);
   for(auto const & timescur : _times) {
      timescur.second.duration->serialize_carbon(writer);
      timescur.second.count->serialize_carbon(writer);
   }
   _times.clear();
}

template< typename TWriter, typename TUID, typename TPID >
void
correlator< TWriter, TUID, TPID >::remove_old_uids(siunits::time ts) {
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
