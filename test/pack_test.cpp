
#include <cstdlib>
#include <iostream>
#include <string>


#include "Serializer.h"
using namespace tcanetpp;



int main ()
{
    const char * strA = "foo1";
    std::string  strB = "foo2";
    int16_t      intA = 1;
    uint32_t     intB = 2;
    char *       buff = NULL;
    
    size_t  bsize = 128;
    
    buff = (char*) ::calloc(bsize, sizeof(char));
    
    if ( buff == NULL )
        return -1;
    
    char * ptr = buff;
    int    r = 0, t = 0;
    
    t = Serializer::Pack(ptr+r, bsize-r, strA, strlen(strA));
    r += t;
    std::cout << "a Pack added " << t << " bytes for '" 
              << strA << "'" << std::endl;
    
    t = Serializer::Pack(ptr+r, bsize-r, strB);
    r += t;
    std::cout << "b Pack added " << t << " bytes for '" 
              << strB << "'" << std::endl;
    
    t = Serializer::Pack(ptr+r, bsize-r, intA);
    r += t;
    std::cout << "c Pack added " << t << " bytes for '" 
              << intA << "'" << std::endl;
    
    t = Serializer::Pack(ptr+r, bsize-r, intB);
    r += t;
    std::cout << "d Pack added " << t << " bytes for '" 
              << intB << "'" << std::endl;
    
    
    std::string ustr;
    int16_t    a = 0;
    uint32_t   b = 0;
    
    r = 0;
    
    t = Serializer::Unpack(ptr+r, bsize-r, ustr);
    r += t;
    std::cout << "a Unpacked " << t << " bytes == " 
              << ustr << std::endl;

    t = Serializer::Unpack(ptr+r, bsize-r, ustr);
    r += t;
    std::cout << "b Unpacked " << t << " bytes == " 
              << ustr << std::endl;

    t = Serializer::Unpack(ptr+r, bsize-r, a);
    r += t;
    std::cout << "c Unpacked " << t << " bytes == " << a << std::endl;
    
    t = Serializer::Unpack(ptr+r, bsize-r, b);
    r += t;
    std::cout << "d Unpacked " << t << " bytes == " << b << std::endl;
    
    return 0;
}

