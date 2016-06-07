#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "Substrate/SubstrateHook.h"

#include "HookBase.h"
#include "HookMain.h"
#include "HookFileOp.h"
#include "HookKill.h"
#include "HookLoadDex.h"
#include "HookDvmCallJNIMethod.h"

#include <libdex/DexFile.h>

void pre_open_handler(PHook_Context context)
{
    POpen_Args args = (POpen_Args)context->args;

    DEBUG_PRINT("pre_open_handler: opening:%s\n", args->path);
    if(!strcmp(args->path,"/proc/self/maps"))
    {
        DEBUG_PRINT("11111");
    }
}

void pre_open_handler2(PHook_Context context)
{
    POpen_Args args = (POpen_Args)context->args;

    DEBUG_PRINT("pre_open_handler: opening:%s\n", args->path);
    if(!strcmp(args->path,"/proc/self/maps"))
    {
        DEBUG_PRINT("22222");
    }
}

void post_open_handler(PHook_Context context)
{
    POpen_Args args = (POpen_Args)context->args;

    DEBUG_PRINT("[libc.open] path: %s ret %d\n", args->path,args->ret);
}
void pre_kill_handler(PHook_Context context)
{
    PKill_Args args = (PKill_Args)context->args;
    // args->pid = 9999;
    DEBUG_PRINT("[libc.kill] pid: %d sig: %d", args->pid, args->sig);
}

void post_kill_handler(PHook_Context context)
{
    PKill_Args args = (PKill_Args)context->args;

    DEBUG_PRINT("[libc.kill] pid: %d sig: %d ret: %d", args->pid, args->sig, args->ret);
}

void post_dvmRawDexFileOpenArray_handler(PHook_Context context)
{
    PdvmRawDexFileOpenArray_args args = (PdvmRawDexFileOpenArray_args)context->args;

    RawDexFile** ppDexFile = args->ppDexFile;
    RawDexFile* pDexFile = *ppDexFile;
    char * cacheName = pDexFile->cacheFileName;
    DvmDex* pDvmDex = pDexFile->pDvmDex;
    const DexHeader*    pHeader = pDvmDex->pHeader;

    DEBUG_PRINT("[libdvm.dvmRawDexFileOpenArray] start: %p,lenth: %d,ret: %d \n", args->pBytes, args->length, args->ret);
    DEBUG_PRINT("[libdvm.dvmRawDexFileOpenArray] DexFile start %p, lenth %x\n",pHeader,pHeader->fileSize);
    
    DEBUG_PRINT("dumping dex..\n");
    int fd = open("/data/local/tmp/dump.dex",O_WRONLY);
    if(fd!=-1)
    {
        void *buf = (void*)pHeader;
        
        write(fd,buf,(size_t)pHeader->fileSize);
        close(fd);
        DEBUG_PRINT("dumping dex success\n");
        return;
    }
    DEBUG_PRINT("open file error\n");
}

void *g_start;
size_t g_len;
char g_filepath[512];

void post_dvmJarFileOpen_handler(PHook_Context context)
{
    PdvmJarFileOpen_args args = (PdvmJarFileOpen_args)context->args;

    JarFile** ppJarFile = args->ppJarFile;
    if(ppJarFile==NULL)
        return;
    JarFile* pJarFile = *ppJarFile;
    if(pJarFile==NULL)
        return;
    char * cacheName = pJarFile->cacheFileName;
    DvmDex* pDvmDex = pJarFile->pDvmDex;
    const DexHeader* pHeader = pDvmDex->pHeader;
    if(pHeader==NULL)
        return;

    // DEBUG_PRINT("[libdvm.dvmJarFileOpen] start: %p,lenth: %d,ret: %d \n", args->pBytes, args->length, args->ret);
    DEBUG_PRINT("[libdvm.dvmJarFileOpen] DexFile start %p, lenth %x\n",pHeader,pHeader->fileSize);
    
    char *filename = strstr_reverse(args->fileName, "/");
    char filepath[512];
    if(filename)
        sprintf(filepath,"/data/local/tmp/%s.dex", filename+1);
    else 
        sprintf(filepath,"/data/local/tmp/%s.dex", "tmp");
    strcpy(g_filepath, filepath);

    // dumpMemToFile((void *)pHeader, pHeader->fileSize, filepath);
    g_start = (void*)pHeader;
    g_len = pHeader->fileSize;
}


void pre_dvmCallJNIMethod_handler(PHook_Context context)
{
    PDvmCallJNIMethod_args args = (PDvmCallJNIMethod_args)context->args;
    Method* method = args->method;
    if(!isIgnoreClass(method->clazz->descriptor))
        DEBUG_PRINT("native call %s->%s:%p\n",method->clazz->descriptor, method->name, method->insns);
    // if(!strcmp(method->name,"Ooo0ooO0oO"))
    //     dumpMemToFile(g_start, g_len, g_filepath);
}

void spy_hook_init()
{
    DEBUG_PRINT("in spy hook init");
    // register_hook_handler("libc.open", PRE_HANDLER, pre_open_handler);
    register_hook_handler("libc.kill", PRE_HANDLER, pre_kill_handler);
    // register_hook_handler("libc.kill", POST_HANDLER, post_kill_handler);
    register_hook_handler("libc.open", POST_HANDLER, post_open_handler);
    register_hook_handler("libdvm.dvmRawDexFileOpenArray", POST_HANDLER, 
        post_dvmRawDexFileOpenArray_handler);
    // register_hook_handler("libdvm.dvmJarFileOpen", POST_HANDLER, 
    //     post_dvmJarFileOpen_handler);
    register_hook_handler("libdvm.so-dvmCallJNIMethod", PRE_HANDLER, 
        pre_dvmCallJNIMethod_handler);

    HookManager::getHookManager()->start();
    // size_t used = MSHookFunction(open, my_open, (void**) (&old_open));
    // used = MSHookFunction(kill, my_kill, (void**) (&old_kill));

    // int tid = syscall(__NR_gettid);
	// DEBUG_PRINT("so has been loaded\n");
}


//======================export==========================//
void register_hook_handler(const char *name,HANDLER_TYPE type,HANDLER handler)
{
    HookManager::getHookManager()->register_handler(name, type, handler);
}