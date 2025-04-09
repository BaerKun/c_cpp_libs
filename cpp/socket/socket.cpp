#include "socket.hpp"
#include <sys/un.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdexcept>


Socket::Socket(const AddressFamily addressFamily, const Protocol protocol)
    : addressFamily_(static_cast<int>(addressFamily)), protocol_(static_cast<int>(protocol)) {
    sockfd_ = socket(addressFamily_, protocol_, 0);
    if (sockfd_ == -1) {
        throw std::runtime_error("socket() failed");
    }
}

Socket::~Socket() {
    close();
}

void Socket::bind(const SocketAddress &addr) const {
    socklen_t len;
    sockaddr *sockAddr = getSockAddr(&len, addr);
    const int flag = ::bind(sockfd_, sockAddr, len);

    freeSockAddr(sockAddr);
    if (flag != 0)
        throw std::runtime_error("bind() failed");
}

void Socket::listen(const int backlog) const {
    if (::listen(sockfd_, backlog) != 0) {
        throw std::runtime_error("listen() failed");
    }
}

std::shared_ptr<Socket> Socket::accept(SocketAddress *const from) const {
    socklen_t len;
    sockaddr *addr = getSockAddrBuff(&len);
    const int fd = ::accept(sockfd_, addr, &len);
    if (fd == -1) {
        freeSockAddr(addr);
        throw std::runtime_error("accept() failed");
    }

    getWhereFrom(from, addr);
    freeSockAddr(addr);

    const auto client = std::shared_ptr<Socket>(new Socket);
    client->sockfd_ = fd;
    client->addressFamily_ = addressFamily_;
    client->protocol_ = protocol_;
    return client;
}

void Socket::connect(const SocketAddress &addr) const {
    socklen_t len;
    sockaddr *sockAddr = getSockAddr(&len, addr);
    const int flag = ::connect(sockfd_, sockAddr, len);

    freeSockAddr(sockAddr);
    if (flag != 0)
        throw std::runtime_error("connect() failed");
}

void Socket::send(const void *const data, const size_t size) const {
    size_t sent = 0;
    auto *ptr = static_cast<const char *>(data);

    while (true) {
        const ssize_t ret = ::send(sockfd_, ptr + sent, size - sent, 0);
        if (ret == -1)
            throw std::runtime_error("send() failed");
        if (ret == 0)
            return;
        sent += ret;
    }
}

size_t Socket::recv(void *const buff, const size_t size) const {
    size_t received = 0;
    auto *ptr = static_cast<char *>(buff);

    while (true) {
        const ssize_t ret = ::recv(sockfd_, ptr + received, size - received, 0);
        if (ret == -1)
            throw std::runtime_error("recv() failed");
        if (ret == 0)
            return received;
        received += ret;
    }
}

void Socket::sendto(const void *const data, const size_t size, const SocketAddress &addr) const {
    socklen_t len;
    sockaddr *sockAddr = getSockAddr(&len, addr);
    const ssize_t flag = ::sendto(sockfd_, data, size, 0, sockAddr, len);

    freeSockAddr(sockAddr);
    if (flag == -1)
        throw std::runtime_error("sendto() failed");
}

size_t Socket::recvfrom(void *const buff, const size_t size, SocketAddress *const from) const {
    socklen_t len;
    sockaddr *addr = getSockAddrBuff(&len);

    const ssize_t received = ::recvfrom(sockfd_, buff, size, 0, addr, &len);
    if (received == -1) {
        freeSockAddr(addr);
        throw std::runtime_error("recvfrom() failed");
    }

    getWhereFrom(from, addr);
    freeSockAddr(addr);
    return received;
}

void Socket::close() {
    if (sockfd_ != -1) {
        ::close(sockfd_);
        sockfd_ = -1;
    }
}


sockaddr *Socket::getSockAddrBuff(socklen_t *const len) const {
    switch (addressFamily_) {
        case AF_INET:
            *len = sizeof(sockaddr_in);
            return reinterpret_cast<sockaddr *>(new sockaddr_in);
        case AF_INET6:
            *len = sizeof(sockaddr_in6);
            return reinterpret_cast<sockaddr *>(new sockaddr_in6);
        case AF_LOCAL:
            *len = sizeof(sockaddr_un);
            return reinterpret_cast<sockaddr *>(new sockaddr_un);
        default:
            throw std::runtime_error("Unknown address family");
    }
}

sockaddr *Socket::getSockAddr(socklen_t *const len, const SocketAddress &addr) const {
    switch (addressFamily_) {
        case AF_INET: {
            const auto ipv4 = new sockaddr_in;
            ipv4->sin_family = AF_INET;
            ipv4->sin_port = htons(addr.port);
            if (inet_pton(AF_INET, addr.ip.c_str(), &ipv4->sin_addr) != 1)
                throw std::runtime_error("inet_pton() failed");

            *len = sizeof(sockaddr_in);
            return reinterpret_cast<sockaddr *>(ipv4);
        }
        case AF_INET6: {
            const auto ipv6 = new sockaddr_in6;
            ipv6->sin6_family = AF_INET6;
            ipv6->sin6_port = htons(addr.port);
            if (inet_pton(AF_INET6, addr.ip.c_str(), &ipv6->sin6_addr) != 1)
                throw std::runtime_error("inet_pton() failed");

            *len = sizeof(sockaddr_in6);
            return reinterpret_cast<sockaddr *>(ipv6);
        }
        case AF_LOCAL: {
            const auto local = new sockaddr_un;
            local->sun_family = AF_LOCAL;
            strncpy(local->sun_path, addr.ip.c_str(), sizeof(local->sun_path));

            *len = sizeof(sockaddr_un);
            return reinterpret_cast<sockaddr *>(local);
        }
        default:
            throw std::runtime_error("Unknown address family");
    }
}

void Socket::getWhereFrom(SocketAddress *from, sockaddr *addr) const {
    if (from == nullptr)
        return;

    switch (addressFamily_) {
        case AF_INET: {
            char buff[INET_ADDRSTRLEN];
            const auto ipv4 = reinterpret_cast<sockaddr_in *>(addr);
            inet_ntop(AF_INET, &ipv4->sin_addr, buff, INET_ADDRSTRLEN);
            from->ip = buff;
            from->port = ntohs(ipv4->sin_port);
            return;
        }
        case AF_INET6: {
            char buff[INET6_ADDRSTRLEN];
            const auto ipv6 = reinterpret_cast<sockaddr_in6 *>(addr);
            inet_ntop(AF_INET6, &ipv6->sin6_addr, buff, INET6_ADDRSTRLEN);
            from->ip = buff;
            from->port = ntohs(ipv6->sin6_port);
            return;
        }
        case AF_LOCAL: {
            const auto local = reinterpret_cast<sockaddr_un *>(addr);
            from->ip = local->sun_path;
            from->port = -1;
        }
        default:
            return;
    }
}

void Socket::freeSockAddr(sockaddr *addr) const {
    switch (addressFamily_) {
        case AF_INET:
            delete reinterpret_cast<sockaddr_in *>(addr);
            return;
        case AF_INET6:
            delete reinterpret_cast<sockaddr_in6 *>(addr);
            return;
        case AF_LOCAL:
            delete reinterpret_cast<sockaddr_un *>(addr);
            return;
        default:
            return;
    }
}
