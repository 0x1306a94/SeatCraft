//
// Created by king on 2025/8/13.
//

#include "AndroidFileReader.h"

#include <tgfx/core/Data.h>

namespace kk {

static const std::string ASSET_PATH_PREFIX = "asset://";
AndroidFileReader::AndroidFileReader(AAssetManager *assetMgr)
    : FileReader()
    , _assetMgr(assetMgr) {
}

std::shared_ptr<tgfx::Data> AndroidFileReader::readData(const std::string &path) {
    if (path.find(ASSET_PATH_PREFIX) == std::string::npos) {
        return tgfx::Data::MakeFromFile(path);
    }

    if (_assetMgr == nullptr) {
        return nullptr;
    }
    auto assetPath = path.substr(ASSET_PATH_PREFIX.length(), path.length() - ASSET_PATH_PREFIX.length());
    auto asset = AAssetManager_open(_assetMgr, assetPath.c_str(), AASSET_MODE_UNKNOWN);
    if (asset == nullptr) {
        return nullptr;
    }

    auto length = AAsset_getLength64(asset);
    if (length == 0) {
        return nullptr;
    }
    auto buffer = AAsset_getBuffer(asset);
    auto data = tgfx::Data::MakeWithCopy(buffer, static_cast<size_t>(length));
    AAsset_close(asset);

    return data;
}

}  // namespace kk