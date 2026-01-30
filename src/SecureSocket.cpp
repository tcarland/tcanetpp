/**
  * @file SecureSocket.cpp
  *
  * Implementation of the SecureSocket class providing TLS/SSL encryption
  * on top of the Socket class.
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
#define _TCANETPP_SECURESOCKET_CPP_

#include "net/SecureSocket.h"

#include <cstring>

#ifndef WIN32
# include <poll.h>
# include <errno.h>
#endif


namespace tcanetpp {


// Static member initialization
bool SecureSocket::_sslInitialized = false;


// ----------------------------------------------------------------------

Socket*
SecureSocket::SecureSocketFactory::operator() ( sockfd_t         & fd,
                                                sockaddr_storage & csock, 
                                                SocketType         type,
                                                int                proto )
{
    if ( _server == nullptr )
        return nullptr;

    SSL_CTX * ctx = _server->getSSLContext();
    if ( ctx == nullptr )
        return nullptr;

    SSL * ssl = SSL_new(ctx);
    if ( ssl == nullptr )
        return nullptr;

    SSL_set_fd(ssl, fd);

    // Perform TLS handshake for incoming connection
    int result = SSL_accept(ssl);
    if ( result <= 0 ) 
    {
        int sslerr = SSL_get_error(ssl, result);
        (void)sslerr;  // Could log this error
        SSL_free(ssl);
        return nullptr;
    }

    // Create new SecureSocket for the accepted connection
    // Note: ctx is NOT owned by the client socket (shared with server)
    return static_cast<Socket*>(new SecureSocket(ssl, ctx, fd, csock, type, proto));
}


// ----------------------------------------------------------------------

SecureSocket::SecureSocket()
    : Socket(),
      _ctx(nullptr),
      _ssl(nullptr),
      _ctxOwned(true),
      _verifyPeer(true),
      _sslConnected(false)
{
    SecureSocket::InitSSL();
}


SecureSocket::SecureSocket ( ipv4addr_t ipaddr, 
                             uint16_t   port, 
                             SocketType type, 
                             int        proto )
    : Socket(ipaddr, port, type, proto),
      _ctx(nullptr),
      _ssl(nullptr),
      _ctxOwned(true),
      _verifyPeer(true),
      _sslConnected(false)
{
    SecureSocket::InitSSL();
}


SecureSocket::SecureSocket ( ipv6addr_t ipaddr, 
                             uint16_t   port, 
                             SocketType type, 
                             int        proto )
    : Socket(ipaddr, port, type, proto),
      _ctx(nullptr),
      _ssl(nullptr),
      _ctxOwned(true),
      _verifyPeer(true),
      _sslConnected(false)
{
    SecureSocket::InitSSL();
}


SecureSocket::SecureSocket ( sockaddr_t * sa, 
                             uint16_t     port, 
                             SocketType   type, 
                             int          proto )
    : Socket(sa, port, type, proto),
      _ctx(nullptr),
      _ssl(nullptr),
      _ctxOwned(true),
      _verifyPeer(true),
      _sslConnected(false)
{
    SecureSocket::InitSSL();
}


SecureSocket::SecureSocket ( addrinfo * ai )
    : Socket(ai),
      _ctx(nullptr),
      _ssl(nullptr),
      _ctxOwned(true),
      _verifyPeer(true),
      _sslConnected(false)
{
    SecureSocket::InitSSL();
}


SecureSocket::SecureSocket ( SSL        * ssl,
                             SSL_CTX    * ctx,
                             sockfd_t   & fd, 
                             sockaddr_t & csock, 
                             SocketType   type, 
                             int          proto )
    : Socket(fd, csock, type, proto),
      _ctx(ctx),
      _ssl(ssl),
      _ctxOwned(false),   // Context is owned by server socket
      _verifyPeer(true),
      _sslConnected(true)
{
    // SSL connection already established via SSL_accept in factory
}


SecureSocket::~SecureSocket()
{
    this->close();
}


// ----------------------------------------------------------------------


/**  Static method to initialize OpenSSL library (called automatically) */
void
SecureSocket::InitSSL()
{
    if ( ! _sslInitialized )
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        SSL_library_init();
        SSL_load_error_strings();
        OpenSSL_add_all_algorithms();
#else
        // OpenSSL 1.1.0+ automatically initializes
        OPENSSL_init_ssl(OPENSSL_INIT_LOAD_SSL_STRINGS | 
                         OPENSSL_INIT_LOAD_CRYPTO_STRINGS, nullptr);
#endif
        _sslInitialized = true;
    }
}


