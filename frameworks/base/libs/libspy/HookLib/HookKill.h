#ifndef __HOOKKILL_H
#define __HOOKKILL_H

#include <signal.h>

typedef struct _kill_args
{
    pid_t pid;
    int sig;
    int ret;
}Kill_Args, *PKill_Args;

#endif