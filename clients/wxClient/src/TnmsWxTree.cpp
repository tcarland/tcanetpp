#define _TNMSWXTREE_CPP_

#include <iostream> 

#include "TnmsWxTree.h"
#include "ClientSubscriber.h"

#include "LogFacility.h"
using namespace tcanetpp;


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------



// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

TnmsWxTree::TnmsWxTree()
{
    this->Init();
}

TnmsWxTree::TnmsWxTree ( wxWindow        * parent,
                         const wxWindowID  id,
                         const wxString  & name,
                         const wxPoint   & pos,
                         const wxSize    & size,
                         long              style )
{
    this->Init();
    this->Create(parent, id, name, pos, size, style);
}


TnmsWxTree::~TnmsWxTree()
{
    delete _treeCtrl;
    delete _visible;
}

// ----------------------------------------------------------------------

void
TnmsWxTree::Init()
{
    _rootId  = 0;
    _visible = new TreeItemMap();
}


bool
TnmsWxTree::Create  ( wxWindow        * parent,
                      const wxWindowID  id,
                      const wxString  & name,
                      const wxPoint   & pos,
                      const wxSize    & size,
                      long              style )
{
    if ( ! wxControl::Create(parent, id, pos, size, style, wxDefaultValidator, name) )
        return false;

    long treeStyle  = wxTR_HAS_BUTTONS;
    treeStyle      |= wxTR_HIDE_ROOT;
    treeStyle      |= style;

    _treeCtrl = new wxTreeCtrl(parent, TNMS_ID_WXTREE, pos, size, treeStyle);

    wxString         rootName = wxT("/");
    TnmsWxTreeItem * rootData = new TnmsWxTreeItem("/");

    _rootId = _treeCtrl->AddRoot(rootName, -1, -1, rootData);
    _treeCtrl->SetItemHasChildren(_rootId);
    
    this->Expand(_rootId);
    SetInitialSize(size);
    DoResize();

    Connect(TNMS_ID_TREE, wxEVT_SIZE, wxSizeEventHandler(TnmsWxTree::OnSize));

    return true;
}

// ----------------------------------------------------------------------

