
#include "HookBase.h"
#include "HookDvmCallJNIMethod.h"

// void dvmInterpret(void* thread, const Method* method, void* pResult);

typedef void (*DVMCALLJNIMETHOD)(u4* args, void* pResult, Method* method, void* thread);

void my_dvmCallJNIMethod(u4* args, void* pResult, Method* method, void* thread);

HookBase *hook_dvmCallJNIMethod = new HookBase("libdvm.so-dvmCallJNIMethod", (void*)NULL, (void*)my_dvmCallJNIMethod);


void my_dvmCallJNIMethod(u4* args, void* pResult, Method* method, void* thread)
{

    // if(!isIgnoreClass(method->clazz->descriptor))
    //     DEBUG_PRINT("native call %s->%s:%p",method->clazz->descriptor, method->name, method->insns);

    DvmCallJNIMethod_args args1;

    args1.args = args;
    args1.pResult = pResult;
    args1.method = method;
    args1.thread = thread;

    HookContext context;
    context.args = (void*)&args1;

    hook_dvmCallJNIMethod->call_pre_handlers(&context);
    // DEBUG_PRINT("call_pre_handlers ok");
    if(context.block)
        return;
    DVMCALLJNIMETHOD old_dvmCallJNIMethod = (DVMCALLJNIMETHOD)(hook_dvmCallJNIMethod->get_old_func());
    // DEBUG_PRINT("old_kill %p", old_kill);
    old_dvmCallJNIMethod(args1.args, args1.pResult, args1.method, args1.thread);
 
    // DEBUG_PRINT("old_kill ok");
    hook_dvmCallJNIMethod->call_post_handlers(&context);
    // DEBUG_PRINT("call_post_handlers ok");
    return;
}
