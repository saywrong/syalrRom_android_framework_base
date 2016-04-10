#include "HookLoadDex.h"

typedef int (*PdvmRawDexFileOpen)(const char* , const char* , RawDexFile** , bool );

int my_dvmRawDexFileOpen(const char* fileName, const char* odexOutputName,
    RawDexFile** ppDexFile, bool isBootstrap);

HookBase *hook_dvmRawDexFileOpen = new HookBase("libdvm.dvmRawDexFileOpen", 
    (void*)dvmRawDexFileOpen, (void*)my_dvmRawDexFileOpen);



int my_dvmRawDexFileOpen(const char* fileName, const char* odexOutputName,
    RawDexFile** ppDexFile, bool isBootstrap)
{
    DvmRawDexFileOpen_args args;

    args.fileName = fileName;
    args.odexOutputName = odexOutputName;
    args.ppDexFile = ppDexFile;
    args.isBootstrap = isBootstrap;

    DEBUG_PRINT("my_dvmRawDexFileOpen : loding dex:%s", fileName);

    HookContext context;
    context.args = (void*)&args;

    hook_dvmRawDexFileOpen->call_pre_handlers(&context);
    // DEBUG_PRINT("call_pre_handlers ok");
    if(context.block)
        return args.ret;
    PdvmRawDexFileOpen old_func = (PdvmRawDexFileOpen)(hook_dvmRawDexFileOpen->get_old_func());
    // DEBUG_PRINT("old_func %p", old_func);
    args.ret = old_func(args.fileName, args.odexOutputName, args.ppDexFile, args.isBootstrap);

    
    // DEBUG_PRINT("old_func ok");
    hook_dvmRawDexFileOpen->call_post_handlers(&context);
    // DEBUG_PRINT("call_post_handlers ok");
    return args.ret;
}

//========================================================//
typedef int (*PdvmJarFileOpen)(const char* fileName, const char* odexOutputName,
    JarFile** ppJarFile, bool isBootstrap);

int my_dvmJarFileOpen(const char* fileName, const char* odexOutputName,
    JarFile** ppJarFile, bool isBootstrap);;

HookBase *hook_dvmJarFileOpen = new HookBase("libdvm.dvmJarFileOpen", 
    (void*)dvmJarFileOpen, (void*)my_dvmJarFileOpen);



int my_dvmJarFileOpen(const char* fileName, const char* odexOutputName,
    JarFile** ppJarFile, bool isBootstrap)
{
    dvmJarFileOpen_args args;

    args.fileName = fileName;
    args.odexOutputName = odexOutputName;
    args.ppJarFile = ppJarFile;
    args.isBootstrap = isBootstrap;

    DEBUG_PRINT("my_dvmJarFileOpen : loding dex:%s", fileName);

    HookContext context;
    context.args = (void*)&args;

    hook_dvmJarFileOpen->call_pre_handlers(&context);
    // DEBUG_PRINT("call_pre_handlers ok");
    if(context.block)
        return args.ret;
    PdvmJarFileOpen old_func = (PdvmJarFileOpen)(hook_dvmJarFileOpen->get_old_func());
    // DEBUG_PRINT("old_func %p", old_func);
    args.ret = old_func(args.fileName, args.odexOutputName, args.ppJarFile, args.isBootstrap);

    
    // DEBUG_PRINT("old_func ok");
    hook_dvmJarFileOpen->call_post_handlers(&context);
    // DEBUG_PRINT("call_post_handlers ok");
    return args.ret;
}

//=======================================================//

typedef int (*PdvmRawDexFileOpenArray)(u1* pBytes, u4 length, RawDexFile** ppDexFile);

int my_dvmRawDexFileOpenArray(u1* pBytes, u4 length, RawDexFile** ppDexFile);

HookBase *hook_dvmRawDexFileOpenArray = new HookBase("libdvm.dvmRawDexFileOpenArray", 
    (void*)dvmRawDexFileOpenArray, (void*)my_dvmRawDexFileOpenArray);



int my_dvmRawDexFileOpenArray(u1* pBytes, u4 length, RawDexFile** ppDexFile)
{
    dvmRawDexFileOpenArray_args args;

    args.pBytes = pBytes;
    args.length = length;
    args.ppDexFile = ppDexFile;

    DEBUG_PRINT("my_dvmRawDexFileOpenArray : loding RawDex:%p", pBytes);

    HookContext context;
    context.args = (void*)&args;

    hook_dvmRawDexFileOpenArray->call_pre_handlers(&context);
    // DEBUG_PRINT("call_pre_handlers ok");
    if(context.block)
        return args.ret;
    PdvmRawDexFileOpenArray old_func = (PdvmRawDexFileOpenArray)(hook_dvmRawDexFileOpenArray->get_old_func());
    // DEBUG_PRINT("old_func %p", old_func);
    args.ret = old_func(args.pBytes, args.length, args.ppDexFile);

    
    // DEBUG_PRINT("old_func ok");
    hook_dvmRawDexFileOpenArray->call_post_handlers(&context);
    // DEBUG_PRINT("call_post_handlers ok");
    return args.ret;
}