/**  Static method to cleanup OpenSSL library */
void
SecureSocket::CleanupSSL()
{
    if ( _sslInitialized )
    {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        EVP_cleanup();
        ERR_free_strings();
#endif
        // OpenSSL 1.1.0+ handles cleanup automatically
        _sslInitialized = false;
    }
}


/**  Static helper to retrieve OpenSSL error string */
std::string
SecureSocket::GetSSLErrorString()
{
    unsigned long err = ERR_get_error();
    if ( err == 0 )
        return std::string();
    
    char buf[256];
    ERR_error_string_n(err, buf, sizeof(buf));
    return std::string(buf);
}


// ----------------------------------------------------------------------

int
SecureSocket::init ( bool block )
{
    // First initialize the underlying socket
    int result = Socket::init(block);
    if ( result < 0 )
        return result;

    // Initialize SSL context
    if ( initSSLContext() < 0 )
        return -1;

    return 1;
}


/**  Creates and configures the SSL_CTX based on socket type */
int
SecureSocket::initSSLContext()
{
    const SSL_METHOD * method = nullptr;

    // Select appropriate SSL method based on socket type
    if ( _socktype == SOCKTYPE_SERVER || _socktype == SOCKTYPE_SERVER_CLIENT )
    {
        // Server mode
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        method = TLSv1_2_server_method();
#else
        method = TLS_server_method();
#endif
    }
    else
    {
        // Client mode
#if OPENSSL_VERSION_NUMBER < 0x10100000L
        method = TLSv1_2_client_method();
#else
        method = TLS_client_method();
#endif
    }

    _ctx = SSL_CTX_new(method);
    if ( _ctx == nullptr )
    {
        _errstr = "SecureSocket::initSSLContext() Failed to create SSL context: ";
        _errstr.append(GetSSLErrorString());
        return -1;
    }

    // Set minimum TLS version to 1.2 for security
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    SSL_CTX_set_min_proto_version(_ctx, TLS1_2_VERSION);
#endif

    // Configure cipher suites (prefer strong ciphers)
    SSL_CTX_set_cipher_list(_ctx, "HIGH:!aNULL:!MD5:!RC4");

#if OPENSSL_VERSION_NUMBER >= 0x10002000L
    // Enable automatic ECDH curve selection
    SSL_CTX_set_ecdh_auto(_ctx, 1);
#endif

    // Server-specific configuration
    if ( _socktype == SOCKTYPE_SERVER )
    {
        // Load server certificate
        if ( ! _certfile.empty() )
        {
            if ( SSL_CTX_use_certificate_file(_ctx, _certfile.c_str(), 
                                              SSL_FILETYPE_PEM) <= 0 )
            {
                _errstr = "SecureSocket::initSSLContext() Error loading certificate file: ";
                _errstr.append(GetSSLErrorString());
                return -1;
            }
        }

        // Load private key
        if ( ! _keyfile.empty() )
        {
            if ( SSL_CTX_use_PrivateKey_file(_ctx, _keyfile.c_str(), 
                                             SSL_FILETYPE_PEM) <= 0 )
            {
                _errstr = "SecureSocket::initSSLContext() Error loading private key file: ";
                _errstr.append(GetSSLErrorString());
                return -1;
            }

            // Verify private key matches certificate
            if ( ! SSL_CTX_check_private_key(_ctx) )
            {
                _errstr = "SecureSocket::initSSLContext() Private key does not match certificate";
                return -1;
            }
        }

        // Load CA certificates for client verification (optional)
        if ( ! _cafile.empty() )
        {
            if ( SSL_CTX_load_verify_locations(_ctx, _cafile.c_str(), nullptr) <= 0 )
            {
                _errstr = "SecureSocket::initSSLContext() Error loading CA file: ";
                _errstr.append(GetSSLErrorString());
                return -1;
            }
            SSL_CTX_set_verify(_ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, 
                               nullptr);
        }
    }
    else // Client mode
    {
        // Set up peer verification
        if ( _verifyPeer )
        {
            SSL_CTX_set_verify(_ctx, SSL_VERIFY_PEER, nullptr);

            // Load CA trust store
            if ( ! _cafile.empty() )
            {
                if ( SSL_CTX_load_verify_locations(_ctx, _cafile.c_str(), nullptr) <= 0 )
                {
                    _errstr = "SecureSocket::initSSLContext() Error loading CA file: ";
                    _errstr.append(GetSSLErrorString());
                    return -1;
                }
            }
            else
            {
                // Use default system CA store
                if ( SSL_CTX_set_default_verify_paths(_ctx) <= 0 )
                {
                    _errstr = "SecureSocket::initSSLContext() Error setting default verify paths";
                    return -1;
                }
            }
        }
        else
        {
            SSL_CTX_set_verify(_ctx, SSL_VERIFY_NONE, nullptr);
        }

        // Load client certificate if provided (for mutual TLS)
        if ( ! _certfile.empty() )
        {
            if ( SSL_CTX_use_certificate_file(_ctx, _certfile.c_str(), 
                                              SSL_FILETYPE_PEM) <= 0 )
            {
                _errstr = "SecureSocket::initSSLContext() Error loading client certificate: ";
                _errstr.append(GetSSLErrorString());
                return -1;
            }
        }

        if ( ! _keyfile.empty() )
        {
            if ( SSL_CTX_use_PrivateKey_file(_ctx, _keyfile.c_str(), 
                                             SSL_FILETYPE_PEM) <= 0 )
            {
                _errstr = "SecureSocket::initSSLContext() Error loading client private key: ";
                _errstr.append(GetSSLErrorString());
                return -1;
            }
        }
    }

    return 1;
}


