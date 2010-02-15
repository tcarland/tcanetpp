#ifndef _FWGEN_FWLOGREPORT_H_
#define _FWGEN_FWLOGREPORT_H_

#include <map>
#include <string>

#include "FwLogEntry.h"
#include "FwService.h"


#include "TnmsAPI.h"
using namespace tnmsApi;


namespace fwgen {


typedef std::map<std::string, FwLogEntry>  FwMap;



class FwLogReport {
  public:

    FwLogReport ( const std::string & agent );
    ~FwLogReport();

    int  FlushApi  ( const time_t & now );
    void SendEntry ( FwLogEntry & fwe, const time_t & now );

  private:

    FwMap     _fwMap;
    FwSvcMap  _svcMap;

    TnmsAPI * _api;
    bool      _connection;

};

} // namespace


#endif  // _FWGEN_FWLOGREPORT_H_
