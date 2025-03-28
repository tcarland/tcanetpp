/**
  * @file ThreadLock.h
  *
  * Threadlock wraps a pthread conditional mutex.
  *
  * @section LICENSE
  *
  * Copyright (c) 2008-2025 Timothy Charlton Arland <tcarland@gmail.com>
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
#ifndef _TCANETPP_THREADLOCK_H_
#define _TCANETPP_THREADLOCK_H_

extern "C" {
# include <pthread.h>
# include <time.h>
}


namespace tcanetpp {


class ThreadLock;


/** Automutex provides a clean way to synchronize around a
  * given method by instantiating one on the stack:
  * void soStuffSafely() {
  *   ThreadAutoMutex auto(this->_lock);
  *   //do stuff safely;
  *   return;
  * }
 **/
class ThreadAutoMutex {
  public:
    ThreadAutoMutex ( ThreadLock * lock, bool sync = true );
    virtual ~ThreadAutoMutex();

    virtual bool isLocked() const;

  protected:
    ThreadLock *  _mutex;
    bool          _sync;
    bool          _locked;
};


/*  The ThreadLock class provides a convenient wrapper to
 *  a pthread mutex.
 */
class ThreadLock {

  public:

    ThreadLock();
    ~ThreadLock();


    int  wait();
    int  waitFor ( time_t usec );
    int  waitFor ( const timespec * ts );

    int  lock();
    int  unlock();

    int  notify();
    int  notifyAll();

    int  tryLock();

    inline int trylock() { return this->tryLock(); }

  private:

    pthread_mutex_t    _mutex;
    pthread_cond_t     _items;
};

}  // namespace

#endif  // _TCANETPP_THREADLOCK_H_
