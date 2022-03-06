#ifndef _NATIVE_SERVICE_SAMPLE_DEATH_RECIPIENT_H_
#define _NATIVE_SERVICE_SAMPLE_DEATH_RECIPIENT_H_

#include "CommonHeader.h"
#include <mutex>
#include <atomic>

namespace android{
    class NativeServiceSampleDeathRecipient : public IBinder::DeathRecipient
    {
        public:
        NativeServiceSampleDeathRecipient(){mValidVar.store(invalid,std::memory_order_relaxed);};
        bool setValid(){
            return mValidVar.compare_exchange_strong(invalid,valid);
        }
        bool setInvalid(){
            return mValidVar.compare_exchange_strong(valid,invalid);
        }
        bool isValid(){
            return mValidVar.load();
        }
        private:
        virtual void binderDied(const wp<IBinder>& who) {
            ALOGE("service died. set service invalid");
            setInvalid();
        };
        std::atomic_bool mValidVar;
        bool valid = true;
        bool invalid = false;
    };
}

#endif