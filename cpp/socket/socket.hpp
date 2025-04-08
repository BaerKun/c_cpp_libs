#ifndef CPP_LIBS_SOCKET_HPP
#define CPP_LIBS_SOCKET_HPP

#include <sys/socket.h>
#include <string>
#include <memory>

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

    // bind会显式绑定端口，若在调用bind前执行其他操作，系统会自动分配端口，此时不能再调用bind
    void bind(const char *ip, int port) const;

    // TCP

    void listen(int backlog) const;

    std::shared_ptr<Socket> accept(std::string *where = nullptr) const;

    void connect(const char *ip, int port) const;

    void send(const void *data, size_t size) const;

    size_t recv(void *buff, size_t size) const;

    // UDP

    void sendto(const void *data, size_t size, const char *ip, int port) const;

    size_t recvfrom(void *buff, size_t size, std::string *where = nullptr) const;


    void close();

private:
    Socket() = default;

    // socket file descriptor
    int sockfd_;
    int addressFamily_;
    int protocol_;
};


#endif //CPP_LIBS_SOCKET_HPP
