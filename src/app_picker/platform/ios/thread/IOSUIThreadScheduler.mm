#import "IOSUIThreadScheduler.h"

#import <dispatch/dispatch.h>

namespace kk::thread {
void IOSUIThreadScheduler::schedule(std::function<void()> task) {
    if (task == nullptr) {
        return;
    }

    dispatch_async(dispatch_get_main_queue(), ^{
        task();
    });
}

}  // namespace kk::thread
