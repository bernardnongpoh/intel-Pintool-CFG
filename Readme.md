
# Dependencies


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


# Possible Errors

While running the tool, you might encounter the following errors. 

```
E: Attach to pid 3828 failed. 
E:   The Operating System configuration prevents Pin from using the default (parent) injection mode.
E:   To resolve this, either execute the following (as root):
E:   $ echo 0 > /proc/sys/kernel/yama/ptrace_scope
E:   Or use the "-injection child" option.
E:   For more information regarding child injection, see the Injection section in the Pin User Manual.
E: 
./functionextractor.sh: line 1:  3828 Killed                  pin -ifeellucky -t FuncExtractor/obj-intel64/FuncExtractor.so -- ./apps/test
```

run the command by running `sudo su` and then `echo 0 > /proc/sys/kernel/yama/ptrace_scope`

Make sure you have Linux Kernel < 4.0 and Intel Pin tool: `pin-2.14-71313-gcc.4.4.7-Linux`. The correct pin tool version is in folder `PIN.`


# Example Walkthrough on how to use the tool

Consider a simple dummy application `apps/test.c` as shows below consisted of integer variables `a` and `b` with  multiple `if statement` in function `hello()` and in `main()` function: 
```
void hello(){
int a=10;
int b=20;
if(b==10){
    b=b-1;
    if(b==0)
    {
        b=b+1;
    }
}
else{
    a=a-1;
    
}
int k=a+b;

if (k==10)
{
    k=1-1;
}

}
int main(){
int a=10;
int b=20;
if(a==10){
b=b+a;
if(b==20)
{
    b=b-1;
}
}
else{
b=b-1;
}
return 0;

}
``` 
Our goal is to check if the analysis tool able to generate a proper Control Graph for each functions `hello()` and `main().` Modern compilers such as `gcc` and `clang` perform optimizations and would remove unnecessary `for loop` or `if statement.` To avoid optimization, we compile the test.c program without specifying the optimization flag. 

`gcc test.c -o test`

Consider the case where source code is available; we can look at the program and list out all functions. For instance, the `test.c` program consisted of `hello()` and `main()` functions. The problem is with large programs and with applications where source code is not available. To extract functions for binaries, we developed a Pin tool `FunctionExtractor` to do the job for us.

# FunctionExtractor

## Rebuild 
```
cd FunctionExtractor
make clean 
make
```

To run the `FunctionExtractor` run the following command:
```
pin -ifeellucky -t FunctionExtractor/obj-intel64/FuncExtractor.so -- ./apps/test 
```

`ifeellucky` is a flag for Linux kernel > 4.0. Meaning `if you're lucky,  you may be able to run, but with no guarantee` (Recommend to use Linux kernel < 4.0)

On running the above command, the tool generates a file `function.list`. 

```
ion.list 
.plt
.plt.got
__do_global_dtors_aux
__libc_csu_fini
__libc_csu_init
_fini
_init
_start
deregister_tm_clones
frame_dummy
hello
main
register_tm_clones

```

As you can see, there are other functions listed in the file. Our interest is `hello()` and `main()` functions. 


# hello() CFG 

## Rebuild 

Again to rebuild `cd CFG` --> `make clean`--> `make`

## CFG generation

```
pin -t CFG/obj-intel64/PDG.so  -function hello  -- ./apps/test/test

```
The above command performs the analysis for a function `hello()` as given (`-function hello`)

The analysis generates a `dot file` `hello.` To view run the command `xdot hello`. 

Additionally, the tool generates a reverse CFG with the name `reverseCFG_hello` which shows the Reverse CFG of the given program.
`xdot reverseCFG_hello`





