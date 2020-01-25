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

