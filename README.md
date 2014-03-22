statsd-cpp
==========

Implementation of the statsd in C++


[![Build
Status](https://secure.travis-ci.org/flonatel/statsd-cpp.png)](http://travis-ci.org/flonatel/statsd-cpp)

# Introduction #

*Measure Anything, Measure Everything* is the philosophy behind the
inventors of the original statsd (https://github.com/etsy/statsd/).

Nevertheless: the existing implementations I found have all some
limitations.  Most of them are written in some interpreted or
*strange* language (from the point of a C / C++ programmer) which
cannot rolled out in the production systems I had to support. There is
one C implementation (https://github.com/jbuchbinder/statsd-c) which
has many bugs and design issues. The test coverage of mostly all
implementations is not that high.

# Status #

This project is currently in basic thinking / design phase. (Phase 1)
(Sorry - no source code until now.)

# Performance Considerations #
See the [performance document](doc/PerformanceTests.md) for performance results
sending and receiving UDP packets.

# Requirements #

There will be some phases of this project. This gives a lean and agile
approach.  This program is implemented using TDD rules.

## Basic Ideas ##

### Major requirements ###

 1. Implement original statds protocol from clients.
 2. Implement protocol to export to graphite.
 3. High performance: handle 25.000 requests per second.

### Implementation considerations ###

 1. Use (massive) multi-threading: let the system decide what to do.
 2. Use TDD: have a test coverage of at least 90%.

## Phase 1 ##

Aim: Get some information and a feeling about sending and receiving UDP
     packets

 1. Write a UDP server and client based on python (my preferred test
    environment).
 2. Measure how many UDP packages can be send and received with the
    python test client.
 3. Decide if there is the need to do further speed improvements.

## Phase N ##

Aim: Design

 1. Write down the design ideas.
 2. Think about the language details (C++03, C++11, g++, clang, ....).
 3. Decide which libraries to use (e.g. openssl, ptl, poco, ...)

