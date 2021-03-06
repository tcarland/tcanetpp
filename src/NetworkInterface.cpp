/**
  * @file NetworkInterface.cpp
  *
  * @section LICENSE
  *
  * Copyright (c) 2008-2018 Timothy Charlton Arland <tcarland@gmail.com?
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
#define _TCANETPP_NETWORKINTERFACE_CPP_

#include "net/NetworkInterface.h"
#include "util/StringUtils.h"


namespace tcanetpp {


NetworkInterface::NetworkInterface()
    : _device_ip(),
      _if_addr(),
      _if_index(0)
{}


NetworkInterface::NetworkInterface ( const IpAddr & deviceip, int indx )
    : _device_ip(deviceip),
      _if_addr(),
      _if_index(indx)
{}


NetworkInterface::NetworkInterface ( const NetworkInterface & nif )
    : _device_ip(nif._device_ip),
      _if_addr(nif._if_addr),
      _if_name(nif._if_name),
      _description(nif._description),
      _vlan(nif._vlan),
      _if_index(nif._if_index),
      _id(nif._id)
{}


NetworkInterface::~NetworkInterface() {}


void
NetworkInterface::operator= ( const NetworkInterface & nif )
{
    _device_ip   = nif._device_ip;
    _if_addr     = nif._if_addr;
    _if_name     = nif._if_name;
    _description = nif._description;
    _vlan        = nif._vlan;
    _if_index    = nif._if_index;
    _id          = nif._id;
}


bool
NetworkInterface::operator== ( const NetworkInterface & nif )
{
    return(_if_addr == nif._if_addr && _if_index == nif._if_index);
}


bool
NetworkInterface::operator< ( const NetworkInterface & nif )
{
    if ( _if_index > 0 && nif._if_index > 0 )
        return(_if_index < nif._if_index);
    return(_if_addr < nif._if_addr);
}


int
NetworkInterface::ifIndex() const
{
    return this->_if_index;
}


void
NetworkInterface::ifName ( const std::string & ifname )
{
    this->_if_name = ifname;
}

std::string
NetworkInterface::ifName() const
{
    return this->_if_name;
}


void
NetworkInterface::description ( const std::string & desc )
{
    this->_description = StringUtils::Trim(desc);
}

const std::string&
NetworkInterface::description() const
{
    return this->_description;
}


const IpAddr&
NetworkInterface::deviceAddr() const
{
    return this->_device_ip;
}


void
NetworkInterface::ifAddr ( const IpAddr & addr )
{
    this->_if_addr = addr;
}

const IpAddr&
NetworkInterface::ifAddr() const
{
    return this->_if_addr;
}


void
NetworkInterface::ifId ( int id )
{
    this->_id = id;
}

int
NetworkInterface::ifId() const
{
    return this->_id;
}


} // namespace


// _TCANETPP_NETWORKINTERFACE_CPP_
