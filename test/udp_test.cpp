#include "net/IpAddr.h"
#include "net/Socket.h"
#include <cstring>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

using namespace tcanetpp;

const int SERVER_PORT = 9555;
const char *SERVER_IP = "127.0.0.1";

void run_server() {
  std::cout << "[Server] Starting..." << std::endl;

  // Create server socket
  ipv4addr_t addr_any = 0; // INADDR_ANY

  try {
    Socket server(addr_any, SERVER_PORT, SOCKTYPE_SERVER, SOCKET_UDP);
    if (server.init(true) < 0) {
      std::cerr << "[Server] Init failed: " << server.getErrorString()
                << std::endl;
      return;
    }

    std::cout << "[Server] Listening on " << server.getHostString()
              << std::endl;

    char buffer[1024];
    sockaddr_t client_addr;

    // server.setBlocking();

    std::cout << "[Server] Waiting for packet..." << std::endl;

    ssize_t n = server.readFrom(buffer, sizeof(buffer) - 1, client_addr);
    if (n > 0) {
      buffer[n] = '\0';
      std::cout << "[Server] Received: " << buffer << std::endl;

      // Reply using UdpSocketFactory
      Socket::UdpSocketFactory factory(client_addr);
      Socket *replySock = server.accept(factory);

      if (replySock) {
        std::string reply = "World";
        replySock->write(reply.c_str(), reply.size());
        std::cout << "[Server] Sent: " << reply << std::endl;
        delete replySock;
      } else {
        std::cerr << "[Server] Failed to create reply socket" << std::endl;
      }
    } else {
      std::cerr << "[Server] Read failed or 0 bytes. Errno: " << strerror(errno)
                << " Return: " << n << std::endl;
    }
  } catch (const SocketException &e) {
    std::cerr << "[Server] Exception: " << e.toString() << std::endl;
  }
}

void run_client() {
  // Wait a bit for server to start
  sleep(1);
  std::cout << "[Client] Starting..." << std::endl;

  try {
    ipv4addr_t dest;
    IpAddr::pton(SERVER_IP, dest);

    Socket client(dest, SERVER_PORT, SOCKTYPE_CLIENT, SOCKET_UDP);
    if (client.init(true) < 0) { // Blocking
      std::cerr << "[Client] Init failed: " << client.getErrorString()
                << std::endl;
      return;
    }

    std::string msg = "Hello";
    // Implicit bind happens on sendto if not bound?
    // Socket::write calls sendto.

    ssize_t sent = client.write(msg.c_str(), msg.size());
    std::cout << "[Client] Sent: " << msg << " (" << sent << " bytes)"
              << std::endl;

    // Wait for reply
    char buffer[1024];
    sockaddr_t server_addr;
    // Need to use readFrom to receive.
    ssize_t n = client.readFrom(buffer, sizeof(buffer) - 1, server_addr);
    if (n > 0) {
      buffer[n] = '\0';
      std::cout << "[Client] Received: " << buffer << std::endl;
    } else {
      std::cerr << "[Client] Read failed" << std::endl;
    }

  } catch (const SocketException &e) {
    std::cerr << "[Client] Exception: " << e.toString() << std::endl;
  }
}

int main(int argc, char **argv) {
  pid_t pid = fork();

  if (pid < 0) {
    std::cerr << "Fork failed" << std::endl;
    return 1;
  } else if (pid == 0) {
    // Child
    run_client();
    return 0;
  } else {
    // Parent
    run_server();
    int status;
    waitpid(pid, &status, 0);
    std::cout << "Done." << std::endl;
  }
  return 0;
}
