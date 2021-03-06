/**
  * @file Whois.cpp
  *
  * @section LICENSE
  *
  * Copyright (c) 2001-2018 Timothy Charlton Arland <tcarland@gmail.com>
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
#define _TCANETPP_WHOIS_CPP_

#include <stdlib.h>
#include <math.h>

#include "net/Whois.h"
#include "net/IpAddr.h"
#include "net/AddrInfo.h"
#include "util/StringUtils.h"


namespace tcanetpp {


Whois::Whois ( const std::string & host, uint16_t port )
    : _sock(NULL)
{
    this->init(host, port);
}


Whois::~Whois()
{
    if ( _sock ) {
        _sock->close();
        delete _sock;
    }
}


void
Whois::init ( const std::string & host, uint16_t port )
{
    if ( host.empty() )
        return;
    if ( port == 0 )
        port = DEFAULT_WHOIS_PORT;
    if ( _sock != NULL ) {
        _sock->close();
        delete _sock;
        _sock = NULL;
    }

    IpAddrList   addrs;
    AddrInfo::GetAddrList(host, addrs);

    if ( addrs.empty() ) {
        _errstr = "GetHostAddrList failed to resolve any addresses";
        return;
    }

    IpAddrList::iterator  hIter;
    for ( hIter = addrs.begin(); hIter != addrs.end(); ++hIter )
    {
        IpAddr & addr = *hIter;

        if ( ! addr.ipv4() )
            continue;

        if ( _sock )
            delete _sock;

        _sock = new Socket(addr.getAddr4(), port, SOCKTYPE_CLIENT, SOCKET_TCP);
        _sock->setBlocking();

        if ( _sock->connect() > 0 )
            break;

        _sock->close();
    }

    if ( ! _sock->isConnected() )  {
        delete _sock;
        _sock = NULL;
        _errstr = "Connection attempts failed to host: ";
        _errstr.append(host);
    }

    return;
}


std::string
Whois::query ( const std::string & query, const std::string & host, uint16_t port )
{
    std::string  str, result;
    ssize_t      rr   = 0;
    long         x, y = 10;
    char         ch;
    ldiv_t       ld;

    if ( ! host.empty() )
        this->init(host, port);

    if ( _sock == NULL ) { // throw or error
        _errstr = "Failed to init socket";
        return result;
    }

    str = query;
    str.append("\r\n");

    if ( ! _sock->isBlocking() )
        _sock->setBlocking();

    rr = _sock->write(str.data(), str.length());

    if ( rr < 0 )
        return result;

    _sock->setNonBlocking();
    sleep(1);

    x = 0;
    while ( (rr = _sock->read(&ch, sizeof(ch))) >= 0 )
    {
        if ( rr == 0 )
        {
            x++;
            ld = ldiv(x, y);
            if ( x >= MAX_READS_IN_BLOCK )
                break;
            else if ( ld.rem == 0 && result.empty() )
                sleep(1);
            continue;
        }
        x = 0;
        result += ch;
    }

    return result;
}


const std::string&
Whois::getErrorStr() const
{
    return _errstr;
}

}  // namespace


#ifdef TWHOIS_MAIN

#include <iostream>

void usage()
{
    std::cout << "Usage: whois -h host querystring" << std::endl;
}

int main ( int argc, char ** argv )
{
    std::string  host, query;
    char *       hoststr = NULL;
    char         optChar;

    while ( (optChar = getopt(argc, argv, "h:")) != EOF )
    {
        switch ( optChar ) {
            case 'h':
                hoststr = strdup(optarg);
                break;
            default:
                break;
        }
    }

    if ( argc > 3 )
    {
        int ac = 3;
        query.append(argv[ac]);
        ac++;
        while ( ac < argc ) {
            query.append(" ").append(argv[ac]);
            ac++;
        }
    }

    if ( hoststr != NULL && strlen(hoststr) > 0 ) {
        host = hoststr;
        free(hoststr);
    } else
        usage();

    std::cout << " host  = " << host << std::endl;
    std::cout << " query = " << query << std::endl << std::endl;

    tcanetpp::Whois  who(host);

    std::string r = who.query(query);
    std::cout << " result = '" << r << "'" << std::endl;

    return 0;
}
#endif  // TWHOIS_MAIN

// _TCANETPP_WHOIS_CPP_
