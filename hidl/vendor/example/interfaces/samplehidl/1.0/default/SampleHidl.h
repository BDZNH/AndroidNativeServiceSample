// FIXME: your file license if you have one

#pragma once

#include <vendor/example/samplehidl/1.0/ISampleHidl.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

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

struct SampleHidl : public ISampleHidl {
    // Methods from ::vendor::example::samplehidl::V1_0::ISampleHidl follow.
    Return<void> hello(const hidl_string& str, hello_cb _hidl_cb) override;
    Return<void> call(const hidl_vec<uint8_t>& data, call_cb _hidl_cb) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" ISampleHidl* HIDL_FETCH_ISampleHidl(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace samplehidl
}  // namespace example
}  // namespace vendor
