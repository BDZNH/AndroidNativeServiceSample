// FIXME: your file license if you have one

#pragma once

#include <vendor/example/samplehidl/1.0/ISampleHidl.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include <thread>
#include <vector>
#include <shared_mutex>

namespace vendor {
namespace example {
namespace samplehidl {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::hidl_handle;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct SampleHidl : public ISampleHidl, virtual public android::hardware::hidl_death_recipient {

    std::map<uint64_t, const sp<::vendor::example::samplehidl::V1_0::ISampleHidlCallBack>> mCallbackList;
    android::sp<android::hardware::hidl_death_recipient> mDeathReciver;
    bool mRunning;
    std::thread *mLocalLoopThread;
    uint64_t mCookie;
    std::shared_mutex mMutex;

    SampleHidl();
    void loopMethod();
    virtual void serviceDied(uint64_t cookie, const android::wp<IBase>& service) override;


    // Methods from ::vendor::example::samplehidl::V1_0::ISampleHidl follow.
    Return<void> debug(const hidl_handle& handle, const hidl_vec<hidl_string>& options) override;
    Return<void> hello(const hidl_string& str, hello_cb _hidl_cb) override;
    Return<void> call(const hidl_vec<uint8_t>& data, call_cb _hidl_cb) override;
    Return<void> registerRemoteCallBack(const sp<::vendor::example::samplehidl::V1_0::ISampleHidlCallBack>& cb) override;
    Return<void> unregisterRemoteCallBack(const sp<::vendor::example::samplehidl::V1_0::ISampleHidlCallBack>& cb) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" ISampleHidl* HIDL_FETCH_ISampleHidl(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace samplehidl
}  // namespace example
}  // namespace vendor
