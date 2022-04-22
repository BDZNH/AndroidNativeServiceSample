// FIXME: your file license if you have one

#pragma once

#include <vendor/example/samplehidl/1.0/ISampleHidlCallBack.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

using RemoteCallBack = std::function<void(int32_t firtst, int32_t second, int32_t third)>;

namespace vendor {
namespace example {
namespace samplehidl {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct SampleHidlCallBack : public ISampleHidlCallBack {
    uint64_t cookie;
    RemoteCallBack callbackfunc;
    SampleHidlCallBack(RemoteCallBack cb);
    // Methods from ::vendor::example::samplehidl::V1_0::ISampleHidlCallBack follow.
    Return<void> onReceive(int32_t firtst, int32_t second, int32_t third) override;
    Return<uint64_t> getCookie() override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" ISampleHidlCallBack* HIDL_FETCH_ISampleHidlCallBack(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace samplehidl
}  // namespace example
}  // namespace vendor
