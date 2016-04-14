#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <dlfcn.h>

#include "spy.h"
#include "HookLib/HookMain.h"

std::vector<const char*> gTarget;

void add_spy_target(const char *name)
{
    gTarget.push_back(name);
}

bool is_spy_target(const char *name)
{
    for(uint i=0;i<gTarget.size();++i)
    {
        if(!strcmp(gTarget[i],name))   // compare package name, if not care, return
            return true;
    }
    return false;
}
void enable_spy_invoke()
{
    typedef void (*PdvmMterpSpyEnable)(void);
    void *libdvm = dlopen("libdvm.so",RTLD_NOW);
    PdvmMterpSpyEnable dvmMterpSpyEnable = (PdvmMterpSpyEnable)dlsym(libdvm,"dvmMterpSpyEnable");
    dvmMterpSpyEnable();
    DEBUG_PRINT("enable_spy_invoke ok\n");
}
void spy_zygote_init()
{
    spy_create_socket();
    // add_spy_target("com.MobileTicket");
    add_spy_target("crackme.a3");
    add_spy_target("com.example.hellojni");
    // add_spy_target("com.tencent.android.qqdownloader");
    // add_spy_target("com.tencent.android.qqdownloader_getapkinfo");
    // add_spy_target("com.tencent.android.qqdownloader_connect");

}

void spy_init()
{
    // DEBUG_PRINT("Im in\n");
    char buffer[256];

    getProcessName(buffer);
    DEBUG_PRINT("package [ %s ] is starting.\n", buffer);

    if(!is_spy_target(buffer))
        return;

    DEBUG_PRINT("package [ %s ] is our target.\n", buffer);

    DEBUG_PRINT("add hooks\n");
    spy_hook_init();
    DEBUG_PRINT("add hooks ok.\n");

    enable_spy_invoke();

    DEBUG_PRINT("connecting to spy server.\n");
    spy_client_init();
}