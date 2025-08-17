
#include <emscripten/bind.h>

#include "core/SeatCraftCoreApp.hpp"
#include "core/renderer/SeatCraftCoreRenderer.hpp"
#include "core/svg/SVGDataProvider.h"
#include "core/ui/ElasticZoomPanController.hpp"
#include "core/ui/SeatCraftViewCore.hpp"
#include "platform/web/renderer/WebRendererBackend.h"
#include "thread/WebUIThreadScheduler.h"

#include <tgfx/core/Data.h>
#include <tgfx/core/Stream.h>
#include <tgfx/platform/Print.h>

using namespace emscripten;

std::shared_ptr<tgfx::Data> GetDataFromEmscripten(const val &emscriptenData) {
    if (emscriptenData.isUndefined()) {
        return nullptr;
    }
    unsigned int length = emscriptenData["length"].as<unsigned int>();
    if (length == 0) {
        return nullptr;
    }
    auto buffer = new (std::nothrow) uint8_t[length];
    if (buffer) {
        auto memory = val::module_property("HEAPU8")["buffer"];
        auto memoryView = emscriptenData["constructor"].new_(memory, reinterpret_cast<uintptr_t>(buffer), length);
        memoryView.call<void>("set", emscriptenData);
        return tgfx::Data::MakeAdopted(buffer, length, tgfx::Data::DeleteProc);
    }
    return nullptr;
}

EMSCRIPTEN_BINDINGS(SeatCraftAppPicker) {

    auto makeFrom = optional_override([](const std::string &canvasID, float density) {
        tgfx::PrintLog("%s canvasID %s density %.f", __PRETTY_FUNCTION__, canvasID.c_str(), density);
        if (canvasID.empty()) {
            return std::shared_ptr<kk::ui::SeatCraftViewCore>(nullptr);
        }

        auto app = std::make_shared<kk::SeatCraftCoreApp>(tgfx::Size::MakeEmpty(), tgfx::Size::MakeEmpty(), density);
        auto backend = std::make_unique<kk::renderer::WebRendererBackend>(canvasID, density);
        auto zoomPanController = std::make_unique<kk::ui::ElasticZoomPanController>();
        auto uiScheduler = std::make_shared<kk::thread::WebUIThreadScheduler>();
        return std::make_shared<kk::ui::SeatCraftViewCore>(app, std::move(backend), std::move(zoomPanController), std::move(uiScheduler));
    });

    auto updateAreaSvgData = optional_override([](kk::ui::SeatCraftViewCore &viewCore, const val &emscriptenData) {
        auto app = viewCore.getApp();
        auto provider = app->getSvgDataProvider();
        auto data = GetDataFromEmscripten(emscriptenData);
        viewCore.postWork([self = viewCore.shared_from_this(), provider, data = std::move(data)] {
            provider->setAreaSVGData(std::move(data));
            self->postUI([self] {
                self->updateAreaAvailable();
            });
        });
    });

    auto handlePan = optional_override([](kk::ui::SeatCraftViewCore &viewCore, float deltaX, float deltaY) {
        viewCore.handlePan(tgfx::Point{deltaX, deltaY});
    });

    auto handlePinch = optional_override([](kk::ui::SeatCraftViewCore &viewCore, float scale, float centerX, float centerY) {
        viewCore.handlePinch(scale, tgfx::Point{centerX, centerY});
    });

    class_<kk::ui::SeatCraftViewCore>("SeatCraftViewCore")
        .smart_ptr<std::shared_ptr<kk::ui::SeatCraftViewCore>>("SeatCraftViewCore")
        .function("updateSize", &kk::ui::SeatCraftViewCore::updateSize)
        .function("updateAreaSvgData", std::move(updateAreaSvgData))
        .function("handlePan", std::move(handlePan))
        .function("handlePinch", std::move(handlePinch))
        .function("draw", &kk::ui::SeatCraftViewCore::draw)
        .class_function("MakeFrom", std::move(makeFrom));
}

int main(int, const char *[]) {
    return 0;
}