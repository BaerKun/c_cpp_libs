#include "socket.hpp"
#include <arpa/inet.h>
#include <memory>
#include <stdexcept>
#include <sys/un.h>
#include <unistd.h>

template <AddressFamily AF> constexpr socklen_t SOCK_ADDR_SIZE{};
template <>
constexpr socklen_t SOCK_ADDR_SIZE<AddressFamily::IPv4> = sizeof(sockaddr_in);
template <>
constexpr socklen_t SOCK_ADDR_SIZE<AddressFamily::IPv6> = sizeof(sockaddr_in6);
template <>
constexpr socklen_t SOCK_ADDR_SIZE<AddressFamily::LOCAL> = sizeof(sockaddr_un);

template <AddressFamily AF> static inline sockaddr *getSockAddrBuff() noexcept {
  return static_cast<sockaddr *>(std::malloc(SOCK_ADDR_SIZE<AF>));
}

template <AddressFamily AF>
static sockaddr *getSockAddr(const SocketAddress &addr) {
  if constexpr (AF == AddressFamily::IPv4) {
    const auto ipv4 =
        static_cast<sockaddr_in *>(std::malloc(SOCK_ADDR_SIZE<AF>));
    ipv4->sin_family = AF_INET;
    ipv4->sin_port = htons(addr.port);
    if (inet_pton(AF_INET, addr.ip.c_str(), &ipv4->sin_addr) != 1)
      throw std::runtime_error("inet_pton() failed");

    return reinterpret_cast<sockaddr *>(ipv4);
  }
  if constexpr (AF == AddressFamily::IPv6) {
    const auto ipv6 =
        static_cast<sockaddr_in6 *>(std::malloc(SOCK_ADDR_SIZE<AF>));
    ipv6->sin6_family = AF_INET6;
    ipv6->sin6_port = htons(addr.port);
    if (inet_pton(AF_INET6, addr.ip.c_str(), &ipv6->sin6_addr) != 1)
      throw std::runtime_error("inet_pton() failed");

    return reinterpret_cast<sockaddr *>(ipv6);
  }

  const auto local =
      static_cast<sockaddr_un *>(std::malloc(SOCK_ADDR_SIZE<AF>));
  local->sun_family = AF_LOCAL;
  strncpy(local->sun_path, addr.ip.c_str(), sizeof(local->sun_path));

  return reinterpret_cast<sockaddr *>(local);
}

template <AddressFamily AF>
static void getWhereFrom(SocketAddress *from, sockaddr *addr) noexcept {
  if (from == nullptr) return;

  if constexpr (AF == AddressFamily::IPv4) {
    char buff[INET_ADDRSTRLEN];
    const auto ipv4 = reinterpret_cast<sockaddr_in *>(addr);
    inet_ntop(AF_INET, &ipv4->sin_addr, buff, INET_ADDRSTRLEN);
    from->ip = buff;
    from->port = ntohs(ipv4->sin_port);
    return;
  }
  if constexpr (AF == AddressFamily::IPv6) {
    char buff[INET6_ADDRSTRLEN];
    const auto ipv6 = reinterpret_cast<sockaddr_in6 *>(addr);
    inet_ntop(AF_INET6, &ipv6->sin6_addr, buff, INET6_ADDRSTRLEN);
    from->ip = buff;
    from->port = ntohs(ipv6->sin6_port);
    return;
  }
  const auto local = reinterpret_cast<sockaddr_un *>(addr);
  from->ip = local->sun_path;
  from->port = -1;
}

template <AddressFamily AF> Socket_<AF>::Socket_(const Protocol pr) {
  sockfd_ = socket(static_cast<int>(AF), static_cast<int>(pr), 0);
  if (sockfd_ == -1) {
    throw std::runtime_error("socket() failed");
  }
}

template <AddressFamily AF> Socket_<AF>::Socket_(Socket_ &&other) noexcept {
  sockfd_ = other.sockfd_;
  other.sockfd_ = -1;
}

static void closeSock(int &fd) noexcept {
  if (fd != -1) {
    ::close(fd);
    fd = -1;
  }
}

