#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <jni.h>
#include <assert.h>

#include <functional>
#include <shared_mutex>

#define LOG_TAG "samplehidl"
#include <log/log_main.h>
#include "SampleHidlJniLoad.h"
#include "SampleHidlClient.h"

#define JNIREG_CLASS "com/jni/JavaHello" //指定要注册的类

static SampleHildClient* samplehidlclient = nullptr;
static android::sp<ISampleHidlCallBack> g_remoteCallBack = nullptr;
static JavaVM* g_jvm = nullptr;
static jclass g_clazz = nullptr;
static jobject g_jboject = nullptr;
static std::shared_mutex g_Mutex;

jstring native_hello(JNIEnv *env, jobject obj)
{
    ALOGD("native_hell called");
    std::string result = samplehidlclient->hello("msg from jni hello");
    return env->NewStringUTF(result.c_str());
}

void callJavaCallback(int32_t first, int32_t second, int32_t third, jobject obj)
{
    std::shared_lock _l(g_Mutex);
    ALOGD("first=%d second=%d third=%d", first, second, third);
    if (g_clazz == NULL)
    {
        ALOGE("missing class");
        return;
    }
    if (g_jboject == nullptr)
    {
        ALOGE("missing class instance");
        return;
    }
    if (g_jvm == nullptr)
    {
        ALOGE("missing java vm");
        return;
    }
    JNIEnv *env = NULL;
    ScopedAttach attach(g_jvm, &env);
    jmethodID nativecallback = env->GetMethodID(g_clazz, "nativeCallBack", "(III)V");
    env->CallVoidMethod(g_jboject, nativecallback, first, second, third);
}

void registerRemoteCallBack(JNIEnv *env, jobject obj)
{
    std::unique_lock _l(g_Mutex);
    if (g_remoteCallBack == nullptr)
    {
        static auto callbackFunc = std::bind(callJavaCallback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, obj);
        g_remoteCallBack = new SampleHidlCallBack(callbackFunc);
    }
    g_jboject = reinterpret_cast<jobject>(env->NewGlobalRef(obj));
    jclass clazz = env->GetObjectClass(obj);
    g_clazz = (jclass)(env->NewGlobalRef(clazz));
    samplehidlclient->registerRemoteCallBack(g_remoteCallBack);
    
}

void unregisterRemoteCallBack(JNIEnv *env, jobject obj)
{
    std::unique_lock _l(g_Mutex);
    if (g_remoteCallBack != nullptr)
    {
        samplehidlclient->unregisterRemoteCallBack(g_remoteCallBack);
    }
    if(g_jboject != nullptr)
    {
        env->DeleteGlobalRef(g_jboject);
        g_jboject = nullptr;
    }
    if(g_clazz != nullptr)
    {
        env->DeleteGlobalRef(g_clazz);
        g_clazz = nullptr;
    }
}



/**
 * Table of methods associated with a single class.
 */
static JNINativeMethod gMethods[] = {
    {"hello", "()Ljava/lang/String;", (void *)native_hello}, //绑定
    {"registerRemoteCallBack","()V",(void*)registerRemoteCallBack},
    {"unregisterRemoteCallBack","()V",(void*)unregisterRemoteCallBack},
};

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv *env, const char *className,
                                 JNINativeMethod *gMethods, int numMethods)
{
    jclass clazz = env->FindClass(className);
    if (clazz == NULL)
    {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0)
    {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 */
static int registerNatives(JNIEnv *env)
{
    if (!registerNativeMethods(env, JNIREG_CLASS, gMethods,
                               sizeof(gMethods) / sizeof(gMethods[0])))
        return JNI_FALSE;

    return JNI_TRUE;
}

/*
 * Set some test stuff up.
 *
 * Returns the JNI version on success, -1 on failure.
 */
jint JNI_OnLoad(JavaVM *vm, void *)
{
    ALOGD("JNI_OnLoad called");
    
    JNIEnv *env = NULL;

    if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK)
    {
        return JNI_ERR;
    }
    assert(env != NULL);
    g_jvm = vm;

    if (!registerNatives(env))
    { //注册
        return JNI_ERR;
    }
    /* success -- return valid version number */

    samplehidlclient = new SampleHildClient();
    if(!samplehidlclient->isServiceRunning())
    {
        ALOGE("service is not running");
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}
