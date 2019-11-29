/*
  * @file INotify.cpp
  *
  * The INotify class wraps linux 'sys/inotify.h' inotify api.
  *
  * Copyright (c) 2019 Timothy Charlton Arland
  * @author tcarland@gmail.com
  *
  * @section LICENSE
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
#define _TCANETPP_INOTIFY_CPP_

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>

#include "util/INotify.h"


namespace tcanetpp {


uint32_t INotify::ON_CHANGE_MASK = IN_MODIFY | IN_CREATE | IN_DELETE;
uint32_t INotify::ON_OPEN_MASK   = IN_OPEN | IN_CLOSE;


/* ---------------------------------------------------------------- */

INotify::INotify()
  : _fd(0),
    _evbuf(new CircularBuffer(INOTIFY_BUFFER_LEN),
    _debug(false)
{ }


INotify::~INotify()
{
    this->close();
    delete _evbuf;
}


bool
INotify::init()
{
    if ( this->_fd > 0 )
        return false;

    this->_fd = inotify_init();

    if ( this->_fd < 0 ) {
        _errstr = "INotify error on init()";
        return false;
    }

    INotify::SetNonBlocking(this->_fd);

    return true;
}

/* ---------------------------------------------------------------- */

void
INotify::close()
{
    ::close(this->_fd);
}


int
INotify::getFileDescriptor() const
{
    return this->_fd;
}


int
INotify::getWatchDescriptor ( const std::string & target ) const
{
    WatchMap::const_iterator  wIter;

    if ( (wIter = _watches.find(target)) == _watches.end() )
        return -1;

    return wIter->second;
}


bool
INotify::addWatch ( const std::string & target )
{
    int wd;

    wd = inotify_add_watch(this->_fd, target.c_str(), INotify::ON_CHANGE_MASK);

    if ( wd < 0 ) {
        _errstr = "Error in inotify_add_watch()";
        return false;
    }

    _watches[target] = wd;

    return true;
}


bool
INotify::removeWatch ( const std::string & target )
{
    int wd;

    wd = _watches[target];
    inotify_rm_watch(this->_fd, wd);

    return true;
}


ssize_t
INotify::readEvents ( IEventQueue & queue )
{
    char * wptr = NULL;
    char * rptr = NULL;
    size_t wlen = 0;
    size_t rlen = 0;
    size_t rt   = 0;
    size_t rtt  = 0;
    ssize_t wt;

    struct inotify_event * event;

    wptr = _evbuf->getWritePtr(&wlen);
    if ( wptr == NULL )
        return -1;

    wt = this->nreadn(this->_fd, (void*)wptr, wlen);

    if ( wt < 0 ) {
        _errstr = "INotify::readEvents() Error in reading inotify descriptor";
        return wt;
    }

     _evbuf->setWritePtr(wt);

    while ( _evbuf->readAvailable() > INOTIFY_EVENT_SIZE ) {
        INotifyEvent ivent;

        rptr = _evbuf->getReadPtr(&rlen);
        if ( rptr == NULL ) {
            _errstr = "INotify::readEvents() Error in obtaining handle from buffer";
            return -1;
        }

        event = (struct inotify_event*) rptr;

        //printf ("wd=%d mask=%u cookie=%u len=%u\n", event->wd, event->mask, event->cookie, event->len);

        if ( event->mask & IN_ISDIR )
            printf(" => IN_ISDIR\n");

        if ( event->len ) {
            //printf ("name=%s\n", event->name);
            ivent.name = event->name;
            ivent.type = this->ReadEventMask(event->mask);
            queue.push(ivent);
        }

        rt   = INOTIFY_EVENT_SIZE + event->len;
        rtt += rt;

        _evbuf->setReadPtr(rt);
    }

    return ((ssize_t) rt);
}

/* ---------------------------------------------------------------- */

std::string
INotify::ReadEventMask ( uint32_t mask, bool debug )
{
    std::string  type;

    switch ( mask & (IN_ALL_EVENTS | IN_UNMOUNT | IN_Q_OVERFLOW | IN_IGNORED) )
    {
        case IN_ACCESS:
            if ( debug )
                printf("Item was accessed\n");
            type = INOTIFY_ACCESS;
            break;
        case IN_MODIFY:
            if ( debug )
                printf("Item was modified\n");
            type = INOTIFY_MODIFY;
            break;
        case IN_CREATE:
            if ( debug )
                printf("Item was created\n");
            type = INOTIFY_CREATE;
            break;
        case IN_OPEN:
            if ( debug )
                printf("Item was opened\n");
            type = INOTIFY_OPEN;
            break;
        case IN_CLOSE:
            if ( debug )
                printf("Item was closed\n");
            type = INOTIFY_CLOSE;
            break;
        case IN_DELETE:
            type = INOTIFY_DELETE;
            break;
        case IN_IGNORED:
            if ( debug )
                printf("Filesystem ignored and dropped from inotify\n");
            break;
        default:
            if ( debug )
                printf("Unknown event\n");
            type = "UNK";
    }

    return type;
}

/* ---------------------------------------------------------------- */

ssize_t
INotify::nreadn ( int fd,  void * vptr, size_t n )
{
    size_t  nleft;
    ssize_t nread;
    char*   ptr;

    ptr   = (char*) vptr;
    nleft = n;

    while ( nleft > 0 )
    {
        if ( (nread = ::read(fd, ptr, nleft)) < 0 ) {
            if ( errno == EINTR || errno == EAGAIN ) {
                return (n-nleft);
            }
            else
                return -1;
        } else if ( nread == 0 ) {
            return (n-nleft);
        }
        nleft -= nread;
        ptr   += nread;
    }

    return(n-nleft);
}

/* ---------------------------------------------------------------- */

void
INotify::SetNonBlocking ( int fd )
{
    int flags = ::fcntl(fd, F_GETFL, 0);
    ::fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/* ---------------------------------------------------------------- */

}  // namespace

// _TCANETPP_INOTIFY_CPP_
