#define _TNMSD_AGENTIOHANDLER_CPP_

#include "AgentIOHandler.h"

#include "EventManager.h"
#include "LogFacility.h"


namespace tnmsd {


AgentIOHandler::AgentIOHandler ( TnmsTree * tree, AuthClient * auth ) 
    throw ( Exception )
    : _tree(tree),
      _auth(auth)
{
    if ( NULL == _tree )
        throw Exception("AgentIOHandler() TnmsTree is NULL");
}

AgentIOHandler::~AgentIOHandler()
{
    ClientSet::iterator  cIter;

    for ( cIter = _clients.begin(); cIter != _clients.end(); ++cIter )
        delete *cIter;
}


void
AgentIOHandler::timeout ( const EventTimer * timer )
{
    int            rd, sd;
    const time_t & now = timer->abstime.tv_sec;

    ClientSet::iterator  cIter;

    for ( cIter = _clients.begin(); cIter != _clients.end(); cIter++ ) {
        TnmsClient * client = (TnmsClient*) *cIter;
        LogFacility::Message  logmsg;

        logmsg << "AgentIOHandler timeout (" << client->getHostStr() << ")";

        if ( client->isMirrorClient() ) {
            if ( ! client->isConnected() || client->isConnecting() ) {
                int c = 0;

                if ( (c = client->connect()) < 0 ) {
                    logmsg << " mirror disconnected.";
                    LogFacility::LogMessage(logmsg);
                    continue;
                } else if ( c >= 0 ) {
                    timer->evmgr->addIOEvent(this, client->getSockFD(), client);
                    continue;
                }

                if ( c > 0 ) {
                    logmsg << " mirror connected.";
                    LogFacility::LogMessage(logmsg);
                }
            } else {
                if ( client->isAuthorized() && ! client->isSubscribed() )
                    // suball?
                    ;
                else if ( ! client->isAuthorized() )
                    //login
                    ;
            }
        }

        if ( (rd = client->receive(now)) < 0 ) {
            logmsg << " error in receive() ";
            LogFacility::LogMessage(logmsg);
            client->close();
            continue;
        } else {
            // rd = ?
            ;
        }

        if ( (sd = client->send(now)) < 0 ) {
            logmsg << " error in send() ";
            LogFacility::LogMessage(logmsg);
            client->close();
            continue;
        } else {
            // sd is our count
            ;
        }

    }

    return;
}


void
AgentIOHandler::addMirrorConnection ( TnmsClient * client )
{
    _clients.insert(client);
}


void
AgentIOHandler::handle_accept ( const EventIO * io )
{
    Socket * svr = (Socket*) io->rock;
    BufferedSocket * sock = NULL;

    sock = (BufferedSocket*) svr->accept(BufferedSocket::factory);

    if ( sock == NULL )
        return;

    TnmsClient * client = new TnmsClient(_tree, _auth, sock, true);
    evid_t id = io->evmgr->addIOEvent(this, client->getSockFD(), (void*) client);

    if ( id == 0 )
        LogFacility::LogMessage("AgentIOHandler::handle_accept() event error: " 
            + io->evmgr->getErrorStr());

    // if compression; enable it
    // client->enableCompression();
    LogFacility::LogMessage("AgentIOHandler::handle_accept() " + client->getHostStr());
    _clients.insert(client);

    return;
}

void
AgentIOHandler::handle_read ( const EventIO * io )
{
    int  rd = 0;

    if ( io->isServer )
        return;

    TnmsClient * client = (TnmsClient*) io->rock;

    if ( (rd = client->receive(io->abstime.tv_sec)) < 0 )
        return this->handle_close(io);
    else if ( rd > 0 )
        LogFacility::LogMessage("AgentIOHandler::handle_read() " + StringUtils::toString(client->getBytesReceived()));

    return;
}

void
AgentIOHandler::handle_write ( const EventIO * io )
{
    int  wt = 0;

    if ( io->isServer )
        return;

    TnmsClient * client = (TnmsClient*) io->rock;

    if ( (wt = client->send(io->abstime.tv_sec)) < 0 )
        return this->handle_close(io);
    else if ( wt > 0 )
        LogFacility::LogMessage("AgentIOHandler::handle_write() " + StringUtils::toString(client->getBytesSent()));

    return;
}

void
AgentIOHandler::handle_close ( const EventIO * io )
{
    if ( io->isServer )
        return;

    TnmsClient * client = (TnmsClient*) io->rock;

    if ( client == NULL )
        return;

    if ( client->isMirror() ) {
        ;
    } else {
        _clients.erase(client);
        _tree->removeClient(client);
    }

    LogFacility::LogMessage("AgentIOHandler::handle_close() " + client->getHostStr());
    client->close();
    io->evmgr->removeEvent(io->evid);

    return;
}


void
AgentIOHandler::handle_shut ( const EventIO * io )
{

}

void
AgentIOHandler::handle_destroy ( const EventIO * io )
{
    LogFacility::LogMessage("AgentIOHandler::handle_destroy()");

    if ( io->isServer ) {
        Socket * svr = (Socket*) io->rock;
        if ( svr )
            delete svr;
    } else {
        TnmsClient * client = (TnmsClient*) io->rock;
        if ( client )
            delete client;
    }

    return;
}


bool
AgentIOHandler::readable ( const EventIO * io )
{
    return true;
}


bool
AgentIOHandler::writeable ( const EventIO * io )
{
    if ( io == NULL || io->isServer )
        return false;

    TnmsClient * client = (TnmsClient*) io->rock;

    if ( client && client->txBytesBuffered() > 0 )
        return true;

    return false;
}


} // namespace 


//  _TNMSD_AGENTIOHANDLER_CPP_ 
