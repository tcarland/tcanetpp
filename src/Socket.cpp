/**   The base Socket class.
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
#define _TCANETPP_SOCKET_CPP_

#include "Socket.h"

#ifndef WIN32
# include <arpa/inet.h>
# include <netinet/in.h>
# include <poll.h>
# include <fcntl.h>
# include <errno.h>
#endif


namespace tcanetpp {


Socket::SocketFactory Socket::factory;


// ----------------------------------------------------------------------

Socket::Socket()
    : _socktype(SOCKET_NONE),
      _proto(0),
      _port(0),
      _bound(false),
      _connected(false),
      _block(false),
      _noUdpClose(false)
{
    Socket::ResetDescriptor(this->_fd);
}


Socket::Socket ( ipv4addr_t ipaddr, uint16_t port, SocketType type, int protocol )
    throw ( SocketException )
    : _socktype(type),
      _proto(protocol),
      _port(port),
      _bound(false),
      _connected(false),
      _block(false),
      _noUdpClose(false)
{
    Socket::ResetDescriptor(this->_fd);
    
    if ( _socktype <= SOCKET_NONE || _socktype > SOCKET_SERVER_CLIENT )
        throw SocketException("Socket error: Invalid Socket type");
    
    if ( _proto < 0 || _proto > 255 )
        throw SocketException("Socket error: Invalid protocol");
    
    memset(&_sock, 0, sizeof(struct sockaddr_in));
    _sock.sin_family = AF_INET;
    _sock.sin_port   = htons(_port);
    
    if ( _socktype == SOCKET_SERVER && ipaddr == 0 ) {
        _sock.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        if ( ipaddr == 0 )
            throw SocketException("Socket error: Invalid IP addr");
            
        _sock.sin_addr.s_addr = ipaddr;
        _addrstr = Socket::ntop(_sock.sin_addr.s_addr);
    }
    
    if ( this->_proto == IPPROTO_UDP )
        this->init(_block);
}


Socket::Socket ( sockfd_t & fd, struct sockaddr_in & csock, int protocol )
    : _fd(fd),
      _sock(csock),
      _socktype(SOCKET_SERVER_CLIENT),
      _proto(protocol),
      _bound(false),
      _connected(false),
      _block(false),
      _noUdpClose(false)
{
    if ( Socket::IsValidDescriptor(this->_fd) ) {
        if ( _proto == IPPROTO_TCP )
            _connected  = true;
        else if ( _proto == IPPROTO_UDP )
            _noUdpClose = true;
        _bound     = true;
    } else {
        _connected = false;
        _bound     = false;
    }
    
    _port    = ntohs(csock.sin_port);
    _addrstr = Socket::ntop(csock.sin_addr.s_addr);
}


Socket::~Socket()
{
    if ( ! this->_noUdpClose )
        this->close();
}

// ----------------------------------------------------------------------

int
Socket::init ( bool block )
{
    if ( ! Socket::IsValidDescriptor(_fd) ) {
        try {
            Socket::InitializeSocket(_fd, _proto);
        } catch ( SocketException & err ) {
            _errstr = err.toString();
            return -1;
        }
    }
      
    if ( _socktype == SOCKET_SERVER ) {
    	this->setSocketOption(SocketOption::SetReuseAddr(1));
        
        if ( ! this->bind() )
            return -1;
            
        if ( _proto == IPPROTO_TCP )
            this->listen();
    }
    
    _block = block;

    if ( ! _block )
        Socket::Unblock(this);
    else
        Socket::Block(this);
        
    return 1;
}

// ----------------------------------------------------------------------

int
Socket::bind()
{
    char  serr[ERRORSTRLEN];
    int   r = 0;

    if ( _socktype != SOCKET_SERVER || _bound || ! Socket::IsValidDescriptor(_fd) ) {
    	_errstr = "Socket::bind() socket is not initialized";
        return r;
    }
        
    r = ::bind(_fd, (struct sockaddr*) &_sock, sizeof(struct sockaddr));
    
    if ( r != 0 ) {
	_errstr = "Socket::bind() Failed to bind";
	
#       ifndef WIN32
	if ( strerror_r(errno, serr, ERRORSTRLEN) == 0 )
            _errstr = serr;
#       endif
	
        return -1;
    }
    
    _bound = true;
    
    return 1;
}

// ----------------------------------------------------------------------

int
Socket::listen()
{
    if ( _socktype != SOCKET_SERVER || _proto != IPPROTO_TCP )
        return 0;
        
    if ( ! this->_bound )
        this->bind();
        
    ::listen(_fd, 1);
    
    _connected = true;
    
    return 1;
}

// ----------------------------------------------------------------------

int
Socket::connect()
{
    char  serr[ERRORSTRLEN];

    if ( _socktype != SOCKET_CLIENT )
	return -1;
	
    if ( _connected )
	return 1;

    if ( ! Socket::IsValidDescriptor(_fd) )
        this->init(_block);

    if ( ::connect(_fd, (struct sockaddr*) &_sock, sizeof(_sock)) < 0 ) 
    {
#       ifdef WIN32
    	
        int  err = WSAGetLastError();
        
        if ( err == WSAEWOULDBLOCK || err == WSAEALREADY || err == WSAEINVAL ) {
            return 0;
        } else if ( err == WSAEISCONN ) {
            _connected = true;
            return 1;
        }
        
#       else
        
        if ( errno == EINPROGRESS )
            return 0;
        else if ( errno == ECONNREFUSED )
            _errstr = "Socket::connect() Connection Refused";
        else 
            _errstr = "Socket::connect() Error in connect attempt";

        if ( strerror_r(errno, serr, ERRORSTRLEN) == 0 )
            _errstr = serr;
        
#     endif

        return -1;
    } else {
    	_connected = true;
    }

    return 1;
}

// ----------------------------------------------------------------------

void
Socket::close()
{
   if ( ! this->_noUdpClose ) {
        if ( Socket::IsValidDescriptor(_fd) ) {
           _connected = false;
     
#          ifdef WIN32
           ::closesocket(this->_fd);
#          else
           ::close(this->_fd);
#          endif
       }
       Socket::ResetDescriptor(_fd);
   }

   return;
}

// ----------------------------------------------------------------------

/**  Shutdowns half or the full connection.
  *
  *  @param shut  is the integer value representing which halves of 
  *  the connection to shut. '0' shuts the receive, '1' shuts the send,
  *  and 2 shuts both the send and the receive.
 **/ 
