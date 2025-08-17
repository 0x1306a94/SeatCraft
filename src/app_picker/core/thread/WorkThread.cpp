#include "WorkThread.h"

#include <tgfx/platform/Print.h>

namespace kk::thread {

WorkThread::WorkThread() {
    _thread = std::thread(&WorkThread::run, this);
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

WorkThread::~WorkThread() {
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _stop = true;
    }
    _cv.notify_one();
    _thread.join();
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

void WorkThread::post(std::function<void()> task) {
    {
        _tasks.push(std::move(task));
    }
    _cv.notify_one();
}

void WorkThread::run() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(_mutex);

            _cv.wait(lock, [this] { return _stop || !_tasks.empty(); });

            if (_stop && _tasks.empty()) {
                return;
            }

            task = std::move(_tasks.front());
            _tasks.pop();
        }
        task();
    }
}

}  // namespace kk::thread
