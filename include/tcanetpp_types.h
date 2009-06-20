/** 
  *   This header provides various defintions and typedefs that help to ensure 
  *   cross-platform compilation, primarily between Unix and Win32.
  *   
  * Copyright (c) 2002,2008,2009 Timothy Charlton Arland 
  *  @Author  tca@charltontechnology.net
  *
  * This file is part of tcanetpp.
  *
  * tcanetpp is free software: you can redistribute it and/or modify
  * it under the terms of the GNU Lesser General Public License as 
  * published by the Free Software Foundation, either version 3 of 
  * the License, or (at your option) any later version.
  *
  * tcanetpp is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public 
  * License along with tcanetpp.  
  * If not, see <http://www.gnu.org/licenses/>.
**/
#ifndef _TCANET_TYPES_H_
#define _TCANET_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WIN32
# include <inttypes.h>
# include <unistd.h>
# include <netinet/in.h>
# include <errno.h>
#endif


// win32 doesn't really standardize inttypes so 
// create our own
#ifdef WIN32  

# define INET_ADDRSTRLEN 16

  typedef long  ssize_t;
  typedef int   socklen_t;

  // from stdint.h 
  typedef signed char          int8_t;
  typedef unsigned char        uint8_t;
  typedef short                int16_t;
  typedef unsigned short       uint16_t;
  typedef int                  int32_t;
  typedef unsigned             uint32_t;
  typedef long long            int64_t;
  typedef unsigned long long   uint64_t;
  
  typedef unsigned long        ipv4addr_t;
  
  typedef SOCKET               sockfd_t;
  

#else  // non-win32 platforms

  typedef uint32_t             ipv4addr_t;
  typedef int                  sockfd_t;

#endif



#define INET_CIDRSTRLEN   20
#define ERRORSTRLEN       256
#define MAXMASKLEN        32

typedef struct in_addr    inaddr_t;

typedef struct cidr {
    ipv4addr_t  addr;
    uint16_t    addrlen;
} cidr_t;


# ifdef __cplusplus
}  // extern C
# endif


#endif // _TCANET_TYPES_H_
