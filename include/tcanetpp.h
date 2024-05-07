/**
  * @file tcanetpp.h
  *
  * Convenience header for pulling in a few tcanetpp common headers
  * to provide an entry point for header inclusion by applications.
  *
  * @section LICENSE
  *
  * Copyright (c) 2001-2024 Timothy Charlton Arland <tcarland@gmail.com>
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
#ifndef _TCANETPP_H_
#define _TCANETPP_H_

#include "tcanetpp_types.h"
#include "tcanetpp_ip.h"
#include "tcanetpp_random.h"
#include "net/AddrInfo.h"
#include "net/IpAddr.h"
#include "net/Socket.h"
#include "event/EventManager.h"

#define TCANETPP_VERSION_TS  "24.05"
#define TCANETPP_VERSION     "1.5.7"

#endif //_TCANETPP_H_