TnmsWxTreeItem*
TnmsWxTree::GetItemData ( wxTreeItemId  id )
{
    return( (TnmsWxTreeItem*) _treeCtrl->GetItemData(id) );
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void
TnmsWxTree::OnExpandItem ( wxTreeEvent & event )
{
    /*
    wxTreeItemId  parentId = event.GetItem();

    if ( ! _rootId.IsOk() )
        _rootId = _treeCtrl->GetRootItem();
    */
    LogFacility::LogMessage("TnmsWxTree::OnExpandItem");

    //return this->Expand(parentId);
}

// ----------------------------------------------------------------------

void
TnmsWxTree::OnCollapseItem ( wxTreeEvent & event )
{
    LogFacility::LogMessage("TnmsWxTree::OnCollapseItem");
    return this->Collapse(event.GetItem());
}

// ----------------------------------------------------------------------

void
TnmsWxTree::OnSize ( wxSizeEvent & WXUNUSED(event) )
{
    this->DoResize();
}

// ----------------------------------------------------------------------

void
TnmsWxTree::OnSelect ( wxTreeEvent & event )
{
    wxTreeItemId     id   = event.GetItem();
    TnmsWxTreeItem * data = (TnmsWxTreeItem*) _treeCtrl->GetItemData(id);

    LogFacility::LogMessage("OnSelect " + data->absoluteName);

    return;
}

// ----------------------------------------------------------------------

void
TnmsWxTree::OnDelete ( wxTreeEvent & event )
{
    LogFacility::LogMessage("TnmsWxTree::OnDelete ");
}

// ----------------------------------------------------------------------

void
TnmsWxTree::OnContext ( wxTreeEvent & event )
{
    wxTreeItemId     id   = event.GetItem();
    TnmsWxTreeItem * data = (TnmsWxTreeItem*) _treeCtrl->GetItemData(id);

    LogFacility::LogMessage("OnContext " + data->absoluteName);
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void
TnmsWxTree::Expand ( wxTreeItemId  parentId )
{

}

// ----------------------------------------------------------------------

void
TnmsWxTree::Collapse ( wxTreeItemId  parentId )
{

}

// ----------------------------------------------------------------------

void
TnmsWxTree::DoResize()
{
    wxSize sz = GetClientSize();
    _treeCtrl->SetSize(0, 0, sz.x, sz.y);
}

// ----------------------------------------------------------------------

TnmsMetric&
TnmsWxTree::GetItemMetric ( wxTreeItemId & id )
{
    TnmsWxTreeItem * data = this->GetItemData(id);

    Node * node = _visible->find(data->absoluteName);

    return node->getValue().metric;
}

// ----------------------------------------------------------------------

wxTreeItemId 
TnmsWxTree::RecursiveAdd ( Node * node )
{
    wxString         name;
    wxTreeItemId     id;
    TnmsWxTreeItem * data = NULL;

    if ( ! node->getParent() )
        id = _rootId;
    else if ( node->getParent()->getValue().id.IsOk() )
        id = node->getParent()->getValue().id;
    else
        id = this->RecursiveAdd(node->getParent());

    name  = wxString::FromAscii(node->getName().c_str());
    data  = new TnmsWxTreeItem(node->getAbsoluteName());
    id    = _treeCtrl->AppendItem(id, name, -1, -1, data);

    node->getValue().id = id;

    if ( id.IsOk())
        _treeCtrl->SetItemHasChildren(id);

    return id;
}

// ----------------------------------------------------------------------


void
TnmsWxTree::Add ( const std::string & name )
{
    NodeSet       adds;
    Node        * node  = NULL;

    node = _visible->insert(name, std::inserter(adds, adds.begin()));
 
    if ( node == NULL )
        return;

    this->RecursiveAdd(node);
}


// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void
TnmsWxTree::Sync()
{
    ClientSubscriber * notifier = _stree->notifier;

    if ( ! notifier->haveUpdates() && ! _visible->empty() )
        return;

    if ( ! notifier->trylock() )
        return;

    if ( _visible->empty() )
        _stree->tree->subscribe("/", _stree->notifier);

    LogFacility::LogMessage("TnmsWxTree::SyncTree() ");

    TreeUpdateSet::iterator  nIter;
    TreeRemoveSet::iterator  rIter;

    Node *           tnode;
    wxString         name;
    wxTreeItemId     pid, id;

    // Adds
    TreeUpdateSet & adds = notifier->adds;
    for ( nIter = adds.begin(); nIter != adds.end(); )
    {
        TnmsTree::Node * node = *nIter;

        if ( node->getValue().erase ) {
            adds.erase(nIter++);
            continue;
        }

        tnode  = _visible->find(node->getAbsoluteName());

        if ( tnode != NULL ) {
            pid   = tnode->getValue().id;
            
            tnode->getValue().metric = node->getValue().metric;

            LogFacility::LogMessage("Add " + node->getAbsoluteName());
        } else {
            this->Add(node->getAbsoluteName());
        }

        adds.erase(nIter++);
    }

    TreeRemoveSet & removes = notifier->removes;
    for ( rIter = removes.begin(); rIter != removes.end(); ) 
    {
        tnode = _visible->find(*rIter);

        if ( tnode != NULL ) 
        {
            StringSet  rems;

            pid = tnode->getValue().id;
            _treeCtrl->Delete(pid);
            
            _visible->erase(*rIter, std::inserter(rems, rems.begin()));
        }

        removes.erase(rIter++);
    }

    TreeUpdateSet & updates = notifier->updates;
    for ( nIter = updates.begin(); nIter != updates.end(); ) 
    {
        TnmsTree::Node * node = *nIter;

        LogFacility::LogMessage("Update " + node->getAbsoluteName());

        tnode  = _visible->find(node->getAbsoluteName());

        if ( tnode == NULL ) {
            this->Add(node->getAbsoluteName());
        } else {
            tnode->getValue().metric = node->getValue().metric;
        }

        updates.erase(nIter++);
    }

    _stree->tree->debugDump();
    _stree->notifier->unlock();

    return;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

void
TnmsWxTree::SetTnmsTree ( TnmsTree_R * tree )
{
    _stree = tree;

    if ( tree == NULL )
        return;

    if ( _stree->mutex->lock() )
    {
        _stree->tree->subStructure((TnmsSubscriber*) _stree->notifier);
        _stree->mutex->unlock();
    }

    return;
}


TnmsTree_R*
TnmsWxTree::GetTnmsTree()
{
    return _stree;
}

// ----------------------------------------------------------------------
// _TNMSWXTREE_CPP_

