#include <unistd.h>
#include <arpa/inet.h>
#include <stdexcept>
#include "socket.hpp"




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

void Socket::close() {
    if (sockfd_ != -1) {
        ::close(sockfd_);
        sockfd_ = -1;
    }
}

void Socket::bind(const char *const ip, const int port) const {
    sockaddr_in addr{};
    addr.sin_family = addressFamily_;
    addr.sin_port = htons(port);
    if (inet_pton(addressFamily_, ip, &addr.sin_addr) != 1) {
        throw std::runtime_error("inet_pton() failed");
    }
    if (::bind(sockfd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) != 0) {
        throw std::runtime_error("bind() failed");
    }
}

void Socket::listen(const int backlog) const {
    if (::listen(sockfd_, backlog) != 0) {
        throw std::runtime_error("listen() failed");
    }
}

void Socket::connect(const char *ip, const int port) const {
    sockaddr_in addr{};
    addr.sin_family = addressFamily_;
    addr.sin_port = htons(port);
    if (inet_pton(addressFamily_, ip, &addr.sin_addr) != 1) {
        throw std::runtime_error("inet_pton() failed");
    }
    if (::connect(sockfd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) != 0) {
        throw std::runtime_error("connect() failed");
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

std::shared_ptr<Socket> Socket::accept(std::string *const where) const {
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);
    const int fd = ::accept(sockfd_, reinterpret_cast<sockaddr *>(&addr), &len);
    if (fd == -1) {
        throw std::runtime_error("accept() failed");
    }
    if (where != nullptr)
        *where = inet_ntoa(addr.sin_addr);

    const auto client = std::shared_ptr<Socket>(new Socket);
    client->sockfd_ = fd;
    client->addressFamily_ = addressFamily_;
    client->protocol_ = protocol_;
    return client;
}

void Socket::sendto(const void *const data, const size_t size, const char *const ip, const int port) const {
    sockaddr_in addr{};
    addr.sin_family = addressFamily_;
    addr.sin_port = htons(port);
    if (inet_pton(addressFamily_, ip, &addr.sin_addr) != 1)
        throw std::runtime_error("inet_pton() failed");

    if (::sendto(sockfd_, data, size, 0, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
        throw std::runtime_error("sendto() failed");
}

size_t Socket::recvfrom(void *const buff, const size_t size, std::string *const where) const {
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);

    const ssize_t received = ::recvfrom(sockfd_, buff, size, 0, reinterpret_cast<sockaddr *>(&addr), &len);
    if (received == -1)
        throw std::runtime_error("recvfrom() failed");

    if (where != nullptr)
        *where = inet_ntoa(addr.sin_addr);

    return received;
}
