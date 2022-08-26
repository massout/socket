#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

template <typename T>
class Client {
    int sock = 0, client_fd;
    struct sockaddr_in serv_addr;

   public:
    Client(const char *_addr, uint16_t _port);
    T get_data(T &_data);
    void send_data(T &_data);
    ~Client();
};

template <typename T>
Client<T>::Client(const char *_addr, uint16_t _port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(_port);

    inet_pton(AF_INET, _addr, &serv_addr.sin_addr);

    client_fd = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
}

template <typename T>
T Client<T>::get_data(T &_data) {
    Data _buffer;

    read(sock, &_buffer, sizeof(_data));

    return _buffer;
}

template <typename T>
void Client<T>::send_data(T &_data) {
    write(sock, _data, sizeof(_data));
}

template <typename T>
Client<T>::~Client() {
    close(client_fd);
}
