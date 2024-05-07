/**
  * @file INotify.h
  *
  * The INotify class wraps linux 'sys/inotify.h' inotify api.
  *
  * @section LICENSE
  *
  * Copyright (c) 2020-2024 Timothy Charlton Arland <tcarland@gmail.com>
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
    int          wd;
    std::string  name;
    std::string  path;
    std::string  type;
    bool         isdir;

    INotifyEvent() : wd(-1), isdir(false) {}
};

typedef std::map<std::string, int>  WatchMap;
typedef std::map<int, std::string>  WatchDescriptorMap;
typedef std::queue<INotifyEvent>    IEventQueue;

class CircularBuffer;

/* ---------------------------------------------------------------- */

/**  INotify wraps the linux inotify api */
class INotify {

  public:

    INotify ( bool recursive = false );
    virtual ~INotify();

    virtual bool  init();
    virtual void  close();

    bool          addWatch    ( const std::string & target );
    bool          addWatch    ( const std::string & target, uint32_t mask );

    bool          removeWatch ( const std::string & target );
    bool          removeWatch ( int wd );

    bool          haveWatch   ( const std::string & target ) const;
    bool          haveWatch   ( int wd ) const;

    ssize_t       readEvents  ( IEventQueue & queue );

    std::string   getWatchName       ( int wd ) const;
    int           getWatchDescriptor ( const std::string & target ) const;
    int           getFileDescriptor() const;
    bool          recursive() const;
    size_t        size() const;

    const
    std::string&  errorStr() const;

    static void   SetNonBlocking ( int fd );

  public:

    static uint32_t   ON_CHANGE_MASK; // IN_{MODIFY|CREATE|DELETE}
    static uint32_t   ON_OPEN_MASK;   // IN_{OPEN|CLOSE}


  protected:

    ssize_t       nreadn ( int fd, void * vptr, size_t len );

    static
    std::string   ReadEventMask ( uint32_t mask );


  private:

    int                 _fd;
    bool                _recursive;
    WatchMap            _watches;
    WatchDescriptorMap  _watched;
    CircularBuffer    * _evbuf;

    std::string         _errstr;
};

}  // namespace

#endif  // _TCANETPP_INOTIFY_H_
