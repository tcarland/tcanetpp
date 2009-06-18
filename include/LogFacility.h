/** 
  *   LogFacility - Logging facility supporting multiple syslog(unix), filelog, or
  *            log iostreams.
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
#ifndef _TCANETPP_LOGFACILITY_H_
#define _TCANETPP_LOGFACILITY_H_

extern "C" {
#ifndef WIN32
# include <syslog.h>
#endif
}

#include <time.h>
#include <map>
#include <string>
#include <iosfwd>
#include <sstream>

#ifndef WIN32
# include "ThreadLock.h"
#endif

namespace tcanetpp {


typedef enum LogLevel
{
    LOGFAC_EMERG   = 0,
    LOGFAC_ALERT   = 1, 
    LOGFAC_ERR     = 3,
    LOGFAC_WARN    = 4,
    LOGFAC_NOTICE  = 5,
    LOGFAC_DEBUG   = 7
} logLevel_t;




class LogFacility {

  public:

    typedef std::ostringstream   Message;


    static bool           InitThreaded    ( bool trylock = false );
    

    static bool           OpenLogFile     ( const std::string & logname,
                                            const std::string & prefix, 
                                            const std::string & filename, 
                                            bool append = true );
    
    static bool           OpenSyslog      ( const std::string & prefix, 
                                            int facility );
    
    static bool           OpenLogStream   ( const std::string & name,
                                            const std::string & prefix,
                                            std::ostream * stream );
    
    
    static bool           AddLogStream    ( const std::string & name, 
                                            const std::string & prefix,
                                            std::ostream * stream );
    
    static std::ostream*  RemoveLogStream ( const std::string & name );
    static void           RemoveLogStreams( bool del  = true );
    
    static void           CloseSyslog();
    static std::ostream*  CloseLogFile    ( const std::string & logname, 
                                            bool del  = true );

    static void           CloseLogFacility();

    
    static void           LogMessage      ( LogFacility::Message & logmsg, 
                                            int level = LOGFAC_NOTICE );

    static void           LogMessage      ( const std::string & entry, 
                                            int   level   = LOGFAC_NOTICE,
                                            bool  newline = true );

    static void           LogMessage      ( const std::string & logname,
                                            const std::string & entry,
                                            int   level   = LOGFAC_NOTICE, 
                                            bool  newline = true );


    static void           LogToAllStreams ( const std::string & entry,
                                            bool  newline = true );
   
    static void           LogToStream     ( const std::string & logname,
                                            const std::string & entry,
                                            bool  newline = true );



    static bool           SetDefaultLogName   ( const std::string & name );
    static std::string    GetDefaultLogName();

    static void           SetDefaultLogPrefix ( const std::string & prefix );
    static std::string    GetDefaultLogPrefix() { return LogFacility::GetLogPrefix(); }

    static void           SetLogPrefix    ( const std::string & logname, 
                                            const std::string & prefix );
    static std::string    GetLogPrefix    ( const std::string & logname = "" );
    

    static void           SetLogTime      ( const time_t & now );
    static time_t         GetLogTime();

    static std::string    GetTimeString   ( const time_t & now );


    static bool           SetEnabled      ( const std::string & logname, bool enable );
    static bool           GetEnabled      ( const std::string & logname, bool & enabled );

    static void           SetBroadcast    ( bool broadcast );

    static bool           IsOpen();

    static void           SetDebug        ( bool debug );
    static bool           GetDebug();

    static std::string    GetLogFileName();


private:

    static bool           InitSyslog      ( int facility );
    static void           InitLogMessage  ( const std::string & logname = "" );

    static bool           Lock();
    static void           Unlock();


private:

    struct LogStream 
    {
        std::string    logName;
        std::string    logPrefix;
        std::ostream*  logStream;
        bool           enabled;

        LogStream ( std::ostream * strm = NULL )
            : logStream(strm),
              enabled(true)
        {
            if ( logStream == NULL )
                enabled = false;
        }

        LogStream ( const std::string & logName_,
                    const std::string & logPrefix_,
                    std::ostream      * strm )
            : logName(logName_),
              logPrefix(logPrefix_),
              logStream(strm),
              enabled(true)
        {
            if ( logStream == NULL )
                enabled = false;
        }
    };


    typedef std::map<std::string, LogStream>    StreamMap;


    static StreamMap             _StreamMap;

#   ifndef WIN32
    static tcanetpp::ThreadLock  _Lock;
#   endif

    static std::string           _LogName;
    static std::string           _LogPrefix;
    static std::string           _LogTimeStr;
    static time_t                _LogTime;
    static bool                  _Init;
    static bool                  _InitLock;
    static bool                  _TryLock;
    static bool                  _Enabled;
    static bool                  _Broadcast;
    static bool                  _Syslog;
    static bool                  _Debug;

};


} // namespace

#endif  // _TCANETPP_LOGFACILITY_H_

