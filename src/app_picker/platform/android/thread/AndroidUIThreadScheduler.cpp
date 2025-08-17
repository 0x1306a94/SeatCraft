#include "AndroidUIThreadScheduler.h"

namespace kk::thread {
void AndroidUIThreadScheduler::schedule(std::function<void()> task) {
    if (task == nullptr) {
        return;
    }
}

}  // namespace kk::thread
