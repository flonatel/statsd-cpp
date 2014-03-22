Performance Tests
=================

Several performance tests were done to get information how many
packages can be handled by the system with the appropriate language.

The timings were done on my laptop: Dell Latitude E7440:
Intel(R) Core(TM) i7-4600U CPU @ 2.10GHz, 16GRam inside a VM.

# Python #

## Client Only ##
When running only the client (which sends out UDP packets)
with the program test/performance/udpclient.py the following results
were measured:

Average of 5 tests sending out 1.000.000 packets each:
0m8.267s 0m5.084s 0m6.806s 0m6.427s 0m5.849s -> 6.4866s

Which gives: 154164 packets / sec.

## Clinet and Server ##
This is the more 'natural' way: a statsd is running on the local
machine as well as some clients.

Programs test/performance/udpclient.py and
test/performance/udpserver.py were used.

### Client Results ###
Average of 5 tests sending out 1.000.000 packets each:
11.598+13.480+11.763+11.264+15.217 -> 12.6644

Which gives: 78961 packets / sec.

### Server Results ###
On the server side each second the number of handled packages is
printed out. This gives also the possibility to check the number of
UDP packet drops.

      0       0
      0       0
  40094   40094
 120404   80310
 184155   63751
 257490   73335
 326661   69171
 394916   68254
 452385   57469
 531973   79580
 590646   58673
 697297  106647
 771987   74685
 828105   56118
 888813   60705
 895249    6436
 895249       0
 895249       0

Which gives a rough number of 50.000 to 100.000 packets / sec with a
drop rate of (sometimes) more then 10%.

# C++ #

## Client Only ##
Running also the client (without any server) using
test/performance/udpclient.cc the following results
were measured:

Average of 5 tests sending out 1.000.000 packets each:
4.512+4.836+4.452+4.168+3.324 -> 4.2584s

Which gives: 234830 packets / sec.

## C++ Client with Python Server ##
Running the C++ client with the Python server gives the
following results:

### Client Results ###
Average of 5 tests sending out 1.000.000 packets each:
2.284+2.519+2.551+2.345+2.254 -> 2.3906s

Which gives: 418305 packets / sec.

### Server Results ###
      0       0
      0       0
  54193   54193
 439778  385552
 734012  294234
 889100  155088
 889100       0
 889100       0

Also here: (sometimes) a drop rate higher than 10%.

## C++ Client with C++ Server ##

### Client Results ###
Average of 5 tests sending out 1.000.000 packets each:
6.327+6.125+5.818+6.086+5.432 -> 5.9576

Which gives: 167853 packets / sec.

### Server Results ###
0 0
122346 122346
257723 135374
407959 150233
578848 170887
720963 142112
886767 165803
1000000 113229
1000000 0
1000000 0

0 0
70739 70739
223194 152455
344472 121276
489376 144904
624795 135416
833841 209042
977454 143610
994663 17208
994663 0
994663 0

There are drops, but the drop rate is much lower than in tests with
the python server.


## 10 C++ Clients with 1 C++ Server ##
Running 10 clients in parallel, increases the time each client runs to
more then 12s. Here is the output of the server:

0 0
0 0
137972 137972
328119 190126
502375 174256
704800 202425
909594 204794
1113274 203680
1313833 200540
1497656 183823
1711644 213961
1909670 198026
2117358 207688
2295063 177705
2507337 212274
2742676 235339
3001129 258432
3001129 0
3001129 0

In this case there was a drop rate of about 70%.
