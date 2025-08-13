
#include <jni.h>

#include <tgfx/core/Data.h>
#include <tgfx/core/Stream.h>
#include <tgfx/platform/android/JNIEnvironment.h>

#include <SeatCraft/common/common_macro.h>

#include "core/SeatCraftCoreApp.hpp"
#include "core/renderer/SeatCraftCoreRenderer.hpp"
#include "platform/android/renderer/AndroidRendererBackend.h"
#include "platform/android/ui/SeatCraftPickerView.h"

static jfieldID SeatCraftPickerView_nativePtr;
static kk::ui::SeatCraftPickerView *GetSeatCraftPickerView(JNIEnv *env, jobject thiz) {
    jlong ptr = env->GetLongField(thiz, SeatCraftPickerView_nativePtr);
    if (ptr == 0) {
        return nullptr;
    }
    return reinterpret_cast<kk::ui::SeatCraftPickerView *>(ptr);
}

extern "C" {

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeUpdateSize(JNIEnv *env, jobject thiz) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    auto handler = GetSeatCraftPickerView(env, thiz);
    if (handler == nullptr) {
        return;
    }
    handler->updateSize();
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeUpdatePan(JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    auto handler = GetSeatCraftPickerView(env, thiz);
    if (handler == nullptr) {
        return;
    }

    handler->handlePan(tgfx::Point{x, y});
    handler->updateZoomPanControllerState();
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeUpdatePinch(JNIEnv *env, jobject thiz, jfloat scale, jfloat cx, jfloat cy) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    auto handler = GetSeatCraftPickerView(env, thiz);
    if (handler == nullptr) {
        return;
    }
    handler->handlePinch(scale, tgfx::Point{cx, cy});
    handler->updateZoomPanControllerState();
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeRelease(JNIEnv *env, jobject thiz) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    auto handler = GetSeatCraftPickerView(env, thiz);
    if (handler == nullptr) {
        return;
    }
    delete handler;
    env->SetLongField(thiz, SeatCraftPickerView_nativePtr, 0L);
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeDraw(JNIEnv *env, jobject thiz, jboolean force) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    UNUSED_PARAM(force);

    auto handler = GetSeatCraftPickerView(env, thiz);
    if (handler == nullptr) {
        return;
    }
    handler->draw(force);
}

JNIEXPORT jlong JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeSetupFromSurface(JNIEnv *env, jobject thiz, jobject surface, jfloat density) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    UNUSED_PARAM(surface);
    UNUSED_PARAM(density);

    auto nativeWindow = ANativeWindow_fromSurface(env, surface);
    if (nativeWindow == nullptr) {
        return 0;
    }
    auto width = ANativeWindow_getWidth(nativeWindow);
    auto height = ANativeWindow_getHeight(nativeWindow);
    tgfx::Size bounds{
        static_cast<float>(width),
        static_cast<float>(height),
    };
    tgfx::Size contentSize{};
    auto app = std::make_shared<kk::SeatCraftCoreApp>(bounds, contentSize, density);
    auto backend = std::make_unique<kk::renderer::AndroidRendererBackend>(nativeWindow, density);
    auto renderer = std::make_shared<kk::renderer::SeatCraftCoreRenderer>(app, std::move(backend));
    auto handler = new kk::ui::SeatCraftPickerView(app, renderer);
    auto ptr = reinterpret_cast<jlong>(handler);
    return ptr;
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeSetAreaMapSvgData(JNIEnv *env, jobject thiz, jbyteArray areaSvgData) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    UNUSED_PARAM(areaSvgData);
    auto handler = GetSeatCraftPickerView(env, thiz);
    if (handler == nullptr) {
        return;
    }
    if (areaSvgData == nullptr) {
        handler->updateAreaSvgData(nullptr);
        return;
    }

    auto bytes = env->GetByteArrayElements(areaSvgData, nullptr);
    auto size = static_cast<size_t>(env->GetArrayLength(areaSvgData));
    auto data = tgfx::Data::MakeWithCopy(bytes, size);
    auto stream = tgfx::Stream::MakeFromData(std::move(data));
    env->ReleaseByteArrayElements(areaSvgData, bytes, 0);
    handler->updateAreaSvgData(std::move(stream));
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
