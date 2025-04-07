#include "thread_pool.hpp"
//#include "infer/vino.h"
#include <iostream>
//#include <opencv2/opencv.hpp>
//#include <filesystem>
#include "socket/socket.hpp"


void testThreadPool() {
    ThreadPool pool(4, 20);
    long long result[100] = {};

    for (long long i = 0; i < 100; ++i) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        ThreadPool::Task task = [i, &result](ThreadPool::State state, int id) {
            if (state == ThreadPool::State::REJECTED) {
                result[i] = -1;
                return;
            }
            long long sum = 0;
            for (long long j = i; j < i * 400; ++j) {
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

    for (int i = 99; i >= 0; --i) {
        std::cout << i << ": " << result[i] << std::endl;
    }
}

//int main() {
//    std::string directory = "/home/beak/wksp/c-libs/cpp/test";
//    std::vector<std::string> image_paths;
//    for (const auto &entry: std::filesystem::directory_iterator(directory)) {
//        if (entry.is_regular_file()) {
//            std::string ext = entry.path().extension().string();
//            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
//
//            if (ext == ".jpg" || ext == ".png" || ext == ".bmp") {
//                image_paths.push_back(entry.path().string());
//            }
//        }
//    }
//
//    ov::Core core;
//    auto model = core.read_model("/home/beak/wksp/c-libs/cpp/model/dafu-n.xml");
//    ov::preprocess::PrePostProcessor ppp(model);
//
//    constexpr int N = 1;
//    ppp.input().tensor().set_layout(ov::Layout("NHWC"))
//            .set_shape(ov::Shape{N, 648, 1152, 3})
//            .set_element_type(ov::element::Type_t::u8)
//            .set_color_format(ov::preprocess::ColorFormat::BGR);
//
//    ppp.input().preprocess().convert_layout(ov::Layout("NCHW"))
//            .resize(ov::preprocess::ResizeAlgorithm::RESIZE_LINEAR, 416, 416)
//            .convert_element_type(ov::element::Type_t::f32)
//            .scale(255.f);
//    model = ppp.build();
//
//    ov::CompiledModel compiledModel
//            = core.compile_model(model, "AUTO", ov::hint::performance_mode(ov::hint::PerformanceMode::THROUGHPUT));
//
//    std::cout << "optimal_number_of_infer_requests: "
//            << compiledModel.get_property(ov::optimal_number_of_infer_requests) << std::endl;
//
//
//    if (0) {
//        ThreadPool pool((int) compiledModel.get_property(ov::optimal_number_of_infer_requests), 10);
//        while (true) {
//            int num = 0;
//            auto start = std::chrono::steady_clock::now();
//
//            for (const auto &image_path: image_paths) {
//                cv::Mat img = cv::imread(image_path);
//                pool.pushTask([&num, &compiledModel, img](ThreadPool::State state, int id) {
//                    if (state == ThreadPool::State::REJECTED) {
//                        return;
//                    }
//                    const ov::Tensor t(ov::element::Type_t::u8, ov::Shape{N, 648, 1152, 3}, img.data);
//
//                    ov::InferRequest req = compiledModel.create_infer_request();
//                    req.set_input_tensor(t);
//                    req.infer();
//                    ++num;
//                });
//            }
//            pool.waitTaskOver();
//            auto end = std::chrono::steady_clock::now();
//            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//            std::cout << "num: " << num << std::endl;
//            std::cout << "total time: " << duration.count() << "ns" << std::endl;
//            std::cout << "speed: " << duration.count() / num << "ns" << std::endl;
//        }
//    }
//
//
//    VinoAsyncInfer infer(compiledModel);
//    cv::Mat buff[infer.getNumReq()];
//
//    infer.setPushInput([&buff](ov::InferRequest &req, const int id) {
//        const ov::Tensor t(ov::element::Type_t::u8, ov::Shape{N, 648, 1152, 3}, buff[id].data);
//        req.set_input_tensor(t);
//    });
//
//    while (true) {
//        int num = 0;
//        auto start = std::chrono::steady_clock::now();
//
//        for (const auto &image_path: image_paths) {
//            const int id = infer.waitReqId();
//
//            buff[id] = cv::imread(image_path);
//            infer.asyncInfer(id);
//            num++;
//        }
//
//        auto end = std::chrono::steady_clock::now();
//        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//        std::cout << "num: " << num << std::endl;
//        std::cout << "total time: " << duration.count() << "ns" << std::endl;
//        std::cout << "speed: " << duration.count() / num << "ns" << std::endl;
//    }
//}

int main(){
//    Socket server(AddressFamily::IPv4, Protocol::TCP);
//    server.bind("127.0.0.1", 6666);
//    server.listen(10);
//    Socket *client = server.accept();
//    client->send("hello", 5);
//    char buff[100] = {};
//    size_t size = client->recv(buff, 100);
//    std::cout << size << std::endl;
//    std::cout << buff << std::endl;
//    delete client;
//    server.close();
    Socket client(AddressFamily::IPv4, Protocol::UDP);
    client.bind("127.0.0.1", 6667);
    client.sendto("hello", 5, "127.0.0.1", 6666);
    char buff[100] = {};
    size_t size = client.recvfrom(buff, 100, nullptr);
    std::cout << size << std::endl;
    std::cout << buff << std::endl;
}