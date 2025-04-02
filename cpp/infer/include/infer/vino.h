#ifndef VINO_INFER_H
#define VINO_INFER_H

#include <condition_variable>
#include <functional>
#include <openvino/openvino.hpp>

class VinoAsyncInfer {
public:
    using PushInput = std::function<void(ov::InferRequest &, int reqId)>;
    using InferCallback = std::function<void(ov::InferRequest &, int reqId)>;

    explicit VinoAsyncInfer(const ov::CompiledModel &model);

    ~VinoAsyncInfer() = default;

    int getReqId();

    int waitReqId();

    void asyncInfer(int reqId);

    [[nodiscard]] int getNumReq() const {
        return static_cast<int>(requests_.size());
    }

    void setCallback(const InferCallback &callback) {
        callback_ = callback;
    }

    void setCallback(InferCallback &&callback) {
        callback_ = callback;
    }

    void setPushInput(const PushInput &pushInput) {
        pushInput_ = pushInput;
    }

    void setPushInput(PushInput &&pushInput) {
        pushInput_ = pushInput;
    }

private:
    ov::CompiledModel model_;

    std::vector<ov::InferRequest> requests_;
    std::stack<int> freeReqId_;

    std::mutex mutex_;
    std::condition_variable inferOver_;

    PushInput pushInput_;
    InferCallback callback_;
};


#endif //VINO_INFER_H
