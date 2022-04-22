#pragma once

#include <jni.h>

class ScopedAttach {
 public:
  ScopedAttach(JavaVM* vm, JNIEnv** env) : vm_(vm) {
    vm_->AttachCurrentThread(env, NULL);
  }

  ~ScopedAttach() { vm_->DetachCurrentThread(); }

 private:
  JavaVM* vm_;
};

static int registerNatives(JNIEnv *env);

jint JNI_OnLoad(JavaVM *vm, void *);