template <AddressFamily AF> void Socket_<AF>::close() noexcept {
  closeSock(sockfd_);
}

template <AddressFamily AF> Socket_<AF>::~Socket_() noexcept {
  closeSock(sockfd_);
}

template <AddressFamily AF>
void Socket_<AF>::bind(const SocketAddress &addr) const {
  constexpr socklen_t len = SOCK_ADDR_SIZE<AF>;
  sockaddr *sockAddr = getSockAddr<AF>(addr);
  const int flag = ::bind(sockfd_, sockAddr, len);

  std::free(sockAddr);
  if (flag != 0) throw std::runtime_error("bind() failed");
}

template <AddressFamily AF>
void Socket_<AF>::sendto(const void *const data, const size_t size,
                         const SocketAddress &addr) const {
  constexpr socklen_t len = SOCK_ADDR_SIZE<AF>;
  sockaddr *sockAddr = getSockAddr<AF>(addr);
  const ssize_t flag = ::sendto(sockfd_, data, size, 0, sockAddr, len);

  free(sockAddr);
  if (flag == -1) throw std::runtime_error("sendto() failed");
}

template <AddressFamily AF>
size_t Socket_<AF>::recvfrom(void *const buff, const size_t size,
                             SocketAddress *const from) const {
  socklen_t len = SOCK_ADDR_SIZE<AF>;
  sockaddr *addr = getSockAddrBuff<AF>();

  const ssize_t received = ::recvfrom(sockfd_, buff, size, 0, addr, &len);
  if (received == -1) {
    std::free(addr);
    throw std::runtime_error("recvfrom() failed");
  }

  getWhereFrom<AF>(from, addr);
  std::free(addr);
  return received;
}

template <AddressFamily AF> void Socket_<AF>::listen(const int backlog) const {
  if (::listen(sockfd_, backlog) != 0) {
    throw std::runtime_error("listen() failed");
  }
}

template <AddressFamily AF>
std::shared_ptr<Socket_<AF>>
Socket_<AF>::accept(SocketAddress *const from) const {
  socklen_t len = SOCK_ADDR_SIZE<AF>;
  sockaddr *addr = getSockAddrBuff<AF>();
  const int fd = ::accept(sockfd_, addr, &len);
  if (fd == -1) {
    std::free(addr);
    throw std::runtime_error("accept() failed");
  }

  getWhereFrom<AF>(from, addr);
  std::free(addr);

  const auto client = std::make_shared<Socket_>();
  client->sockfd_ = fd;
  return client;
}

template <AddressFamily AF>
void Socket_<AF>::connect(const SocketAddress &addr) const {
  constexpr socklen_t len = SOCK_ADDR_SIZE<AF>;
  sockaddr *sockAddr = getSockAddr<AF>(addr);
  const int flag = ::connect(sockfd_, sockAddr, len);

  std::free(sockAddr);
  if (flag != 0) throw std::runtime_error("connect() failed");
}

static void sockSend(const int fd, const uint8_t *const ptr,
                     const size_t size) {
  size_t sent = 0;

  while (true) {
    const ssize_t ret = ::send(fd, ptr + sent, size - sent, 0);
    if (ret == -1) throw std::runtime_error("send() failed");
    if (ret == 0) return;
    sent += ret;
  }
}

template <AddressFamily AF>
void Socket_<AF>::send(const void *const data, const size_t size) const {
  sockSend(sockfd_, static_cast<const uint8_t *>(data), size);
}

static size_t sockRecv(const int fd, uint8_t *const ptr, const size_t size) {
  size_t received = 0;

  while (true) {
    const ssize_t ret = ::recv(fd, ptr + received, size - received, 0);
    if (ret == -1) throw std::runtime_error("recv() failed");
    if (ret == 0) return received;
    received += ret;
  }
}

template <AddressFamily AF>
size_t Socket_<AF>::recv(void *const buff, const size_t size) const {
  return sockRecv(sockfd_, static_cast<uint8_t *>(buff), size);
}

template class Socket_<AddressFamily::IPv4>;
template class Socket_<AddressFamily::IPv6>;
template class Socket_<AddressFamily::LOCAL>;