/**
  * @file BufferedSocket.h
  *
  * @section LICENSE
  *
  * Copyright (c) 2002-2018 Timothy Charlton Arland <tcarland@gmail.com>
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
#ifndef _TCANETPP_SECURESOCKET_H_
#define _TCANETPP_SECURESOCKET_H_

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "Socket.h"
#include "CircularBuffer.h"


namespace tcanetpp {


class SecureSocket : public Socket {
  public:

    class SecureSocketFactory : public Socket::SocketFactory {
        SecureSocket  * server;
      public:
        explicit SecureSocketFactory ( SecureSocket * ss ) 
            : server(ss) 
        {}
        virtual ~SecureSocketFactory();

        virtual Socket* operator() ( sockfd_t         & fd, 
                                     sockaddr_storage & csock,
                                     SocketType         type,
                                     int                proto );
    };

  public:
    
    SecureSocket();


    SecureSocket ( ipv4addr_t   ip,
                   uint16_t     port,
                   SocketType   socktype,
                   int          proto );

    SecureSocket ( ipv6addr_t   ip,
                   uint16_t     port,
                   SocketType   socktype,
                   int          proto );

    SecureSocket ( sockaddr_t * sa,
                   uint16_t     port,
                   SocketType   type,
                   int          protocol );

    SecureSocket ( addrinfo   * ai );

    virtual ~SecureSocket();


  protected:

    SecureSocket ( SSL        * ssl,
                   sockfd_t   & fd,
                   sockaddr_t & csock,
                   SocketType   type,
                   int          proto );

  public:

  /*  from Socket */
    virtual int         init ( bool block = false );

    virtual int         connect();
    virtual void        close();
    virtual void        shutdown ( int shut );
    virtual bool        isConnected();

    virtual ssize_t     read     ( void       * vptr, size_t n );
    virtual ssize_t     write    ( const void * vptr, size_t n );
 

    SSL_CTX*            getSSLContext();


    void                setCertificateFile ( const std::string & cert );
    void                setPrivateKeyFile  ( const std::string & key );
    void                setTrustFile       ( const std::string & trust );


  protected:

    SSL_CTX*    _ctx;
    SSL*        _ssl;

    std::string  _certfile;
    std::string  _keyfile;
    std::string  _cafile;
};

} // namespace



#endif  // _TCANETPP_SECURESOCKET_H_