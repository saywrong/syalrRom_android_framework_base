#include "HookTypes.h"
#include "HookBase.h"


typedef struct _dvmCallJNIMethod_args
{
    u4*     args;
    void*   pResult;
    Method* method;
    void* thread;
}DvmCallJNIMethod_args, *PDvmCallJNIMethod_args;
