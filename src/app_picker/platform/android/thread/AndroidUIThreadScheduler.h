#ifndef AndroidUIThreadScheduler_h
#define AndroidUIThreadScheduler_h

#include "core/thread/UIThreadScheduler.h"

#include <android/looper.h>
#include <functional>
#include <mutex>
#include <vector>

namespace kk::thread {
class AndroidUIThreadScheduler : public UIThreadScheduler {
  public:
    AndroidUIThreadScheduler();
    virtual ~AndroidUIThreadScheduler();
    /**
     * @brief 将一个任务调度到主线程执行。
     * @param task 要在主线程上执行的函数。
     */
    virtual void schedule(std::function<void()> task) override;

  private:
    static int looperCallback(int fd, int events, void *data);
    void init();
    void handleCallbacks();

    ALooper *looper_ = nullptr;
    int readFd_ = -1;
    int writeFd_ = -1;

    std::vector<std::function<void()>> pendingTasks_;
    std::mutex mutex_;
};

}  // namespace kk::thread

#endif /* AndroidUIThreadScheduler_h */
