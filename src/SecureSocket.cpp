#define _TCANETPP_SECURESOCKET_CPP_

#include "net/SecureSocket.h"


namespace tcanetpp {

// ----------------------------------------------------------------------

Socket*
SecureSocket::SecureSocketFactory::operator() ( sockfd_t         & fd,
                                                sockaddr_storage & csock, 
                                                SocketType         type,
                                                int                proto )
{
    if ( this->server == NULL )
        return NULL;

    SSL_CTX* ctx   = this->server->getSSLContext();
    SSL*     ssl   = SSL_new(ctx);
    SSL_set_fd(ssl, fd);

    if ( SSL_accept(ssl) <= 0 )
        return NULL;

    return ( (Socket*) new SecureSocket(ssl, fd, csock, type, proto) );
}

// ----------------------------------------------------------------------

SecureSocket::SecureSocket()
    : Socket(),
      _ctx(NULL),
      _ssl(NULL)
{}


/**  IPV4 constructor for creating a client or server socket utilizing an IPV4
  *  address and port.
 **/
SecureSocket::SecureSocket ( ipv4addr_t ipaddr, 
                             uint16_t   port, 
                             SocketType type, 
                             int        proto )
    : Socket(ipaddr, port, type, proto),
      _ctx(NULL),
      _ssl(NULL)
{
}

SecureSocket::SecureSocket ( ipv6addr_t ipaddr, 
                             uint16_t   port, 
                             SocketType type, 
                             int        proto )
    : Socket(ipaddr, port, type, proto),
      _ctx(NULL),
      _ssl(NULL)
{
}


SecureSocket::SecureSocket ( sockaddr_t * sa, 
                             uint16_t     port, 
                             SocketType   type, 
                             int          proto )
    : Socket(sa, port, type, proto),
      _ctx(NULL),
      _ssl(NULL)
{
}


SecureSocket::SecureSocket ( addrinfo * ai )
    : Socket(ai),
      _ctx(NULL),
      _ssl(NULL)
{
}

SecureSocket::SecureSocket ( SSL        * ssl,
                             sockfd_t   & fd, 
                             sockaddr_t & csock, 
                             SocketType   type, 
                             int          proto )
    : Socket(fd, csock, type, proto),
      _ctx(NULL),
      _ssl(ssl)
{
}


SecureSocket::~SecureSocket()
{
    this->close();
    if ( _ctx ) {
        SSL_CTX_free(_ctx);
        EVP_cleanup();
    }
}

// ----------------------------------------------------------------------

int
SecureSocket::init ( bool block )
{
    const SSL_METHOD * method;

    int i = Socket::init(block);

    if ( _socktype != SOCKTYPE_SERVER || i < 0 )
        return i;

    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();

    method = SSLv23_server_method();
    _ctx   = SSL_CTX_new(method);

    if ( ! _ctx ) {
        _errstr = "Failed to create SSL context";
        i = -1;
    }

    SSL_CTX_set_ecdh_auto(_ctx, 1);

    if ( SSL_CTX_use_certificate_file(_ctx, _certfile.c_str(), SSL_FILETYPE_PEM) <= 0 ) 
    {
        _errstr = "Error in Certificate file";
        return -1;
    }
    if ( SSL_CTX_use_PrivateKey_file(_ctx, _keyfile.c_str(), SSL_FILETYPE_PEM) <= 0 )
    {
        _errstr = "Error in Private Key file";
        return -1;
    }

    return i;
}

// ----------------------------------------------------------------------

int
SecureSocket::connect()
{
    return 1;
}

// ----------------------------------------------------------------------

void
SecureSocket::close()
{
    Socket::close();
    if ( _ssl )
        SSL_free(_ssl);
    _ssl = NULL;
    return;
}

// ----------------------------------------------------------------------


ssize_t
SecureSocket::read ( void * vptr, size_t n )
{
    ssize_t  rt = 0;

    rt = SSL_read(_ssl, vptr, n);
    
    return rt;
}

ssize_t
SecureSocket::write ( const void * vptr, size_t n )
{
    ssize_t   st  = 0;

    st = SSL_write(_ssl, vptr, n);

    return st;
}


} // namespace


// _TCANETPP_SECURESOCKET_CPP_
