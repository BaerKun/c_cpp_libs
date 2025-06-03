#include "vino.h"

VinoAsyncInfer::VinoAsyncInfer(const ov::CompiledModel &model,
                               const size_t numReq) {
  model_ = model;
  requests_ = std::vector<ov::InferRequest>(numReq);

  for (int i = static_cast<int>(requests_.size()) - 1; i >= 0; --i) {
    requests_[i] = model_.create_infer_request();
    freeReqId_.push(i);
  }

  for (int id = 0; id < requests_.size(); ++id) {
    ov::InferRequest &request = requests_[id];
    request.set_callback([this, &request, id](const std::exception_ptr &eptr) {
      if (eptr) {
        try {
          std::rethrow_exception(eptr);
        } catch (const std::exception &e) {
          std::cerr << e.what() << std::endl;
        }
      }

      if (callback_) callback_(request, id);

      mutex_.lock();
      freeReqId_.push(id);
      mutex_.unlock();

      inferOver_.notify_one();
    });
  }
}

int VinoAsyncInfer::getReqId() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (freeReqId_.empty()) return -1;

  const int reqId = freeReqId_.top();
  freeReqId_.pop();
  return reqId;
}

int VinoAsyncInfer::waitReqId() {
  std::unique_lock<std::mutex> lock(mutex_);
  if (freeReqId_.empty())
    inferOver_.wait(lock, [this] { return !freeReqId_.empty(); });

  const int reqId = freeReqId_.top();
  freeReqId_.pop();
  return reqId;
}

void VinoAsyncInfer::asyncInfer(const int reqId) {
  if (reqId < 0 || reqId >= requests_.size())
    throw std::runtime_error("Invalid request id");
  if (!pushInput_) throw std::runtime_error("Push input function is not set");

  auto &req = requests_[reqId];
  pushInput_(req, reqId);
  req.start_async();
}