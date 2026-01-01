tcanetpp
========

### LICENSE

  Copyright (c) 2002-2026 Timothy Charlton Arland 
        <tcarland@gmail.com>, <tca@charltontechnology.net>

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

The 'tcanetpp' library is a C++ library for providing core networking
functionality with an object-oriented interface. The library additionally
contains a collection of utility classes that are not all necessarily 
networking specific, but are frequently used in such a context.

The library has been used in production software and has been
refined over many years. At the heart, it provides an Object-Oriented
Socket class that wraps the Standard Library socket functions as well
as (WIN32) Winsock2, providing a cross-platform implementation.   

The original goal is to provide core network functionality based solely 
on Berkeley Sockets, libc, and libstdc++, with no other dependencies 
(with the exception of pthreads for the Thread-related classes). While 
written in C++, the overall design is intended to have little inter-
dependency, such that by use of any one feature of the library 
(eg. event api) does not depend on other features of the library.  

Celebrating 20 years as a project, it has evolved much over the years 
to stay current to the C++ standards, but remaining lean and usable. 
While there may be better options for some components, especially 
when adopting libboost, some components are still very useful today. 

libtcanetpp provides the following core functionality:
 * Containers: Patricia/Radix trie, Hashmap, Circular Buffer, Synchronized Queue
 * Event Management: For handling both network I/O and high-resolution timers
 * Threads: An interface for managing pthreads, thread pools and mutexes
 * Network: Socket and IP classes for manipulating IPv4 and IPv6 Addresses
 * Logging: A thread-safe logging facility  
 
The library has few external dependencies outside of libc. The project can 
build an additional individual library for the command buffer class. The 
*pthreads* library dependency is needed for the various *Thread* classes 
and *librt* is also used on linux systems for the Event API.  


### Building

The project uses the **tcamake** build environment for managing project
dependencies. This provides a Makefile template for projects.  To build,
the **tcamake** project should be pulled down to the parent directory of
the project.
``` bash
cd ..
git clone https://github.com/tcarland/tcamake
cd tcanetpp
```

Select a build profile and run `make`.
```bash
source resources/profile_release_mt
make
```

The project has been tested against stdc++23 and gnu 11.4.0

- **Unix:**  
  The library requires linking *libpthread*, *librt* and *libdl*.
  The *librt* library should be included on linux systems, especially for the
  high-resolution timer support provided by the *EventManager*.

- **Mac OSX:**  
  *tcamake* is needed in addition to the gnu tools (provided with xcode).

- **Win32/64:**
  Only the thread classes (based on libpthread are unsupported with windows.
  The win32 pthread library is mostly be compatible, but ymmv.


### Documentation

The library is documented using doxygen. Run `make documentation` to 
generate the docs (doxygen required).
