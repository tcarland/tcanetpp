/**
  * @file SecureSocket.h
  *
  * The SecureSocket class provides TLS/SSL encryption on top of the 
  * Socket class for secure network communication.
  *
  * @section LICENSE
  *
  * Copyright (c) 2002-2026 Timothy Charlton Arland <tcarland@gmail.com>
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
#include <openssl/x509v3.h>

#include "Socket.h"


namespace tcanetpp {


/**  SecureSocketException class for fatal SecureSocket errors */
class SecureSocketException : public SocketException {
  public:
    SecureSocketException ( const std::string & errstr )
        : SocketException(errstr) {}
    virtual ~SecureSocketException() {}
};


/**  The SecureSocket class extends Socket to provide TLS/SSL encrypted
  *  communication. It supports both client and server modes with 
  *  certificate-based authentication and optional hostname verification.
 **/
class SecureSocket : public Socket {

  public:

    /**  Factory class for creating SecureSocket instances from accept() */
    class SecureSocketFactory : public Socket::SocketFactory {
        SecureSocket  * _server;
      public:
        explicit SecureSocketFactory ( SecureSocket * ss ) 
            : _server(ss) 
        {}
        virtual ~SecureSocketFactory() {}

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

    /**  Protected constructor used by SecureSocketFactory for accepted connections */
    SecureSocket ( SSL        * ssl,
                   SSL_CTX    * ctx,
                   sockfd_t   & fd,
                   sockaddr_t & csock,
                   SocketType   type,
                   int          proto );


  public:

    /* Overridden methods from Socket */

    virtual int         init ( bool block = false );

    virtual int         connect();
    virtual void        close();
    virtual void        shutdown ( int shut );
    virtual bool        isConnected();

    virtual ssize_t     read     ( void       * vptr, size_t n );
    virtual ssize_t     write    ( const void * vptr, size_t n );


    /* TLS-specific methods */

    SSL_CTX*            getSSLContext();

    SSL*                getSSL();

    void                setCertificateFile ( const std::string & cert );
    void                setPrivateKeyFile  ( const std::string & key );
    void                setTrustFile       ( const std::string & trust );

    void                setHostname        ( const std::string & hostname );

    void                setVerifyPeer      ( bool verify );
    bool                getVerifyPeer() const;

    X509*               getPeerCertificate();
    long                getVerifyResult() const;
    const char*         getProtocolVersion() const;
    const char*         getCipherName() const;


    static void         InitSSL();
    static void         CleanupSSL();
    static std::string  GetSSLErrorString();

  protected:

    int                 initSSLContext();
    int                 sslHandshake();

  protected:

    SSL_CTX*     _ctx;
    SSL*         _ssl;
    bool         _ctxOwned;      // true if this socket owns the SSL_CTX
    bool         _verifyPeer;
    bool         _sslConnected;

    std::string  _certfile;
    std::string  _keyfile;
    std::string  _cafile;
    std::string  _hostname;

    static bool  _sslInitialized;
};

} // namespace


#endif  // _TCANETPP_SECURESOCKET_H_