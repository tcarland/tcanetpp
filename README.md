tcanetpp
========

  Copyright (c) 2002-2021 Timothy Charlton Arland 
        <tcarland@gmail.com>, <tca@charltontechnology.net>

### LICENSE

  tcanetpp is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  tcanetpp is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with tcanetpp.  
  If not, see <http://www.gnu.org/licenses/>.

<!--
  @mainpage tcanetpp
 -->

<!--
  @section overview
  -->
### Overview

  The 'tcanetpp' library is C++ library for providing core networking
functionality with an object-oriented interface. The library is a
collection of utility classes that are not all necessarily networking
specific, but are included for convenience.  

  The library has been used in production software and has been
refined over many years. At the heart, it provides an Object-Oriented
Socket class that wraps the Standard Library socket functions as well
as (WIN32) Winsock2, providing a cross-platform implementation.   

  The general goal of the library is to provide core network
functionality based solely on Berkeley Sockets, libc, and libstdc++,
with no other dependencies (with the exception of pthreads for the
Thread-related classes). While written in C++, the overall design is
intended to be not-so object heavy with as little inter-dependency as
possible, such that by use of any one feature of the library
(eg. event api) does not depend on other features of the library.  

The library provides the following functionality:
 * Containers: patricia/radix tree, hashmap, circular buffer, synchronized queue
 * Event Management for handling both network I/O and sub-second timers
 * Threads: An interface for pthreads and pthread mutexes
 * Network: Socket and IP functions for manipulating IPv4 and IPv6 Addresses
 * Logging: A thread-safe logging facility

  The library has few external dependencies outside of libc. The
project can build an additional individual library for the command buffer
class. The pthreads library dependency is needed for the *Thread* classes 
and librt on linux systems.  


### Building

 The project uses the **tcamake** build environment for managing project
dependencies. This provides a Makefile template for projects.  To build,
the **tcamake** project should be pulled down to the parent directory of
the project.

```
cd ..
git clone https://github.com/tcarland/tcamake
cd tcanetpp
export TCAMAKE_PROJECT=1
```

Select a build profile and run `make`.
```
source resources/profile_release_mt
make
```

**Unix:**
   The library requires linking libpthread, librt and libdl.
The library librt should be included on linux systems, especially for the
high resolution timing support provided by the *EventManager*.

**Mac OSX:**
  *tcamake* is needed in addition to the gnu tools (provided with xcode).

**Win32/64:**
  Only the thread classes (based on libpthread are unsupported with windows.
The win32 pthread library is mostly be compatible, but ymmv.


### Documentation

  The library is documented in an API format via doxygen.
Run 'make documentation' to generate the docs (doxygen required).  
