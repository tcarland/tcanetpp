/**
  * @file CmdBuffer.h
  *
  * @section LICENSE
  *
  * Copyright (c) 2001-2026 Timothy Charlton Arland <tcarland@gmail.com>
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
#ifndef _TCANETPP_CMDBUFFER_H_
#define _TCANETPP_CMDBUFFER_H_

#include <iostream>
#include <string>
#include <vector>

#if defined __GNUC__ && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
# include <ext/stdio_filebuf.h>
#endif

#define MINIMUM_CMDBUFFER_SIZE  16
#define MAXIMUM_CMDBUFFER_SIZE  2147483648
#define DEFAULT_CMDBUFFER_SIZE  1024


namespace tcanetpp {


typedef __gnu_cxx::stdio_filebuf<char>  StreamBuffer;
typedef std::vector<std::string>        StringBuffer;
typedef std::streamsize                 strmsz_t;


/**  Provides a simpler wrapper to the gnu_cxx:stdio_filebuf interface
  *  offering the buffering system file io such as stdout, for parsing.
 **/
class CmdBuffer {

  public:

    CmdBuffer ( size_t bufsize = DEFAULT_CMDBUFFER_SIZE );
    CmdBuffer ( const std::string & cmd, size_t bufsize = DEFAULT_CMDBUFFER_SIZE )
        noexcept(false);

    virtual ~CmdBuffer();


    bool         open              ( const std::string & cmd );
    int          close();

    bool         putLine           ( const std::string & line );
    std::string  getLine();
    void         getAllLines       ( StringBuffer      & lines );

    bool         isOpen()   const;
    bool         haveData() const;
    bool         hasData()  const  { return this->haveData(); }
    strmsz_t     inAvail();

    std::string  getCommand() const;
    std::string  getErrorStr() const;

    char         getEOL() const;
    void         setEOL ( char eol );

  private:

    StreamBuffer*              _cmdbuf;
    FILE*                      _file;
    std::streamsize            _bufsize;

    std::string                _cmd;
    std::string                _errstr;
    char                       _eol;
    bool                       _init;

    static const char*          EOL;
};

} // namespace

#endif // _TCANETPP_CMDBUFFER_H_
