// FIXME: your file license if you have one

#include "SampleHidl.h"

namespace vendor {
namespace example {
namespace samplehidl {
namespace V1_0 {
namespace implementation {

// Methods from ::vendor::example::samplehidl::V1_0::ISampleHidl follow.
Return<void> SampleHidl::hello(const hidl_string& str, hello_cb _hidl_cb) {
    char buf[100];
    ::memset(buf, 0x00, 100);
    ::snprintf(buf, 100, "Hello World, %s", str.c_str());
    hidl_string result(buf);

    _hidl_cb(result);
    return Void();
}

Return<void> SampleHidl::call(const hidl_vec<uint8_t>& data, call_cb _hidl_cb) {
    size_t size = data.size();
    printf("server received %zu size\n",size);
    hidl_vec<uint8_t> reply(size);
    for(size_t i = 0 ; i< size ;i++)
    {
        reply[i] = i+65;
    }
    _hidl_cb(reply);
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//ISampleHidl* HIDL_FETCH_ISampleHidl(const char* /* name */) {
    //return new SampleHidl();
//}
//
}  // namespace implementation
}  // namespace V1_0
}  // namespace samplehidl
}  // namespace example
}  // namespace vendor
