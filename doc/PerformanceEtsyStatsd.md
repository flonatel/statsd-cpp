Performace Tests with Original Etsy statsd
==========================================

To get an impression what the original authors of *Measure Anything,
Measure Everything* had in mind when writing their implementation,
there were some basic tests done with the original [etsy
statsd](https://github.com/etsy/statsd/).

The tests were run on a laptop: Dell Latitude with
Intel(R) Core(TM) i7-4600U CPU @ 2.10GHz, 16GRam inside a VM.

# Setup #

[Original statsd trunk from
2014-03-22](https://github.com/etsy/statsd/commit/de7a7bf0097f220ef4ed77c822ba9be5409831ad)
is used.

As a client the [udpclient.cc](../test/performance/udpclient.cc) was
used. It sends out 1.000.000 times the same counter.

As the server <code>socat</code> was used: <code>socat TCP-LISTEN:12003,fork -</code>

# Running tests #
All the described tests were run a couple of time with similar results
as shown here.

## One Client ##
<code>socat</code> and <code>statds.js</code> were started, then one
instance of the client was started.

The output:

    stats.statsd.bad_lines_seen 0 1395483593
    stats_counts.statsd.bad_lines_seen 0 1395483593
    stats.statsd.packets_received 0 1395483593
    stats_counts.statsd.packets_received 0 1395483593
    statsd.numStats 2 1395483593
    stats.statsd.graphiteStats.calculationtime 0 1395483593
    stats.statsd.processing_time 0 1395483593
    stats.statsd.graphiteStats.last_exception 1395483583 1395483593
    stats.statsd.graphiteStats.last_flush 1395483583 1395483593
    stats.statsd.graphiteStats.flush_time 0 1395483593
    stats.statsd.graphiteStats.flush_length 0 1395483593
    
    stats.statsd.bad_lines_seen 0 1395483603
    stats_counts.statsd.bad_lines_seen 0 1395483603
    stats.statsd.packets_received 11122.7 1395483603
    stats_counts.statsd.packets_received 111227 1395483603
    stats.org.flonatel.statsd-cpp.test.performance.python.test01 11122.7 1395483603
    stats_counts.org.flonatel.statsd-cpp.test.performance.python.test01 111227 1395483603
    stats.gauges.statsd.timestamp_lag 0 1395483603
    statsd.numStats 4 1395483603
    stats.statsd.graphiteStats.calculationtime 0 1395483603
    stats.statsd.processing_time 0 1395483603
    stats.statsd.graphiteStats.last_exception 1395483583 1395483603
    stats.statsd.graphiteStats.last_flush 1395483593 1395483603
    stats.statsd.graphiteStats.flush_time 1 1395483603
    stats.statsd.graphiteStats.flush_length 537 1395483603

    stats.statsd.bad_lines_seen 0 1395483613
    stats_counts.statsd.bad_lines_seen 0 1395483613
    stats.statsd.packets_received 0 1395483613
    stats_counts.statsd.packets_received 0 1395483613
    stats.org.flonatel.statsd-cpp.test.performance.python.test01 0 1395483613
    stats_counts.org.flonatel.statsd-cpp.test.performance.python.test01 0 1395483613
    stats.gauges.statsd.timestamp_lag 0 1395483613
    statsd.numStats 4 1395483613
    stats.statsd.graphiteStats.calculationtime 0 1395483613
    stats.statsd.processing_time 0 1395483613
    stats.statsd.graphiteStats.last_exception 1395483583 1395483613
    stats.statsd.graphiteStats.last_flush 1395483603 1395483613
    stats.statsd.graphiteStats.flush_time 0 1395483613
    stats.statsd.graphiteStats.flush_length 763 1395483613

1.000.000 UDP packets were send out. The counter is at: 111227. This
gives a drop rate of about 89%.

## Ten Clients ##
<code>socat</code> and <code>statds.js</code> were started, then ten
instance of the client were started simultaneously.

The output:

    stats.statsd.bad_lines_seen 0 1395484915
    stats_counts.statsd.bad_lines_seen 0 1395484915
    stats.statsd.packets_received 0 1395484915
    stats_counts.statsd.packets_received 0 1395484915
    statsd.numStats 2 1395484915
    stats.statsd.graphiteStats.calculationtime 0 1395484915
    stats.statsd.processing_time 0 1395484915
    stats.statsd.graphiteStats.last_exception 1395484905 1395484915
    stats.statsd.graphiteStats.last_flush 1395484905 1395484915
    stats.statsd.graphiteStats.flush_time 0 1395484915
    stats.statsd.graphiteStats.flush_length 0 1395484915
    
    stats.statsd.bad_lines_seen 0 1395484925
    stats_counts.statsd.bad_lines_seen 0 1395484925
    stats.statsd.packets_received 5307.9 1395484925
    stats_counts.statsd.packets_received 53079 1395484925
    stats.org.flonatel.statsd-cpp.test.performance.python.test01 5307.9 1395484925
    stats_counts.org.flonatel.statsd-cpp.test.performance.python.test01 53079 1395484925
    stats.gauges.statsd.timestamp_lag 0 1395484925
    statsd.numStats 4 1395484925
    stats.statsd.graphiteStats.calculationtime 0 1395484925
    stats.statsd.processing_time 0 1395484925
    stats.statsd.graphiteStats.last_exception 1395484905 1395484925
    stats.statsd.graphiteStats.last_flush 1395484915 1395484925
    stats.statsd.graphiteStats.flush_time 1 1395484925
    stats.statsd.graphiteStats.flush_length 537 1395484925
    
    stats.statsd.bad_lines_seen 0 1395484935
    stats_counts.statsd.bad_lines_seen 0 1395484935
    stats.statsd.packets_received 7028 1395484935
    stats_counts.statsd.packets_received 70280 1395484935
    stats.org.flonatel.statsd-cpp.test.performance.python.test01 7028 1395484935
    stats_counts.org.flonatel.statsd-cpp.test.performance.python.test01 70280 1395484935
    stats.gauges.statsd.timestamp_lag 0 1395484935
    statsd.numStats 4 1395484935
    stats.statsd.graphiteStats.calculationtime 0 1395484935
    stats.statsd.processing_time 0 1395484935
    stats.statsd.graphiteStats.last_exception 1395484905 1395484935
    stats.statsd.graphiteStats.last_flush 1395484925 1395484935
    stats.statsd.graphiteStats.flush_time 1 1395484935
    stats.statsd.graphiteStats.flush_length 759 1395484935
    
    stats.statsd.bad_lines_seen 0 1395484945
    stats_counts.statsd.bad_lines_seen 0 1395484945
    stats.statsd.packets_received 0 1395484945
    stats_counts.statsd.packets_received 0 1395484945
    stats.org.flonatel.statsd-cpp.test.performance.python.test01 0 1395484945
    stats_counts.org.flonatel.statsd-cpp.test.performance.python.test01 0 1395484945
    stats.gauges.statsd.timestamp_lag 0 1395484945
    statsd.numStats 4 1395484945
    stats.statsd.graphiteStats.calculationtime 0 1395484945
    stats.statsd.processing_time 0 1395484945
    stats.statsd.graphiteStats.last_exception 1395484905 1395484945
    stats.statsd.graphiteStats.last_flush 1395484935 1395484945
    stats.statsd.graphiteStats.flush_time 1 1395484945
    stats.statsd.graphiteStats.flush_length 755 1395484945

Because the ten clients run more than 12s, the counters increase does
not fit into one 10s interval.

The complete number of packets in this test is: 53079 + 70280 =
123359

Ten clients send out 10.000.000 packets, therefore the drop rate is:
98.77%.
