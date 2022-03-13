# define LOG_TAG "samplehidl-server"

# include <vendor/example/samplehidl/1.0/ISampleHidl.h>
#include "SampleHidl.h"
# include <hidl/LegacySupport.h>

using vendor::example::samplehidl::V1_0::ISampleHidl;
using vendor::example::samplehidl::V1_0::implementation::SampleHidl;

int main()
{
    android::hardware::configureRpcThreadpool(4,true);
    android::sp<ISampleHidl> service = new SampleHidl();
    android::status_t ret = service->registerAsService();

    if (ret == android::NO_ERROR) {
        printf("start service stucces\n");
        android::hardware::joinRpcThreadpool();
    }else{
        printf("start service failed\n");
    }

    return 0;
}