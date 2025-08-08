//
//  SeatAreaLayer.cpp
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#include "SeatAreaLayer.hpp"

#include "../SeatCraftCoreApp.hpp"

#include <tgfx/svg/SVGDOM.h>

#include <filesystem>

namespace fs = std::filesystem;

namespace kk::drawers {
std::shared_ptr<SeatAreaLayer> SeatAreaLayer::Make(const kk::SeatCraftCoreApp *app) {
    return std::shared_ptr<SeatAreaLayer>(new SeatAreaLayer(app));
}

SeatAreaLayer::SeatAreaLayer(const kk::SeatCraftCoreApp *app)
    : _app(app) {
}

std::shared_ptr<tgfx::SVGDOM> SeatAreaLayer::loadSvgDom(const std::string &path) {
    if (path.empty()) {
        return nullptr;
    }

    if (!fs::exists(path)) {
        return nullptr;
    }

    auto stream = tgfx::Stream::MakeFromFile(path);
    if (!stream) {
        return nullptr;
    }

    auto svgDom = tgfx::SVGDOM::Make(*stream);
    if (svgDom == nullptr) {
        return nullptr;
    }

    return svgDom;
}

void SeatAreaLayer::onUpdateContent(tgfx::LayerRecorder *recorder) {
    // if (_contentSize.isEmpty()) {
    //     return;
    // }

    // auto canvas = recorder->getCanvas(tgfx::LayerContentType::Default);

    // tgfx::Rect displayRect = tgfx::Rect::MakeWH(_contentSize.width, _contentSize.height);
    // auto backPath = tgfx::Path();
    // backPath.addRect(displayRect);

    // tgfx::Paint paint = {};
    // paint.setColor(tgfx::Color::Red());
    // canvas->drawPath(backPath, paint);

    auto areaSvgDom = loadSvgDom(_areaSvgPath);
    if (areaSvgDom == nullptr) {
        return;
    }

    auto canvas = recorder->getCanvas(tgfx::LayerContentType::Default);

    auto areaDomSize = areaSvgDom->getContainerSize();

    auto rootMatrix = areaSvgDom->getRoot()->getTransform();

    do {
        // 渲染区域图
        auto renderRect = tgfx::Rect::MakeSize(areaDomSize);
        renderRect = rootMatrix.mapRect(renderRect);

        float fitScaleX = areaDomSize.width / renderRect.width();
        float fitScaleY = areaDomSize.height / renderRect.height();
        float fitScale = std::min(fitScaleX, fitScaleY);

        float scaledRenderWidth = renderRect.width() * fitScale;
        float scaledRenderHeight = renderRect.height() * fitScale;
        float translateX = (areaDomSize.width - scaledRenderWidth) / 2.0f - renderRect.left * fitScale;
        float translateY = (areaDomSize.height - scaledRenderHeight) / 2.0f - renderRect.top * fitScale;

        tgfx::Matrix initialMatrix = tgfx::Matrix::I();
        initialMatrix.postScale(fitScale, fitScale);
        initialMatrix.postTranslate(translateX, translateY);

        canvas->save();

        canvas->concat(initialMatrix);
        areaSvgDom->render(canvas);

        canvas->restore();
    } while (0);

    do {
        // 渲染座位 Mock
        const auto &seatStatusSvgMap = _app->getSeatStatusSvgMap();
        auto iter = seatStatusSvgMap.find(1);
        if (iter == seatStatusSvgMap.end()) {
            return;
        }

        auto seatSvgDom = loadSvgDom(iter->second);
        if (seatSvgDom == nullptr) {
            return;
        }

        auto seatDomSize = seatSvgDom->getContainerSize();

        float seatStartX = areaDomSize.width * 0.1;
        float seatStartY = areaDomSize.height * 0.3;
        float lineSpacing = 10.0f;
        float itemSpacing = 10.0f;
        int rows = 100, columns = 10;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                float itemX = seatStartX + row * lineSpacing + row * seatDomSize.width;
                float itemY = seatStartY + col * itemSpacing + col * seatDomSize.height;

                auto matrix = tgfx::Matrix::MakeTrans(itemX, itemY);
                canvas->save();
                canvas->concat(matrix);
                seatSvgDom->render(canvas);

                canvas->restore();
            }
        }

    } while (0);
}

};  // namespace kk::drawers
