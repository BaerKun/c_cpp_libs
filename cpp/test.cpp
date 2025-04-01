#include "thread_pool.hpp"
#include "infer/vino.h"
#include <iostream>
#include <opencv2/opencv.hpp>

void testThreadPool() {
    ThreadPool pool(4, 20);
    long long result[100] = {};

    for (long long i = 0; i < 100; ++i){
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        ThreadPool::Task task = [i, &result]()  {
            long long sum = 0;
            for(long long j = i; j < i * 400; ++j){
                sum += j;
            }
            result[i] = sum;
            std::this_thread::sleep_for(std::chrono::microseconds(500));
        };
        if (pool.isQueueFull()) {
            pool.runTask();
        }
        pool.pushTask(task);
    }

    while (!pool.isTaskOver()) {
        pool.runTask();
    }

    for (int i = 99; i >= 0; --i){
        std::cout << i << ": " << result[i] << std::endl;
    }
}

int main() {
    cpp_libs::VinoInfer infer("/home/beak/wksp/c-libs/cpp/model/mobilenetv3.xml", "AUTO", cpp_libs::VinoInfer::PerformanceMode::THROUGHPUT);
    auto callback = [](void *data, const ov::Shape &shape) {
        auto ptr = (float *)data;
        for (int i = 0; i < 10; ++i) {
            std::cout << i << ": " << ptr[i] << std::endl;
        }
        std::cout << shape << std::endl;
    };
    infer.setCallback(callback);
    cv::Mat img = cv::imread("/home/beak/wksp/c-libs/cpp/test/0.png");
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    img.convertTo(img, CV_32FC1, 1.0 / 255.0);
    infer.infer(img.data, cpp_libs::VinoInfer::InferMode::SYNC);

    return 0;
}
