#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Data yapısının içerisini serverla aynı olması şartıyla istediğiniz gibi doldurun.
struct Data {};

class Client {
    int sock = 0, client_fd;
    struct sockaddr_in serv_addr;

   public:
    Client(const char *_addr, uint16_t _port);
    Data get_data(Data &_data);
    ~Client();
};

Client::Client(const char *_addr, uint16_t _port) {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(_port);

    inet_pton(AF_INET, _addr, &serv_addr.sin_addr);

    client_fd = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
}

Data Client::get_data(Data &_data) {
    Data _buffer;

    read(sock, &_buffer, sizeof(_data));

    return _buffer;
}

Client::~Client() {
    close(client_fd);
}

#endif
