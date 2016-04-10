#include <dlfcn.h>
#include "HookBase.h"

_hook_context::_hook_context()
{
    args = NULL;
    block = false;
}

HookManager *HookManager::_instance = NULL; 

void HookManager::register_hook(HookBase* hook)
{
    DEBUG_PRINT("HookManager: new hook registered {%s}\n", hook->getname());

    hooks.push_back(hook);
    DEBUG_PRINT("HookManager: hook count %d\n", hooks.size());
}

void HookManager::register_handler(const char *name,HANDLER_TYPE type,HANDLER handler)
{
    DEBUG_PRINT("registering handler for %s\n", name);
    for(size_t i=0;i<hooks.size();++i)
    {
        if(strcmp(hooks[i]->getname(),name))
            continue;
        DEBUG_PRINT("find hookbase");
        if(type == PRE_HANDLER)
            hooks[i]->add_pre_handler(handler);
        else if(type == POST_HANDLER)
            hooks[i]->add_post_handler(handler);
        return;
    }
}   

void HookManager::start()
{
    for(size_t i=0;i<hooks.size();++i)
        hooks[i]->startHook();
}

HookManager::HookManager()
{

}

HookManager* HookManager::getHookManager()
{
    //TODO: 同步问题
    if(_instance==NULL)
        _instance = new HookManager();
    return _instance;
}


HookBase::HookBase(const char* name, void *target, void *new_func)
{
    DEBUG_PRINT("new hookbase %s\n", name);
    this->target = target;
    this->new_func = new_func;
    old_func = NULL;
    strcpy(this->name, name);
    HookManager::getHookManager()->register_hook(this);
}
char *HookBase::getname()
{
    return name;
}

void HookBase::add_pre_handler(HANDLER handler)
{
    preHandler_list.push_back(handler);
}

void HookBase::add_post_handler(HANDLER handler)
{
    postHandler_list.push_back(handler);
}

void HookBase::call_pre_handlers(PHook_Context context)
{
    for(size_t i=0;i<preHandler_list.size();++i)
    {
        HANDLER hanlder = preHandler_list[i];
        (*hanlder)(context);
        if(context->block)
            return;
    }
}

void HookBase::call_post_handlers(PHook_Context context)
{
    for(size_t i=0;i<postHandler_list.size();++i)
    {
        HANDLER hanlder = postHandler_list[i];
        (*hanlder)(context);
    }
}

void* HookBase::get_old_func()
{
    return old_func;
}

void HookBase::startHook()
{
    DEBUG_PRINT("Hooking %s\n", this->name);
    if(target == NULL)
    {
        // 
        // if target func is not provide
        // then resolve the libname and 
        // symname from the hookname 
        //
        char libname[HOOK_NAME_MAX_LEN];
        char *symbol = strstr(name,".") + 1;
        int libname_len = symbol - this->name;
        memcpy(libname,name,libname_len);

        DEBUG_PRINT("target is NULL, libname:%s symname:%s\n",
                                             libname, symbol);
        void *lib = dlopen(libname, RTLD_NOW);
        if(lib == NULL)
            ERROR_PRINT("open lib faild\n");
        target = dlsym(lib,symbol);
        if(target == NULL)
            ERROR_PRINT("dlsym faild\n");
        MSHookFunction(target, new_func, (void**) (&old_func));
        
    }
    // DEBUG_PRINT("%p %p %p", target,new_func,old_func);
    else 
        MSHookFunction(target, new_func, (void**) (&old_func));
    // DEBUG_PRINT("%p %p %p", target,new_func,old_func);
}
// void HookBase::endHook();

