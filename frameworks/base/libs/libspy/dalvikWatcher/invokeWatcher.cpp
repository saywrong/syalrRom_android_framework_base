#include "invokeWatcher.h"
#include "spy.h"
#include "../utils.h"

#define SAVEAREA_FROM_FP(_fp)   ((char*)(_fp) -20)

#define OUTS_FROM_SP(_sp, _argCount) \
    ((u4*) ((u1*)_sp - sizeof(u4) * (_argCount)))

void *getArg(void *sp)
{
    // void *sp = SAVEAREA_FROM_FP(fp);
    return (void*)(sp-1);
}

// char* getMethodProto(DexProto *dp)
// {
//     const DexFile* dexFile = pProto->dexFile;
//     const DexProtoId* protoId = getProtoId(pProto);
//     const DexTypeList* typeList = dexGetProtoParameters(dexFile, protoId);
//     size_t length = 3; // parens and terminating '\0'
//     u4 paramCount = (typeList == NULL) ? 0 : typeList->size;
//     u4 i;

//     for (i = 0; i < paramCount; i++) {
//         u4 idx = dexTypeListGetIdx(typeList, i);
//         length += strlen(dexStringByTypeIdx(dexFile, idx));
//     }

//     length += strlen(dexStringByTypeIdx(dexFile, protoId->returnTypeIdx));

//     dexStringCacheAlloc(pCache, length);

//     char *at = (char*) pCache->value;
//     *(at++) = '(';

//     for (i = 0; i < paramCount; i++) {
//         u4 idx = dexTypeListGetIdx(typeList, i);
//         const char* desc = dexStringByTypeIdx(dexFile, idx);
//         strcpy(at, desc);
//         at += strlen(desc);
//     }

//     *(at++) = ')';

//     strcpy(at, dexStringByTypeIdx(dexFile, protoId->returnTypeIdx));
//     return pCache->value;
// }

void dalvik_invoke_watcher(Method *curMethod, Method *callMethod, void *sp, void *thread)
{
    ClassObject* curclazz = curMethod->clazz;
    ClassObject* callClazz = callMethod->clazz;

    // if(isIgnoreClass(curclazz->descriptor) && isIgnoreClass(callClazz->descriptor))
    //     return;
    // DEBUG_PRINT("%s.%s:%p calling %s.%s:%p\n",curclazz->descriptor, curMethod->name, curMethod->insns,
    //                               callClazz->descriptor, callMethod->name, callMethod->insns);
    if(!strcmp(callClazz->descriptor,"Ljava/lang/System;")&&!strcmp(callMethod->name,"loadLibrary"))
    {
        ERROR_PRINT("loadLibrary is called\n");
        ERROR_PRINT("shorty: %s\n", callMethod->shorty);
        u4* outs = OUTS_FROM_SP(sp,1);
        char *str = jstring2string((void*)outs);
        ERROR_PRINT("loadLibrary name: %s",str);
    }

}
