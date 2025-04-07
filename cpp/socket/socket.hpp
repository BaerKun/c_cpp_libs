#ifndef CPP_LIBS_SOCKET_HPP
#define CPP_LIBS_SOCKET_HPP

#include <sys/socket.h>
#include <string>

enum class AddressFamily {
    IPv4 = AF_INET,
    IPv6 = AF_INET6,
    LOCAL = AF_LOCAL // unix-like only
};

enum class Protocol {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM
};

class Socket {
public:
    Socket(AddressFamily addressFamily, Protocol protocol);

    ~Socket();

    void bind(const char *ip, int port);

    void listen(int backlog);

    void connect(const char *ip, int port);

    void close();

    void send(const void *data, size_t size);

    void sendto(const void *data, size_t size, const char *ip, int port);

    size_t recv(void *buff, size_t size);

    size_t recvfrom(void *buff, size_t size, const char *ip);

    Socket *accept(std::string *ip = nullptr);

private:
    Socket() = default;

    // socket file descriptor
    int sockfd_;
    int addressFamily_;
    int protocol_;
};


#endif //CPP_LIBS_SOCKET_HPP
