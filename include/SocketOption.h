/**
  *   SocketOption.h
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
#ifndef _TCANETPP_SOCKETOPTION_H_
#define _TCANETPP_SOCKETOPTION_H_

#include <string>


namespace tcanetpp {


class SocketOption {
	
public:
    
    SocketOption();
    SocketOption ( int optname, int optval, const std::string & name = "" );
    
    virtual ~SocketOption() {}
    
    
    int   getOptionId() const;
    int   getOptionValue() const;

    const std::string&  getOptionName() const;
    const std::string&  getOptionNameString() const  { return this->getOptionName(); }
    const std::string&  toString() const { return this->getOptionName(); }
    

    /*  static Factory Methods  */
            
    static SocketOption  SetReuseAddr  ( int val );
    static SocketOption  SetLinger     ( int val );
    static SocketOption  SetKeepalive  ( int val );
    static SocketOption  SetRcvBuf     ( int val );
    static SocketOption  SetSndBuf     ( int val );
    static SocketOption  SetRcvLoWat   ( int val );
    static SocketOption  SetSndLoWat   ( int val );
    static SocketOption  SetRcvTimeout ( int val );
    static SocketOption  SetSndTimeout ( int val );
    
private:
	
    int             _optid;
    int             _optval;
    std::string     _namestr;
	
};

} // namespace

#endif  // _TCANETPP_SOCKETOPTION_H_ 

