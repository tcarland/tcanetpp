/**   EventManager
  * 
  *   Class for handling multiplexing data I/O and timer events.
  *   Note that Timer events currently lack sub-second granularity.
  *
  *   @Author  tcarland@gmail.com
  *   @Version 5.0
 **/
#ifndef _TCANETPP_EVENTMANAGER_H_
#define _TCANETPP_EVENTMANAGER_H_

#include <map>
#include <list>
#include <string>

#include "tcanetpp_types.h"
#include "EventHandlers.hpp"
#include "Socket.h"


namespace tcanetpp {


// macros for normalizing event time units
#define sectoevu(x)  (x * 1000000)
#define msectoevu(x) (x * 1000)
#define evutosec(x)  (x / 1000000)
#define evutomsec(x) (x / 1000)


#define DEFAULT_EVU 10000  // default select timeout
#define MAX_FDVAL   1024   // max file descriptors


 
typedef uint32_t evid_t;  // event registration id 

class EventManager;



/**  The EventTimer struct is created and used internally to 
  *  represent a timer event. When an event is fired, a const 
  *  pointer to this struct is provided to the event handler.
  *  ( see EventHandlers.h )
 **/
struct EventTimer {
    
    EventTimer()
        : evid(0),  evmgr(0),  handler(0), count(0), fired(0), 
          evsec(0), evusec(0), absolute(false), enabled(false) 
    {}
    
    evid_t         	evid;      // event id
    EventManager*  	evmgr;     // evmgr owning this event 
    TimerHandler*  	handler;   // event handler for this event.
    uint32_t       	count;     // number of times to fire event (0 == forever)
    uint32_t       	fired;     // number of times event has fired
    long           	evsec;     // event interval in seconds (if applicable).
    long           	evusec;    // event interval in microseconds (if applicable).
    struct timeval 	abstime;   // absolute timeval used to track event time
    bool           	absolute;  // boolean indicating a single shot event.
    bool           	enabled;   // boolean indicating whether event is active.
    
    bool operator== ( const EventTimer & timer );
    bool operator<  ( const EventTimer & timer );
};

typedef std::map<evid_t, EventTimer>  EventTimerMap;



/**  The EventIO struct is created and used internally to represent 
  *  an IO event. When an event is fired, a const pointer to this 
  *  struct is provided to the application's event handler.
  *  ( see EventHandlers.h )
 **/
struct EventIO {
    EventIO() 
        : evid(0), evmgr(0), handler(0), rock(0),
          enabled(false), isServer(false)
    {
        Socket::ResetDescriptor(sfd);
    }

    evid_t         	evid;      // event id
    EventManager*  	evmgr;     // evmgr owning this event (ie. this)
    IOHandler*     	handler;   // event handler for this event.
    sockfd_t      	sfd;       // event socket id
    void*               rock;      // event object
    struct timeval      abstime;   // time of event firing.
    bool           	enabled;   // boolean indicating whether event is valid.
    bool           	isServer;  // IO socket event is a server socket.

    bool operator== ( const EventIO & io );
};

typedef std::map<evid_t, EventIO>  EventIOMap;


/**  The EventManager class provides an interface to using select for a
  *  variety of I/O and timer events.  By default the main loop will
  *  run endlessly until setAlarm() is called.  Alternatively, upon
  *  construction, if 'dieoff' is true, the event loop will exit once
  *  there are no remaining, enabled events in the system. 
 **/
class EventManager {

  public:

    EventManager( bool dieoff = false );

    virtual ~EventManager();


    /*  EventManager main loop */
    void   eventLoop();


    evid_t addTimerEvent ( TimerHandler * handler, uint32_t sec, uint32_t msec, 
                           int count = 0 );

    evid_t addTimerEvent ( TimerHandler * handler, time_t abstime );

    evid_t addIOEvent    ( IOHandler * handler, const sockfd_t & sfd, 
                           void * rock = NULL, bool isServer = false );

    bool   removeEvent   ( const evid_t & id );
    bool   validEvent    ( const evid_t & id );
    bool   isValidEvent  ( const evid_t & id ) { return this->validEvent(id); }


    const EventTimer&  findTimerEvent ( const evid_t & id );
    const EventIO&     findIOEvent    ( const evid_t & id );


    int    activeRegistrations()  { return(this->activeTimers() 
                                        + this->activeClients()); }
    int    activeTimers()         { return((int)_timers.size()); }
    int    activeClients()        { return((int)_clients.size()); }
    bool   isActive ( evid_t id ) { return this->validEvent(id); }
    void   clearTimers();

    void   setDebug ( bool d );
    void   setAlarm();


  protected:

    void   verifyTimers();
    void   checkTimers   ( const timeval    & now );
    void   checkMinTimer ( const EventTimer & timer );

    void   destroyEvent  ( EventTimer & timer );
    void   destroyEvent  ( EventIO    & io );

    evid_t getNewEventId();

    static void GetTimeOfDay ( timeval & t );
    

  protected:

    EventTimerMap               _timers;
    EventIOMap		        _clients;

    evid_t                      _evid;
    fd_set                      _rset, _wset, _xset;
    long                        _minevu;

    std::string                 _errstr;
    bool                        _dieoff;
    bool                        _alarm;
    bool                        _debug;

    static int	                _maxfdp;
 
};

}  // namespace


#endif  // _TCANETPP_EVENTMANAGER_H_

