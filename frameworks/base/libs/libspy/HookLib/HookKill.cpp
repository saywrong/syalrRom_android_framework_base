
#include "HookBase.h"
#include "HookKill.h"

// int (*old_open)(char*, int , int);
typedef int (*KILL_FUNC)(pid_t,int);

int my_kill(pid_t pid ,int sig);

HookBase *hook_kill = new HookBase("libc.kill", (void*)kill, (void*)my_kill);


int my_kill(pid_t pid,  int sig)
{

    // DEBUG_PRINT("[+] {kill} pid: %d,sig:%d\n", pid, sig);
    // if ((pid==target_pid || pid==0) && sig==9)
    // {
    //     DEBUG_PRINT("[+] target attamp to kill self.\n");
    //     return 0;
    // }
    // int ret = old_kill(pid, sig);
    // DEBUG_PRINT("[+] {kill} returned %d\n", ret);
    // return ret;

    Kill_Args args;

    args.pid = pid;
    args.sig = sig;

    HookContext context;
    context.args = (void*)&args;

    hook_kill->call_pre_handlers(&context);
    // DEBUG_PRINT("call_pre_handlers ok");
    if(context.block)
        return args.ret;
    KILL_FUNC old_kill = (KILL_FUNC)(hook_kill->get_old_func());
    // DEBUG_PRINT("old_kill %p", old_kill);
    args.ret = old_kill(args.pid, args.sig);

    
    // DEBUG_PRINT("old_kill ok");
    hook_kill->call_post_handlers(&context);
    // DEBUG_PRINT("call_post_handlers ok");
    return args.ret;
}
