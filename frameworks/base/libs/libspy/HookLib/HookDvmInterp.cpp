
#include "HookBase.h"
#include "HookDvmInterp.h"

// void dvmInterpret(void* thread, const Method* method, void* pResult);

typedef void (*DVMINTERPRET_FUNC)(void*, Method*, void*);

void my_dvmInterpret(void* thread,Method* method, void* pResult);

HookBase *hook_dvmInterpret = new HookBase("libdvm.dvmInterpret", (void*)NULL, (void*)my_dvmInterpret);


void my_dvmInterpret(void* thread, Method* method, void* pResult)
{

    DEBUG_PRINT("my_dvmInterpret is called.");
    DEBUG_PRINT("method name:%s", method->name);

    DvmInterp_args args;

    args.thread = thread;
    args.method = method;
    args.pResult = pResult;

    HookContext context;
    context.args = (void*)&args;

    hook_dvmInterpret->call_pre_handlers(&context);
    // DEBUG_PRINT("call_pre_handlers ok");
    if(context.block)
        return;
    DVMINTERPRET_FUNC old_dvmInterpret = (DVMINTERPRET_FUNC)(hook_dvmInterpret->get_old_func());
    // DEBUG_PRINT("old_kill %p", old_kill);
    old_dvmInterpret(args.thread, args.method, args.pResult);

    
    // DEBUG_PRINT("old_kill ok");
    hook_dvmInterpret->call_post_handlers(&context);
    // DEBUG_PRINT("call_post_handlers ok");
    return;
}
