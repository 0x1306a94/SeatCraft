#ifndef WebUIThreadScheduler_h
#define WebUIThreadScheduler_h

#include "core/thread/UIThreadScheduler.h"

namespace kk::thread {
class WebUIThreadScheduler : public UIThreadScheduler {
  public:
    virtual ~WebUIThreadScheduler() = default;
    /**
     * @brief 将一个任务调度到主线程执行。
     * @param task 要在主线程上执行的函数。
     */
    virtual void schedule(std::function<void()> task) override;
};

}  // namespace kk::thread

#endif /* WebUIThreadScheduler_h */
