#include "NativeServiceSampleService.h"
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <utils/String8.h>
#include <utils/String16.h>

int main(int argc, char **argv)
{
    printf("start %s\n",NATIVESERVICE_NAME);
    ALOGI("start %s",NATIVESERVICE_NAME);
    android::defaultServiceManager()->addService(android::String16(NATIVESERVICE_NAME), new android::NativeServiceSampleService());
    android::ProcessState::self()->startThreadPool();
    android::IPCThreadState::self()->joinThreadPool();
    return 0;
}