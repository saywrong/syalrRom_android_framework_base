#ifndef _INVOKE_WATCHER_H
#define _INVOKE_WATCHER_H

#include "../HookLib/HookTypes.h"

extern "C" void dalvik_invoke_watcher(Method *curMethod, Method *callMethod, void *sp, void *thread);

#endif