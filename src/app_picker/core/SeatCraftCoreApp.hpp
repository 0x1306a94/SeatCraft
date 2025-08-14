//
//  SeatCraftCoreApp.hpp
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#ifndef SeatCraftCoreApp_hpp
#define SeatCraftCoreApp_hpp

#include <memory>
#include <string>
#include <unordered_map>

#include <tgfx/core/Point.h>
#include <tgfx/core/Size.h>

#include "SeatStatusKey.h"

namespace tgfx {
class SVGDOM;
class Stream;
};  // namespace tgfx

namespace kk {
class FileReader;
class SeatCraftCoreApp {
  public:
    explicit SeatCraftCoreApp(const tgfx::Size &boundSize = tgfx::Size{1280, 720}, const tgfx::Size &contentSize = tgfx::Size{0, 0}, float density = 1.0f);

    ~SeatCraftCoreApp();

    void setFileReader(std::shared_ptr<FileReader> fileReader);

    std::shared_ptr<FileReader> getFileReader() const;
    /// canvas 尺寸
    tgfx::Size getBoundsSize() const;
    /// 内容尺寸
    tgfx::Size getContentSize() const;

    /// svg原始尺寸
    tgfx::Size getOriginSize() const;

    float density() const;

    /// 当前缩放比例
    float zoomScale() const;

    /// 当前滑动偏移
    const tgfx::Point &contentOffset() const;

    std::shared_ptr<tgfx::SVGDOM> getSvgDom() const;

    std::string getAreaSvgPath() const;

    const kk::SeatStatusSVGPathMap &getSeatStatusSvgMap() const;

    bool updateContentSize(const tgfx::Size &contentSize);
    bool updateScreen(const tgfx::Size &boundSize, float density);
    bool updateZoomAndOffset(float zoomScale, const tgfx::Point &contentOffset);
    bool updateAreaSvgPath(const std::string &path);
    bool updateAreaSvgData(std::unique_ptr<tgfx::Stream> data);
    bool updateSeatStatusSVGPathMap(kk::SeatStatusSVGPathMap map);

  private:
    /// canvas 尺寸
    tgfx::Size _boundSize{1280, 720};
    /// 内容尺寸
    tgfx::Size _contentSize{};
    float _density = 1.0f;
    float _zoomScale = 1.0f;
    tgfx::Point _contentOffset = {};
    std::string _areaSvgPath{};
    std::shared_ptr<tgfx::SVGDOM> _svgDom{nullptr};
    kk::SeatStatusSVGPathMap _seatStatusSvgMap{};
    std::shared_ptr<FileReader> _fileReader{nullptr};
};

};  // namespace kk

#endif /* SeatCraftCoreApp_hpp */
