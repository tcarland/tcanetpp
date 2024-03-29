/**
  * @file FileUtils.h
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
#ifndef _TCANETPP_FILEUTILS_H_
#define _TCANETPP_FILEUTILS_H_

#include <string>
#include <list>

#include "FileStat.h"

namespace tcanetpp {


typedef std::list< std::string >  FileNameList;


class FileUtils {

  public:

    static time_t  LastTouched    ( const std::string & filename );
    static bool    IsReadable     ( const std::string & filename );
    static bool    IsBlockDevice  ( const std::string & filename );
    static bool    IsPipe         ( const std::string & filename );
    static bool    IsDirectory    ( const std::string & dirname  );
    static bool    IsSymlink      ( const std::string & filename );

    static bool    GetFilenames   ( const std::string & path,
                                    FileNameList      & files,
                                    bool  recursive   = true );

    static 
    std::string    GetFilename    ( const std::string & fqfn );
    static
    std::string    GetPathname    ( const std::string & fqfn );

    static
    std::string    GetCurrentPath();

    static bool    InitFileStat   ( const std::string & file,
                                    filestat_t        * fsb );

  public:

    static std::string  FS;
};

}  // namespace

#endif // _TCANETPP_FILEUTILS_H_
