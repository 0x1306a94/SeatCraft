
#include <jni.h>
#include <tgfx/platform/android/JNIEnvironment.h>

#include <SeatCraft/common/common_macro.h>

extern "C" {

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeUpdateSize(JNIEnv *env, jobject thiz) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeRelease(JNIEnv *env, jobject thiz) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_nativeDraw(JNIEnv *env, jobject thiz, jboolean force) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    UNUSED_PARAM(force);
}

JNIEXPORT void JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_00024Companion_nativeInit(JNIEnv *env, jobject thiz) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
}

JNIEXPORT jlong JNICALL Java_com_seatcraft_picker_SeatCraftPickerView_00024Companion_setupFromSurface(JNIEnv *env, jobject thiz, jobject surface, jfloat density) {
    UNUSED_PARAM(env);
    UNUSED_PARAM(thiz);
    UNUSED_PARAM(surface);
    UNUSED_PARAM(density);
    return 0;
}

jint JNI_OnLoad(JavaVM *vm, void *) {
    tgfx::JNIEnvironment::SetJavaVM(vm);
    return JNI_VERSION_1_4;
}

void JNI_OnUnload(JavaVM *, void *) {
    tgfx::JNIEnvironment::SetJavaVM(nullptr);
}

}
