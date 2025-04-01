#include "infer/vino.h"

namespace cpp_libs {
    VinoInfer::VinoInfer(const std::string &model_path, const std::string &device, const PerformanceMode mode) {
        const auto model = core_.read_model(model_path);
        model_ = core_.compile_model(model, device, ov::hint::performance_mode(mode));

        requests_ = std::vector<ov::InferRequest>(model_.get_property(ov::optimal_number_of_infer_requests));

        std::vector<ov::InferRequest *> buff(requests_.size());
        freeRequests_ = std::stack<ov::InferRequest *, std::vector<ov::InferRequest *> >(std::move(buff));

        for (auto &request: requests_) {
            request = model_.create_infer_request();
            freeRequests_.push(&request);
        }

        callback_ = [](void *, const ov::Shape &) {
        };
    }

    void VinoInfer::setCallback(const InferCallback &callback) {
        callback_ = callback;

        for (auto &request: requests_) {
            request.set_callback([this, &request](const std::exception_ptr &eptr) {
                if (eptr) {
                    try {
                        std::rethrow_exception(eptr);
                    } catch (const std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }
                }

                const ov::Tensor &output = request.get_output_tensor();
                callback_(output.data(), output.get_shape());

                mutex_.lock();
                freeRequests_.push(&request);
                mutex_.unlock();
            });
        }
    }


    bool VinoInfer::infer(const void *data, const InferMode mode) {
        mutex_.lock();
        if (freeRequests_.empty()) {
            mutex_.unlock();
            return false;
        }

        const auto request = freeRequests_.top();
        freeRequests_.pop();
        mutex_.unlock();

        const ov::Tensor tensor(model_.input().get_element_type(), ov::Shape({1, 1, 224, 224}));
        std::memcpy(tensor.data(), data, tensor.get_byte_size());
        request->set_input_tensor(tensor);

        if (mode == InferMode::SYNC) {
            request->infer();
            const auto output = request->get_output_tensor();
            callback_(output.data(), output.get_shape());

            mutex_.lock();
            freeRequests_.push(request);
            mutex_.unlock();
        } else {
            request->start_async();
        }
        return true;
    }
} // cpp_libs