// ----------------------------------------------------------------------


/** connect() - Establish TCP connection and perform TLS handshake */
int
SecureSocket::connect()
{
    if ( _socktype != SOCKTYPE_CLIENT )
        return -1;

    if ( _sslConnected )
        return 1;

    // First establish TCP connection using base class
    if ( ! _connected )
    {
        int result = Socket::connect();
        if ( result < 0 )
            return result;
        if ( result == 0 )
            return 0;  // Connection in progress (non-blocking)
    }

    // TCP is connected, now perform TLS handshake
    return sslHandshake();
}


/**  Performs the TLS handshake (connect for client, accept for server) */
int
SecureSocket::sslHandshake()
{
    if ( _ssl == nullptr )
    {
        // Create SSL object
        _ssl = SSL_new(_ctx);
        if ( _ssl == nullptr )
        {
            _errstr = "SecureSocket::sslHandshake() Failed to create SSL object: ";
            _errstr.append(GetSSLErrorString());
            return -1;
        }

        // Attach SSL to socket descriptor
        if ( SSL_set_fd(_ssl, _fd) == 0 )
        {
            _errstr = "SecureSocket::sslHandshake() Failed to set SSL fd";
            SSL_free(_ssl);
            _ssl = nullptr;
            return -1;
        }

        // Set SNI hostname for client connections
        if ( _socktype == SOCKTYPE_CLIENT && ! _hostname.empty() )
        {
            SSL_set_tlsext_host_name(_ssl, _hostname.c_str());
            
#if OPENSSL_VERSION_NUMBER >= 0x10002000L
            // Set hostname for certificate verification
            if ( _verifyPeer )
            {
                X509_VERIFY_PARAM * param = SSL_get0_param(_ssl);
                X509_VERIFY_PARAM_set_hostflags(param, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
                X509_VERIFY_PARAM_set1_host(param, _hostname.c_str(), _hostname.length());
            }
#endif
        }
    }

    int result;
    if ( _socktype == SOCKTYPE_CLIENT )
        result = SSL_connect(_ssl);
    else
        result = SSL_accept(_ssl);

    if ( result <= 0 )
    {
        int sslerr = SSL_get_error(_ssl, result);

        switch ( sslerr )
        {
            case SSL_ERROR_WANT_READ:
            case SSL_ERROR_WANT_WRITE:
                // Non-blocking, try again later
                return 0;

            case SSL_ERROR_ZERO_RETURN:
                _errstr = "SecureSocket::sslHandshake() Connection closed during handshake";
                break;

            case SSL_ERROR_SYSCALL:
                if ( errno == EINPROGRESS || errno == EAGAIN || errno == EWOULDBLOCK )
                    return 0;
                _errstr = "SecureSocket::sslHandshake() System error: ";
                _errstr.append(std::to_string(errno));
                break;

            case SSL_ERROR_SSL:
                _errstr = "SecureSocket::sslHandshake() SSL error: ";
                _errstr.append(GetSSLErrorString());
                break;

            default:
                _errstr = "SecureSocket::sslHandshake() Unknown error";
                break;
        }

        return -1;
    }

    // Verify peer certificate if required
    if ( _verifyPeer && _socktype == SOCKTYPE_CLIENT )
    {
        long verifyResult = SSL_get_verify_result(_ssl);
        if ( verifyResult != X509_V_OK ) {
            _errstr = "SecureSocket::sslHandshake() Certificate verification failed: ";
            _errstr.append(X509_verify_cert_error_string(verifyResult));
            return -1;
        }

        // Verify peer certificate exists
        X509 * cert = SSL_get_peer_certificate(_ssl);
        if ( cert == nullptr ) {
            _errstr = "SecureSocket::sslHandshake() No peer certificate";
            return -1;
        }
        X509_free(cert);
    }

    _sslConnected = true;
    return 1;
}


// ----------------------------------------------------------------------


/**  close() - Close SSL connection and underlying socket  */
void
SecureSocket::close()
{
    if ( _ssl != nullptr )
    {
        if ( _sslConnected ) {
            // Perform clean SSL shutdown
            int result = SSL_shutdown(_ssl);
            if ( result == 0 )  // Need to call again for bidirectional shutdown
                SSL_shutdown(_ssl);
        }
        SSL_free(_ssl);
        _ssl = nullptr;
    }

    _sslConnected = false;

    if ( _ctx != nullptr && _ctxOwned ) {
        SSL_CTX_free(_ctx);
        _ctx = nullptr;
    }

    Socket::close();
}


// ----------------------------------------------------------------------


/*  shutdown() - Shutdown SSL and/or socket. */
void
SecureSocket::shutdown ( int shut )
{
    if ( _ssl != nullptr && _sslConnected )
    {
        // SSL_shutdown handles bidirectional shutdown
        if ( shut >= 2 )
        {
            int result = SSL_shutdown(_ssl);
            if ( result == 0 )
                SSL_shutdown(_ssl);
            _sslConnected = false;
        }
    }

    // Also call base class shutdown
    Socket::shutdown(shut);
}


// ----------------------------------------------------------------------


/*  isConnected() - Check if SSL connection is established.  */
bool
SecureSocket::isConnected()
{
    if ( ! _sslConnected )
        return false;

    // Also check underlying socket
    return Socket::isConnected();
}


// ----------------------------------------------------------------------


/*  read() - Read data through SSL connection.  */
ssize_t
SecureSocket::read ( void * vptr, size_t n )
{
    if ( _ssl == nullptr || ! _sslConnected ) {
        _errstr = "SecureSocket::read() SSL not connected";
        return -1;
    }

    int result = SSL_read(_ssl, vptr, static_cast<int>(n));

    if ( result <= 0 )
    {
        int sslerr = SSL_get_error(_ssl, result);

        switch ( sslerr )
        {
            case SSL_ERROR_WANT_READ:
            case SSL_ERROR_WANT_WRITE:
                // Would block - return 0 for non-blocking sockets
                return 0;

            case SSL_ERROR_ZERO_RETURN:
                // Clean shutdown from peer
                _sslConnected = false;
                return -1;

            case SSL_ERROR_SYSCALL:
                if ( errno == 0 ) {
                    // EOF
                    _sslConnected = false;
                    return -1;
                }
                if ( errno == EAGAIN || errno == EWOULDBLOCK )
                    return 0;
                _errstr = "SecureSocket::read() System error";
                return -1;

            case SSL_ERROR_SSL:
                _errstr = "SecureSocket::read() SSL error: ";
                _errstr.append(GetSSLErrorString());
                return -1;

            default:
                return -1;
        }
    }

    return static_cast<ssize_t>(result);
}


// ----------------------------------------------------------------------


/*  write() - Write data through SSL connection.  */
ssize_t
SecureSocket::write ( const void * vptr, size_t n )
{
    if ( _ssl == nullptr || ! _sslConnected ) {
        _errstr = "SecureSocket::write() SSL not connected";
        return -1;
    }

    int result = SSL_write(_ssl, vptr, static_cast<int>(n));

    if ( result <= 0 )
    {
        int sslerr = SSL_get_error(_ssl, result);

        switch ( sslerr )
        {
            case SSL_ERROR_WANT_READ:
            case SSL_ERROR_WANT_WRITE:
                // Would block - return 0 for non-blocking sockets
                return 0;

            case SSL_ERROR_ZERO_RETURN:
                _sslConnected = false;
                return -1;

            case SSL_ERROR_SYSCALL:
                if ( errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS )
                    return 0;
                _errstr = "SecureSocket::write() System error";
                return -1;

            case SSL_ERROR_SSL:
                _errstr = "SecureSocket::write() SSL error: ";
                _errstr.append(GetSSLErrorString());
                return -1;

            default:
                return -1;
        }
    }

    return static_cast<ssize_t>(result);
}


// ----------------------------------------------------------------------


/**  Returns the SSL context (for server sockets, this is shared with clients) */
SSL_CTX*
SecureSocket::getSSLContext()
{
    return _ctx;
}


/**  Returns the SSL connection object */
SSL*
SecureSocket::getSSL()
{
    return _ssl;
}


/**  Sets the certificate file path (PEM format) for server or client auth */
void
SecureSocket::setCertificateFile ( const std::string & cert )
{
    _certfile = cert;
}


/**  Sets the private key file path (PEM format) */
void
SecureSocket::setPrivateKeyFile ( const std::string & key )
{
    _keyfile = key;
}


/**  Sets the CA trust store file (PEM format) for peer verification */
void
SecureSocket::setTrustFile ( const std::string & trust )
{
    _cafile = trust;
}


/**  Sets the hostname for SNI and certificate verification (client mode) */
void
SecureSocket::setHostname ( const std::string & hostname )
{
    _hostname = hostname;
}


/**  Enables or disables peer certificate verification (default: enabled) */
void
SecureSocket::setVerifyPeer ( bool verify )
{
    _verifyPeer = verify;
}


/**  Returns whether peer verification is enabled */
bool
SecureSocket::getVerifyPeer() const
{
    return _verifyPeer;
}


/**  Returns the peer certificate (if available) - caller must NOT free */
X509*
SecureSocket::getPeerCertificate()
{
    if ( _ssl == nullptr )
        return nullptr;
    return SSL_get_peer_certificate(_ssl);
}


/**  Returns the result of peer certificate verification */
long
SecureSocket::getVerifyResult() const
{
    if ( _ssl == nullptr )
        return X509_V_ERR_APPLICATION_VERIFICATION;
    return SSL_get_verify_result(_ssl);
}


/**  Returns the negotiated TLS protocol version string */
const char*
SecureSocket::getProtocolVersion() const
{
    if ( _ssl == nullptr )
        return nullptr;
    return SSL_get_version(_ssl);
}


/**  Returns the negotiated cipher suite name */
const char*
SecureSocket::getCipherName() const
{
    if ( _ssl == nullptr )
        return nullptr;
    return SSL_get_cipher_name(_ssl);
}


} // namespace


// _TCANETPP_SECURESOCKET_CPP_
