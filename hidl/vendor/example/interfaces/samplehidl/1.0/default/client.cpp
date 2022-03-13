# define LOG_TAG "samplehidl-client"

# include <vendor/example/samplehidl/1.0/ISampleHidl.h>

# include <hidl/Status.h>

# include <hidl/LegacySupport.h>

# include <utils/misc.h>

# include <hidl/HidlSupport.h>

# include <stdio.h>

using vendor::example::samplehidl::V1_0::ISampleHidl;
using android::sp;
using android::hardware::hidl_string;
using android::hardware::hidl_vec;

int main()
{
    android::sp<ISampleHidl> service = ISampleHidl::getService();
    if(service == nullptr)
    {
        printf("get service failed");
        return -1;
    }
    service->hello("clientmsg",[](hidl_string result){
        printf("from service: %s\n",result.c_str());
    });
    return 0;
}