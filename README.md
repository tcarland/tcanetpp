tcanetpp
========
 
  Copyright (c) 2002-2016 Timothy Charlton Arland <tca@charltontechnology.net>

#### LICENSE
 
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
#### Overview

  The 'tcanetpp' library is C++ library for providing core
networking functionality with an object-oriented interface. The 
library is a collection of utility classes that are not all 
necessarily networking specific, but are included for convenience.  

  The library has been used in production software and has been 
refined over many years. At the heart, it provides an Object-Oriented 
Socket class that wraps the Standard Library socket functions as well 
as (WIN32) Winsock2, providing a cross-platform implementation.   

  The general goal of the library is to provide core network functionality 
based completely on berkely sockets, libc, and libstdc++, with no other 
dependencies (with the exception of pthreads for the Thread-related classes).  

The library provides the following additional functionality: 
 * Containers: patricia/radix tree, hashmap, circular buffer, synchronized queue
 * An event management class for handling both network I/O and sub-second timers 
 * An Object-Oriented interface for pthreads and pthread mutexes
 * IP functions for manipulating IPv4 and IPv6 Addresses
 * A thread-safe logging facility
 * Network Device objects for management purposes   
  
   
  The library has few external dependencies outside of libc. The project can 
build individual libraries consisting of just the sub-components, or a single, 
encompassing lib of 'libtcanetpp'.  The pthreads library is only needed for the 
Thread related classes.   


#### Building

 The project uses the **tcamake** build environment for managing project dependencies
and running 'make'.  To build, the **tcamake** project should be pulled down to the parent
directory of the project by running:
```
cd ..
git clone https://github.com/tcarland/tcamake
cd tcanetpp
export TCAMAKE_PROJECT=1
make
``` 

**Unix:**
   The library requires linking only libpthread and libdl.
The library librt should be included on linux systems, especially for the 
high resolution timing support provided by the **EventManager**. 

**Mac OSX:**
  **tcamake** is needed plus gnu tools provided with xcode.

**Win32/64:**
  Only the thread classes (based on libpthread are unsupported with windows.
The win32 pthread library may be compatible, but ymmv. 


#### Documentation
  The library is being documented in an API format via doxygen. 
Run 'make documentation' to generate the docs (doxygen required).  



