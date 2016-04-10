#ifndef __HOOKMAIN_H
#define __HOOKMAIN_H

#include "HookBase.h"

void spy_hook_init();
void register_hook_handler(const char *name,HANDLER_TYPE type,HANDLER handler);

#endif