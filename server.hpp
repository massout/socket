#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

template <typename T>
class Server {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

   public:
    Server(const char *_addr, uint16_t _port, int _max_connections);
    void send_data(T &_data);
    T get_data();
    ~Server();
};

template <typename T>
Server<T>::Server(const char *_addr, uint16_t _port, int _max_connections) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(_addr);
    address.sin_port = htons(_port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, _max_connections);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
}

template <typename T>
void Server<T>::send_data(T &_data) {
    send(new_socket, &_data, sizeof(_data), 0);
}

template <typename T>
T Server<T>::get_data() {
    T _buffer;
    read(new_socket, _buffer, sizeof(T));

    return _buffer;
}

template <typename T>
Server<T>::~Server() {
    close(new_socket);
    shutdown(server_fd, SHUT_RDWR);
}
