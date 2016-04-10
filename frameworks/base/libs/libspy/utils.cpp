#include <stdio.h>    
#include <stdlib.h>    
#include <fcntl.h>
#include <pthread.h>
#include <dlfcn.h>    
#include <dirent.h>    
#include <unistd.h>    
#include <string.h>  

#include "spy.h"

#define BUFLEN 1024
//get packagename from pid
int getProcessName(char * buffer){
    char path_t[256]={0};
    pid_t pid=getpid();
    char str[15];
    sprintf(str, "%d", pid);
    memset(path_t, 0 , sizeof(path_t));
    strcat(path_t, "/proc/");
    strcat(path_t, str);
    strcat(path_t, "/cmdline");
    //LOG_ERROR("zhw", "path:%s", path_t);
    int fd_t = open(path_t, O_RDONLY);
    if(fd_t>0){
        int read_count = read(fd_t, buffer, BUFLEN);
 
        if(read_count>0){
              uint  processIndex=0;
              for(processIndex=0;processIndex<strlen(buffer);processIndex++){
                  if(buffer[processIndex]==':'){
                      buffer[processIndex]='_';
                  }
 
              }
            return 1;
        }
    }
    return 0;
}