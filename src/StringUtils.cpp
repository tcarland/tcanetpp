/**
  * @file StringUtils.cpp
  *
  * A collection of static methods for c++ string manipulation.
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
#define _TCANETPP_STRINGUTILS_CPP_

extern "C" {
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
}
#include <cctype>
#include <iomanip>

#include "util/StringUtils.h"
#include "tcanetpp_types.h"


namespace tcanetpp {


const size_t
StringUtils::MAXLINE = TCANET_BIGSTRLINE;


// ----------------------------------------------------------------------

/**@{  Indicates whether a given string starts or ends with the provided
  *    prefix or suffix, respectively.
 **/
bool
StringUtils::StartsWith ( const std::string & str, const std::string & prefix )
{
    std::string::const_iterator  sIter, pIter;

    if ( str.size() < prefix.size() )
        return false;

    for ( sIter = str.begin(), pIter = prefix.begin();
          pIter != prefix.end();
          ++sIter, ++pIter )
    {
        if ( *sIter != *pIter )
            return false;
    }

    return true;
}

bool
StringUtils::EndsWith ( const std::string & str, const std::string & suffix )
{
    std::string::const_reverse_iterator  sIter, pIter;

    if ( str.size() < suffix.size() )
        return false;

    for ( sIter = str.rbegin(), pIter = suffix.rbegin();
          pIter != suffix.rend();
          ++sIter, ++pIter )
    {
        if ( *sIter != *pIter )
            return false;
    }

    return true;
}
/*@}*/


// ----------------------------------------------------------------------

/**  Returns a string representing the character at the provided index. */
std::string
StringUtils::CharAt ( const std::string & str, size_t index )
{
    return str.substr(index, index + 1);
}

// ----------------------------------------------------------------------

/**@{   Indicates string equality with or without considering case */
bool
StringUtils::Equals ( const std::string & stra, const std::string & strb )
{
    return( stra.compare(strb) == 0 );
}

bool
StringUtils::EqualsIgnoreCase ( const std::string & stra, const std::string & strb )
{
    std::string  a = stra;
    std::string  b = strb;

    StringUtils::ToLowerCase(a);
    StringUtils::ToLowerCase(b);

    return( a.compare(b) == 0 );
}
/*@}*/

// ----------------------------------------------------------------------

/**@{  Returns the first (or last) index of @param match in the given string,
  *    @param str or a negative value if no match.
 **/
int
StringUtils::IndexOf ( const std::string & str, const std::string & match )
{
    return StringUtils::IndexOf(str, match, 0);
}

int
StringUtils::IndexOf ( const std::string & str, const std::string & match, size_t from )
{
    std::string::size_type  indx;

    if ( from > str.length() )
        return -1;

    if ( (indx = str.find(match, from)) == std::string::npos )
        return -1;

    return( (int) indx );
}

int
StringUtils::LastIndexOf ( const std::string & str, const std::string & match )
{
    return StringUtils::LastIndexOf(str, match, str.length());
}

int
StringUtils::LastIndexOf ( const std::string & str, const std::string & match, size_t from )
{
    std::string::size_type  indx;

    if ( from > str.length() )
        from = str.length();

    if ( (indx = str.find_last_of(match, from)) == std::string::npos )
        return -1;

    return ( (int) indx );
}
/*@}*/

// ----------------------------------------------------------------------

/**@{  Transforms the provided string.  */
std::string
StringUtils::ToLowerCase ( const std::string & str )
{
    std::string  tmp = str;
    StringUtils::ToLowerCase(tmp);
    return tmp;
}


void
StringUtils::ToLowerCase ( std::string & str )
{
    std::string::size_type  i;
    for ( i = 0; i < str.length(); i++ )
        str[i] = tolower( (int) str[i] );
    return;
}


std::string
StringUtils::ToUpperCase ( const std::string & str )
{
    std::string  tmp = str;
    StringUtils::ToUpperCase(tmp);
    return tmp;
}


void
StringUtils::ToUpperCase ( std::string & str )
{
    std::string::size_type  i;
    for ( i = 0; i < str.length(); i++ )
        str[i] = toupper( (int) str[i] );
    return;
}
/*@}*/

// ----------------------------------------------------------------------