void
Socket::shutdown ( int shut )
{
#   ifdef WIN32
    
    if ( shut == 0 ) {
        ::shutdown(_fd, SD_RECEIVE);
    } else if ( shut == 1 ) {
        ::shutdown(_fd, SD_SEND);
    } else if ( shut == 3 ) {
        ::shutdown(_fd, SD_BOTH);
        _connected = false;
    }
    
#   else
    
    ::shutdown(_fd, shut);
    
#   endif
    
    return;
}

// ----------------------------------------------------------------------

Socket*
Socket::accept()
{
    return this->accept(Socket::factory);
}

/**  The SocketFactory provides a mechanism for instantiating derived Socket 
  *  objects while properly maintaining this interface. A derived implementation 
  *  would also derive the SocketFactory class in order to instantiate the 
  *  derived type prior to casting back to the base Socket* type.
 **/
Socket*
Socket::accept ( SocketFactory & factory )
{
    Socket             *client = NULL;
    struct sockaddr_in  csock;
    socklen_t           len;
    sockfd_t            cfd;

    if ( _socktype != SOCKET_SERVER )
    	return NULL;

    len = sizeof(csock);
    memset(&csock, 0, len);

    if ( _proto == SOCKET_TCP )
    {
        if ( (cfd = ::accept(_fd, (struct sockaddr*) &csock, &len)) < 0 )
        	return NULL;

        client = factory(cfd, csock);
    }
    else if ( _proto == SOCKET_UDP )
    {
    	client = factory(_fd, csock);
    }

    if ( !_block )
    	Socket::Unblock(client);

    return client;
}

// ----------------------------------------------------------------------

const sockfd_t&
Socket::getDescriptor() const
{
    return _fd;
}

// ----------------------------------------------------------------------

const SocketType&
Socket::getSocketType() const
{
    return this->_socktype;
}

// ----------------------------------------------------------------------

const uint16_t&
Socket::getPort() const
{
    return this->_port;
}

// ----------------------------------------------------------------------

const int&
Socket::getSocketProtocol() const
{
    return this->_proto;
}

// ----------------------------------------------------------------------

const std::string&
Socket::getAddressString() const
{
    return this->_addrstr;
}

// ----------------------------------------------------------------------

ipv4addr_t
Socket::getAddress() const
{
    return( (ipv4addr_t) _sock.sin_addr.s_addr );
}

// ----------------------------------------------------------------------
/**  set/get methods for avoiding calling close on a udp socket
 *   descriptor. This only gets set true by the protected constructor
 *   when creating a UDP construct of a 'server-client' object. Calling
 *   close on a server-client fd in UDP would result in closing on our
 *   server object as well. This is a special-case condition only needed
 *   when utilizing the SocketFactory with UDP sockets.
 */
void
Socket::setUdpNoClose ( bool noclose )
{
    this->_noUdpClose = noclose;
}

bool
Socket::getUdpNoClose() const
{
    return this->_noUdpClose;
}

