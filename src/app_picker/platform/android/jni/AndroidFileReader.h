//
// Created by king on 2025/8/13.
//

#ifndef SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_PLATFORM_ANDROID_JNI_ANDROIDFILEREADER_H
#define SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_PLATFORM_ANDROID_JNI_ANDROIDFILEREADER_H

#include "core/FileReader.h"

#include <android/asset_manager.h>

namespace kk {
class AndroidFileReader : public FileReader {
  public:
    explicit AndroidFileReader(AAssetManager *assetMgr);
    virtual ~AndroidFileReader() = default;

    std::shared_ptr<tgfx::Data> readData(const std::string &path) override;

  private:
    AAssetManager *_assetMgr{nullptr};
};
}  // namespace kk

#endif  // SEATCRAFTAPPPICKEREXAMPLE_SRC_APP_PICKER_PLATFORM_ANDROID_JNI_ANDROIDFILEREADER_H
