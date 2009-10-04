#define _CLIENTFRAME_CPP_

#include "ClientFrame.h"
#include "TnmsClientIOThread.h"

#include <wx/treectrl.h>
#include <wx/dirctrl.h>
#include <wx/dir.h>
#include <wx/splitter.h>

#include "ConnectDialog.h"

#include "StringUtils.h"
#include "LogFacility.h"
using namespace tcanetpp;



// ----------------------------------------------------------------------

// ----------------------------------------------------------------------

ClientFrame::ClientFrame ( const wxString & title, TnmsTree_R * tree )
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(950,600)),
      _stree(tree),
      _tree(NULL)
{
    wxSplitterWindow * spl1 = new wxSplitterWindow(this, -1); 
    wxSplitterWindow * spl2 = new wxSplitterWindow(spl1, -1);

    _tree   = new TnmsWxTree(spl1, TNMS_ID_TREE, wxT("tcanms"), 
                wxPoint(-1, -1), spl1->GetSize());
    _tree->SetTnmsTree(_stree);

    spl1->SplitVertically(_tree, spl2);
    
    _mlist  = new MetricListView(spl2, TNMS_ID_MLIST,  
                 wxPoint(-1, -1), wxSize(-1, -1), wxSUNKEN_BORDER);
    _lCtrl2 = new wxListCtrl(spl2, -1, 
                 wxPoint(-1, -1), wxSize(-1, -1), wxLC_LIST);
    
    spl2->SplitHorizontally(_mlist, _lCtrl2);

    //this->initEvents();
    

    //Connect(wxID_ANY, wxEVT_CONTEXT_MENU,
        //wxContextMenuEventHandler(ClientFrame::OnContextAny));
    
    //-------------------------------------------------------
    //  Tree events
    //Connect(TNMS_ID_TREE, wxEVT_SIZE,
        //wxSizeEventHandler(TnmsWxTree::OnSize));

    //Connect(TNMS_ID_WXTREE, wxEVT_COMMAND_TREE_ITEM_EXPANDED, 
        //wxCommandEventHandler(ClientFrame::OnExpandItem));
    Connect(TNMS_ID_WXTREE, wxEVT_COMMAND_TREE_ITEM_EXPANDED, 
        wxTreeEventHandler(TnmsWxTree::OnExpandItem));

    Connect(TNMS_ID_WXTREE, wxEVT_COMMAND_TREE_ITEM_COLLAPSING,
        wxTreeEventHandler(TnmsWxTree::OnCollapseItem));

    Connect(TNMS_ID_WXTREE, wxEVT_COMMAND_TREE_SEL_CHANGED,
        wxTreeEventHandler(ClientFrame::OnTreeSelect));
    //Connect(TNMS_ID_WXTREE, wxEVT_COMMAND_TREE_ITEM_ACTIVATED,
        //wxTreeEventHandler(TnmsWxTree::OnSelect));

    Connect(TNMS_ID_WXTREE, wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK,
        wxTreeEventHandler(ClientFrame::OnTreeContext));

    Connect(TNMS_ID_WXTREE, wxEVT_COMMAND_TREE_DELETE_ITEM,
        wxTreeEventHandler(TnmsWxTree::OnDelete));
    
    //-------------------------------------------------------
    //  List1 events
    Connect(TNMS_ID_MLIST, wxEVT_COMMAND_LIST_ITEM_ACTIVATED,
        wxListEventHandler(ClientFrame::OnListActivate));
    Connect(TNMS_ID_MLIST, wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,
        wxListEventHandler(ClientFrame::OnListContext));

    this->initMenuBar();

    CreateStatusBar(3);
    Center();
}


ClientFrame::~ClientFrame() {}


void
ClientFrame::initMenuBar()
{
    _menuBar   = new wxMenuBar();
    _menuFile  = new wxMenu();
    // Top-level :  File
    _menuFile->Append(new wxMenuItem(_menuFile, TNMS_ID_CONNECT, wxT("&Connect")));
    _menuFile->Append(new wxMenuItem(_menuFile, TNMS_ID_DISCONN, wxT("&Disconnect")));
    _menuFile->AppendSeparator();
    _menuFile->Append(new wxMenuItem(_menuFile, wxID_EXIT, wxT("&Quit")));
    _menuBar->Append(_menuFile, wxT("&File"));

    SetMenuBar(_menuBar);

    Connect(TNMS_ID_CONNECT, wxEVT_COMMAND_MENU_SELECTED, 
        wxCommandEventHandler(ClientFrame::OnConnect));
    Connect(TNMS_ID_DISCONN, wxEVT_COMMAND_MENU_SELECTED, 
        wxCommandEventHandler(ClientFrame::OnDisconnect));
    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, 
        wxCommandEventHandler(ClientFrame::OnQuit));
}


void
ClientFrame::OnListActivate ( wxListEvent & event )
{
    LogFacility::LogMessage("ClientFrame::OnActivate(list)");
}


