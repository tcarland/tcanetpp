/**
  * @file StringUtils.h
  *
  * Static utility methods for c++ string manipulation of both ascii
  * and wide char strings.
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
#ifndef _TCANETPP_STRINGUTILS_H_
#define _TCANETPP_STRINGUTILS_H_

#include <string.h>
#include <string>
#include <sstream>


#include "tcanetpp_types.h"


namespace tcanetpp {

// line length limits
#define TCANET_TINYSTRLINE  32
#define TCANET_SMALLSTRLINE 256
#define TCANET_MEDSTRLINE   4096
#define TCANET_BIGSTRLINE   8192
#define TCANET_MAXSTRLINE   16384


/**   A class of static methods for convenient string manipulation. */
class StringUtils {

  public:

    static bool          Equals           ( const std::string & stra,
                                            const std::string & strb );

    static bool          EqualsIgnoreCase ( const std::string & stra,
                                            const std::string & strb );

    static bool          StartsWith       ( const std::string & str,
                                            const std::string & prefix );

    static bool          EndsWith         ( const std::string & str,
                                            const std::string & suffix );

    static int           IndexOf          ( const std::string & str,
                                            const std::string & match );

    static int           IndexOf          ( const std::string & str,
                                            const std::string & match,
                                            size_t from );

    static int           LastIndexOf      ( const std::string & str,
                                            const std::string & match );

    static int           LastIndexOf      ( const std::string & str,
                                            const std::string & match,
                                            size_t from );

    static std::string   CharAt           ( const std::string & str,
                                            size_t index );

    static std::string   ToLowerCase      ( const std::string & str );
    static void          ToLowerCase      ( std::string & str );

    static std::string   ToUpperCase      ( const std::string & str );
    static void          ToUpperCase      ( std::string & str );

    static std::string   ToHexString      ( const uint8_t * buf,
                                            size_t len, size_t offset );

    static std::string   Trim             ( const std::string & str );
    static void          Trim             ( std::string & str );

    static void          Strip            ( std::string & str,
                                            const std::string & remove );
    static void          Strip            ( std::string & str, char c );

    static void          StripComments    ( std::string & strline );
    static void          ReplaceTabs      ( std::string & strline );


    /*  Note: the to/fromString functions do NOT work with (u)int8_t
     *  types since these are typedef'd from an unsigned char
     *  and thus get manipulated differently by stringstream.
     */
    template<typename T>
    static inline std::string ToString    ( const T & a )
    {
        std::stringstream  stream;
        stream << a;
        return stream.str();
    }

    template<typename T>
    static inline  T     FromString       ( const std::string & str )
    {
        T target = T();
        std::stringstream stream(str);
        stream >> target;
        return target;
    }

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


    static const size_t  MAXLINE;


// ----------------------------------------------------------------------
//  The same interface is provided for wide chars 

    static bool          Equals           ( const std::wstring & wstra,
                                            const std::wstring & wstrb );

    static bool          EqualsIgnoreCase ( const std::wstring & wstra,
                                            const std::wstring & wstrb );

    static bool          StartsWith       ( const std::wstring & wstr,
                                            const std::wstring & prefix );

    static bool          EndsWith         ( const std::wstring & wstr,
                                            const std::wstring & suffix );

    static int           IndexOf          ( const std::wstring & wstr,
                                            const std::wstring & match );

    static int           IndexOf          ( const std::wstring & wstr,
                                            const std::wstring & match,
                                            size_t from );

    static int           LastIndexOf      ( const std::wstring & wstr,
                                            const std::wstring & match );

    static int           LastIndexOf      ( const std::wstring & wstr,
                                            const std::wstring & match,
                                            size_t from );

    static std::wstring  CharAt           ( const std::wstring & wstr, size_t index );
    static std::wstring  ToLowerCase      ( const std::wstring & wstr );
    static void          ToLowerCase      ( std::wstring & wstr );
    static std::wstring  ToUpperCase      ( const std::wstring & wstr );
    static void          ToUpperCase      ( std::wstring & wstr );
    static std::wstring  Trim             ( const std::wstring & wstr );
    static void          Trim             ( std::wstring & wstr );
    static void          Strip            ( std::wstring & wstr, const std::wstring & rem );
    static void          Strip            ( std::wstring & wstr, wchar_t c );

    static void          StripComments    ( std::wstring & wstrline );
    static void          ReplaceTabs      ( std::wstring & wstrline );

    static std::wstring  CtoWstr          ( const std::string  & wstr );
    static std::string   WtoCstr          ( const std::wstring & wstr );


    template<typename T>
    static inline
    std::wstring         ToWString        ( const T & a )
    {
        std::wstringstream  stream;
        stream << a;
        return stream.str();
    }

    template<typename T>
    static inline  T     FromWString      ( const std::wstring & wstr )
    {
        T target = T();
        std::wstringstream stream(wstr);
        stream >> target;
        return target;
    }


    template< typename OutputIterator_ >
    static inline void   Split            ( const std::wstring  & wstr,
                                            char                  delimiter,
                                            OutputIterator_       outI )
    {
        std::wstring::size_type  begin = 0, end = 0;

        while ( (begin = wstr.find_first_not_of(delimiter, begin)) != std::wstring::npos )
        {
            end     = wstr.find_first_of(delimiter, begin);
            *outI++ = wstr.substr(begin, end - begin);
            begin   = end;
        }
    }


    template< typename OutputIterator_ >
    static inline void   Split            ( const std::wstring  & str,
                                            const std::wstring  & delimiter,
                                            OutputIterator_       outI )
    {
        std::wstring::size_type  begin = 0, end = 0;

        while ( (begin = str.find_first_not_of(delimiter, begin)) != std::wstring::npos )
        {
            end     = str.find_first_of(delimiter, begin);
            *outI++ = str.substr(begin, end - begin);
            begin   = end;
        }
    }

}; // class StringUtils

}  // namespace

#endif  // _TCANETPP_STRINGUTILS_H_
