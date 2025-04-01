#ifndef VINO_INFER_H
#define VINO_INFER_H

#include <functional>
#include <openvino/openvino.hpp>

namespace cpp_libs {
    class VinoInfer {
    public:
        using PerformanceMode = ov::hint::PerformanceMode;
        using InferCallback = std::function<void(void *, const ov::Shape &)>;

        enum class InferMode { SYNC, ASYNC };

        explicit VinoInfer(const std::string &model_path,
                           const std::string &device = "AUTO",
                           PerformanceMode mode = PerformanceMode::LATENCY);

        void setCallback(const InferCallback &callback);

        bool infer(const void *data, InferMode mode = InferMode::ASYNC);

        ~VinoInfer() = default;

    private:
        ov::Core core_;
        ov::CompiledModel model_;

        std::mutex mutex_;
        std::vector<ov::InferRequest> requests_;
        std::stack<ov::InferRequest *, std::vector<ov::InferRequest *>> freeRequests_;

        InferCallback callback_;
    };
} // cpp_libs

#endif //VINO_INFER_H
