#ifndef _INVOKE_WATCHER_H
#define _INVOKE_WATCHER_H

#include "../HookLib/HookTypes.h"

typedef uint8_t             _u1;
typedef uint16_t            _u2;
typedef uint32_t            _u4;
typedef uint64_t            _u8;

extern "C" void dalvik_invoke_watcher(Method *curMethod, Method *callMethod, _u4 *outs, char *desc, void *thread);



#endif