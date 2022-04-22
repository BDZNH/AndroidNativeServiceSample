#include "SampleHidlClient.h"

#define LOG_TAG "samplehidl"
#include <log/log_main.h>

size_t SampleHildClient::cookieCount = 0;

SampleHildClient::SampleHildClient(/* args */)
{
    value.store(false);
    init();
}

SampleHildClient::~SampleHildClient()
{
    ALOGD("~SampleHildClient() called");
    if(mService != nullptr)
    {
        if(isValid())
        {
            ALOGD("unlink to death");
            mService->unlinkToDeath(mDeathReceiver);
        }
        mDeathReceiver = nullptr;
        mService = nullptr;
    }
}

bool SampleHildClient::init()
{
    ALOGD("init() called");
    if(isInvalid())
    {
        std::lock_guard<std::mutex> _l(mLock);
        if(isValid())
        {
            return true;
        }
        android::sp<ISampleHidl> service = ISampleHidl::getService();
        if(service == nullptr)
        {
            ALOGE("get service failed");
            return false;
        }
        else
        {
            mService = service;
            mDeathReceiver = this;
            mService->linkToDeath(mDeathReceiver,cookieCount++);
            ALOGD("service is valid ? %s", (isValid() ? "true" : "false"));
            if(setValid())
            {
                ALOGD("set death reciver valid success");
                ALOGD("service is valid ? %s", (isValid() ? "true" : "false"));
            }
            else
            {
                ALOGE("set Valid failed");
            }
            ALOGD("service get success, and linkto death");
            return true;
        }
    }
    return true;
}

bool SampleHildClient::isServiceRunning()
{
    return (mService != nullptr) && (isValid());
}

bool SampleHildClient::remoteCall(const Parcel& data,Parcel* reply)
{
    return false;
}

std::string SampleHildClient::hello(std::string str)
{
    std::string result;
    ALOGD("invalid ? %s",(isValid() ? "true" : "false"));
    if(this->isValid())
    {
        mService->hello(str,[&](hidl_string ret){
            result = ret.c_str();
        });
    }
    else
    {
        if(init())
        {
            mService->hello(str,[&](hidl_string ret){
                result = ret.c_str();
            });
        }
        else
        {
            ALOGE("it seemed service has died");
        }
    }
    return result;
}

void SampleHildClient::registerRemoteCallBack(const android::sp<ISampleHidlCallBack>& cb)
{
    ALOGD("invalid ? %s",(isValid() ? "true" : "false"));
    if(this->isValid())
    {
        mService->registerRemoteCallBack(cb);
    }
    else
    {
        if(init())
        {
            mService->registerRemoteCallBack(cb);
        }
        else
        {
            ALOGE("it seemed service has died");
        }
    }
}

void SampleHildClient::unregisterRemoteCallBack(const android::sp<ISampleHidlCallBack>& cb)
{
    ALOGD("invalid ? %s",(isValid() ? "true" : "false"));
    if(this->isValid())
    {
        mService->unregisterRemoteCallBack(cb);
    }
    else
    {
        if(init())
        {
            mService->unregisterRemoteCallBack(cb);
        }
        else
        {
            ALOGE("it seemed service has died");
        }
    }
}

void SampleHildClient::serviceDied(uint64_t cookie, const android::wp<IBase>& service)
{
    ALOGE("service died, cookie %zu",(size_t)cookie);
    if(!setInvalid())
    {
        ALOGE("setInvalid failed and why?");
    }
    ALOGD("invalid ? %s",(isValid() ? "true" : "false"));
}

bool SampleHildClient::isInvalid()
{
    return !(value.load());
}

bool SampleHildClient::isValid()
{
    return value.load();
}

bool SampleHildClient::setValid()
{
    bool var = false;
    return value.compare_exchange_strong(var,true);
}

bool SampleHildClient::setInvalid()
{
    bool var = true;
    return value.compare_exchange_strong(var,false);
}
