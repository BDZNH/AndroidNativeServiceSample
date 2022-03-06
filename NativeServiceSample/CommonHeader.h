#ifndef _COMMON_HEADER_H_
#define _COMMON_HEADER_H_
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <binder/PermissionController.h>
#include <binder/ProcessInfoService.h>
#include <binder/IResultReceiver.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include "Cmdid.h"
#include <log/log_main.h>
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "NativeService"
#define NATIVESERVICE_NAME "nativeservice"
#endif