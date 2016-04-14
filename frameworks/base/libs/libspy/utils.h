#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>    
#include <stdlib.h>    
#include <fcntl.h>
#include <pthread.h>
#include <dlfcn.h>    
#include <dirent.h>    
#include <unistd.h>    
#include <string.h>  



#include "spy.h"

#include "jni.h"
#ifdef __cplusplus
extern "C" {
#endif

int getProcessName(char * buffer);

bool isIgnoreClass(const char *str);

char* strstr_reverse(const char* str, char* substr);

int touchFile(char *filename);

bool dumpMemToFile(void *start, size_t len,char *file_path);

JNIEnv *getEnv();

char* jstring2string(void *jobj);
#ifdef __cplusplus
}
#endif

#endif