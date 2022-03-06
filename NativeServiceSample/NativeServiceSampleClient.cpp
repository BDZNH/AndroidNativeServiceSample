#include "NativeServiceSampleClient.h"

namespace android{
    NativeServiceSampleClient::NativeServiceSampleClient(){
        sp<IServiceManager> sm = defaultServiceManager();
        if (sm == NULL)
        {
            ALOGE("can not get service manager");
            return;
        }
        service = sm->getService(String16(NATIVESERVICE_NAME));
        if (service == NULL)
        {
            ALOGE("can not get service");
            return;
        }
        mdeathRecipient = new NativeServiceSampleDeathRecipient();
        if(service->linkToDeath(mdeathRecipient) != NO_ERROR)
        {
            ALOGE("link to death failed");
            return;
        }
        ALOGI("client link to death success");
        if(!mdeathRecipient->setValid()){
            ALOGE("set death recipient valid failed");
        }
        if(mdeathRecipient->isValid()){
            ALOGI("service is running");
        }else{
            ALOGE("service is not running and why ?");
        }
    }

    NativeServiceSampleClient::~NativeServiceSampleClient()
    {
        if(service != nullptr)
        {
            if(mdeathRecipient->isValid())
            {
                if(service->unlinkToDeath(mdeathRecipient) == NO_ERROR)
                    ALOGD("unlinktodeath success");
                else
                    ALOGE("unlinktodeath failed");
            }
            else
            {
                ALOGE("service is not running. need unlinkToDeath ?");
            }
        }
    }

    status_t NativeServiceSampleClient::invoke(uint32_t code, const Parcel &data, Parcel *reply)
    {
        return invoke(code,data,reply,0);
    }

    status_t NativeServiceSampleClient::invoke(uint32_t code, const Parcel &data, Parcel *reply,int flags)
    {
        if (!mdeathRecipient->isValid())
        {
            ALOGE("service is not running");
            return -1;
        }
            
        Parcel send;
        send.writeInterfaceToken(android::String16(NATIVESERVICE_NAME));
        send.appendFrom(&data, 0, data.dataSize());
        return service->transact(code, send, reply,flags);
    }
}