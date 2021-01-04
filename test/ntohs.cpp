
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <byte_order.h>
#include <tcanetpp_types.h>


void usage()
{
    std::cout << "Usage: ntohs <number>\n\n"
        << "ntohs or \"Network to Host Short\" converts the provided argument (as an integer)  \n"
        << "from Network Order or Big-Endian to Host Order or Little-Endian. \n"
        << "The number is assumed to be an unsigned short (2 bytes).\n"
        << "This tool is intended for convenience when using tools like network sniffers\n"
        << "that typically display bytes in network order\n";
    exit(0);
}

int main ( int argc, char **argv )
{
    if ( argc < 2 ) 
        usage();

    uint32_t inp = atoi(argv[1]);
    uint32_t oup = ntohs(inp);

    printf("%u > %u\n", inp, oup);

    return 0;
}


