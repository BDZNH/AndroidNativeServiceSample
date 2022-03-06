#include "CommonHeader.h"
#include "NativeServiceSampleClient.h"
#include "cmdline.h"
#include <string>
#include <unistd.h>
using namespace android;

void sampleSendStringToServer(NativeServiceSampleClient &service)
{
    Parcel data, reply;
    
    printf("send \"sample message\" to service\n");
    String16 str = String16("sample message to service\"");
    data.writeString16(str);
    int ret = service.invoke(android::SAMPLE_CALL, data, &reply);
    if (ret == NO_ERROR)
    {
        printf("service has received message\n");
    }
    else
    {
        printf("send msg to service failed\n");
    }
}

void sampleGetStringFromServer(NativeServiceSampleClient &service)
{
    Parcel data, reply;
    int ret = service.invoke(android::SAMPLE_GET, data, &reply);
    if (ret == NO_ERROR)
    {
        String8 str = String8(reply.readString16());
        printf("received msg from server: \"%s\"\n",str.string());
    }
}

void sampleExitServer(NativeServiceSampleClient &service)
{
    Parcel data,reply;
    // no wait reply;
    int ret = service.invoke(android::SAMPLE_EXIT_SERVER, data, &reply,1);
    if(ret == NO_ERROR)
    {
        printf("call service exit function success\n");
    }else{
        printf("call service exit function failed\n");
    }
}

void sampleGetmacByInterfaceName(NativeServiceSampleClient &service,std::string& ifname)
{
    Parcel data, reply;
    data.writeString16(String16(ifname.c_str()));
    int ret = service.invoke(android::SAMPLE_GET_MAC, data, &reply);
    if (ret == NO_ERROR)
    {
        printf("get mac %s's mac address is %s\n",ifname.c_str(), String8(reply.readString16()).string());
    }
    else
    {
        printf("call get mac function failed\n");
    }
}

int main(int argc, char *argv[])
{
    cmdline::parser args;
    args.add("call",'\0',"call sample send msg to service");
    args.add("get",'\0',"call sample get msg from service");
    args.add<std::string>("ifname",'i',"get supplied interface name",false,"");
    args.add("exit",'\0',"call function make service exit and link death recipient");

    args.parse_check(argc,argv);

    NativeServiceSampleClient service;
    ProcessState::self()->startThreadPool();
    if(args.exist("call")){
        sampleSendStringToServer(service);
    }
    if(args.exist("get")){
        sampleGetStringFromServer(service);
    }
    if(args.exist("exit")){
        sampleExitServer(service);
        // listen to wait service died msg;
        sleep(5);
    }
    if(args.exist("ifname")){
        std::string ifname = args.get<std::string>("ifname");
        sampleGetmacByInterfaceName(service,ifname);
    }
    // printf("press any key exit\n");
    // getchar();
    return 0;
}