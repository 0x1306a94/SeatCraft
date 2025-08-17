#ifndef UIThreadScheduler_h
#define UIThreadScheduler_h

#include <functional>

namespace kk::thread {
class UIThreadScheduler {
  public:
    virtual ~UIThreadScheduler() = default;
    /**
     * @brief 将一个任务调度到主线程执行。
     * @param task 要在主线程上执行的函数。
     */
    virtual void schedule(std::function<void()> task) = 0;
};

}  // namespace kk::thread

#endif /* UIThreadScheduler_h */
