// FIXME: your file license if you have one

#include "SampleHidlCallBack.h"
#include <chrono>
namespace vendor {
namespace example {
namespace samplehidl {
namespace V1_0 {
namespace implementation {

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

SampleHidlCallBack::SampleHidlCallBack(RemoteCallBack cb) {
    callbackfunc = cb;
    cookie = timeSinceEpochMillisec();
}

// Methods from ::vendor::example::samplehidl::V1_0::ISampleHidlCallBack follow.
Return<void> SampleHidlCallBack::onReceive(int32_t first, int32_t second, int32_t third) {
    if(callbackfunc != nullptr){
        callbackfunc(first,second,third);
    }
    return Void();
}

Return<uint64_t> SampleHidlCallBack::getCookie() {
    // TODO implement
    // return uint64_t {};
    return cookie;
}



// Methods from ::android::hidl::base::V1_0::IBase follow.

//ISampleHidlCallBack* HIDL_FETCH_ISampleHidlCallBack(const char* /* name */) {
    //return new SampleHidlCallBack();
//}
//
}  // namespace implementation
}  // namespace V1_0
}  // namespace samplehidl
}  // namespace example
}  // namespace vendor
