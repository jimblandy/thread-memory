# Measuring the minimal memory overhead of a Linux thread

This repository includes a C program and a script meant to measure the minimal
overhead of a thread on Linux.

The C program creates a given number of threads that simply indicate that
they've started running, and then block in a system call. The main thread then
runs the `pmap` utility to get an accurate measurement of the program's resident
set size.

The shell script compiles and runs the C program on varying numbers of threads:


```
$ ./measure.sh 
100	822044	2432
150	1231844	2848
200	1641644	3260
250	2051444	3676
300	2461244	4088
350	2871044	4504
400	3280844	4920
450	3690644	5332
500	4100576	5748
550	4510376	6160
600	4920176	6576
650	5329976	6992
700	5739776	7404
750	6149576	7820
800	6559376	8232
850	6969176	8648
900	7378976	9064
950	7788908	9476
1000	8198708	9892
$ 
```

The columns are:
- number of threads
- virtual memory reserved, in KiB
- resident set size, in KiB

When you allocate a bunch of pages on Linux, the kernel reserves the given
portion of your address space, but doesn't actually allocate real memory to
those pages until you use them. The “virtual memory reserved” is the number of
pages Linux has pretended to give you, and the “resident set size” is the number
of pages with actual memory backing them.

Note that the resident set size includes pages shared with other processes, so
just exiting the program doesn't necessarily free up that much space. But by
measuring the slope of the lines described by the data, and ignoring their
y-intercepts, we can see the incremental contribution of each new thread.

The above data suggest that each new thread costs around 8KiB of RSS.
