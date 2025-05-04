
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath> 

#include <net/IpAddr.h>
#include <tcanetpp_types.h>

using namespace tcanetpp;

void usage()
{
    std::cout << "Usage: iprange <starting IP> <ending IP>\n";
    exit(0);
}

int IpRangeSize ( ipv4addr_t start, ipv4addr_t end )
{
    uint8_t *a, *b;
    uint8_t  octets[4];
    int      diff;
    uint8_t  mb = 0;

    a = (uint8_t*) &start;
    b = (uint8_t*) &end;

    for ( int i = 0; i < 4; i++ )
    {
        if ( *a != *b ) {
            diff = (int) *b - *a;
            break;
        }
        mb += 8;
        a  += sizeof(uint8_t);
        b  += sizeof(uint8_t);
    }

    return diff; 
}


int main ( int argc, char **argv )
{
    if ( argc < 2 ) 
        usage();

    uint32_t start, end;

    if ( IpAddr::pton(argv[1], start) < 0 ) {
        printf("error converting start range\n");
        exit(1);
    
    }
    if ( IpAddr::pton(argv[2], end) < 0 ) {
        printf("error converting end range\n");
        exit(1);
    }

    printf("start: %s > %u\n", argv[1], start);
    printf("end: %s > %u\n", argv[2], end);

    int diff = IpRangeSize(start, end);

    printf("diff: %d \n", diff);

    double x = log((double)256.0);

    printf("x is %f \n", x);


    return 0;
}


