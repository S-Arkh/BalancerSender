#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class senderToBalancer {
private:
  int m_socket;
  sockaddr_in m_socket_in;
  int slen;

public:
  senderToBalancer(const std::string &address, const unsigned long int &port) {
    std::string message;

    if ((m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
      throw "Getting socket error";
    }

    memset((char *)&m_socket_in, 0, sizeof(m_socket_in));
    m_socket_in.sin_family = AF_INET;
    m_socket_in.sin_port = htons(port);

    if (inet_aton(address.c_str(), &m_socket_in.sin_addr) == 0) {
      close(m_socket);
      throw "inet_aton error";
    }

    slen = sizeof(m_socket_in);
  };

  ~senderToBalancer() {
    close(m_socket);
  };

  void sendData(const std::string &data) {
    if (sendto(m_socket, data.c_str(), data.length(), 0, (struct sockaddr *)&m_socket_in, slen) == -1) {
      std::cerr << "Can't send data" << std::endl;
      return;
    }
  }
};

int main() {
  try {
    senderToBalancer sender("127.0.0.1", 2353);

    for (int counter = 0;; counter++) {
      sender.sendData(std::to_string(counter));
    }
  } catch (const char *exec) {
    std::cerr << exec << std::endl;
  }

  return 0;
}
