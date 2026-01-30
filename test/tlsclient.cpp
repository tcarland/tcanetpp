/**
 * @file tlsclient.cpp
 *
 * Test application for SecureSocket client functionality.
 * Creates a TLS client that connects to a server and sends/receives data.
 *
 * Usage: tlsclient <host> <port> [cafile] [certfile] [keyfile]
 * 
 * openssl req -x509 -newkey rsa:4096 -keyout server.key -out server.crt \
 *   -days 365 -nodes -subj "/CN=localhost"
 * 
 */

#include <iostream>
#include <cstring>
#include <string>

#include "net/SecureSocket.h"
#include "net/AddrInfo.h"

using namespace tcanetpp;


void printUsage ( const char * prog )
{
    std::cerr << "Usage: " << prog << " <host> <port> [options]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "  host      - Server hostname or IP address" << std::endl;
    std::cerr << "  port      - Server port number" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  --cafile <file>    - CA certificate file for server verification" << std::endl;
    std::cerr << "  --cert <file>      - Client certificate file (for mutual TLS)" << std::endl;
    std::cerr << "  --key <file>       - Client private key file (for mutual TLS)" << std::endl;
    std::cerr << "  --no-verify        - Disable server certificate verification" << std::endl;
    std::cerr << "  --help             - Show this help message" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Interactive commands:" << std::endl;
    std::cerr << "  quit/exit  - Disconnect from server" << std::endl;
    std::cerr << "  shutdown   - Request server shutdown" << std::endl;
}


int main ( int argc, char ** argv )
{
    if ( argc < 3 ) {
        printUsage(argv[0]);
        return 1;
    }

    std::string host     = argv[1];
    uint16_t    port     = static_cast<uint16_t>(std::atoi(argv[2]));
    std::string cafile;
    std::string certfile;
    std::string keyfile;
    bool        verify   = true;

    // Parse optional arguments
    for ( int i = 3; i < argc; ++i )
    {
        std::string arg = argv[i];

        if ( arg == "--cafile" && i + 1 < argc ) {
            cafile = argv[++i];
        }
        else if ( arg == "--cert" && i + 1 < argc ) {
            certfile = argv[++i];
        }
        else if ( arg == "--key" && i + 1 < argc ) {
            keyfile = argv[++i];
        }
        else if ( arg == "--no-verify" ) {
            verify = false;
        }
        else if ( arg == "--help" ) {
            printUsage(argv[0]);
            return 0;
        }
        else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }

    std::cout << "TLS Client Test Application" << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "Host:        " << host << std::endl;
    std::cout << "Port:        " << port << std::endl;
    std::cout << "Verify:      " << (verify ? "enabled" : "disabled") << std::endl;
    if ( ! cafile.empty() )
        std::cout << "CA File:     " << cafile << std::endl;
    if ( ! certfile.empty() )
        std::cout << "Client Cert: " << certfile << std::endl;
    std::cout << std::endl;

    try 
    {
        // Resolve hostname to address using AddrInfo
        addrinfo hints;
        ::memset(&hints, 0, sizeof(hints));
        hints.ai_family   = AF_UNSPEC;     // Allow IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM;   // TCP
        hints.ai_protocol = IPPROTO_TCP;

        AddrInfo * addrInfo = AddrInfo::GetAddrInfo(host, port, &hints);
        
        if ( addrInfo == nullptr || ! addrInfo->isValid() ) {
            std::cerr << "Failed to resolve hostname: " << host << std::endl;
            if ( addrInfo != nullptr )
                delete addrInfo;
            return 1;
        }

        // Create client socket from resolved address
        addrinfo * ai = addrInfo->begin();
        SecureSocket client(ai);

        // Done with AddrInfo, socket has copied the address
        delete addrInfo;
        addrInfo = nullptr;

        // Configure TLS options
        client.setHostname(host);
        client.setVerifyPeer(verify);

        if ( ! cafile.empty() )
            client.setTrustFile(cafile);

        if ( ! certfile.empty() )
            client.setCertificateFile(certfile);

        if ( ! keyfile.empty() )
            client.setPrivateKeyFile(keyfile);

        // Initialize the socket and SSL context
        int result = client.init(true);  // blocking mode
        if ( result < 0 ) {
            std::cerr << "Failed to initialize client: " << client.errorStr() << std::endl;
            return 1;
        }

        std::cout << "Connecting to " << host << ":" << port << "..." << std::endl;

        // Connect (TCP + TLS handshake)
        result = client.connect();
        if ( result < 0 ) {
            std::cerr << "Connection failed: " << client.errorStr() << std::endl;
            return 1;
        }

        std::cout << "Connected!" << std::endl;
        std::cout << "  TLS Version: " << client.getProtocolVersion() << std::endl;
        std::cout << "  Cipher:      " << client.getCipherName() << std::endl;

        // Display server certificate info
        X509 * cert = client.getPeerCertificate();
        if ( cert != nullptr ) {
            char subject[256];
            X509_NAME_oneline(X509_get_subject_name(cert), subject, sizeof(subject));
            std::cout << "  Server:      " << subject << std::endl;
            X509_free(cert);
        }

        std::cout << std::endl;
        std::cout << "Enter messages to send (type 'quit' or 'exit' to disconnect):" << std::endl;
        std::cout << std::endl;

        // Interactive send/receive loop
        std::string input;
        char buffer[4096];

        while ( client.isConnected() )
        {
            std::cout << "> ";
            std::cout.flush();

            if ( ! std::getline(std::cin, input) ) {
                // EOF on stdin
                break;
            }

            if ( input.empty() )
                continue;

            // Add newline for the server
            input += "\n";

            // Send message
            ssize_t bytesSent = client.write(input.c_str(), input.length());
            if ( bytesSent < 0 ) {
                std::cerr << "Write error: " << client.errorStr() << std::endl;
                break;
            }

            // Check for quit before waiting for response
            if ( input.substr(0, 4) == "quit" || input.substr(0, 4) == "exit" ) {
                std::cout << "Disconnecting..." << std::endl;
                break;
            }

            // Receive response (echo from server)
            ssize_t bytesRead = client.read(buffer, sizeof(buffer) - 1);
            if ( bytesRead < 0 ) {
                std::cerr << "Connection closed by server" << std::endl;
                break;
            }

            if ( bytesRead > 0 ) {
                buffer[bytesRead] = '\0';
                std::cout << "< " << buffer;
                if ( buffer[bytesRead - 1] != '\n' )
                    std::cout << std::endl;
            }
        }

        client.close();
        std::cout << "Disconnected." << std::endl;
    }
    catch ( const SocketException & e )
    {
        std::cerr << "SocketException: " << e.toString() << std::endl;
        return 1;
    }
    catch ( const std::exception & e )
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
