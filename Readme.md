# Compiling the Control Flow Graph Construction 

This library is compatible with `pin-2.14-71313-gcc.4.4.7-linux` only. I tried testing with newer version of intel pintool i.e `pin-3.11-97998-g7ecce2dac-gcc-linux`. I got an error as follows: 

```
In file included from /usr/include/x86_64-linux-gnu/c++/7/bits/c++config.h:533:0,
                 from /usr/include/c++/7/cfenv:38,
                 from /usr/include/x86_64-linux-gnu/c++/7/bits/stdc++.h:53,
                 from DataDependencyGraph.h:13,
                 from PDG.cpp:14:
/usr/include/x86_64-linux-gnu/c++/7/bits/os_defines.h:44:19: error: missing binary operator before token "("
 #if __GLIBC_PREREQ(2,15) && defined(_GNU_SOURCE)
                   ^
In file included from /usr/include/c++/7/cfenv:41:0,
                 from /usr/include/x86_64-linux-gnu/c++/7/bits/stdc++.h:53,
                 from DataDependencyGraph.h:13,
                 from PDG.cpp:14:
/home/bernard/PIN/pin-3.11-97998-g7ecce2dac-gcc-linux/extras/crt/include/fenv.h:49:10: fatal error: machine/fenv.h: No such file or directory
 #include <machine/fenv.h>
```

not able to run the compile binaries: 
Error 
```
profiling: /home/mukta/Mukta/s3cbench/bubble_sort/bubble.gcda: cannot open: No such file or directory
```

The alternative approach is to install system-c. I follow the tutorial found (https://howto.tech.blog/2016/11/27/installing-systemc-2-3-1/)[] and install version 2.3.3. 
`Not able to compile bubble sort (systemc)`

# Function Extractor
`cd functionExtractor` and run `make` to compile

and running the functionextrator.sh if any error such as the following

```
E: Attach to pid 3828 failed. 
E:   The Operating System configuration prevents Pin from using the default (parent) injection mode.
E:   To resolve this, either execute the following (as root):
E:   $ echo 0 > /proc/sys/kernel/yama/ptrace_scope
E:   Or use the "-injection child" option.
E:   For more information, regarding child injection, see Injection section in the Pin User Manual.
E: 
./functionextractor.sh: line 1:  3828 Killed                  pin -ifeellucky -t FuncExtractor/obj-intel64/FuncExtractor.so -- ./apps/test
```
run the command by running `sudo su` and then `echo 0 > /proc/sys/kernel/yama/ptrace_scope`

Make sure you have Linux Kernel < 4.0 and Intel Pintool: `pin-2.14-71313-gcc.4.4.7-linux` (In folder PIN)