void
ClientFrame::OnTreeSelect ( wxTreeEvent & event )
{
    wxTreeItemId  id  = event.GetItem();
    TnmsMetric metric = _tree->GetItemMetric(id);

    _mlist->AddMetric(metric);

    LogFacility::LogMessage("ClientFrame::OnSelect(tree)");
}


void
ClientFrame::OnTreeContext ( wxTreeEvent & event )
{
    LogFacility::LogMessage("ClientFrame::OnContext(tree) ");
    
    wxTreeItemId  id  = event.GetItem();
    wxMenu    * menu  = new wxMenu();
    wxPoint     point = wxGetMousePosition();
    
    point = ScreenToClient(point);

    menu->AppendRadioItem(wxID_ANY, wxT("Nothing"), wxT(""));
    menu->AppendRadioItem(TNMS_ID_SUBSCRIBE_ITEM, wxT("Subscribe to Item"), wxT(""));
    menu->AppendRadioItem(TNMS_ID_UNSUBSCRIBE_ITEM, wxT("Unsubscribe to Item"), wxT(""));
    
    PopupMenu(menu, point);

    if ( menu->IsChecked(TNMS_ID_SUBSCRIBE_ITEM) )
        LogFacility::LogMessage(" OnContext Subscribe");
    else if ( menu->IsChecked(TNMS_ID_UNSUBSCRIBE_ITEM) )
        LogFacility::LogMessage(" OnContext Unsubscribe");

    return;
}

void
ClientFrame::OnListContext ( wxListEvent & event )
{
    LogFacility::LogMessage("ClientFrame::OnContext(list) ");
}



void
ClientFrame::OnContextAny ( wxContextMenuEvent & event )
{
    LogFacility::LogMessage("ClientFrame::OnContextAny ");

    wxMenu  * menu  = new wxMenu();
    wxPoint   point = wxGetMousePosition();

    menu->AppendRadioItem(TNMS_ID_SUBSCRIBE_ITEM, wxT("Subscribe to Item"), wxT(""));
    menu->AppendRadioItem(TNMS_ID_SUBSCRIBE_PATH, wxT("Subscribe to Level"), wxT(""));

    PopupMenu(menu, point.x, point.y);

    return;
}


void
ClientFrame::OnConnect ( wxCommandEvent & event )
{
    ConnectDialog * dlg = new ConnectDialog(wxT("Connect"));

    if ( dlg->ShowModal() == wxID_OK ) {
        LogFacility::LogMessage("OnConnect() OK");
    } else {
        LogFacility::LogMessage("OnConnect() Cancelled");
        return;
    }

    wxString  servname = dlg->tcServer->GetValue();
    wxString  portname = dlg->tcPort->GetValue();
    wxString  username = dlg->tcUser->GetValue();
    wxString  password = dlg->tcPass->GetValue();

    Connection  cl;
    cl.servername  = StringUtils::wtocstr(servname.c_str());
    cl.username    = StringUtils::wtocstr(username.c_str());
    cl.password    = StringUtils::wtocstr(password.c_str());
    cl.port        = StringUtils::fromString<uint16_t>(StringUtils::wtocstr(portname.c_str()));
    cl.client      = new TnmsClient(_stree->tree);
    cl.enabled     = true;

    //uint16_t pn = StringUtils::fromString<uint16_t>(port);
    
    cl.client->openConnection(cl.servername, cl.port);
    _stree->iomgr->addClient(cl.client);
    
    //user.append("@").append(sn).append(":").append(port);
    username.append(_T("@")).append(servname).append(_T(":")).append(portname);
    _clientMap[username] = cl;

    dlg->Destroy();
}

void
ClientFrame::OnDisconnect ( wxCommandEvent & event )
{
    this->DropAllConnections();
}

void
ClientFrame::DropAllConnections()
{
    // for now just drop all our connections
    ClientMap::iterator  cIter;
    for ( cIter = _clientMap.begin(); cIter != _clientMap.end(); ++cIter )
    {
        Connection & clin = cIter->second;

        if ( clin.client ) {
            clin.enabled = false;
            clin.client->close();
            _stree->iomgr->removeClient(clin.client);
        }
    }
    _clientMap.clear();
}


void
ClientFrame::OnQuit ( wxCommandEvent & event )
{
    Close(true);
}


void
ClientFrame::OnExpandItem ( wxCommandEvent & event )
{
    //LogFacility::LogMessage("ClientFrame::OnExpandItem");
}


void
ClientFrame::OnCollapseItem ( wxCommandEvent & event )
{
    //LogFacility::LogMessage("ClientFrame::OnCollapseItem");
}


void
ClientFrame::OnDelete ( wxCommandEvent & event )
{
    LogFacility::LogMessage("ClientFrame::OnDelete ");
}


void
ClientFrame::OnTimer ( wxTimerEvent & event )
{    
    ClientMap::iterator  cIter;
    for ( cIter = _clientMap.begin(); cIter != _clientMap.end(); ++cIter )
    {
        Connection & clin = cIter->second;

        if ( clin.client->isAuthorized() && ! clin.req ) 
        {
            _stree->tree->request("*", _stree->notifier);
            clin.req = true;
        }
    }
    _tree->Sync();
}