// ----------------------------------------------------------------------

bool
Socket::isConnected()
{
#   ifdef WIN32
    
    if ( ! Socket::IsValidDescriptor(_fd) )
        _connected = false;

    return _connected;
    
#   else

    if ( !_connected || _proto == IPPROTO_UDP )
    	return _connected;

    pollfd  wset;
    char    serr[ERRORSTRLEN];

    wset.fd     = this->getDescriptor();
    wset.events = POLLOUT | POLLERR;

    if ( poll(&wset, 1, 0) < 0 ) {

	if ( errno == EINTR )
	    return true;

        if ( strerror_r(errno, serr, ERRORSTRLEN) == 0 )
            _errstr = serr;
	
	return false;
    }

    return true;
#   endif
}

// ----------------------------------------------------------------------

ssize_t
Socket::write ( const void * vptr, size_t n )
{
    socklen_t len = 0;
    ssize_t   st  = 0;

    if ( _proto == IPPROTO_UDP && ! _connected ) {
        len = sizeof(_sock);
        st  = ::sendto(_fd, (const char*) vptr, n, 0, (struct sockaddr*) &_sock, len);
    } else {
        st  = this->nwriten(vptr, n);
    }

    return st;
}

// ----------------------------------------------------------------------

ssize_t
Socket::readFrom ( void * vptr, size_t n, sockaddr_in & csock )
{
    socklen_t len = 0;
    ssize_t   rd  = 0;

    if ( _proto != IPPROTO_UDP )
        return -1;

    len = sizeof(struct sockaddr_in);
    rd  = ::recvfrom(_fd, (char*) vptr, n, 0, (struct sockaddr*) &csock, &len);

    if ( rd < 0 ) {
#   ifdef WIN32
        int  err  = WSAGetLastError();
        if ( err == WSAEINTR || err == WSAEWOULDBLOCK )
            rd = 0;
#   else
        if ( errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK )
            rd = 0;
#   endif
    }

    return rd;
}

// ----------------------------------------------------------------------

ssize_t
Socket::read ( void * vptr, size_t n )
{
    struct sockaddr_in csock;
    ssize_t   rd  = 0;

    if ( _proto == IPPROTO_UDP && ! _connected ) {
    	rd  = this->readFrom(vptr, n, csock);
    } else {
        rd = this->nreadn(vptr, n);
    }

    return rd;
}

// ----------------------------------------------------------------------

void
Socket::setBlocking()
{
    Socket::Block(this);
    _block = true;
}


void
Socket::setNonBlocking()
{
    Socket::Unblock(this);
    _block = false;
}


bool
Socket::isBlocking()
{
    return this->_block;
}

// ----------------------------------------------------------------------

SocketOption
Socket::getSocketOption ( SocketOption opt )
{
    SocketOption ropt;
    socklen_t    vlen;
    int          r, v;

    vlen = sizeof(v);

    r = ::getsockopt(_fd, opt.level(), opt.id(), &v, &vlen);

    if ( r == 0 )
        ropt = SocketOption(opt.level(), opt.id(), v, opt.name());

    return ropt;
}

// ----------------------------------------------------------------------

int
Socket::setSocketOption ( int level, int optname, int optval )
{
    char       serr[ERRORSTRLEN];
    socklen_t  len;

    len = (socklen_t) sizeof(int);

    if ( ! Socket::IsValidDescriptor(_fd) )
        return -1;

    if ( ::setsockopt(_fd, level, optname, (const void*) &optval, len) < 0 ) {

#       ifdef WIN32
        _errstr = "Socket: Error in call to setsockopt()";
#       else
        // could test for EOPNOTSUPP here
        if ( ::strerror_r(errno, serr, ERRORSTRLEN) == 0 )
            _errstr = serr;
#       endif

        return -1;
    }

    return 1;
}


int 
Socket::setSocketOption ( SocketOption opt )
{
    return this->setSocketOption(opt.level(),
                                 opt.id(),
                                 opt.value());
}

// ----------------------------------------------------------------------

const std::string&
Socket::getErrorString() const
{
    return _errstr;
}

// ----------------------------------------------------------------------

void
Socket::Unblock ( Socket * s )
{
    if ( s == NULL )
        return;

#   ifdef WIN32
    
    unsigned long  on = 1;
    ioctlsocket(s->getFD(), FIONBIO, &on);
    
#   else
    
    int flags = fcntl(s->getDescriptor(), F_GETFL, 0);
    fcntl(s->getDescriptor(), F_SETFL, flags | O_NONBLOCK);

#   endif

    return;
}


