/**
  * @file StringUtils.h
  *
  *    Static utility methods for c++ string manipulation of both ascii
  *  and wide char strings.
  *
  * Copyright (c) 2002,2008 Timothy Charlton Arland 
  * @author tca@charltontechnology.net
  *
  * @section LICENSE
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


#include "tcanetpp_config.h"


namespace tcanetpp {


#define TCANET_SMLSTRLINE  256
#define TCANET_MEDSTRLINE  1024
#define TCANET_BIGSTRLINE  2048
#define TCANET_MAXSTRLINE  4096


/**   A class of static methods for convenient string manipulation. */
class StringUtils {

  public:

    static bool          equals           ( const std::string & strA, 
                                            const std::string & strB );

    static bool          equalsIgnoreCase ( const std::string & strA, 
                                            const std::string & strB );

    static bool          startsWith       ( const std::string & str,  
                                            const std::string & prefix );

    static bool          endsWith         ( const std::string & str,  
                                            const std::string & suffix );

    static int           indexOf          ( const std::string & str,  
                                            const std::string & match );

    static int           indexOf          ( const std::string & str,  
                                            const std::string & match, 
                                            size_t from );

    static int           lastIndexOf      ( const std::string & str,  
                                            const std::string & match );

    static int           lastIndexOf      ( const std::string & str,  
                                            const std::string & match, 
                                            size_t from );

    static std::string   charAt           ( const std::string & str, 
                                            size_t index );

    static std::string   toLowerCase      ( const std::string & str );
    static void          toLowerCase      ( std::string & str );

    static std::string   toUpperCase      ( const std::string & str );
    static void          toUpperCase      ( std::string & str );

    static std::string   trim             ( const std::string & str );
    static void          trim             ( std::string & str );

    static void          strip            ( std::string & str, 
                                            const std::string & remove );
    static void          strip            ( std::string & str, char c );

    static void          stripComments    ( std::string & strline );
    static void          replaceTabs      ( std::string & strline );

    /*  Note: the to/fromString functions do NOT work with (u)int8_t 
     *  types since these are often typedef'd from (unsigned)char 
     *  and thus get manipulated differently by stringstream.
     */
    template<typename T>
    static inline std::string toString    ( const T & a )
    {
        std::stringstream  stream;
        stream << a;
        return stream.str();
    }

    template<typename T>
    static inline  T     fromString       ( const std::string & str )
    {
        T target = T();
        std::stringstream stream(str);
        stream >> target;
        return target;
    }

    template< typename OutputIterator_ >
    static inline void   split            ( const std::string  & str, 
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
    static inline void   split            ( const std::string  & str,
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


#ifdef TCANETPP_WIDECHAR
// ----------------------------------------------------------------------
//  The same interface is provided for wide chars and is enabled by the
//  following compile-time definition.

    static bool          equals           ( const std::wstring & wstrA, 
                                            const std::wstring & wstrB );

    static bool          equalsIgnoreCase ( const std::wstring & wstrA, 
                                            const std::wstring & wstrB );

    static bool          startsWith       ( const std::wstring & wstr,  
                                            const std::wstring & prefix );

    static bool          endsWith         ( const std::wstring & wstr,  
                                            const std::wstring & suffix );

    static int           indexOf          ( const std::wstring & wstr,  
                                            const std::wstring & match );

    static int           indexOf          ( const std::wstring & wstr,  
                                            const std::wstring & match, 
                                            size_t from );

    static int           lastIndexOf      ( const std::wstring & wstr,  
                                            const std::wstring & match );

    static int           lastIndexOf      ( const std::wstring & wstr,  
                                            const std::wstring & match, 
                                            size_t from );

    static std::wstring  charAt           ( const std::wstring & wstr, size_t index );
    static std::wstring  toLowerCase      ( const std::wstring & wstr );
    static void          toLowerCase      ( std::wstring & wstr );
    static std::wstring  toUpperCase      ( const std::wstring & wstr );
    static void          toUpperCase      ( std::wstring & wstr );
    static std::wstring  trim             ( const std::wstring & wstr );
    static void          trim             ( std::wstring & wstr );
    static void          strip            ( std::wstring & wstr, const std::wstring & rem );
    static void          strip            ( std::wstring & wstr, wchar_t c );
   
    static void          stripComments    ( std::wstring & wstrline );
    static void          replaceTabs      ( std::wstring & wstrline );

    static std::wstring  ctowstr          ( const std::string  & wstr );
    static std::string   wtocstr          ( const std::wstring & wstr );

    template<typename T>
    static inline std::wstring toWString   ( const T & a )
    {
        std::wstringstream  stream;
        stream << a;
        return stream.str();
    }

    template<typename T>
    static inline  T     fromWString       ( const std::wstring & wstr )
    {
        T target = T();
        std::wstringstream stream(wstr);
        stream >> target;
        return target;
    }


    template< typename OutputIterator_ >
    static inline void   split            ( const std::wstring  & wstr, 
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
    static inline void   split            ( const std::wstring  & str,
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

#endif  // TCANET_WIDECHAR


}; // class StringUtils

 
}  // namespace

#endif  // _TCANETPP_STRINGUTILS_H_

