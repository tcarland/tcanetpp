/**
  * @file CmdBuffer.cpp
  *
  * @section LICENSE
  *
  * Copyright (c) 2001-2024 Timothy Charlton Arland <tcarland@gmail.com>
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
#define _TCANETPP_CMDBUFFER_CPP_

#include "util/CmdBuffer.h"
#include "Exception.hpp"


namespace tcanetpp {


const char*  CmdBuffer::EOL = "\n";


CmdBuffer::CmdBuffer ( size_t bufsize )
    : _cmdbuf(NULL),
      _file(NULL),
      _bufsize(bufsize),
      _eol(*CmdBuffer::EOL),
      _init(false)
{
    if ( (bufsize < MINIMUM_CMDBUFFER_SIZE) || (bufsize > MAXIMUM_CMDBUFFER_SIZE) )
        throw ( Exception("CmdBuffer error, buffersize out of bounds") );
}


CmdBuffer::CmdBuffer ( const std::string & cmd, size_t bufsize )
    : _cmdbuf(NULL),
      _file(NULL),
      _bufsize(bufsize),
      _cmd(cmd),
      _eol(*CmdBuffer::EOL),
      _init(false)
{
    if ( (bufsize < MINIMUM_CMDBUFFER_SIZE) || (bufsize > MAXIMUM_CMDBUFFER_SIZE) )
        throw ( Exception("CmdBuffer error, buffersize out of bounds") );
    if ( ! this->open(cmd) )
        throw ( Exception(this->_errstr) );
}


CmdBuffer::~CmdBuffer()
{
    if ( _init )
        this->close();
    if ( _cmdbuf )
        delete _cmdbuf;
}


bool
CmdBuffer::open ( const std::string & cmd )
{
    FILE * f = NULL;

    if ( _init )
        this->close();

    if ( (f = ::popen(cmd.c_str(), "r")) == NULL ) {
        _errstr = "CmdBuffer::Open() error in popen, failed to open pipe stream";
        return false;
    }

    _cmdbuf = new StreamBuffer(f, std::ios_base::in, _bufsize);

    if ( ! _cmdbuf->is_open() ) {
        _errstr = "CmdBuffer::Open() failed.";
        delete _cmdbuf;
    } else {
        _init = true;
        _file = f;
    }

    return _init;
}


void
CmdBuffer::close()
{
    if ( _init && _cmdbuf ) {
        _cmdbuf->close();
        ::pclose(_file);
        delete _cmdbuf;
        _cmdbuf = NULL;
    }

    _init = false;

    return;
}


bool
CmdBuffer::isOpen() const
{
    if ( _init )
        return _cmdbuf->is_open();

    return false;
}


bool
CmdBuffer::haveData() const
{
    if ( ! _init )
        return false;

    if ( _cmdbuf->sgetc() == EOF )
        return false;

    return true;
}


strmsz_t
CmdBuffer::inAvail()
{
    if ( ! _init )
    	return 0;

    return _cmdbuf->in_avail();
}


std::string
CmdBuffer::getLine()
{
    char        ch;
    std::string line = "";

    while ( _cmdbuf->sgetc() != EOF && (ch = _cmdbuf->sbumpc()) != _eol )
        line += ch;

    return line;
}


void
CmdBuffer::getAllLines ( StringBuffer & lines )
{
    std::string line = "";

    //lines = std::vector<std::string>();
    lines.clear();

    if ( ! _init )
        return;

    while ( this->haveData() )
    {
        line = this->getLine();
        lines.push_back(line);
    }

    return;
}


bool
CmdBuffer::putLine ( const std::string & line )
{
    std::streamsize  n;

    n  = _cmdbuf->sputn(line.c_str(), line.length());

    if ( n != (std::streamsize) line.length() )
        return true;

    return true;
}


std::string
CmdBuffer::getCommand() const
{
    return _cmd;
}


std::string
CmdBuffer::getErrorStr() const
{
    return _errstr;
}


char
CmdBuffer::getEOL() const
{
    return _eol;
}


void
CmdBuffer::setEOL ( char eol )
{
    _eol = eol;
}

} // namespace

//  _TCANETPP_CMDBUFFER_CPP_
