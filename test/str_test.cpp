#define STRTEST_CPP

#include <iostream>
#include <vector>

#include "net/IpAddr.h"
#include "util/StringUtils.h"
using namespace tcanetpp;



int main() {

    // string conversions
    //int inta      = 256;
    //uint32_t intb = 512;
    uint16_t intc = 1;
    uint16_t  mb  = 28;
    uint16_t mbb  = 29;


    std::string mbs  = "16";
    std::string ints = "1024";
    std::string cidrStr = "10.10.10.0/24";
    std::string addrstr;
    uint32_t    addr;

    std::cout << "mb = " << mb << std::endl;
    std::cout << "mbb = " << mbb << std::endl;

    uint16_t uintmb = StringUtils::FromString<uint16_t>(mbs);


    std::cout << "string of '" << mbs << "' converted to '" << uintmb << "'"
        << std::endl;

    intc = StringUtils::FromString<uint16_t>(ints);
    std::cout << "string of '" << ints << "' converted to uint16 of '" << intc << "'"
        << std::endl;

    mbs = StringUtils::ToString(mb);
    std::cout << "uint8 of " << mb << " converted to '" << mbs << "'" 
        << std::endl;
    mbs = StringUtils::ToString(mbb);
    std::cout << "uint16 of " << mbb << " converted to '" << mbs << "'" 
        << std::endl;
    
    
    std::vector<std::string>  stok;
    
    StringUtils::Split(cidrStr, '/', std::back_inserter(stok));
    
    if ( stok.size() >= 1 )
        addrstr = stok[0];

    if ( IpAddr::pton(addrstr, addr) <= 0 )
	return -1;

    if ( stok.size() == 2 )
	uintmb = StringUtils::FromString<uint16_t>(stok[1]);

    std::cout << "string of '" << stok[1] << "' converted to '" << uintmb << "'"
        << std::endl;

    if ( uintmb <= 0 || uintmb > 32 )
	uintmb = 32;

    std::cout << "string of '" << cidrStr << "' split to '" << addrstr 
        << "' and '" << uintmb << "'" << std::endl;

    return 0;
}


