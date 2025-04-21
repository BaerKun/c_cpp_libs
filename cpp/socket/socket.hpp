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

struct SocketAddress {
    std::string ip;
    int port;
};

template<AddressFamily>
class Socket {
public:
    Socket() = default;

    Socket(const Socket &) = delete;

    Socket(Socket &&) noexcept;

    explicit Socket(Protocol pr);

    ~Socket()noexcept;

    void close()noexcept;

    /*
     * bind会显式绑定端口，若在没有调用bind时执行其他操作，系统会自动分配端口，包括UDP，此后不能再调用bind
     * 使用LOCAL模式时，ip为文件路径，如/tmp/xxx.sock，不用port
     * LOCAL模式下，客户端可以bind路径；也可以不bind，此时服务端收到的ip无意义
     */
    void bind(const SocketAddress &addr) const;


    // UDP

    void sendto(const void *data, size_t size, const SocketAddress &addr) const;

    size_t recvfrom(void *buff, size_t size, SocketAddress *from = nullptr) const;


    // TCP

    void listen(int backlog) const;

    std::shared_ptr<Socket> accept(SocketAddress *from = nullptr) const;

    void connect(const SocketAddress &addr) const;

    void send(const void *data, size_t size) const;

    size_t recv(void *buff, size_t size) const;

private:
    // socket file descriptor
    int sockfd_{};
};

extern template class Socket<AddressFamily::IPv4>;
extern template class Socket<AddressFamily::IPv6>;
extern template class Socket<AddressFamily::LOCAL>;

#define createSocket(af, pr) Socket<af>(pr)

#endif //CPP_LIBS_SOCKET_HPP
