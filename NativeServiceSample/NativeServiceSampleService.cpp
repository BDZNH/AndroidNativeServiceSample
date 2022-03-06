#include "NativeServiceSampleService.h"
#include "Cmdid.h"
namespace android
{
    status_t sampleCallFunction(const Parcel&data, Parcel* reply){
        String8 str = String8(data.readString16());
        ALOGD("call funcion called with \"%s\"",str.string());
        return SUCCESS;
    }

    status_t sampleGetFunction(const Parcel&data, Parcel* reply){
        ALOGD("get function called");
        String16 hello = String16("this is message from service");
        reply->writeString16(hello);
        return SUCCESS;
    }

    status_t getMacByInterfaceName(const Parcel &data, Parcel *reply)
    {
        String16 ifname = data.readString16();
        ALOGV("get mac of ifname: %s",String8(ifname).string());
        String8 path = String8("/sys/class/net/");
        path += String8(ifname);
        path += String8("/address");
        FILE* fp = fopen(path.string(),"ro");
        if(fp == NULL){
            ALOGE("open %s failed(%d): %s",path.string(),errno,strerror(errno));
            return FAILED;
        }

        char mac[18] = {0};
        size_t readsize = fread(mac,1,17,fp);
        if(readsize != 17){
            ALOGE("read mac failed");
            fclose(fp);
            return FAILED;
        }
        String16 result = String16(mac);
        reply->writeString16(result);
        fclose(fp);
        return SUCCESS;
    }

    status_t SampleExitService(const Parcel &data,Parcel *reply)
    {
        ALOGI("service exit now");
        printf("service received exit message\n");
        exit(0);
        return SUCCESS;
    }

    NativeServiceSampleService::NativeServiceSampleService()
    {
        methods[SAMPLE_CALL] = &sampleCallFunction;
        methods[SAMPLE_GET] = &sampleGetFunction;
        methods[SAMPLE_GET_MAC] = &getMacByInterfaceName;
        methods[SAMPLE_EXIT_SERVER] = &SampleExitService;
    }

    status_t NativeServiceSampleService::onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags)
    {
        if (code == SHELL_COMMAND_TRANSACTION)
        {
            int in = data.readFileDescriptor();
            int out = data.readFileDescriptor();
            int err = data.readFileDescriptor();
            int argc = data.readInt32();
            Vector<String16> args;
            for (int i = 0; i < argc && data.dataAvail() > 0; i++)
            {
                args.add(data.readString16());
            }
            status_t result = 0;
            sp<IBinder> unusedCallback;
            sp<IResultReceiver> resultReceiver;
            if ((result = data.readNullableStrongBinder(&unusedCallback)) != NO_ERROR)
            {
                return result;
            }
            if ((result = data.readNullableStrongBinder(&resultReceiver)) != NO_ERROR)
            {
                return result;
            }
            result = shellCommand(in, out, err, args);
            if (resultReceiver != nullptr)
            {
                resultReceiver->send(result);
            }
            return result;
        }
        else if (IBinder::FIRST_CALL_TRANSACTION <= code && code <= IBinder::LAST_CALL_TRANSACTION)
        {
            CHECK_INTERFACE(INativeServiceSampleService, data, reply);
            return invoke(code, data, reply, flags);
        }
        else
        {
            return BBinder::onTransact(code, data, reply, flags);
        }
    }

    status_t NativeServiceSampleService::shellCommand(int in, int out, int err, const Vector<String16> &args)
    {
        return onShellCommand(in, out, err, args);
    }

    status_t NativeServiceSampleService::onShellCommand(int in, int out, int err, const Vector<String16> &args)
    {
        ALOGI("onShellCommand not implementation yet");
        write(out, "onShellCommand not implementation yet\n", 38);
        size_t size = args.size();
        for (size_t i = 0; i < size; i++)
        {
            ALOGI("Received No.%zu arg: %s", i, String8(args[i]).string());
        }
        return SUCCESS;
    }

    status_t NativeServiceSampleService::dump(int fd, const Vector<String16> &args)
    {
        ALOGI("dump not implementation yet");
        write(fd, "dump not implementation yet\n", 28);
        size_t size = args.size();
        for (size_t i = 0; i < size; i++)
        {
            ALOGI("Received No.%zu arg: %s", i, String8(args[i]).string());
        }
        return SUCCESS;
    }

    status_t NativeServiceSampleService::invoke(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
    {
        std::map<uint32_t, local_method>::iterator iter = methods.find(code);
        ALOGV("uid:%d pid:%d called method %u",getCallingUid(),getCallingPid(),code);
        if (iter != methods.end())
        {
            return (*(iter->second))(data, reply);
        }
        else
        {
            ALOGE("can't find such a method whose cmdid is %u", code);
            return -1;
        }
    }
}