/**  Converts the provided buffer to a human readable, formatted hex string */
std::string
StringUtils::ToHexString ( const uint8_t * buf, size_t len, size_t offset )
{
    const uint8_t *ptr;
    std::string    hexl;
    char           hexc[TCANET_SMALLSTRLINE];
    char          *hx;
    size_t         i, gap, lt;
    int            rt = 0;

    lt  = TCANET_SMALLSTRLINE;
    hx  = &hexc[0];
    rt  = ::snprintf(hx, lt, "%05lu  ", offset);
    hx += rt;
    lt -= rt;
    ptr = buf;

    for ( i = 0; i < len; ++i )
    {
        rt  = ::snprintf(hx, lt, "%02x ", *ptr++);
        hx += rt;
        lt -= rt;

        if ( i == 7 ) {
            rt = ::snprintf(hx, lt, " ");
            hx += rt;
            lt -= rt;
        }
    }

    if ( len < 8 ) {
        rt  = ::snprintf(hx, lt, " ");
        hx += rt;
        lt -= rt;
    }

    if ( len < 16)  {
        gap = 16 - len;
        for ( i = 0; i < gap; ++i )  {
            rt  = ::snprintf(hx, lt, "   ");
            hx += rt;
            lt -= rt;
        }
    }

    rt  = ::snprintf(hx, lt, "   ");
    hx += rt;
    lt -= rt;
    ptr = buf;

    for ( i = 0; i < len; ++i )
    {
        if ( ::isprint(*ptr) )
            rt  = ::snprintf(hx, lt, "%c", *ptr);
        else
            rt  = ::snprintf(hx, lt, ".");

        hx += rt;
        lt -= rt;
    }

    ::snprintf(hx, lt, "\n");
    hexl.assign(hexc);

    return hexl;
}


// ----------------------------------------------------------------------

/**@{  Trims the whitespace at the front and back of the string. */
std::string
StringUtils::Trim ( const std::string & str )
{
    std::string  tmp = str;
    StringUtils::Trim(tmp);
    return tmp;
}

void
StringUtils::Trim ( std::string & str )
{
    while ( isspace(str[0]) )
        str.erase(0,1);
    while ( isspace(str[str.length() - 1]) )
        str.erase(str.length() - 1, str.length());
    return;
}
/*@}*/


// ----------------------------------------------------------------------

/**@{
  *  Strips all occurances of one string from another string.
  *  @param str is the provided string on which to operate.
  *  @param rem is the string to match and remove from the provided
  *  string.
 **/
void
StringUtils::Strip ( std::string & str, const std::string & rem )
{
    std::string::size_type indx = 0;

    while ( (indx = str.find(rem)) != std::string::npos )
        str.erase(indx, rem.length());

    return;
}

void
StringUtils::Strip ( std::string & str, char c )
{
    std::string::size_type indx = 0;

    while ( (indx = str.find_first_of(c)) != std::string::npos )
        str.erase(indx, 1);

    return;
}
/*@}*/

// ----------------------------------------------------------------------

/**  Strips inline comments from a given string line. Valid inline or
  *  to end of line comments are '#', ';', and '//'
 **/
void
StringUtils::StripComments ( std::string & strline )
{
    std::string::size_type indx = 0;

    if ( (indx = strline.find_first_of('#')) != std::string::npos )
        strline = strline.substr(0, indx);

    if ( (indx = strline.find_first_of(';')) != std::string::npos )
        strline = strline.substr(0, indx);

    if ( (indx = strline.find("//")) != std::string::npos )
        strline = strline.substr(0, indx);

    return;
}

void
StringUtils::ReplaceTabs ( std::string & strline )
{
    std::string::size_type indx = 0;
    while ( (indx = strline.find_first_of('\t')) != std::string::npos ) {
        strline.erase(indx, 1);
        strline.insert(indx, 8, ' ');
    }
    return;
}



// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//   Widechar support for the same methods including conversion from
//   wide to narrow/ascii and back.
#ifdef TCANETPP_WIDECHAR


extern "C" {
# include <wchar.h>
}

#ifdef WIN32
# pragma warning (disable:4996)
#endif


// ----------------------------------------------------------------------

bool
StringUtils::StartsWith ( const std::wstring & str, const std::wstring & prefix )
{
    std::wstring::size_type  indx;

    indx = str.find(prefix);
    if ( indx == 0 )
        return true;

    return false;
}


bool
StringUtils::EndsWith ( const std::wstring & str, const std::wstring & suffix )
{
    std::wstring::size_type  indx;

    indx = str.rfind(suffix);

    if ( indx == std::wstring::npos )
        return false;
    if ( (str.length() - indx) == suffix.length() )
        return true;

    return false;
}

// ----------------------------------------------------------------------

std::wstring
StringUtils::CharAt ( const std::wstring & str, size_t index )
{
    return str.substr(index, index + 1);
}

// ----------------------------------------------------------------------

bool
StringUtils::Equals ( const std::wstring & stra, const std::wstring & strb )
{
    return(stra.compare(strb) == 0);
}


bool
StringUtils::EqualsIgnoreCase ( const std::wstring & stra, const std::wstring & strb )
{
    std::wstring a = stra;
    std::wstring b = strb;
    StringUtils::ToLowerCase(a);
    StringUtils::ToLowerCase(b);
    return( a.compare(b) == 0 );
}

// ----------------------------------------------------------------------

int
StringUtils::IndexOf ( const std::wstring & str, const std::wstring & match )
{
    return StringUtils::IndexOf(str, match, 0);
}


