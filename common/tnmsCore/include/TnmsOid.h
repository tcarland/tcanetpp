/*
 * TnmsOid.h
 *
 *  Copyright(c) 2008,2009  Timothy Charlton Arland
 *  Charlton Technology LLC
 *  tcarland@gmail.com
 */
#ifndef _TNMSCORE_TNMSOID_H_
#define _TNMSCORE_TNMSOID_H_

#include <vector>
#include <string>

#include <inttypes.h>

#include "Serializable.hpp"


namespace tnmsCore {


typedef std::vector<uint32_t>    OidList;
typedef uint32_t*                tOid;



class TnmsOid : public Serializable {

public:

    typedef OidList::iterator        iterator;
    typedef OidList::const_iterator  const_iterator;

public:

    TnmsOid ( OidList::size_type   size = 0 );
    TnmsOid ( const OidList      & oidlist );
    TnmsOid ( const std::string  & oidstr );
    TnmsOid ( const TnmsOid      & toid );

    virtual ~TnmsOid();

    uint32_t         operator[] ( OidList::size_type indx ) const;
    bool             operator<  ( const TnmsOid    & toid ) const;
    void             operator=  ( const TnmsOid    & toid );

    std::string      toString() const;
    tOid             toArray()  const;

    const OidList&   getOidList() const;
    OidList&         getOidList();

    iterator         begin();
    iterator         end();
    const_iterator   begin() const;
    const_iterator   end()   const;
    bool             empty() const;

    uint32_t         lastValue()    const;
    size_t           getOidLength() const;
    size_t           length() const { return this->getOidLength(); }

    // Serializable
    virtual ssize_t  serialize       ( char * buffer, size_t len );
    virtual ssize_t  deserialize     ( const char * buffer, size_t len );
    virtual size_t   size() const;
    virtual uint16_t message_type() const { return 0; }

public:

    static void      StringToOidList ( const std::string  & oidstr,
                                       OidList            & oidlist );

    static TnmsOid*  OidFromString   ( const std::string  & oidstr );

    static TnmsOid*  OidFromOidIndex ( const TnmsOid      & oid,
                                       OidList::size_type & index );

protected:

    OidList         _oidlist;

};

}  // namespace

#endif // _TNMSCORE_TNMSOID_H_

