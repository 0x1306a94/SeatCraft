
#include <android/asset_manager_jni.h>
#include <jni.h>

#include <tgfx/core/Data.h>
#include <tgfx/core/Stream.h>
#include <tgfx/platform/android/JNIEnvironment.h>

#include <SeatCraft/common/common_macro.h>

#include "AndroidFileReader.h"
#include "core/SeatCraftCoreApp.hpp"
#include "core/renderer/SeatCraftCoreRenderer.hpp"
#include "core/svg/SVGDataProvider.h"
#include "core/ui/ElasticZoomPanController.hpp"
#include "core/ui/SeatCraftViewCore.hpp"
#include "platform/android/renderer/AndroidRendererBackend.h"
#include "platform/android/thread/AndroidUIThreadScheduler.h"

namespace kk::jni {
struct SeatCraftViewCoreWrapper {
    std::shared_ptr<kk::ui::SeatCraftViewCore> viewCore;
    explicit SeatCraftViewCoreWrapper(std::shared_ptr<kk::ui::SeatCraftViewCore> viewCore)
        : viewCore(std::move(viewCore)) {}
};
};  // namespace kk::jni

static jfieldID SeatCraftPickerView_nativePtr;
static std::shared_ptr<kk::ui::SeatCraftViewCore> GetSeatCraftViewCore(JNIEnv *env, jobject thiz) {
    jlong ptr = env->GetLongField(thiz, SeatCraftPickerView_nativePtr);
    if (ptr == 0) {
        return nullptr;
    }
    auto wrapper = reinterpret_cast<kk::jni::SeatCraftViewCoreWrapper *>(ptr);
    return wrapper->viewCore;
}

static void DeleteSeatCraftViewCore(JNIEnv *env, jobject thiz) {
    jlong ptr = env->GetLongField(thiz, SeatCraftPickerView_nativePtr);
    if (ptr == 0) {
        return;
    }
    auto wrapper = reinterpret_cast<kk::jni::SeatCraftViewCoreWrapper *>(ptr);
    delete wrapper;
    env->SetLongField(thiz, SeatCraftPickerView_nativePtr, 0L);
}

