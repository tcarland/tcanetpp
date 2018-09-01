/**
  * @file PrefixTree.h
  *
  *   A STL wrapper to the patricia tree (patricia.h)
  *
  * Copyright (c) 2002,2008-2018 Timothy Charlton Arland
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
#ifndef _TCANETPP_PREFIXTREE_HPP_
#define _TCANETPP_PREFIXTREE_HPP_

#include "tcanetpp_types.h"
#include "patricia.h"
#include "IpAddr.h"


namespace tcanetpp {


/**    A C++ template that wraps the use of the radix tree implementation
  *  from 'patricia.h'.
  *
  *    The underlying trie handles user data as void pointers so as a result,
  *  this template, although not type specific, must be instantiated as a
  *  container of object pointers.
  *
  *  The following example demonstrates usage:
  *
  *  {
  *    PrefixTree<Object*>  pt;
  *
  *    IpAddr   pfx(addr, mask);
  *    Object*  obj = new Object(pfx);
  *
  *    if ( pt.insert(pfx, obj) )
  *        printf("Insert succeeded\n");
  *  }
  *
 **/
template<class T>
class PrefixTree {

  public:

    PrefixTree()
        : _pt(NULL),
          _freeHandler(NULL)
    {
        _pt   = pt_init();
    }

    virtual ~PrefixTree()
    {
        this->clear();
        ::free(_pt);
    }


    int insert  ( const IpAddr & p, T obj )
    {
        int       r = 0;
        prefix_t  c = p.getPrefixType();

        r  = pt_insert(_pt, &c, (void*) obj);

        return r;
    }


    T   remove  ( const IpAddr & p )
    {
        prefix_t c = p.getPrefixType();
        T      obj = (T) pt_remove(_pt, &c);
        return obj;
    }


    T   exactMatch ( const IpAddr & p )
    {
        prefix_t c  = p.getPrefixType();
        T      obj  = (T) pt_match(_pt, &c);
        return obj;
    }


    T   longestMatch ( IpAddr & p )
    {
        prefix_t c  = p.getPrefixType();
        T      obj  = (T) pt_matchLongest(_pt, &c);
        return obj;
    }


    size_t size() const
    {
        return pt_size(_pt);
    }


    size_t nodes() const
    {
        return pt_nodes(_pt);
    }


    size_t memUsage() const
    {
        size_t sz    = 0;
        int    nodes = 0;

        nodes = pt_nodes(_pt);
        sz    = (nodes * sizeof(ptNode_t));

        return sz;
    }


    size_t totalMemUsage() const
    {
        return( memUsage() + (size() * sizeof(T)) );
    }


    void clear()
    {
        if ( _freeHandler )
            pt_free(_pt, _freeHandler);
        else
            pt_free(_pt, &PTClearHandler);
    }


    void setFreeHandler ( ptNodeHandler_t handler )
    {
        _freeHandler = handler;
    }


    void traverse ( ptNodeHandler_t handler )
    {
        if ( handler )
            pt_visit(_pt, handler);
    }


  protected:

    static void PTClearHandler ( uint64_t addrA, uint64_t addrB, 
                                 uint16_t mb,    void   * rock )
    {
        T  obj = (T) rock;
        if ( obj )
            delete obj;
        return;
    }


  private:

    ptNode_t*            _pt;
    ptNodeHandler_t      _freeHandler;
};

} // namespace


#endif  //  _TCANETPP_PREFIXTREE_HPP_
