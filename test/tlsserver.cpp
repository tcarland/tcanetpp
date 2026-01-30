/**
 * @file tlsserver.cpp
 *
 * Test application for SecureSocket server functionality.
 * Creates a TLS server that accepts connections and echoes received data.
 *
 * Usage: tlsserver <port> <certfile> <keyfile>
 *
 * Generate test certificates:
 *   openssl req -x509 -newkey rsa:4096 -keyout server.key -out server.crt \
 *           -days 365 -nodes -subj "/CN=localhost"
 */

#include <iostream>
#include <cstring>
#include <csignal>

#include "net/SecureSocket.h"

using namespace tcanetpp;


static bool running = true;

void signalHandler ( int signum )
{
    std::cout << "\nShutting down server..." << std::endl;
    running = false;
}


void printUsage ( const char * prog )
{
    std::cerr << "Usage: " << prog << " <port> <certfile> <keyfile> [cafile]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "  port      - Port number to listen on" << std::endl;
    std::cerr << "  certfile  - Server certificate file (PEM format)" << std::endl;
    std::cerr << "  keyfile   - Server private key file (PEM format)" << std::endl;
    std::cerr << "  cafile    - CA file for client verification (optional)" << std::endl;
    std::cerr << std::endl;
    std::cerr << "Generate test certificates:" << std::endl;
    std::cerr << "  openssl req -x509 -newkey rsa:4096 -keyout server.key -out server.crt \\" << std::endl;
    std::cerr << "          -days 365 -nodes -subj \"/CN=localhost\"" << std::endl;
}


int main ( int argc, char ** argv )
{
    if ( argc < 4 ) {
        printUsage(argv[0]);
        return 1;
    }

    uint16_t    port     = static_cast<uint16_t>(std::atoi(argv[1]));
    std::string certfile = argv[2];
    std::string keyfile  = argv[3];
    std::string cafile;

    if ( argc > 4 )
        cafile = argv[4];

    // Set up signal handler for graceful shutdown
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    std::cout << "TLS Server Test Application" << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << "Port:        " << port << std::endl;
    std::cout << "Certificate: " << certfile << std::endl;
    std::cout << "Private Key: " << keyfile << std::endl;
    if ( ! cafile.empty() )
        std::cout << "CA File:     " << cafile << " (client verification enabled)" << std::endl;
    std::cout << std::endl;

    try 
    {
        // Create server socket bound to all interfaces (INADDR_ANY = 0)
        SecureSocket server(static_cast<ipv4addr_t>(0), port, 
                           SOCKTYPE_SERVER, SOCKET_TCP);

        // Configure TLS certificates
        server.setCertificateFile(certfile);
        server.setPrivateKeyFile(keyfile);

        if ( ! cafile.empty() )
            server.setTrustFile(cafile);

        // Initialize the server (creates socket, binds, listens, and sets up SSL context)
        int result = server.init(true);  // blocking mode
        if ( result < 0 ) {
            std::cerr << "Failed to initialize server: " << server.errorStr() << std::endl;
            return 1;
        }

        std::cout << "Server listening on port " << port << "..." << std::endl;
        std::cout << "Press Ctrl+C to stop" << std::endl;
        std::cout << std::endl;

        // Create factory for accepting secure connections
        SecureSocket::SecureSocketFactory factory(&server);

        while ( running )
        {
            // Accept incoming connection (blocking)
            Socket * clientSocket = server.accept(factory);

            if ( clientSocket == nullptr ) {
                if ( running ) {
                    std::cerr << "Accept failed or TLS handshake error" << std::endl;
                }
                continue;
            }

            SecureSocket * client = dynamic_cast<SecureSocket*>(clientSocket);
            if ( client == nullptr ) {
                std::cerr << "Failed to cast to SecureSocket" << std::endl;
                delete clientSocket;
                continue;
            }

            std::cout << "Client connected from: " << client->getHostStr() << std::endl;
            std::cout << "  TLS Version: " << client->getProtocolVersion() << std::endl;
            std::cout << "  Cipher:      " << client->getCipherName() << std::endl;

            // Simple echo loop for this client
            char buffer[4096];
            ssize_t bytesRead;

            while ( running && client->isConnected() )
            {
                bytesRead = client->read(buffer, sizeof(buffer) - 1);

                if ( bytesRead < 0 ) {
                    std::cout << "Client disconnected" << std::endl;
                    break;
                }

                if ( bytesRead == 0 ) {
                    // No data available (shouldn't happen in blocking mode)
                    continue;
                }

                buffer[bytesRead] = '\0';
                std::cout << "Received (" << bytesRead << " bytes): " << buffer;

                // Echo back to client
                ssize_t bytesWritten = client->write(buffer, bytesRead);
                if ( bytesWritten < 0 ) {
                    std::cerr << "Write error: " << client->errorStr() << std::endl;
                    break;
                }

                // Check for quit command
                if ( strncmp(buffer, "quit", 4) == 0 || 
                     strncmp(buffer, "exit", 4) == 0 ) 
                {
                    std::cout << "Client requested disconnect" << std::endl;
                    break;
                }

                // Check for server shutdown command
                if ( strncmp(buffer, "shutdown", 8) == 0 ) {
                    std::cout << "Shutdown command received" << std::endl;
                    running = false;
                    break;
                }
            }

            client->close();
            delete client;
            std::cout << std::endl;
        }

        server.close();
        std::cout << "Server stopped." << std::endl;
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