extern "C" {

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeUpdateSize(JNIEnv *env, jobject thiz) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    auto handler = GetSeatCraftViewCore(env, thiz);
    if (handler == nullptr) {
        return;
    }
    handler->updateSize();
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeUpdatePan(JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    auto handler = GetSeatCraftViewCore(env, thiz);
    if (handler == nullptr) {
        return;
    }

    handler->handlePan(tgfx::Point{x, y});
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeUpdatePinch(JNIEnv *env, jobject thiz, jfloat scale, jfloat cx, jfloat cy) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    auto handler = GetSeatCraftViewCore(env, thiz);
    if (handler == nullptr) {
        return;
    }
    handler->handlePinch(scale, tgfx::Point{cx, cy});
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeSetAreaMapSvgData(JNIEnv *env, jobject thiz, jbyteArray areaSvgData) {
    auto handler = GetSeatCraftViewCore(env, thiz);
    if (handler == nullptr) {
        return;
    }

    auto app = handler->getApp();
    auto provider = app->getSvgDataProvider();
    if (areaSvgData == nullptr) {
        auto viewCore = handler->shared_from_this();
        viewCore->postWork([viewCore, provider] {
            provider->setAreaSVGData(nullptr);
            viewCore->postUI([=] {
                viewCore->updateAreaAvailable();
            });
        });
        return;
    }

    auto bytes = env->GetByteArrayElements(areaSvgData, nullptr);
    auto size = static_cast<size_t>(env->GetArrayLength(areaSvgData));
    auto data = tgfx::Data::MakeWithCopy(bytes, size);
    env->ReleaseByteArrayElements(areaSvgData, bytes, 0);

    auto viewCore = handler->shared_from_this();
    viewCore->postWork([viewCore, provider, data = std::move(data)] {
        provider->setAreaSVGData(data);
        viewCore->postUI([=] {
            viewCore->updateAreaAvailable();
        });
    });
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeSetAreaMapSvgPath(JNIEnv *env, jobject thiz, jstring areaSvgPath) {
    auto handler = GetSeatCraftViewCore(env, thiz);
    if (handler == nullptr) {
        return;
    }
    auto app = handler->getApp();
    auto fileReader = app->getFileReader();
    if (!fileReader) {
        return;
    }

    auto provider = app->getSvgDataProvider();
    const char *cStr = nullptr;
    if (areaSvgPath == nullptr) {
        goto FAILURE;
        return;
    }

    cStr = env->GetStringUTFChars(areaSvgPath, nullptr);
    if (cStr) {
        goto SUCCESS;
        return;
    }

FAILURE: {
    auto viewCore = handler->shared_from_this();
    viewCore->postWork([viewCore, provider] {
        provider->setAreaSVGData(nullptr);
        viewCore->postUI([viewCore] {
            viewCore->updateAreaAvailable();
        });
    });
    return;
}

SUCCESS:
    auto data = fileReader->readData(cStr);
    env->ReleaseStringUTFChars(areaSvgPath, cStr);

    auto viewCore = handler->shared_from_this();
    viewCore->postWork([viewCore, provider, data = std::move(data)] {
        provider->setAreaSVGData(data);
        viewCore->postUI([viewCore] {
            viewCore->updateAreaAvailable();
        });
    });
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeUpdateSurface(JNIEnv *env, jobject thiz, jobject surface, jfloat density) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    UNUSED_PARAM(surface);
    auto handler = GetSeatCraftViewCore(env, thiz);
    if (handler == nullptr) {
        return;
    }

    if (surface == nullptr) {
        handler->replaceBackend(nullptr);
        return;
    }

    auto nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (nativeWindow == nullptr) {
        handler->replaceBackend(nullptr);
        return;
    }

    auto backend = std::make_unique<kk::renderer::AndroidRendererBackend>(nativeWindow, density);
    handler->replaceBackend(std::move(backend));
    handler->updateSize();
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeDraw(JNIEnv *env, jobject thiz, jboolean force) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    UNUSED_PARAM(force);

    auto handler = GetSeatCraftViewCore(env, thiz);
    if (handler == nullptr) {
        return;
    }
    handler->draw(force);
}

JNIEXPORT jlong JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeCreate(JNIEnv *env, jobject thiz, jobject assetManager, jfloat density) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    UNUSED_PARAM(density);

    auto assetMgr = AAssetManager_fromJava(env, assetManager);
    if (assetMgr == nullptr) {
        return 0L;
    }

    auto app = std::make_shared<kk::SeatCraftCoreApp>(tgfx::Size::MakeEmpty(), tgfx::Size::MakeEmpty(), density);
    app->setFileReader(std::make_shared<kk::AndroidFileReader>(assetMgr));

    //    kk::SeatStatusSVGPathMap pathMap{
    //        {1, "asset://svg/icon_chooseSeat_canSelected.svg"},
    //    };
    //    app->updateSeatStatusSVGPathMap(pathMap);

    auto zoomPanController = std::make_unique<kk::ui::ElasticZoomPanController>();
    auto uiScheduler = std::make_shared<kk::thread::AndroidUIThreadScheduler>();
    auto viewCore = std::make_shared<kk::ui::SeatCraftViewCore>(app, nullptr, std::move(zoomPanController), std::move(uiScheduler));
    auto wrapper = new kk::jni::SeatCraftViewCoreWrapper(std::move(viewCore));
    auto ptr = reinterpret_cast<jlong>(wrapper);
    return ptr;
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeRelease(JNIEnv *env, jobject thiz) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    DeleteSeatCraftViewCore(env, thiz);
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_00024Companion_nativeInit(JNIEnv *env, jobject thiz) {
    UNUSED_PARAM(thiz);
    auto clazz = env->FindClass("com/seatcraft/picker/SeatCraftPickerView");
    SeatCraftPickerView_nativePtr = env->GetFieldID(clazz, "nativePtr", "J");
}

jint JNI_OnLoad(JavaVM *vm, void *) {
    tgfx::JNIEnvironment::SetJavaVM(vm);
    return JNI_VERSION_1_4;
}

void JNI_OnUnload(JavaVM *, void *) {
    tgfx::JNIEnvironment::SetJavaVM(nullptr);
}
}
