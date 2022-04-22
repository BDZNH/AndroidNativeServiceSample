// FIXME: your file license if you have one

#include "SampleHidl.h"

#define LOG_TAG "samplehidl-server"
#include <log/log_main.h>

#include <unistd.h>

namespace vendor {
namespace example {
namespace samplehidl {
namespace V1_0 {
namespace implementation {

SampleHidl::SampleHidl() {
    mCookie = 0;
    mRunning = true;
    mDeathReciver = this;
    mLocalLoopThread = new std::thread(&SampleHidl::loopMethod,this);
    if(mLocalLoopThread != nullptr){
        mLocalLoopThread->detach();
        ALOGD("service thread stared");
    }
}

void SampleHidl::loopMethod() {
    int32_t loopcount = 0;
    while (mRunning)
    {
        {
            std::shared_lock _l(mMutex);
            ALOGD("service is running and send broadcast");
            auto iter = mCallbackList.begin();
            while (iter != mCallbackList.end())
            {
                if (iter->second->ping().isOk())
                {
                    iter->second->onReceive(loopcount, loopcount, loopcount);
                    loopcount++;
                    iter++;
                }
                else
                {
                    ALOGE("ping failed. maybe remote callback has died?");
                    iter = mCallbackList.erase(iter);
                }
            }
        }
        sleep(5);
    }
}

void SampleHidl::serviceDied(uint64_t cookie, const android::wp<IBase>& service) {
    std::unique_lock _l(mMutex);
    ALOGD("client died %zu with address %p",(size_t)cookie,&service);
    {
        auto iter = mCallbackList.find(cookie);
        if(iter != mCallbackList.end())
        {
            mCallbackList.erase(iter);
        }
        else
        {
            ALOGE("don't find such a callback %zu",(size_t)cookie);
        }
    }
}

Return<void> SampleHidl::debug(const hidl_handle& handle, const hidl_vec<hidl_string>& /*options*/) {
    if (handle != nullptr && handle->numFds >= 1) {
        int fd = handle->data[0];
        dprintf(fd,"there are %zu registed client\n",mCallbackList.size());
    } else {
        ALOGE("File handle error");
    }
    return Void();
}

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

Return<void> SampleHidl::registerRemoteCallBack(const sp<::vendor::example::samplehidl::V1_0::ISampleHidlCallBack>& cb) {
    std::unique_lock _l(mMutex);
    uint64_t remoteCookie = cb->getCookie();
    auto iter = mCallbackList.find(remoteCookie);
    if(iter != mCallbackList.end())
    {
        ALOGE("this client has register %p",&cb);
    }
    else
    {
        mCookie++;
        ALOGD("register No.%zu client",(size_t)remoteCookie);
        cb->linkToDeath(mDeathReciver,remoteCookie);
        mCallbackList.insert(std::make_pair(remoteCookie,cb));
    }
    return Void();
}

Return<void> SampleHidl::unregisterRemoteCallBack(const sp<::vendor::example::samplehidl::V1_0::ISampleHidlCallBack>& cb) {
    std::unique_lock _l(mMutex);
    uint64_t remoteCookie = cb->getCookie();
    auto iter = mCallbackList.find(remoteCookie);
    if(iter != mCallbackList.end())
    {
        ALOGD("unregister remote callback %zu",(size_t)remoteCookie);
        mCallbackList.erase(iter);
    }
    else
    {
        ALOGE("no such a registed remote callback %zu",(size_t)remoteCookie);
    }
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
