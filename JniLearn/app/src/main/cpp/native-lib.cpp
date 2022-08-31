#include <jni.h>
#include <string>
#include <android/log.h>

#define LOGV(FMT, ...) __android_log_print(ANDROID_LOG_VERBOSE, "nativeLog", FMT, __VA_ARGS__)

extern "C" JNIEXPORT jstring JNICALL
Java_com_aire_jnilearn_NdkHelper_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_aire_jnilearn_NdkHelper_setIntData(JNIEnv *env, jobject thiz, jintArray data) {
//    // 方法1 GetIntArrayElements
//    int len = env->GetArrayLength(data);
//    jint *arrData = env->GetIntArrayElements(data, JNI_FALSE);
//    for (int i = 0; i < len; i++) {
//        LOGV("jni data: %d \n", arrData[i]);
//    }
//    env->ReleaseIntArrayElements(data, arrData, 0);
//    // 方法2 GetIntArrayRegion
//    int len = env->GetArrayLength(data);
//    jint *arrData = new int[len];
//    env->GetIntArrayRegion(data, 0, len, arrData);
//    for (int i = 0; i < len; i++) {
//        LOGV("jni data: %d \n", arrData[i]);
//    }
    // 方法3 GetPrimitiveArrayCritical
    int len = env->GetArrayLength(data);
    jint *arrData = static_cast<jint *>(env->GetPrimitiveArrayCritical(data, JNI_FALSE));
    for (int i = 0; i < len; i++) {
        LOGV("jni data: %d \n", arrData[i]);
    }
    env->ReleasePrimitiveArrayCritical(data, arrData, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_aire_jnilearn_NdkHelper_trigger(JNIEnv *env, jobject thiz, jstring msg) {
    jclass clazz = env->FindClass("com/aire/jnilearn/NdkHelper");
    jmethodID methodId = env->GetMethodID(clazz, "onJniCallback", "(Ljava/lang/String;)V");
    env->CallVoidMethod(thiz, methodId, env->NewStringUTF("hello I'm from jni"));
//    int* data = nullptr;
//    LOGV("%d \n", data[0]);
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    LOGV("hello jni onLoad, %d", 123);

    // 获取JNI env变量
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        // 失败返回-1
        return result;
    }

    // 返回成功
    result = JNI_VERSION_1_6;
    return result;
}