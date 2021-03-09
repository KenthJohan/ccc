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

* https://github.com/eclipse/mosquitto
Mosquitto is an open source implementation of a server for version 5.0, 3.1.1, and 3.1 of the MQTT protocol. It also includes a C and C++ client library, and the mosquitto_pub and mosquitto_sub utilities for publishing and subscribing.

<details>
  <summary>https://github.com/eclipse/mosquitto/issues/1707</summary>
  
```cmake
# This is a cmake script. Process it with the CMake gui or command line utility
# to produce makefiles / Visual Studio project files on Mac OS X and Windows.
#
# To configure the build options either use the CMake gui, or run the command
# line utility including the "-i" option.

cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0042 NEW)

project(mosquitto)
set (VERSION 2.0.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

add_definitions (-DCMAKE -DVERSION=\"${VERSION}\")

if (WIN32)
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
	add_definitions("-D_CRT_NONSTDC_NO_DEPRECATE")
	if (MINGW)
		add_definitions("-D_WIN32_WINNT=_WIN32_WINNT_VISTA")
	endif (MINGW)
endif (WIN32)

if(APPLE)
	set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS} -undefined dynamic_lookup")
endif(APPLE)

include(GNUInstallDirs)

option(WITH_BUNDLED_DEPS "Build with bundled dependencies?" ON)
option(WITH_TLS
	"Include SSL/TLS support?" ON)
option(WITH_TLS_PSK
	"Include TLS-PSK support (requires WITH_TLS)?" ON)
option(WITH_EC
	"Include Elliptic Curve support (requires WITH_TLS)?" ON)
if (WITH_TLS)
	find_package(OpenSSL REQUIRED)
	add_definitions("-DWITH_TLS")

	if (WITH_TLS_PSK)
		add_definitions("-DWITH_TLS_PSK")
	endif (WITH_TLS_PSK)

	if (WITH_EC)
		add_definitions("-DWITH_EC")
	endif (WITH_EC)
else (WITH_TLS)
	set (OPENSSL_INCLUDE_DIR "")
endif (WITH_TLS)


option(WITH_UNIX_SOCKETS "Include Unix Domain Socket support?" ON)
if (WITH_UNIX_SOCKETS AND NOT WIN32)
	add_definitions("-DWITH_UNIX_SOCKETS")
endif (WITH_UNIX_SOCKETS AND NOT WIN32)

option(WITH_SOCKS "Include SOCKS5 support?" ON)
if (WITH_SOCKS)
	add_definitions("-DWITH_SOCKS")
endif (WITH_SOCKS)

option(WITH_SRV "Include SRV lookup support?" OFF)

option(WITH_STATIC_LIBRARIES "Build static versions of the libmosquitto/pp libraries?" OFF)
option(WITH_PIC "Build the static library with PIC (Position Independent Code) enabled archives?" OFF)

option(WITH_THREADING "Include client library threading support?" ON)
if (WITH_THREADING)
	add_definitions("-DWITH_THREADING")
	if (WIN32 AND NOT MINGW)
		if (CMAKE_CL_64)
			set (PTHREAD_LIBRARIES C:\\pthreads\\Pre-built.2\\lib\\x64\\pthreadVC2.lib)
		else (CMAKE_CL_64)
			set (PTHREAD_LIBRARIES C:\\pthreads\\Pre-built.2\\lib\\x86\\pthreadVC2.lib)
		endif (CMAKE_CL_64)
		set (PTHREAD_INCLUDE_DIR C:\\pthreads\\Pre-built.2\\include)
	elseif (ANDROID)
		set (PTHREAD_LIBRARIES "")
		set (PTHREAD_INCLUDE_DIR "")
	else (WIN32 AND NOT MINGW)
		find_library(LIBPTHREAD pthread)
		if (LIBPTHREAD)
			set (PTHREAD_LIBRARIES pthread)
		else (LIBPTHREAD)
			set (PTHREAD_LIBRARIES "")
		endif()
		set (PTHREAD_INCLUDE_DIR "")
	endif (WIN32 AND NOT MINGW)
else (WITH_THREADING)
	set (PTHREAD_LIBRARIES "")
	set (PTHREAD_INCLUDE_DIR "")
endif (WITH_THREADING)

option(WITH_DLT "Include DLT support?" OFF)
message(STATUS "WITH_DLT = ${WITH_DLT}")
if (WITH_DLT)
	#find_package(DLT REQUIRED)
	find_package(PkgConfig)
	pkg_check_modules(DLT "automotive-dlt >= 2.11")
	add_definitions("-DWITH_DLT")
endif (WITH_DLT)

option(WITH_CJSON "Build with cJSON support (required for dynamic security plugin and useful for mosquitto_sub)?" ON)
if (WITH_CJSON)
    FIND_PACKAGE(cJSON)
    if (CJSON_FOUND)
	    message(STATUS ${CJSON_FOUND})
    else (CJSON_FOUND)
	    message(STATUS "Optional dependency cJSON not found. Some features will be disabled.")
    endif(CJSON_FOUND)
endif()

# ========================================
# Include projects
# ========================================

option(WITH_CLIENTS "Build clients?" ON)
option(WITH_BROKER "Build broker?" ON)
option(WITH_APPS "Build apps?" ON)
option(WITH_PLUGINS "Build plugins?" ON)
option(DOCUMENTATION "Build documentation?" ON)

add_subdirectory(lib)
if (WITH_CLIENTS)
	add_subdirectory(client)
endif (WITH_CLIENTS)

if (WITH_BROKER)
	add_subdirectory(src)
endif (WITH_BROKER)

if (WITH_APPS)
	add_subdirectory(apps)
endif (WITH_APPS)

if (WITH_PLUGINS)
	add_subdirectory(plugins)
endif (WITH_PLUGINS)

if (DOCUMENTATION)
	add_subdirectory(man)
endif (DOCUMENTATION)

# ========================================
# Install config file
# ========================================

if (WITH_BROKER)
	install(FILES mosquitto.conf aclfile.example pskfile.example pwfile.example DESTINATION "${CMAKE_INSTALL_SYSCONFDIR}/mosquitto")
endif (WITH_BROKER)

# ========================================
# Install pkg-config files
# ========================================

configure_file(libmosquitto.pc.in libmosquitto.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libmosquitto.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
configure_file(libmosquittopp.pc.in libmosquittopp.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libmosquittopp.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

# ========================================
# Testing
# ========================================
enable_testing()
```
</details>

```bash
git clone https://github.com/eclipse/mosquitto
cd nng && mkdir build && cd build
cmake -G"MinGW Makefiles" .. -DCMAKE_INSTALL_PREFIX="C:\msys64\mingw64"
pacman -R cmake
pacman -S mingw-w64-x86_64-cmake
mingw32-make -j4
mingw32-make test
mingw32-make install
cp libmosquitto.dll.a C:\msys64\mingw64\lib\libmosquitto.dll.a
```

