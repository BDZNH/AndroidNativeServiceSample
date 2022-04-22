#pragma once

# include <vendor/example/samplehidl/1.0/ISampleHidl.h>
#include <hwbinder/Parcel.h>

# include "SampleHidlCallBack.h"

using vendor::example::samplehidl::V1_0::ISampleHidl;
using vendor::example::samplehidl::V1_0::ISampleHidlCallBack;
using vendor::example::samplehidl::V1_0::implementation::SampleHidlCallBack;
using android::sp;
using android::hardware::hidl_string;
using android::hardware::hidl_vec;

#include <atomic>
#include <mutex>

using IBase = ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_death_recipient;
using Parcel = ::android::hardware::Parcel;

class SampleHildClient : virtual public hidl_death_recipient
{
private:
    android::sp<ISampleHidl> mService;
    android::sp<SampleHildClient> mDeathReceiver;
    static size_t cookieCount;
    std::atomic_bool value;
    std::mutex mLock;

    virtual void serviceDied(uint64_t cookie, const android::wp<IBase>& service) override;
    bool setValid();
    bool setInvalid();
    bool isValid();
    bool isInvalid();
    bool init();
public:
    SampleHildClient(/* args */);
    ~SampleHildClient();
    bool isServiceRunning();
    bool remoteCall(const Parcel& data,Parcel* reply);
    std::string hello(std::string str);
    void registerRemoteCallBack(const android::sp<ISampleHidlCallBack>& cb);
    void unregisterRemoteCallBack(const android::sp<ISampleHidlCallBack>& cb);
};
