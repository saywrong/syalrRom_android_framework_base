#ifndef __HOOKLOADDEX_H
#define __HOOKLOADDEX_H

#include <RawDexFile.h>
#include <DvmDex.h>
#include <JarFile.h>

#include "HookBase.h"

typedef struct _dvmRawDexFileOpen_args
{
    const char* fileName;
    const char* odexOutputName;
    RawDexFile** ppDexFile;
    bool isBootstrap;
    int ret;
}DvmRawDexFileOpen_args, *PDvmRawDexFileOpen_args;

typedef struct _dvmJarFileOpen_args
{
    const char* fileName;
    const char* odexOutputName;
    JarFile** ppJarFile;
    bool isBootstrap;
    int ret;
}dvmJarFileOpen_args, *PdvmJarFileOpen_args;

typedef struct _dvmRawDexFileOpenArray_args
{
    u1* pBytes;
    u4 length;
    RawDexFile** ppDexFile;
    int ret;
}dvmRawDexFileOpenArray_args, *PdvmRawDexFileOpenArray_args;



#endif