# Measuring the minimal memory overhead of a Linux thread

This repository includes a C program and a script meant to measure the minimal
overhead of a thread on Linux.

On my machine, these data suggest that each new thread costs around 8KiB of RSS,
even if that thread does almost nothing. A modification of the code here (see
the comments) suggests that each of these threads consumes around 11KiB of
memory in the kernel.

To run this script, you'll need to have the Linux `pmap` utility installed. On
Fedora, this is included in the `procps-ng` package.

The C program `thread-memory.c` creates a given number of threads that simply
indicate that they've started running, and then block in a system call. The main
thread then runs the `pmap` utility to get an accurate measurement of the
program's resident set size.

The shell script `measure.sh` compiles the C program and runs it creating
varying numbers of threads:


```
$ ./measure.sh
num threads	virtual KiB	resident KiB
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

When you ask the kernel for a block of memory on Linux, e.g. for a thread stack,
the kernel reserves an appropriately-sized portion of your address space, but
doesn't actually allocate real memory to those pages until you use them. The
“virtual KiB” is the amount of memory Linux has pretended to give you, and the
“resident KiB” is the amount of actual memory backing them, as determined by
whether the program actually used those pages.

Note that the total resident set size (RSS) includes not just the thread-related
costs that we're interested in, but everything else the program is using. By
measuring the slope of the lines described by the data, and ignoring their
y-intercepts, we can see the incremental contribution of each new thread.

(RSS also includes memory shared with other processes, so exiting a program
doesn't necessarily free up as much memory as its RSS says it's using. But that
isn't our concern here.)
