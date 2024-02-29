/**
  * @file INotify.h
  *
  * The INotify class wraps linux 'sys/inotify.h' inotify api.
  *
  * @section LICENSE
  *
  * Copyright (c) 2024 Timothy Charlton Arland <tcarland@gmail.com>
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
#ifndef _TCANETPP_INOTIFY_H_
#define _TCANETPP_INOTIFY_H_

#include <sys/inotify.h>

#include <set>
#include <map>
#include <queue>


namespace tcanetpp {

/* ---------------------------------------------------------------- */

#define INOTIFY_EVENT_SIZE (sizeof (struct inotify_event))
#define INOTIFY_BUFFER_LEN (1024 * (INOTIFY_EVENT_SIZE + 32))

#define INOTIFY_UNKNOWN "UNK"
#define INOTIFY_ACCESS  "ACC"
#define INOTIFY_CREATE  "ADD"
#define INOTIFY_MODIFY  "MOD"
#define INOTIFY_DELETE  "DEL"
#define INOTIFY_OPEN    "OPN"
#define INOTIFY_CLOSE   "CLS"

/* ---------------------------------------------------------------- */

struct INotifyEvent {
    std::string  name;
    std::string  type;
};

typedef std::map<std::string, int>  WatchMap;
typedef std::queue<INotifyEvent>    IEventQueue;

class CircularBuffer;

/* ---------------------------------------------------------------- */

/**  INotify wraps the linux inotify api */
class INotify {

  public:

    INotify();
    ~INotify();

    bool          init();
    void          close();

    bool          addWatch ( const std::string & target );
    bool          addWatch ( const std::string & target, uint32_t mask );

    bool          removeWatch ( const std::string & target );
    bool          removeWatch ( int wd );

    ssize_t       readEvents ( IEventQueue & queue );

    int           getFileDescriptor() const;
    int           getWatchDescriptor ( const std::string & target ) const;

    const
    std::string&  errorStr() const;

    static void   SetNonBlocking ( int fd );


  public:

    static uint32_t   ON_CHANGE_MASK; // IN_{MODIFY|CREATE|DELETE}
    static uint32_t   ON_OPEN_MASK;   // IN_{OPEN|CLOSE}


  protected:

    ssize_t       nreadn ( int fd, void * vptr, size_t len );

    static
    std::string   ReadEventMask ( uint32_t mask, bool debug = false );


  private:

    int                 _fd;
    WatchMap            _watches;
    CircularBuffer    * _evbuf;

    std::string         _errstr;
    bool                _debug;

};

/* ---------------------------------------------------------------- */

}  // namespace

#endif  // _TCANETPP_INOTIFY_H_
