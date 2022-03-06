#ifndef _CMDID_H_
#define _CMDID_H_
#include "binder/IBinder.h"
namespace android{
    enum ENUME_CMD{
        SAMPLE_CALL = IBinder::FIRST_CALL_TRANSACTION,
        SAMPLE_GET,
        SAMPLE_GET_MAC,
        SAMPLE_EXIT_SERVER,
        LAST_COMMAND = IBinder::LAST_CALL_TRANSACTION
    };
}
#endif