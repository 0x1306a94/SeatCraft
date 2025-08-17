#include "AndroidUIThreadScheduler.h"

#include <SeatCraft/common/common_macro.h>

#include <android/log.h>
#include <fcntl.h>
#include <unistd.h>

#define LOG_TAG "AndroidUIThreadScheduler"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace kk::thread {
AndroidUIThreadScheduler::AndroidUIThreadScheduler() {
    init();
}

AndroidUIThreadScheduler::~AndroidUIThreadScheduler() {
    if (looper_) {
        if (readFd_ != -1) {
            ALooper_removeFd(looper_, readFd_);
        }
        ALooper_release(looper_);
    }
    if (readFd_ != -1) {
        close(readFd_);
    }
    if (writeFd_ != -1) {
        close(writeFd_);
    }
}

void AndroidUIThreadScheduler::schedule(std::function<void()> task) {
    if (task == nullptr) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        pendingTasks_.emplace_back(std::move(task));
    }

    char dummy = 1;
    if (write(writeFd_, &dummy, sizeof(dummy)) != sizeof(dummy)) {
        LOGE("Failed to write to pipe to wake up looper.");
    }
}

void AndroidUIThreadScheduler::init() {
    looper_ = ALooper_forThread();
    if (!looper_) {
        LOGE("ALooper_forThread() failed. Must be called from the main thread.");
        return;
    }

    ALooper_acquire(looper_);

    int pipeFd[2];
    if (pipe(pipeFd) != 0) {
        LOGE("Could not create pipe.");
        return;
    }
    readFd_ = pipeFd[0];
    writeFd_ = pipeFd[1];

    fcntl(readFd_, F_SETFL, O_NONBLOCK);
    ALooper_addFd(looper_, readFd_, ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT, &AndroidUIThreadScheduler::looperCallback, this);
}

int AndroidUIThreadScheduler::looperCallback(int fd, int events, void *data) {
    UNUSED_PARAM(fd);
    UNUSED_PARAM(events);
    auto *self = static_cast<AndroidUIThreadScheduler *>(data);
    self->handleCallbacks();
    return 1;
}

void AndroidUIThreadScheduler::handleCallbacks() {
    char dummy;
    while (read(readFd_, &dummy, sizeof(dummy)) > 0) {
        // nothing
    }

    std::vector<std::function<void()>> tasksToRun;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasksToRun.swap(pendingTasks_);
    }

    for (const auto &task : tasksToRun) {
        task();
    }
}

}  // namespace kk::thread
