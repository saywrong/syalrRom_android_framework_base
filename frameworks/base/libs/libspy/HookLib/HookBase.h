#ifndef __HOOK_BASE_H
#define __HOOK_BASE_H

#include <vector>

#include "../Substrate/SubstrateHook.h"

#include "../spy.h"

typedef struct _hook_context
{
    void *args;
    bool block;
    _hook_context();
}HookContext, *PHook_Context;

typedef void (*HANDLER)(PHook_Context context);

#define HOOK_NAME_MAX_LEN 512
class HookBase
{
    void *target;
    void *old_func;
    void *new_func;
    char name[HOOK_NAME_MAX_LEN];
    std::vector<HANDLER> preHandler_list;
    std::vector<HANDLER> postHandler_list;
public:
    HookBase(const char* name, void *target, void *new_func);

    char *getname();
    void add_pre_handler(HANDLER );
    void add_post_handler(HANDLER );
    void call_pre_handlers(PHook_Context context);
    void call_post_handlers(PHook_Context context);
    void startHook();
    void* get_old_func();
    // void endHook();

};

enum HANDLER_TYPE{
    PRE_HANDLER,
    POST_HANDLER
};

class HookManager
{
    std::vector<HookBase*> hooks;
    static HookManager *_instance;
    HookManager();
public:
    static HookManager* getHookManager();
    void register_hook(HookBase*);
    void start();
    void register_handler(const char *name,HANDLER_TYPE,HANDLER);
};

#endif