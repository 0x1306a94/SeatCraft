#ifndef WorkThread_h
#define WorkThread_h

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace kk::thread {

/**
 * @class WorkThread
 * @brief 一个生命周期受管理的后台工作线程，包含一个任务队列。
 * 任务被 post 到队列中，由后台线程按顺序执行。
 * 在析构时，会安全地等待所有剩余任务完成并终止线程。
 */
class WorkThread {
  public:
    WorkThread();
    ~WorkThread();

    WorkThread(const WorkThread &) = delete;
    WorkThread &operator=(const WorkThread &) = delete;

    /**
     * @brief 向任务队列中投递一个任务。
     * @param task 要在后台线程执行的任务。
     */
    void post(std::function<void()> task);

  private:
    void run();

    std::atomic<bool> _stop{false};
    std::thread _thread;
    std::queue<std::function<void()>> _tasks;
    std::mutex _mutex;
    std::condition_variable _cv;
};

}  // namespace kk::thread

#endif /* WorkThread_h */