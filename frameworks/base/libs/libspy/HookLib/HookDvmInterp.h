// #include <DvmDex.h>

#include "HookBase.h"
#include "HookTypes.h"

typedef struct _dvmInterp_args
{
    void* thread;
    Method* method;
    void* pResult;
}DvmInterp_args, *PDvmInterp_args;
