#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

// Data yapısının içerisini istediğiniz gibi doldurun.
struct Data {};

class Server {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

   public:
    Server(std::string &_addr, uint8_t _port);
    void send_data(Data &_data);
    ~Server();
};

Server::Server(std::string &_addr, uint8_t _port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(&_addr[0]);
    address.sin_port = htons(_port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
}

void Server::send_data(Data &_data) {
    send(new_socket, &_data, sizeof(_data), 0);
}

Server::~Server() {
    close(new_socket);
    shutdown(server_fd, SHUT_RDWR);
}

#endif