void
Socket::Block ( Socket * s ) 
{
    if ( s == NULL )
        return;

#   ifdef WIN32
    
    unsigned long  on = 0;
    ioctlsocket(s->getFD(), FIONBIO, &on);
    
#   else 
    
    int  flags  = fcntl(s->getDescriptor(), F_GETFL, 0);
    fcntl(s->getDescriptor(), F_SETFD, flags & ~O_NONBLOCK);

#   endif

    return;
}

// ----------------------------------------------------------------------

bool
Socket::IsValidDescriptor ( const sockfd_t & fd )
{
#   ifdef WIN32
    if ( fd <= 0 ) 
        return false;
#   else
    if ( fd < 0 ) 
        return false;
#   endif
    return true;
}
    
void
Socket::ResetDescriptor ( sockfd_t & fd )
{
#   ifdef WIN32
    fd = 0;
#   else
    fd = -1;
#   endif
}

// ----------------------------------------------------------------------

ssize_t
Socket::nwriten ( const void * vptr, size_t n )
{
    size_t      nleft;
    ssize_t     nwritten;
    const char* ptr = (const char*) vptr;

    nleft = n;

    while ( nleft > 0 ) {
        if ( (nwritten = ::send(_fd, ptr, nleft, 0)) <= 0 ) {

#           ifdef WIN32

	    int err = WSAGetLastError();
            if ( err == WSAEINTR )
                nwritten = 0;
            else if ( err == WSAEWOULDBLOCK )
                return(n-nleft);
            else
                return -1;
		
#         else

            if ( errno == EINTR )
                nwritten = 0;
            else if ( errno == EWOULDBLOCK || errno == EAGAIN || errno == EINPROGRESS )
                return(n-nleft);
            else
                return -1;
		
#         endif
        }
        nleft -= nwritten;
        ptr   += nwritten;
    }

    return n;
}

// ----------------------------------------------------------------------

ssize_t
Socket::nreadn ( void * vptr, size_t n )
{
    size_t nleft;
    int    nread;
    char*  ptr;
 
    ptr   = (char*) vptr;
    nleft = n;

    while ( nleft > 0 ) {
        if ( (nread = ::recv(_fd, ptr, nleft, 0)) < 0 ) {
	    
#       ifdef WIN32
        	
	    int err = WSAGetLastError();
            if ( err == WSAEINTR ) 
                nread = 0;
            else if ( err == WSAEWOULDBLOCK )
                return(n-nleft);
            else
                return -1;
		
#         else
            
            if ( errno == EINTR )
                nread = 0;
            else if ( errno == EWOULDBLOCK || errno == EAGAIN || errno == EINPROGRESS )  
                return(n-nleft);
            else
                return -1;
            
#         endif

        } else if ( nread == 0 ) {
            return -1;
        }
        nleft -= nread;
        ptr   += nread;
    }

    return(n-nleft);
    
}

// ----------------------------------------------------------------------

void
Socket::InitializeSocket ( sockfd_t & fd, int & proto ) throw ( SocketException )
{
    char   serr[ERRORSTRLEN];
    std::string errstr = "Socket::initSocket() Fatal Error";

    if ( proto == IPPROTO_TCP ) {
    	fd = socket(AF_INET, SOCK_STREAM, 0);
    } else if ( proto == IPPROTO_UDP ) {
    	fd = socket(AF_INET, SOCK_DGRAM, 0);
    } else {
        errstr.append(": Unsupported protocol");
        throw SocketException(errstr);
    }

    if ( ! Socket::IsValidDescriptor(fd) ) {

#       ifdef WIN32
    	errstr.append(": Failed to initialize socket");
#       else	
    	strerror_r(errno, serr, ERRORSTRLEN);
    	errstr.append(serr);
#     	endif

    	throw SocketException(errstr);
    }

    return;
}

// ----------------------------------------------------------------------

ipv4addr_t
Socket::pton ( const std::string & ipstr )
{
    ipv4addr_t addr = 0;

#   ifdef WIN32
    addr = (unsigned long) inet_addr(ipstr.c_str());
    if ( addr < 0 )
        addr = 0;
#   else
    ::inet_pton(AF_INET, ipstr.c_str(), &addr);
#   endif
    
    return addr;
}


std::string
Socket::ntop ( ipv4addr_t addr )
{
    char ip[INET_ADDRSTRLEN];

#   ifdef WIN32
    inaddr_t ipaddr;
    ipaddr.s_addr = addr;
    ::strncpy(ip, inet_ntoa(ipaddr), INET_ADDRSTRLEN);
#   else
    ::inet_ntop(AF_INET, &addr, ip, sizeof(ip));
#   endif

    return((std::string) ip);
}

// ----------------------------------------------------------------------

} // namespace

//  _TCANETPP_SOCKET_CPP_ 

