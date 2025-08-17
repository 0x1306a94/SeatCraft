#ifndef AndroidUIThreadScheduler_h
#define AndroidUIThreadScheduler_h

#include "core/thread/UIThreadScheduler.h"

namespace kk::thread {
class AndroidUIThreadScheduler : public UIThreadScheduler {
  public:
    virtual ~AndroidUIThreadScheduler() = default;
    /**
     * @brief 将一个任务调度到主线程执行。
     * @param task 要在主线程上执行的函数。
     */
    virtual void schedule(std::function<void()> task) override;
};

}  // namespace kk::thread

#endif /* AndroidUIThreadScheduler_h */
