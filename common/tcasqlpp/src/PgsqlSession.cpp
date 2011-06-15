/**  PgsqlSession.cpp
  *    c++ wrapper to a MYSQL session instance.
  *  
  * Copyright (c) 2008,2009 Timothy Charlton Arland 
  *  @Author  tca@charltontechnology.net
  *
  * This file is part of tcasqlpp.
  *
  * tcasqlpp is free software: you can redistribute it and/or modify
  * it under the terms of the GNU Lesser General Public License as 
  * published by the Free Software Foundation, either version 3 of 
  * the License, or (at your option) any later version.
  *
  * tcasqlpp is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public 
  * License along with tcasqlpp.  
  * If not, see <http://www.gnu.org/licenses/>.
  *
 **/
#define _TCASQLPP_PGSQLSESSION_CPP_

#include "PgsqlSession.h"


namespace tcasqlpp {


PgsqlSession::PgsqlSession() { }

PgsqlSession::PgsqlSession ( const std::string & dbname, 
                             const std::string & dbuser,
                             const std::string & dbpass,
                             const std::string & dbhost,
                             const std::string & dbport )
{
    _connstr = PgsqlSession::MakeConnStr(dbname, dbuser, dbpass, dbhost, dbport);
}

PgsqlSession::PgsqlSession ( const PgsqlSession & sql )
    : _connstr(sql._connstr)
{
    _dbcon = NULL;
}

PgsqlSession::~PgsqlSession() 
{
    this->dbclose();
}

bool
PgsqlSession::dbconnect()
{
    if ( _connstr.empty() )
        return false;
    
    if ( _dbcon != NULL && _dbcon->is_open() )
        this->dbclose();

    if ( _dbcon == NULL )
        _dbcon = new connection(_connstr);

    return true;
}


bool
PgsqlSession::dbconnect ( const std::string & dbname, 
                          const std::string & dbuser, 
                          const std::string & dbpass,
                          const std::string & dbhost,
                          const std::string & dbport )
{
    this->_connstr = PgsqlSession::MakeConnStr(dbname, dbuser, dbpass, dbhost, dbport);
    return this->dbconnect();
}


void
PgsqlSession::dbclose()
{
    if ( _dbcon != NULL ) {
        _dbcon->disconnect();
        delete _dbcon;
        _dbcon = NULL;
    }
    return;
}


bool
PgsqlSession::isConnected()
{
    if ( _dbcon != NULL && _dbcon->is_open() )
        return true;
    return false;
}

const std::string&
PgsqlSession::sqlErrorStr()
{
    return _sqlerr;
}


/*  PgsqlSession methods */



template<typename TRANSACTOR>
bool
PgsqlSession::run ( const TRANSACTOR & transaction ) const
{
    try {

        _dbcon->perform(transaction);

    } catch ( const sql_error & e ) {
        std::ostringstream  msg;
        msg << "SQL Error: trying to execute transactor:\n"
            << "\tquery : " << e.query() << "\n"
            << "\tmessage : " << e.what();
        _sqlerr = msg.str();
        return false;
    }

    return true;
}


std::string
PgsqlSession::MakeConnStr ( const std::string & dbname, 
                            const std::string & dbuser, 
                            const std::string & dbpass,
                            const std::string & dbhost,
                            const std::string & dbport )
{
    std::string connstr = "";

    if ( ! dbname.empty() )
        connstr += (" dbname=" + dbname);
    if ( ! dbhost.empty() )
        connstr += (" host=" + dbhost);
    if ( ! dbuser.empty() )
        connstr += (" user=" + dbuser);
    if ( ! dbpass.empty() )
        connstr += (" password=" + dbpass);
    if ( ! dbport.empty() )
        connstr += (" port=" + dbport);

    return connstr;
}


} // namespace

//  _TCASQLPP_PGSQLSESSION_CPP_

