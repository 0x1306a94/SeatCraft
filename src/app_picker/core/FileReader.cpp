//
// Created by king on 2025/8/13.
//

#include "FileReader.h"

#include <tgfx/core/Data.h>
namespace kk {
std::shared_ptr<tgfx::Data> FileReader::readData(const std::string &path) {
    return tgfx::Data::MakeFromFile(path);
}
}  // namespace kk