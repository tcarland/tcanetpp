#define _TNMSAUTH_TNMSAUTHDMANAGER_CPP_

#include "TnmsAuthdManager.h"


namespace tnmsauth {


TnmsAuthdManager::TnmsAuthdManager() 
    : _evmgr(new EventManager()),
      _tree(new TnmsTree()),
      _authThread(NULL),
      _sql(NULL),
      _svr(NULL),
      _soap(NULL),
      _svrId(0), _soapId(0), _reportId(0), _logId(0),
      _soapHandler(new SoapIOHandler(_tree)),
      _authHandler(new AuthIOHandler(_tree)),
      _lastTouched(0),
      _reportDelay(30),
      _logCheck(3600),
      _today(0),
      _hup(false),
      _usr(false),
      _debug(false)
{}


TnmsAuthdManager::~TnmsAuthdManager() 
{
    delete _tree;
    delete _evmgr;
    delete _soapHandler;
    delete _authHandler;
}



void
TnmsAuthdManager::run()
{
    time_t  now = ::time(NULL);

    LogFacility::SetLogTime(now);

    if ( _debug ) {
        LogFacility::OpenLogStream("stdout", "", &std::cout);
        LogFacility::LogMessage("TnmsAuthdManager::run()");
        LogFacility::SetBroadcast(true);
    }

    _evmgr->addTimerEvent(this, 5, 0);
    _reportId  = _evmgr->addTimerEvent(this, _reportDelay, 0);
    _logId     = _evmgr->addTimerEvent(this, _logCheck, 0);

    if ( ! this->parseConfig(_configfile, now) ) {
        // error parsing config
        if ( _debug )
            LogFacility::LogMessage("Error parsing config");
        return;
    }

    //  Enter Main Loop
    _evmgr->eventLoop();

    if ( _debug )
        LogFacility::RemoveLogStream("stdout");

    LogFacility::CloseLogFacility();

    return;
}

void
TnmsAuthdManager::timeout ( const EventTimer * timer ) 
{
    const time_t & now = timer->abstime.tv_sec;

    LogFacility::SetLogTime(now);

    //check for logrotate
    if ( timer->evid == _logId ) {
        if ( ! _tconfig.syslog )
            this->logRotate(_tconfig.logfile, now);
        return;
    }

    // internal metrics
    if ( timer->evid == _reportId ) {
        LogFacility::LogMessage("TnmsAuthdManager::run()"); // <<add some stats
        return;
    }
/*
    // if startup_delay is expired; then add IOHandler for clients
    if ( _startDelay > 0 ) {
        if ( _startAt == 0 )
            _startAt = now + _startDelay;

        if ( _startAt <= now && _client ) {
            _clientId   = _evmgr->addIOEvent(_clientHandler, _client->getDescriptor(), _client, true);
            _startDelay = 0;
        }
    }
*/
    // re-parse config signal
    if ( _hup ) { 
        if ( ! this->parseConfig(_configfile, now) )
            LogFacility::LogMessage("Config error");
        _hup = false;
    }

    if ( _usr ) {
        // special function
        _tree->debugDump();
        _usr = false;
    }

    // give time to our handlers
    //_auth->timeout(timer);
    _authHandler->timeout(timer);
    _soapHandler->timeout(timer);

    return;
}

void
TnmsAuthdManager::setAlarm()
{
    this->_alarm = true;
}


void
TnmsAuthdManager::setHUP()
{
    this->_hup = true;
}


void
TnmsAuthdManager::setUSR()
{
    this->_usr = true;
}

void
TnmsAuthdManager::setDebug ( bool d )
{
    _debug = d;
}


std::string
TnmsAuthdManager::getErrorStr()
{
    return this->_errstr;
}


bool
TnmsAuthdManager::parseConfig ( const std::string & cfg, const time_t & now )
{
    TnmsConfigHandler  cfgmgr(cfg, TNMSAUTHD_CONFIG_ROOT);
    std::string        prefix    = TNMSAUTHD_CONFIG_ROOT;

    if ( ! cfgmgr.parse() ) {
        if ( LogFacility::IsOpen() ) {
            LogFacility::LogMessage("TnmsAuthdManager::parseConfig() error: " 
                    + cfgmgr.getErrorStr());
        } else {
            fprintf(stderr, "TnmsAuthdManager::parseConfig() error: %s\n",
                    cfgmgr.getErrorStr().c_str());
        }
        return false;
    }

    TnmsConfig & config = cfgmgr.config;

    prefix.append("/").append(config.agent_name);
    LogFacility::SetDefaultLogPrefix(prefix);

    if ( config.syslog ) {
        if ( _tconfig.syslog )
            LogFacility::CloseSyslog();
        LogFacility::OpenSyslog(prefix, LOG_LOCAL3);
    } else {
        if ( _tconfig.syslog )
            LogFacility::CloseSyslog();
        if ( config.logfile.compare(_tconfig.logfile) != 0 )
            _today = 0;
        this->logRotate(config.logfile, now);
    }

    // auth attributes
    AuthConfig  acfg;

    acfg.db_host = cfgmgr.getAttribute("db_host");
    acfg.db_port = cfgmgr.getAttribute("db_port");
    acfg.db_name = cfgmgr.getAttribute("db_name");
    acfg.db_user = cfgmgr.getAttribute("db_user");
    acfg.db_pass = cfgmgr.getAttribute("db_pass");
    acfg.timeout = cfgmgr.getAttribute("soap_timeout");

    acfg.max_connections = cfgmgr.getAttribute("max_connections");
    acfg.min_threads     = cfgmgr.getAttribute("min_db_threads");
    acfg.min_threads     = cfgmgr.getAttribute("min_db_threads");

    acfg.tnms_port       = cfgmgr.getAttribute("tnms_port");
    acfg.soap_port       = cfgmgr.getAttribute("soap_port");

    if ( acfg.db_host.compare(_aconfig.db_host) != 0 ||
         acfg.db_port.compare(_aconfig.db_port) != 0 || 
         acfg.db_name.compare(_aconfig.db_name) != 0 ||
         acfg.db_user.compare(_aconfig.db_user) != 0 ||
         acfg.db_pass.compare(_aconfig.db_pass) != 0 )
    {
        if ( _authThread )
            delete _authThread;

        if ( _sql )
            delete _sql;

        _sql        = new SqlSession(acfg.db_name, acfg.db_host, acfg.db_user,
                                     acfg.db_pass, acfg.db_port);
        _authThread = new TnmsAuthThread((SqlSessionInterface*) _sql);
        _authThread->start();
    }

    if ( acfg.tnms_port > 0 && acfg.tnms_port != _aconfig.tnms_port ) 
    {
        LogFacility::LogMessage("Config setting auth port to " 
            + StringUtils::toString(acfg.tnms_port));

        if ( _svr ) {
            _svr->close();
            _evmgr->removeEvent(_svrId);
            // delete _svr;
        }

        _svr = new Socket(0, acfg.tnms_port, SOCKET_SERVER, TCP);

        if ( _svr->init(false) < 0 ) {
            LogFacility::LogMessage("Config error creating server " 
                + _svr->getErrorStr());
            return false;
        }
    } else if ( acfg.tnms_port == 0 ) {
        LogFacility("Config error, auth port set to zero");
        return false;
    }

    if ( acfg.soap_port > 0 && acfg.soap_port != _aconfig.soap_port )
    {
        LogFacility::LogMessage("Config setting soap port to " 
            + StringUtils::toString(acfg.soap_port));
        
        if ( _soap ) {
            _soap->close();
            _evmgr->removeEvent(_soapId);
            // delete _soap;
        }

        _soap = new SoapClient();

        if ( _soap->bind(acfg.soap_port, _authThread) < 0 ) {
            LogFacility::LogMessage("Config error in bind " + _soap->getErrorStr());
            return false;
        }

        _soapId = _evmgr->addIOEvent(_soapHandler, _soap->getFD(), _soap, true);
    }

    _aconfig = acfg;

    return true;
}

void
TnmsAuthdManager::logRotate ( std::string logfile, const time_t & now )
{
    if ( logfile.empty() ) 
        return;

    char        datestr[64];
    struct tm  *ltm   = ::localtime(&now);
    int         today = ltm->tm_yday;

    if ( _today != today ) {
        LogFacility::CloseLogFile(_logname);

        ::strftime(datestr, 64, ".%Y%02m%02d", ltm);

        logfile.append(datestr);
        _logname = logfile;
        _today   = today;

        LogFacility::OpenLogFile(_logname, LogFacility::GetLogPrefix(), logfile, true);
        LogFacility::SetDefaultLogName(_logname);
    }

    return;
}




}  // namespace


// _TNMSAUTH_TNMSAUTHDMANAGER_CPP_
