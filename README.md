# csc

### Tools
* http://www.msys2.org/<br>
At its core is an independent rewrite of MSYS, based on modern Cygwin (POSIX compatibility layer) and MinGW-w64 with the aim of better interoperability with native Windows software. It provides a bash shell, Autotools, revision control systems and the like for building native Windows applications using MinGW-w64 toolchains.
It features a package management system to provide easy installation of packages, Pacman. It brings many powerful features such as dependency resolution and simple complete system upgrades, as well as straight-forward package building.


### Libs
* https://bellard.org/tcc/<br>
TinyCC (aka TCC) is a small but hyper fast C compiler. Unlike other C compilers, it is meant to be self-relying: you do not need an external assembler or linker because TCC does that for you. 
* https://github.com/michaelrsweet/mxml<br>
Mini-XML is a small XML parsing library that you can use to read XML data files or strings in your application without requiring large non-standard libraries. Mini-XML only requires a "make" program and an ANSI C compatible compiler - GCC works, as do most vendors' ANSI C compilers.
* https://liblfds.org/<br>
Welcome to liblfds, a portable, license-free, lock-free data structure library written in C.
Lock-free data structures are process, thread and interrupt safe (i.e. the same data structure instance can be safely used concurrently and simultaneously across cores, processes, threads and both inside and outside of interrupt handlers), never sleep (and so are safe for kernel use when sleeping is not permitted), operate without context switches, cannot fail (no need to handle error cases, as there are none), perform and scale literally orders of magnitude better than locking data structures, and liblfds itself (as of release 7.0.0) is implemented such that it performs no allocations (and so works with NUMA, stack, heap and shared memory) and compiles not just on a freestanding C89 implementation, but on a bare C89 implementation.
* https://github.com/nanomsg/nng<br>
NNG, like its predecessors nanomsg (and to some extent ZeroMQ), is a lightweight, broker-less library, offering a simple API to solve common recurring messaging problems, such as publish/subscribe, RPC-style request/reply, or service discovery. The API frees the programmer from worrying about details like connection management, retries, and other common considerations, so that they can focus on the application instead of the plumbing.
NNG is implemented in C, requiring only C99 and CMake to build. It can be built as a shared or a static library, and is readily embeddable. It is also designed to be easy to port to new platforms if your platform is not already supported.
Link with `-lnng`.
```bash
git clone https://github.com/nanomsg/nng
cd nng && mkdir build && cd build
cmake -G"MinGW Makefiles" .. -DCMAKE_INSTALL_PREFIX="C:\msys64\mingw64"
pacman -R cmake
pacman -S mingw-w64-x86_64-cmake
mingw32-make -j4
mingw32-make test
mingw32-make install
```
qmake:
```
DEFINES += NNG_STATIC_LIB
LIBS += -Wl,-Bstatic
LIBS += -lnng
LIBS += -lws2_32 -lmswsock -ladvapi32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32
```
* https://sourceforge.net/projects/iup/<br>
IUP is a portable toolkit for building graphical user interfaces. It offers a configuration API in three basic languages: C, Lua and LED. IUP purpose is to allow a program to be executed in different systems without any modification.
* https://sigrok.org/wiki/Libserialport<br>
libserialport (sometimes abbreviated as "sp") is a minimal, cross-platform shared library written in C that is intended to take care of the OS-specific details when writing software that uses serial ports. 



