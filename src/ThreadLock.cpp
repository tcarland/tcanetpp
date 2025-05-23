/**
  * @file ThreadLock.cpp
  *
  * @section LICENSE
  *
  * Copyright (c) 2001-2025 Timothy Charlton Arland <tcarland@gmail.com>
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
#define _TCANETPP_THREADLOCK_CPP_


extern "C" {
# include <string.h>
# include <stdio.h>
# include <errno.h>
}

#include "ThreadLock.h"
#include "event/EventManager.h"


namespace tcanetpp {



/* ----------------------------------------------------------------------- */
/**  Class to lock/unlock a mutex on construction/destruction
  *  allowing for synchronization of a class method by simply
  *  instantiating this object with the ThreadLock instance.
 **/
ThreadAutoMutex::ThreadAutoMutex ( ThreadLock * lock, bool sync )
    : _mutex(lock),
      _sync(sync),
      _locked(false)
{
    if ( this->_sync )
        if ( this->_mutex->lock() )
            _locked = true;
}

ThreadAutoMutex::~ThreadAutoMutex()
{
    if ( this->_sync )
        this->_mutex->unlock();
}

bool
ThreadAutoMutex::isLocked() const
{
    return _locked;
}

/* ----------------------------------------------------------------------- */
/**  The ThreadLock class is a simple wrapper for a pthread mutex and
  *  condition variable intended primarily for providing wait/notify
  *  behavior to a derived Thread object. The methods lock(), tryLock(),
  *  and unlock() each exhibit the behaviors of the corresponding pthread
  *  calls to pthread_mutex_(lock|trylock|unlock).
  *  This class was NOT intended to offer any additional safety or deadlock
  *  protection from the misuse of a pthread mutex, but is perfectly safe
  *  if used correctly.
 **/

ThreadLock::ThreadLock()
{
    ::pthread_mutex_init(&_mutex, NULL);
    ::pthread_cond_init(&_items, NULL);
}


ThreadLock::~ThreadLock()
{
    ::pthread_mutex_destroy(&_mutex);
    ::pthread_cond_destroy(&_items);
}

/* ----------------------------------------------------------------------- */


/**  Obtains the lock. This is a blocking operation which
  *  returns 1 on success.
 **/
int
ThreadLock::lock()
{
    if ( ::pthread_mutex_lock(&_mutex) != 0 )
        return -1;
    return 1;
}


/**  Trys to obtain the lock. The lock will be acquired if it
  *  is available and the function will return 1. A return of
  *  0 indicates the lock failed (EBUSY), and -1 is returned
  *  in case of a pthread failure in pthread_mutex_trylock()
 **/
int
ThreadLock::tryLock()
{
    int status;

    status = ::pthread_mutex_trylock(&_mutex);

    if ( status == EBUSY )
        return 0;
    else if ( status != 0 )
        return -1;
    return 1;
}


int
ThreadLock::unlock()
{
    ::pthread_mutex_unlock(&_mutex);
    return 1;
}

/* ----------------------------------------------------------------------- */


/**  A blocking wait condition.
  *
  *  As with the pthreads API, the mutex must be locked first.
 **/
int
ThreadLock::wait()
{
    if ( ::pthread_cond_wait(&_items, &_mutex) != 0 )
        return -1;
    return 1;
}

/**  A timed wait that blocks for given number of seconds.
  *  As with the pthreads API, the mutex must be locked first.
  *
  *  @param usec is the number of microseconds to wait.
 **/
int
ThreadLock::waitFor ( time_t usec )
{
    struct timespec  to;

    if ( usec < 1 )
        return 0;

    ::memset(&to, 0, sizeof(to));

    to.tv_sec  = ::time(NULL);
    to.tv_nsec = usec * 1000;

    EventManager::TimespecNorm(&to);

    return this->waitFor(&to);
}

/**  A timed wait that blocks for given number of seconds.
  *  As with the pthreads API, the mutex must be locked first.
  *
  *  @param ts is a struct timespec of the amount of time to wait.
 **/
int
ThreadLock::waitFor ( const timespec * ts )
{
    int status;

    status   = ::pthread_cond_timedwait(&_items, &_mutex, ts);

    if ( status == ETIMEDOUT )
        return 0;
    else if ( status != 0 )
        return -1;

    return 1;
}


/**  Signals a thread waiting for on the lock. */
int
ThreadLock::notify()
{
    return(::pthread_cond_signal(&_items));
}


/**  Signals all threads waiting on the lock */
int
ThreadLock::notifyAll()
{
    return(::pthread_cond_broadcast(&_items));
}

/* ----------------------------------------------------------------------- */

} // namespace


//  _TCANETPP_THREADLOCK_CPP_
