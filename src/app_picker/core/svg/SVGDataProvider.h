#ifndef SVGDataProvider_h
#define SVGDataProvider_h

#include "core/SeatStatusKey.h"

#include <memory>
#include <mutex>

namespace tgfx {
class Data;
class SVGDOM;
}  // namespace tgfx

namespace kk::svg {

/**
 * @class SVGDataProvider
 * @brief 负责管理、解析和提供所有SVG资源。
 * 这是一个核心的资源管理类，它接收原始的SVG二进制数据，
 * 内部完成解析并缓存SVGDOM对象，供渲染层按需获取。
 * @note 该类是线程安全的。
 */
class SVGDataProvider {
  public:
    SVGDataProvider() = default;
    ~SVGDataProvider() = default;

    SVGDataProvider(const SVGDataProvider &) = delete;
    SVGDataProvider &operator=(const SVGDataProvider &) = delete;

    // --- 数据输入接口 ---

    /**
     * @brief 设置场馆区域的SVG数据，并立即进行解析和缓存。
     * @param data 场馆SVG的二进制数据。
     * @return 如果数据有效且解析成功，返回 true。
     */
    bool setAreaSVGData(std::shared_ptr<tgfx::Data> data);

    /**
     * @brief 批量设置所有座位状态的SVG数据，并立即进行解析和缓存。
     * 这会清空所有旧的座位状态数据。
     * @param dataMap 从座位状态Key到SVG二进制数据的映射。
     */
    void setSeatStatusSVGData(const kk::SeatStatusSVGDataMap &dataMap);

    // --- 数据输出接口 ---

    /**
     * @brief 获取解析后的场馆区域SVGDOM对象。
     * @return 返回缓存的SVGDOM对象，如果解析失败或未设置则返回nullptr。
     */
    std::shared_ptr<tgfx::SVGDOM> getAreaSVGDOM() const;

    bool isSeatStatusEmpty() const;

    kk::SeatStatusSVGDOMMap::size_type seatStatusSize() const;
    /**
     * @brief 根据座位状态Key获取对应的SVGDOM对象。
     * @param key 座位状态的唯一标识。
     * @return 返回对应的SVGDOM对象，如果未找到或解析失败则返回nullptr。
     */
    std::shared_ptr<tgfx::SVGDOM> getSeatSVGDOM(SeatStatusKey key) const;

    kk::SeatStatusSVGDOMMap seatStatusDOMs() const;
    /**
     * @brief 清空所有已加载和解析的SVG数据，用于资源重新加载。
     */
    void clear();

  private:
    // 缓存解析后的场馆SVGDOM
    std::shared_ptr<tgfx::SVGDOM> _areaDOM;

    // 缓存解析后的座位状态SVGDOM
    kk::SeatStatusSVGDOMMap _seatStatusDOMs;

    mutable std::mutex _mutex;
};

}  // namespace kk::svg

#endif /* SVGDataProvider_h */
