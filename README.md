statsd-cpp
==========

Implementation of the statsd in C++ (Status: planning)

[![Build
Status](https://secure.travis-ci.org/flonatel/statsd-cpp.png)](http://travis-ci.org/flonatel/statsd-cpp)

# Introduction #

## Think! Measure reliable! ##
Think what you want to measure and do this reliable. Numbers which are
generated only from a small portion of the data (and it is even not known
how many and which data was deleted) is worthless.

[*Measure Anything, Measure
Everything*](http://codeascraft.com/2011/02/15/measure-anything-measure-everything/)
is the philosophy behind the inventors of the original [Etsy
statsd](https://github.com/etsy/statsd/).  [Performance
tests](doc/PerformanceEtsyStatsd.md) have shown that this aim cannot be
accomplished with the ideas and technology implemented in the
Etsy statsd - especially for systems with a lot of events.

## Reinvent the Wheel? ##

[There are many implementations of
statsd](http://joemiller.me/2011/09/21/list-of-statsd-server-implementations)
available.  Nevertheless: the existing implementations have all some
limitations.  Most of them are written in some interpreted or
*strange* language (from the point of a C / C++ programmer) which
cannot rolled out in the production systems (at least where I am
working). There is one [C
implementation](https://github.com/jbuchbinder/statsd-c) which has
many bugs and design issues. The test coverage of mostly all
implementations is not that high - if there exists a test at all.

# Status #

This project is currently in basic thinking / design phase. (Phase 1)
(Sorry - no source code yet.)

# Performance Considerations #

Some [performance tests](doc/PerformanceEtsyStatsd.md) were done with
the original [etsy statsd](https://github.com/etsy/statsd/).

The results: when you have clients which send out every statistics
event in an separate UDP packet, this implementation is not fast / reliable
enough.  Depending on the tests drop rates from nearly 90% to more
then 98% could be seen.

Tests with other languages were done (Python and C++). These tests
just send out / receive UDP packets without any parsing or
computation. See the [document of performance
results](doc/PerformanceTests.md) sending and receiving UDP packets.

Using a lower-level programing language lowers the drop rate: here
drop rates between 0% and 70% were measured. This is better than the
original Javascript based implementation - but still not what is
expected.  Mostly the same results were measured using the
[statsite](https://github.com/armon/statsite) implementation: when
using UDP 70-80% drop rate when using ten clients.

# Requirements #

There will be some phases of this project. This gives a lean and agile
approach.  This program is implemented using TDD.

## Requirements ##

### High Level ###
 1. Implement original statsd protocol for clients.
 2. Implement additional reliable protocol for clients.
 3. Implement protocol to export to graphite.
 4. Program must be reliable.
 5. Program must be stable.
 6. Program must be easy testable.
 7. Statistics should consume only very few resources.

### Performance ###
Lets assume the following *prototype system*:
A system with which handles about 10.000 packages / sec.  The packages
are worked on, business decisions are made, some are thrown away -
others are handled to the next process.  In this example it is assumed
that each process makes 20 decisions - which gives 20 counter
increments per packet and process.  Additionally there are additional
counters for queue lengths, timings and so on, which easily doubles the
needed performance.

This results in 10.000 * 20 * 10 * 2 = 4.000.000 events to count each
second. 

# Phase 1 #

Aim: Get some information and a feeling about sending and receiving UDP
     packets and existing statsd implementations.

 1. Perform some basic performance tests with the original etsy
    statsd. 
 2. Write a UDP server and client based on python (my preferred test
    environment).
 3. Measure how many UDP packages can be send and received with the
    python test client.
 4. Decide if there is the need to do further speed improvements.

## Very first results ##
The very first partial implementation (which makes massive use of
strings), was tested.  The bench.py script from statsite was used:

    3.39 sec	 - 309130 ops/sec
    7.11 sec	 - 294770 ops/sec
    10.85 sec	 - 289992 ops/sec
    14.52 sec	 - 288949 ops/sec

Which gives some room for optimization :-)


# Development Environment Setup #

To get the current environment set up, do:

    git clone https://github.com/flonatel/statsd-cpp.git
    cd statsd-cpp
    ./build/init_autotools.sh
    cd ..
    mkdir BUILD-statsd
    cd BUILD-statsd
    ${PWD}/../statsd-cpp/configure --enable-debug
    make
    make bbtests

You need at least g++-4.7 installed. If you are running
Ubuntu 12.04 LTS you might want to have a look at the [Travis CI
configuration file](.travis.yml).


# Collection of Ideas #

This is a list of ideas and open points.

 1. What about reading from stdin and passing the output to stdout?
    (statsite has a similar approach):
    <code>socat UDP-LISTEN:8125,fork - | statsd | socat - TCP:localhost:2003</code>
    This will also simplify testing dramatically.



