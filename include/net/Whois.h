/**
  * @file Whois.h
  *
  * @section LICENSE
  *
  * Copyright (c) 2008-2018 Timothy Charlton Arland <tcarland@gmail.com>
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
#ifndef _TCANETPP_WHOIS_H_
#define _TCANETPP_WHOIS_H_

#include <string>

#include "Socket.h"


namespace tcanetpp  {

#define MAX_READS_IN_BLOCK 20
#define DEFAULT_WHOIS_PORT 43


class Whois {

  public:

    Whois ( const std::string & host = "",
            uint16_t            port = DEFAULT_WHOIS_PORT );

    virtual ~Whois();


    virtual std::string  query ( const std::string & query,
                                 const std::string & host = "",
                                 uint16_t            port = 0 );


    const std::string&   getErrorStr() const;

  protected:

    virtual void  init  ( const std::string & host,
                          uint16_t     port = 0 );
  private:

    Socket *            _sock;
    std::string         _errstr;
};

}  // namespace

#endif  // _TCANETPP_WHOIS_H_
