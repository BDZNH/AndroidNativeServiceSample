// #include "NativeServiceSampleClient.h"
/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <android_os_Parcel.h>
#include <binder/Parcel.h>
#include "jni.h"

#include "NativeServiceSampleClient.h"

static android::NativeServiceSampleClient* g_client = nullptr;

static jint
add(JNIEnv */* env */, jobject /* thiz */, jint a, jint b) {
int result = a + b;
    ALOGI("%d + %d = %d", a, b, result);
    return result;
}

static int
invoke(JNIEnv* env , jobject /* thiz */,jint code, jobject dataObj, jobject replyObj, jint flags) {
    ALOGI("invoke called");
    if (dataObj == NULL) {
        // jniThrowNullPointerException(env, NULL);
        return -1;
    }

    android::Parcel* data = android::parcelForJavaObject(env, dataObj);
    if (data == NULL) {
        return -1;
    }
    android::Parcel* reply = android::parcelForJavaObject(env, replyObj);
    if (reply == NULL && replyObj != NULL) {
        return -1;
    }

    if(g_client == nullptr){
        return -1;
    }

    return g_client->invoke(code, *data,reply,flags);
}

static const char *classPathName = "com/example/NativeServiceSample";

static const JNINativeMethod methods[] = {
  {"add", "(II)I", (void*)add },
  {"invoke","(ILandroid/os/Parcel;Landroid/os/Parcel;I)I",(void*)invoke},
};

/*
 * Register several native methods for one class.
 */
static int registerNativeMethods(JNIEnv* env, const char* className,
    const JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        ALOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        ALOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, classPathName,
                 methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }

  return JNI_TRUE;
}


// ----------------------------------------------------------------------------

/*
 * This is called by the VM when the shared library is first loaded.
 */

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

jint JNI_OnLoad(JavaVM* vm, void* /* reserved */)
{
    ALOGI("Jni_OnLoad called");
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;

    ALOGI("JNI_OnLoad");

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed");
        goto bail;
    }
    env = uenv.env;

    if (registerNatives(env) != JNI_TRUE) {
        ALOGE("ERROR: registerNatives failed");
        goto bail;
    }

    if(g_client == nullptr){
        g_client = new android::NativeServiceSampleClient();
    }

    result = JNI_VERSION_1_4;

bail:
    return result;
}