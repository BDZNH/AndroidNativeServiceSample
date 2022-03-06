#ifndef _NATIVE_SERVICE_SAMPLE_SERVICE_H_
#define _NATIVE_SERVICE_SAMPLE_SERVICE_H_

#include "CommonHeader.h"
#include <map>

namespace android
{
    enum STATUS {
        SUCCESS = NO_ERROR,
        FAILED = UNKNOWN_TRANSACTION
    };
    typedef status_t (*local_method)(const Parcel &data, Parcel *reply);
    class NativeServiceSampleService : public BBinder {
    public:
        NativeServiceSampleService();
        virtual status_t onTransact(uint32_t code,const Parcel& data, Parcel* reply,uint32_t flags = 0);
        status_t dump(int fd, const Vector<String16> &args);
        int onShellCommand(int in, int out, int err, const Vector<String16> &args);
        virtual status_t invoke(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags);
        virtual const String16& getInterfaceDescriptor() const{return this->interface;};
    private:
        int shellCommand(int in, int out, int err, const Vector<String16> &args);
        int getCallingPid(){return IPCThreadState::self()->getCallingPid();}
        int getCallingUid(){return IPCThreadState::self()->getCallingUid();}
        std::map<uint32_t,local_method> methods;
        String16 interface = String16(NATIVESERVICE_NAME);
    };
} // namespace android

#endif