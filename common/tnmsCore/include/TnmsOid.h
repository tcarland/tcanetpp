#ifndef _TNMSCORE_TNMSOID_H_
#define _TNMSCORE_TNMSOID_H_

#include <vector>
#include <string>

#include <inttypes.h>

#include "Serializable.hpp"


namespace tnmscore {


typedef std::vector<uint16_t>    OidList;
typedef uint16_t*                tOid;



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

    uint16_t        operator[] ( OidList::size_type indx ) const;
    bool            operator<  ( const TnmsOid    & toid ) const;
    void            operator=  ( const TnmsOid    & toid );

    std::string     toString() const;
    tOid            toArray() const;

    const OidList&  getOidList() const;
    OidList&        getOidList();

    iterator        begin();
    iterator        end();
    const_iterator  begin() const;
    const_iterator  end() const;
    bool            empty() const;

    uint16_t        lastValue() const;
    uint32_t        getOidLength() const;


    ssize_t         serialize       ( char * buffer, size_t len );
    size_t          size() const    { return this->getOidLength(); }
    uint16_t        message_type() const { return 0; }


    static void     StringToOidList ( const std::string  & oidstr,
                                      OidList            & oidlist );

    static TnmsOid* OidFromString   ( const std::string  & oidstr );

    static TnmsOid* OidFromIndex    ( const TnmsOid      & oid,
                                      OidList::size_type & indx );

private:

    OidList        _oidlist;

};

}  // namespace

#endif // _TNMSCORE_TNMSOID_H_
