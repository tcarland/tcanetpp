/** Simple command-line tool to convert a raw uint32_t format integer to CIDR formatted 
  * Internet address.
 **/
#define IP_CONVERT_CPP

#include <iostream>
#include <iomanip>

#include "net/IpAddr.h"
#include "util/StringUtils.h"
using namespace tcanetpp;



int main ( int argc, char **argv )
{
    if ( argc == 1 ) {
        std::cout << "Usage: " << argv[0] << " ipaddr  ipaddr ..." << std::endl
            << " example: ipconvert 192.168.0.0/16 172.16.64.1/32 43200/32 20975788" 
            << std::endl;
        return 0;
    }

    IpAddr      pfx;
    std::string input;
    ipv4addr_t  ip;
    uint16_t    mb;
    int         i, r = 0;

    for ( i = 1; i < argc; ++i ) {
        input = argv[i];

        if ( StringUtils::IndexOf(input, ".") >= 0 ) {  // ipv4
            IpAddr::ToIpAddr(input, pfx);

            ip = pfx.getPrefix();
            mb = pfx.getPrefixLen();

            std::cout <<  "  "              << std::setiosflags(std::ios::left) << std::setw(20)
                      << input << "  ==  "  << std::setiosflags(std::ios::right) << std::setw(20)
                      << ip << " / " << mb  << std::resetiosflags(std::ios::right) << std::endl;

        } else if ( StringUtils::IndexOf(input, ":") >= 0 ) {  // ipv6
            ipv6addr_t  ip6addr;

            r = IpAddr::pton(input, ip6addr);
            if ( r != 1 )
                std::cout << "Error in IpAddr::pton " << r << std::endl;

            //in6addr_t  addr = *((in6addr_t*)&ip6addr);
            in6addr_t  addr = ip6addr.toAddr();
            std::string rev = IpAddr::ntop(addr);

            std::cout << "   "           << std::setiosflags(std::ios::left) << std::setw(38)
                      << input << " == " << std::setiosflags(std::ios::right) << std::setw(38)
                      << rev             << std::resetiosflags(std::ios::right);

            if ( IpAddr::IsLoopback(ip6addr) )
                std::cout << " <loopback>";
            std::cout << std::endl;

        } else {  // uint32_t representation of an ipv4 addr
            int indx = 0;

            if ( (indx = StringUtils::IndexOf(input, "/")) > 0 ) {
                std::string ips = input.substr(0, indx);
                std::string mbs = input.substr(indx+1);

                StringUtils::Trim(ips);
                StringUtils::Trim(mbs);
                
                ip = StringUtils::FromString<uint32_t>(ips);
                mb = StringUtils::FromString<uint16_t>(mbs);
            } else {
                ip = StringUtils::FromString<uint32_t>(input);
                mb = 32;
            }

            pfx  = IpAddr(ip, mb);

            std::cout << "  "
                << std::setiosflags(std::ios::left) << std::setw(20) 
                << input << "  ==  " 
                << std::setiosflags(std::ios::right) << std::setw(20)
                << pfx.toString()
                << std::resetiosflags(std::ios::right) << std::endl;
        }
    }

    return 0;
}


