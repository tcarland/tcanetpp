/**
  * @file FileStat.cpp
  *
  * @section LICENSE
  *
  * Copyright (c) 2001-2025 Timothy Charlton Arland <tcarland@gmail.com>
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
#define _TCANETPP_FILESTAT_CPP_

#include <fstream>

#include "Exception.hpp"
#include "util/FileStat.h"
#include "util/FileUtils.h"
#include "util/StringUtils.h"


namespace tcanetpp {


FileStat::FileStat() {}

FileStat::FileStat ( const std::string & filename )
  : _file(filename)
{
    if ( ! FileUtils::InitFileStat(_file, &_statb) )
        throw Exception("Failure performing ::stat on file: " + filename);
}


bool
FileStat::operator< ( const FileStat & fs ) const
{
    return(this->_file < fs.getFileName());
}


std::string
FileStat::getFileName() const
{
    std::string name;
    int indx = StringUtils::LastIndexOf(_file, FileUtils::FS);

    if ( indx >= 0 )
        name = _file.substr(indx+1);
    else
        name = _file;

    return name;
}

std::string
FileStat::getAbsoluteName() const
{
    return _file;
}

std::string
FileStat::getPathName() const
{
    std::string path;
    int indx = StringUtils::LastIndexOf(_file, FileUtils::FS);

    if ( indx >= 0 )
        path = _file.substr(0, indx+1);

    return path;
}

bool
FileStat::isReadable() const
{
    std::ifstream ifs(_file.c_str());

    if ( ! ifs )
        return false;
    ifs.close();

    return true;
}

bool
FileStat::isSymlink() const
{
#   ifndef WIN32
    if ( S_ISLNK(_statb.st_mode) )
        return true;
#   endif
    return false;
}

bool
FileStat::isDirectory()  const
{
#   ifdef WIN32
    if ( S_IFDIR & _statb.st_mode )
        return true;
#   else
    if ( S_ISDIR(_statb.st_mode) )
        return true;
# endif
    return false;
}

bool
FileStat::isBlockDevice() const
{
#   ifndef WIN32
    if ( S_ISBLK(_statb.st_mode) )
        return true;
#   endif
    return false;
}

bool
FileStat::isCharDevice() const
{
#   ifndef WIN32
    if ( S_ISCHR(_statb.st_mode) )
        return true;
#   endif
    return false;
}

bool
FileStat::isPipe() const
{
#   ifndef WIN32
    if ( S_ISFIFO(_statb.st_mode) )
        return true;
#   endif
    return false;
}


bool
FileStat::isSocket() const
{
#   ifndef WIN32
    if ( S_ISFIFO(_statb.st_mode) )
        return true;
#   endif
    return false;
}

/** Return timestamp of last modification */
time_t
FileStat::lastTouched() const
{
    return _statb.st_mtime;
}

/** Return timestamp of last access */
time_t
FileStat::lastAccessed() const
{
    return _statb.st_atime;
}

/**  Returns the owner's uid. */
uid_t
FileStat::uidOwner() const
{
    return _statb.st_uid;
}

/**  Returns the owner's group id. */
gid_t
FileStat::gidOwner() const
{
    return _statb.st_gid;
}

/** Returns the file size in bytes */
size_t
FileStat::size() const
{
    return((size_t)_statb.st_size);
}

/** Returns the total block size used by the file
  * basically blockCount() * blockSize()
 **/
size_t
FileStat::blocks() const
{
    return(this->blockCount() * this->blockSize());
}

/** Returns the number of disk blocks used by the file */
size_t
FileStat::blockCount() const
{
    return((size_t)_statb.st_blocks);
}

/** Returns the size of one block for the underlying filesystem */
size_t
FileStat::blockSize() const
{
    return((size_t)_statb.st_blksize);
}

}  // namespace


// _TCANETPP_FILESTAT_CPP_
