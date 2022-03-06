#ifndef _NATIVE_SERVICE_SAMPLE_CLIENT_H_
#define _NATIVE_SERVICE_SAMPLE_CLIENT_H_
#include "CommonHeader.h"
#include "NativeServiceSampleDeathRecipient.h"
namespace android{
    class NativeServiceSampleClient{
        public:
        NativeServiceSampleClient();
        ~NativeServiceSampleClient();
        status_t invoke(uint32_t code, const Parcel &data, Parcel *reply,int flag);
        status_t invoke(uint32_t code, const Parcel &data, Parcel *reply);
        private:
        sp<IBinder> service;
        sp<NativeServiceSampleDeathRecipient> mdeathRecipient;
    };
}
#endif