//
// Created by king on 2025/8/13.
//

#ifndef SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_CORE_FILEREADER_H
#define SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_CORE_FILEREADER_H

#include <memory>
#include <string>

namespace tgfx {
class Data;
};

namespace kk {
class FileReader {
  public:
    FileReader() = default;
    virtual ~FileReader() = default;
    virtual std::shared_ptr<tgfx::Data> readData(const std::string &path);
};

}  // namespace kk

#endif  // SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_CORE_FILEREADER_H
