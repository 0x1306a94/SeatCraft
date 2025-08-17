#include "WebUIThreadScheduler.h"

#include <emscripten.h>

namespace kk::thread {
void WebUIThreadScheduler::schedule(std::function<void()> task) {
    if (task == nullptr) {
        return;
    }
    task();
}

}  // namespace kk::thread
