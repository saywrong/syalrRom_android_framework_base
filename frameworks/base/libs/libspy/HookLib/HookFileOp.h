#include "HookBase.h"

typedef struct _open_args
{
    char* path;
    int flag;
    int mode;
    int ret;
}Open_Args, *POpen_Args;
