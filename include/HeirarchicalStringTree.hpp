/**
  * @file HeirarchicalStringTree.hpp
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
#ifndef _TCANETPP_HEIRARCHICALSTRINGTREE_HPP_
#define _TCANETPP_HEIRARCHICALSTRINGTREE_HPP_

#include <string>
#include <map>
#include <list>
#include <vector>


namespace tcanetpp {


template < typename ValueType >
class HeirarchicalStringTreeNode {
  public:

    typedef HeirarchicalStringTreeNode<ValueType>   TreeNode;
    typedef std::map<std::string, TreeNode*>        TreeNodeMap;
    typedef typename TreeNodeMap::iterator          TreeNodeMapIter;

  public:

    HeirarchicalStringTreeNode ( const std::string & nodeName,
                                 TreeNode          * parent,
                                 char                delimiter = '/' );

    virtual ~HeirarchicalStringTreeNode();


    const std::string&   getName() const;
    std::string          getAbsoluteName() const;

    ValueType&           getValue();
    const ValueType&     getValue() const;

    TreeNode*            getParent();
    const TreeNode*      getParent() const;

    TreeNodeMap&         getChildren();
    const TreeNodeMap&   getChildren() const;

  private:

    ValueType           _value;
    std::string         _nodeName;
    TreeNode *          _parent;
    char                _delim;
    TreeNodeMap         _children;

};


template < typename ValueType >
class HeirarchicalStringTree {

  public:

    typedef HeirarchicalStringTreeNode<ValueType>   Node;
    typedef typename Node::TreeNodeMap              NodeMap;
    typedef typename Node::TreeNodeMapIter          NodeMapIter;

    typedef std::vector<std::string>                StringList;
    typedef typename StringList::iterator           StringListIter;
    typedef std::list<Node*>                        BranchNodeList;
    typedef typename BranchNodeList::iterator       BranchNodeListIter;

  public:

    HeirarchicalStringTree ( char delimiter = '/' );
    virtual ~HeirarchicalStringTree();


    char            getDelimiter() const;
    NodeMap&        getRoots();
    const NodeMap&  getRoots() const;


    Node*           find   ( const std::string & absoluteName );


    template<typename OutputIterator_>
    Node*           insert ( const std::string & absoluteName,
                             OutputIterator_ outIter ) noexcept(false);


    template<typename OutputIterator_>
    bool            erase  ( const std::string & absoluteName,
                             OutputIterator_ outIter );

    template<typename OutputIterator_>
    bool            erase  ( Node * node,
                             OutputIterator_ outIter );

    int             size()  const;
    bool            empty() const;
    void            clear();


    template<typename Predicate_>
    void            depthFirstTraversal ( Node       * node,
                                          Predicate_ & predicate );


    template<typename BranchIterator_, typename OutputIterator_>
    bool            branchToNodes   ( BranchIterator_  begin,
                                      BranchIterator_  end,
                                      OutputIterator_  outIter );

  public:

    struct BreadthOrderingFunctor
    {
        BranchNodeList  nodes;
        void operator() ( Node * node ) { nodes.push_front(node); }
    };

    struct DepthOrderingFunctor
    {
        BranchNodeList  nodes;
        void operator() ( Node * node ) { nodes.push_back(node); }
    };


    
    template< typename OutputIterator_ >
    static inline void   Split            ( const std::string  & str,
                                            const char           delimiter,
                                            OutputIterator_      outI )
    {
        std::string::size_type  begin = 0, end = 0;

        while ( (begin = str.find_first_not_of(delimiter, begin)) != std::string::npos )
        {
            end     = str.find_first_of(delimiter, begin);
            *outI++ = str.substr(begin, end - begin);
            begin   = end;
        }
    }

    template< typename OutputIterator_ >
    static inline void   Split            ( const std::string  & str,
                                            const std::string  & delimiter,
                                            OutputIterator_      outI )
    {
        std::string::size_type  begin = 0, end = 0;

        while ( (begin = str.find_first_not_of(delimiter, begin)) != std::string::npos )
        {
            end     = str.find_first_of(delimiter, begin);
            *outI++ = str.substr(begin, end - begin);
            begin   = end;
        }
    }


  private:

    HeirarchicalStringTree ( const HeirarchicalStringTree & tree );
    HeirarchicalStringTree&  operator= ( const HeirarchicalStringTree & tree );

  private:

    NodeMap             _roots;
    char                _delim;
    int                 _size;
};


}  // namespace

#include "HeirarchicalStringTree.hxx"

#endif // _TCANETPP_HEIRARCHICALSTRINGTREE_HPP_