int
StringUtils::IndexOf ( const std::wstring & str, const std::wstring & match, size_t from )
{
    std::wstring::size_type  indx;

    if ( from > str.length() )
        return -1;

    if ( (indx = str.find(match, from)) == std::wstring::npos )
        return -1;

    return( (int) indx );
}


int
StringUtils::LastIndexOf ( const std::wstring & str, const std::wstring & match )
{
    return StringUtils::LastIndexOf(str, match, str.length());
}


int
StringUtils::LastIndexOf ( const std::wstring & str, const std::wstring & match, size_t from )
{
    std::wstring::size_type  indx;

    if ( from > str.length() )
        from = str.length();

    if ( (indx = str.find_last_of(match)) == std::wstring::npos )
        return -1;

    return ( (int) indx );
}

// ----------------------------------------------------------------------

std::wstring
StringUtils::ToLowerCase ( const std::wstring & str )
{
    std::wstring  tmp = str;
    StringUtils::ToLowerCase(tmp);
    return tmp;
}


void
StringUtils::ToLowerCase ( std::wstring & str )
{
    for ( uint32_t i = 0; i < str.length(); i++ )
        str[i] = tolower( (int) str[i] );
    return;
}


std::wstring
StringUtils::ToUpperCase ( const std::wstring & str )
{
    std::wstring  tmp = str;
    StringUtils::ToUpperCase(tmp);
    return tmp;
}


void
StringUtils::ToUpperCase ( std::wstring & str )
{
    for ( uint32_t i = 0; i < str.length(); i++ )
        str[i] = toupper( (int) str[i] );
    return;
}

// ----------------------------------------------------------------------

std::wstring
StringUtils::Trim ( const std::wstring & str )
{
    std::wstring  tmp = str;
    StringUtils::Trim(tmp);
    return tmp;
}


void
StringUtils::Trim ( std::wstring & str )
{
    while ( isspace(str[0]) )
        str.erase(0,1);

    while ( isspace(str[str.length() - 1]) )
        str.erase(str.length() - 1, str.length());

    return;
}

// ----------------------------------------------------------------------

void
StringUtils::Strip ( std::wstring & str, const std::wstring & rem )
{
    std::wstring::size_type indx = 0;

    while ( (indx = str.find(rem)) != std::string::npos )
        str.erase(indx, str.length());

    return;
}

void
StringUtils::Strip ( std::wstring & str, wchar_t c )
{
    std::wstring::size_type indx = 0;

    while ( (indx = str.find_first_of(c)) != std::wstring::npos )
        str.erase(indx, str.length());

    return;
}


// ----------------------------------------------------------------------

/**  Strips inline comments from a given string line. Valid inline or
  *  to end of line comments are '#', ';', and '//'
 **/
void
StringUtils::StripComments ( std::wstring & wstrline )
{
    std::wstring::size_type indx = 0;

    if ( (indx = wstrline.find_first_of('#')) != std::string::npos )
        wstrline = wstrline.substr(0, indx);

    if ( (indx = wstrline.find_first_of(';')) != std::string::npos )
        wstrline = wstrline.substr(0, indx);

    if ( (indx = wstrline.find(L"//")) != std::string::npos )
        wstrline = wstrline.substr(0, indx);

    return;
}

void
StringUtils::ReplaceTabs ( std::wstring & wstrline )
{
    std::wstring::size_type indx = 0;

    if ( (indx = wstrline.find_first_of('\t')) != std::string::npos ) {
        wstrline.erase(indx, 1);
        wstrline.insert(indx, 8, ' ');
    }

    return;
}

// ----------------------------------------------------------------------

/**@{  Provides conversion between ascii and wide strings  */
std::wstring
StringUtils::CtoWstr ( const std::string & str )
{
    std::wstring  wstr = L"";
    wchar_t      *tmp  = NULL;

    size_t slen = ::strlen(str.c_str());
    size_t wlen = ::mbstowcs(NULL, str.c_str(), slen) + 1;

    tmp  = (wchar_t*) ::malloc(wlen * sizeof(wchar_t*));
    wlen = ::mbstowcs(tmp, str.c_str(), slen + 1);

    if ( tmp == NULL )
        return wstr;

    wstr = std::wstring(tmp);
    ::free(tmp);

    return wstr;
}


std::string
StringUtils::WtoCstr ( const std::wstring & wstr )
{
    std::string  str = "";
    char        *tmp = NULL;

    size_t wlen = ::wcslen(wstr.c_str());
    size_t slen = ::wcstombs(NULL, wstr.c_str(), wlen) + 1;

    tmp  = (char*) ::malloc(slen * sizeof(char*));
    slen = ::wcstombs(tmp, wstr.c_str(), wlen+1);

    if ( tmp == NULL )
        return str;

    str = std::string(tmp);
    free(tmp);

    return str;
}
/*@}*/

// ----------------------------------------------------------------------

#endif  // TCANETPP_WIDECHAR


} // namespace


// _TCANETPP_STRINGUTILS_CPP_
