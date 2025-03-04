/**
  * @file FileUtils.cpp
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
#define _TCANETPP_FILEUTILS_CPP_


extern "C" {
#ifndef WIN32
#  include <dirent.h>
#  include <unistd.h>
#endif
}

#include <fstream>


#include "util/FileUtils.h"
#include "util/StringUtils.h"


namespace tcanetpp {

std::string FileUtils::FS = "/";

bool
FileUtils::InitFileStat ( const std::string & file, filestat_t * fsb )
{
    bool result = true;

#   ifdef WIN32
    if ( ::_stat(file.c_str(), fsb) != 0 )
        result = false;
#   else
    if ( ::stat(file.c_str(), fsb) != 0 )
        result = false;
#   endif

    return result;
}


bool
FileUtils::IsReadable ( const std::string & filename )
{
    std::ifstream ifn(filename.c_str());
    if ( ! ifn )
        return false;
    ifn.close();
    return true;
}


time_t
FileUtils::LastTouched ( const std::string & filename )
{
    filestat_t  sb;

    if ( ! FileUtils::InitFileStat(filename, &sb) )
        return 0;

    return sb.st_mtime;
}

bool
FileUtils::IsDirectory ( const std::string & dirname )
{
    filestat_t  sb;

    if ( ! FileUtils::InitFileStat(dirname, &sb) )
        return false;

#   ifdef WIN32
    if ( S_IFDIR & sb.st_mode )
        return true;
#   else
    if ( S_ISDIR(sb.st_mode) )
        return true;
#   endif

    return false;
}


bool
FileUtils::IsSymlink ( const std::string & filename )
{
    filestat_t  sb;

    if ( ! FileUtils::InitFileStat(filename, &sb) )
        return false;

#   ifndef WIN32
    if ( S_ISLNK(sb.st_mode) )
        return true;
#   endif

    return false;
}

bool
FileUtils::IsBlockDevice ( const std::string & filename )
{
    filestat_t  sb;

    if ( ! FileUtils::InitFileStat(filename, &sb) )
        return false;

#   ifndef WIN32
    if ( S_ISBLK(sb.st_mode) )
        return true;
#   endif

    return false;
}


bool
FileUtils::GetFilenames ( const std::string & path, FileNameList & files, bool recursive )
{
#ifdef WIN32
    WIN32_FIND_DATA  findData;
    HANDLE           fileHandle;
    int              flag = 1;
    std::string      search ("*.*");

    search = path + FS + search;

    fileHandle = FindFirstFile(search.c_str(), &findData);

    if (fileHandle == INVALID_HANDLE_VALUE)
        return false;

    while ( flag ) {
        std::string dname = findData.cFileName;

        if ( dname.compare(".") == 0 || dname.compare("..") == 0 ) {
            flag = FindNextFile(fileHandle, &findData);
            continue;
        }

        dname = path + FS + dname;

        if ( FileUtils::IsDirectory(dname.c_str()) ) {
            if ( recursive )
                FileUtils::GetFilenames(dname, files, true);
        } else {
            files.push_back(dname);
        }

        flag = FindNextFile(fileHandle, &findData);
    }

    FindClose(fileHandle);

    return true;

#else  /* UNIX */

    DIR*            dirp;
    struct dirent*  dirEntry;

    if ( (dirp = ::opendir(path.c_str())) == NULL )
        return false;

    while ( (dirEntry = ::readdir(dirp)) != NULL ) {
        std::string dname = dirEntry->d_name;

        if ( dname.compare(".") == 0 || dname.compare("..") == 0 )
            continue;

        dname = path + FS + dname;

        if ( FileUtils::IsDirectory(dname) ) {
            if ( recursive )
                FileUtils::GetFilenames(dname, files, true);
        } else {
            files.push_back(dname);
        }
    }

    ::closedir(dirp);
#endif

    return true;
}


/** GetFilename returns only the last string in a typical filesytem
  * path separated filename string.
  *
  * @param fqfn  - Fully qualified filename. eg. '/home/USER/.bashrc'
 **/
std::string
FileUtils::GetFilename ( const std::string & fqfn )
{
    std::string filename;
    int indx = StringUtils::LastIndexOf(fqfn, FS);

    if ( indx >= 0 )
        filename = fqfn.substr(indx);

    return filename;
}


/** GetPathname returns the path portion of a typical filesystem 
  * path separated filename.
  * 
  * @param fqfn  - a fully qualified filename
 **/
std::string
FileUtils::GetPathname ( const std::string & fqfn )
{
    std::string pathname;
    int indx = StringUtils::LastIndexOf(fqfn, FS);

    if ( indx >= 0 )
        pathname = fqfn.substr(0, indx + 1);

    return pathname;
}
    

std::string
FileUtils::GetCurrentPath()
{
    std::string  path;
    char         pname[TCANET_MEDSTRLINE];
    size_t       psz = TCANET_MEDSTRLINE;

#   ifdef WIN32
#   else
    if ( ::getcwd(&pname[0], psz) == NULL )
        return path;

    path.assign(pname);
#   endif

    return path;
}


}  // namespace
