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

    if (_areaSvgPath.empty()) {
        return;
    }

    if (!fs::exists(_areaSvgPath)) {
        return;
    }

    auto stream = tgfx::Stream::MakeFromFile(_areaSvgPath);
    if (!stream) {
        return;
    }

    auto svgDom = tgfx::SVGDOM::Make(*stream);
    if (svgDom == nullptr) {
        return;
    }

    auto domSize = svgDom->getContainerSize();

    auto rootMatrix = svgDom->getRoot()->getTransform();
    auto renderRect = tgfx::Rect::MakeSize(domSize);
    renderRect = rootMatrix.mapRect(renderRect);

    float fitScaleX = domSize.width / renderRect.width();
    float fitScaleY = domSize.height / renderRect.height();
    float fitScale = std::min(fitScaleX, fitScaleY);

    float scaledRenderWidth = renderRect.width() * fitScale;
    float scaledRenderHeight = renderRect.height() * fitScale;
    float translateX = (domSize.width - scaledRenderWidth) / 2.0f - renderRect.left * fitScale;
    float translateY = (domSize.height - scaledRenderHeight) / 2.0f - renderRect.top * fitScale;

    tgfx::Matrix initialMatrix = tgfx::Matrix::I();
    initialMatrix.postScale(fitScale, fitScale);
    initialMatrix.postTranslate(translateX, translateY);

    auto canvas = recorder->getCanvas(tgfx::LayerContentType::Default);
    canvas->concat(initialMatrix);
    svgDom->render(canvas);
}

};  // namespace kk::drawers
