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

struct SocketAddress {
    std::string ip;
    int port;
};

class Socket {
public:
    Socket(AddressFamily addressFamily, Protocol protocol);

    ~Socket();

    /*
     * bind会显式绑定端口，若在没有调用bind时执行其他操作，系统会自动分配端口，包括UDP，此后不能再调用bind
     * 使用LOCAL模式时，ip为文件路径，如/tmp/xxx.sock，不用port
     * LOCAL模式下，客户端可以bind路径；也可以不bind，此时服务端收到的ip无意义
     */
    void bind(const SocketAddress &addr) const;

    // TCP

    void listen(int backlog) const;

    std::shared_ptr<Socket> accept(SocketAddress *from = nullptr) const;

    void connect(const SocketAddress &addr) const;

    void send(const void *data, size_t size) const;

    size_t recv(void *buff, size_t size) const;

    // UDP

    void sendto(const void *data, size_t size, const SocketAddress &addr) const;

    size_t recvfrom(void *buff, size_t size, SocketAddress *from = nullptr) const;


    void close();

private:
    Socket() = default;

    sockaddr *getSockAddrBuff(socklen_t *len) const;

    sockaddr *getSockAddr(socklen_t *len, const SocketAddress &addr) const;

    void freeSockAddr(sockaddr *addr) const;

    void getWhereFrom(SocketAddress *from, sockaddr *addr) const;

    // socket file descriptor
    int sockfd_;
    int addressFamily_;
    int protocol_;
};


#endif //CPP_LIBS_SOCKET_HPP
