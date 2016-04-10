#include <fcntl.h>
#include "HookBase.h"
#include "HookFileOp.h"

typedef int (*OPEN_FUNC)(char*, int , int);

int my_open(char* path, int flag, int mode);

HookBase *hook_open = new HookBase("libc.open", (void*)open, (void*)my_open);

int my_open(char* path, int flag, int mode)
{
    // DEBUG_PRINT("[+] {open} file: %s\n", path);

    Open_Args args;

    args.path = path;
    args.flag = flag;
    args.mode = mode;

    HookContext context;
    context.args = (void*)&args;

    hook_open->call_pre_handlers(&context);
    // DEBUG_PRINT("call_pre_handlers ok");
    if(context.block)
        return args.ret;
    OPEN_FUNC old_open = (OPEN_FUNC)(hook_open->get_old_func());
    // DEBUG_PRINT("old_open %p", old_open);
    args.ret = old_open(args.path, args.flag, args.mode);

    
    // DEBUG_PRINT("old_open ok");
    hook_open->call_post_handlers(&context);
    // DEBUG_PRINT("call_post_handlers ok");
    return args.ret;
}



// class HookOpen :public HookBase
// {
// public:
//     virtual void call_pre_handlers(void *context)
//     {
//         for handler in preHandler_list
//             (pre_handler)handler(context,block)
//             if block
//                 return;
//     }
//     virtual void call_post_handlers(void *context)
//     {
//         for handler in postHandler_list
//     }       (post_handler)handler(args,ret_obj);
// }