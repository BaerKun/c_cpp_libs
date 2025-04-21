#include "thread_pool.hpp"
//#include "infer/vino.h"
#include <iostream>
//#include <opencv2/opencv.hpp>
//#include <filesystem>
#include "socket/socket.hpp"


void testThreadPool() {
    ThreadPool<FixedTaskQueue<100>> pool(4, 100);
    long long result[1000] = {};

    auto start = std::chrono::steady_clock::now();
    for (long long i = 0; i < 1000; ++i) {
        // std::this_thread::sleep_for(std::chrono::microseconds(1));
        Task task = [i, &result](const TaskState state, int id) {
            if (state == TaskState::REJECTED) {
                result[i] = -1;
                return;
            }
            long long sum = 0;
            for (long long j = i; j < i * 400; ++j) {
                sum += j;
            }
            result[i] = sum;
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        };
        if (pool.isQueueFull()) {
            pool.runTask();
        }
        pool.pushTask(task);
    }

    while (!pool.isTaskOver()) {
        pool.runTask();
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "ns"
              << std::endl;

    // for (int i = 99; i >= 0; --i) {
    //     std::cout << i << ": " << result[i] << std::endl;
    // }
}

int main(){
    testThreadPool();
    // const auto server = createSocket(AddressFamily::IPv4, Protocol::TCP);
    // server.bind({"0.0.0.0", 6667});
    // server.listen(10);
    //
    // SocketAddress from;
    // const auto client = server.accept(&from);
    // client->send("hello", 5);
    // char buff[100] = {};
    // size_t size = client->recv(buff, 100);
    // std::cout << from.ip << ":" << from.port << std::endl;
    // std::cout << buff << std::endl;
    // std::cout << size << std::endl;

    // auto client = createSocket(AddressFamily::IPv4, Protocol::UDP);
    // client.bind({"0.0.0.0", 6667});
    // client.sendto("hello", 5, {"127.0.0.1", 6666});
    // char buff[100] = {};
    // SocketAddress from;
    // size_t size = client.recvfrom(buff, 100, &from);
    // std::cout << from.ip << ":" << from.port << std::endl;
    // std::cout << buff << std::endl;
    // std::cout << size << std::endl;
    // client.close();
    // std::cout << sizeof(std::function<void()>) << std::endl;
}