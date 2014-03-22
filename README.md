statsd-cpp
==========

Implementation of the statsd in C++ (Status: planning)

[![Build
Status](https://secure.travis-ci.org/flonatel/statsd-cpp.png)](http://travis-ci.org/flonatel/statsd-cpp)

# Introduction #

## Think! Measure reliable! ##
Think what you want to measure and do this reliable. Numbers which are
generated only from a small portion of the data (and you don't known
how many data sets and which data was deleted) is worthless.

*Measure Anything, Measure Everything* is the philosophy behind the
inventors of the original [etsy
statsd](https://github.com/etsy/statsd/).  Performance tests
has shown, that this cannot be accomplished with the philosophy
and technology implemented in the etsy statds especially for systems
with a lot of events.

## Reinvent the Wheel? ##
There are many implementation of statds available.
Nevertheless: the existing implementations have all some
limitations.  Most of them are written in some interpreted or
*strange* language (from the point of a C / C++ programmer) which
cannot rolled out in the production systems (at least where I am
working). There is one C implementation
(https://github.com/jbuchbinder/statsd-c) which has many bugs and
design issues. The test coverage of mostly all implementations is not
that high - if there exists a test at all.

# Status #

This project is currently in basic thinking / design phase. (Phase 1)
(Sorry - no source code until now.)

# Performance Considerations #
Some [performance tests](doc/PerformanceEtsyStatsd.md) were done with
the original [etsy statsd](https://github.com/etsy/statsd/).

The results: when you have clients which send out every small
statistics event in an own UDP packet, this implementation is not fast
/ reliable enough.  Depending on the tests drop rates from nearly 90%
to more then 98% could be seen.

Tests with other languages were done (Python and C++). These tests
just send out / receive UDP packets without any parsing or
computation. See the [performance document](doc/PerformanceTests.md)
for performance results sending and receiving UDP packets.

This lowers the drop rate: here drop rates between 0% and 70% were
measured. This is better than the original Javascript based
implementation - but still not what is expected.

# Requirements #

There will be some phases of this project. This gives a lean and agile
approach.  This program is implemented using TDD rules.

## Requirements ##

### High Level ###
 1. Implement original statds protocol from clients.
 2. Implement protocol to export to graphite.
 3. Program must be reliable.
 4. Program must be stable.

### Performance ###
Lets assume the following *prototype system*:
A system with which handles about 5000 packages / sec.  The packages
are worked on, business decisions are made, some are thrown away -
others are handled to the next process.  In this example it is assumed
that each process makes ten decisions - which gives 20 counter
increments per packet and process.  Additionally there are additional
counters for queue length, timings and so on, which easily doubles the
needed performance.

This results in 5.000 * 20 * 10 * 2 = 2.000.000 events to count each
second. 

Each process increments a counter 

## Phase 1 ##

Aim: Get some information and a feeling about sending and receiving UDP
     packets and existing statds implementations.

 1. Perform some basic performance tests with the original etsy
    statsd. 
 2. Write a UDP server and client based on python (my preferred test
    environment).
 3. Measure how many UDP packages can be send and received with the
    python test client.
 4. Decide if there is the need to do further speed improvements.

