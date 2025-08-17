#include "core/svg/SVGDataProvider.h"

#include <tgfx/core/Data.h>
#include <tgfx/core/Stream.h>
#include <tgfx/svg/SVGDOM.h>

namespace kk::svg {

bool SVGDataProvider::setAreaSVGData(std::shared_ptr<tgfx::Data> data) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (!data || data->empty()) {
        _areaDOM = nullptr;
        return false;
    }
    auto stream = tgfx::Stream::MakeFromData(data);
    if (!stream) {
        _areaDOM = nullptr;
        return false;
    }

    _areaDOM = tgfx::SVGDOM::Make(*stream);
    return _areaDOM != nullptr;
}

void SVGDataProvider::setSeatStatusSVGData(const kk::SeatStatusSVGDataMap &dataMap) {
    std::lock_guard<std::mutex> lock(_mutex);
    _seatStatusDOMs.clear();
    for (const auto &[key, data] : dataMap) {
        if (data && !data->empty()) {
            auto stream = tgfx::Stream::MakeFromData(data);
            if (!stream) {
                continue;
            }
            auto dom = tgfx::SVGDOM::Make(*stream);
            if (dom) {
                _seatStatusDOMs[key] = dom;
            }
        }
    }
}

std::shared_ptr<tgfx::SVGDOM> SVGDataProvider::getAreaSVGDOM() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _areaDOM;
}

bool SVGDataProvider::isSeatStatusEmpty() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _seatStatusDOMs.empty();
}

kk::SeatStatusSVGDOMMap::size_type SVGDataProvider::seatStatusSize() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _seatStatusDOMs.size();
}

std::shared_ptr<tgfx::SVGDOM> SVGDataProvider::getSeatSVGDOM(SeatStatusKey key) const {
    std::lock_guard<std::mutex> lock(_mutex);
    auto it = _seatStatusDOMs.find(key);
    if (it != _seatStatusDOMs.end()) {
        return it->second;
    }
    return nullptr;
}

kk::SeatStatusSVGDOMMap SVGDataProvider::seatStatusDOMs() const {
    std::lock_guard<std::mutex> lock(_mutex);
    return _seatStatusDOMs;
}

void SVGDataProvider::clear() {
    std::lock_guard<std::mutex> lock(_mutex);
    _areaDOM = nullptr;
    _seatStatusDOMs.clear();
}

}  // namespace kk::svg
