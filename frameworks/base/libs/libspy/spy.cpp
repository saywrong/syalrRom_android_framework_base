#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <dlfcn.h>

#include "spy.h"
#include "HookLib/HookMain.h"
#include "dalvikWatcher/dalvikWatcher.h"

std::vector<char*> gTarget;

void add_spy_target(char *name)
{
    gTarget.push_back(name);
}

bool is_spy_target(char *name)
{
    for(uint i=0;i<gTarget.size();++i)
    {
        uint lenA = strlen(gTarget[i]);
        uint lenB = strlen(name);
        if(lenA!=lenB)
            continue;

        if(!strcmp(gTarget[i],name))   // compare package name, if not care, return
            return true;
    }
    return false;
}
void enable_spy_invoke()
{
    typedef void (*PdvmMterpSpyEnable)(void);
    void *libdvm = dlopen("libdvm.so",RTLD_NOW);
    PdvmMterpSpyEnable dvmMterpSpyEnable = (PdvmMterpSpyEnable)dlsym(libdvm,"dvmMterpSpyEnable");
    dvmMterpSpyEnable();
    dalvikWatcherInit();
    DEBUG_PRINT("enable_spy_invoke ok\n");
}

void print_hex(void *inbuff, uint length,const char* note)
{
    uint i, j, width = 16;
    unsigned char *buff = (unsigned char *)inbuff;
    note = note ? note : "hex data at";
    DEBUG_PRINT("%s: 0x%08x\n", note, (unsigned int)inbuff);

    if(buff == NULL)
        return;

    for(i = 0; i < length; ++i)
    {
        DEBUG_PRINT("%02x ", buff[i]);
        if((i + 1) % width == 0)
        {
            for(j = i - width + 1; j <= i; ++j)
            {
                //print ascii
                if(buff[j] >= ' ' && buff[j] <= '~')
                    DEBUG_PRINT("%c", buff[j]);
                else
                    DEBUG_PRINT(".");
            }
            DEBUG_PRINT("\n");
        }
    }
    
    DEBUG_PRINT("\n");
}

#define LINE_BUFFER_LEN 512

void read_config()
{

    DEBUG_PRINT("Reading config...\n");
    FILE* fd;
    char *buff = (char*)malloc(LINE_BUFFER_LEN);
    do{
        if(buff==NULL)
            break;
        fd = fopen(CONFIG_FILE, "r");
        if(fd==0)
            break;

        while(fgets(buff,512,fd))
        {
            
            uint len = strlen(buff);
            char *temp = (char*)malloc(len+1);
            strcpy(temp, buff);
            temp[len-1] = '\0';
            add_spy_target(temp);
            DEBUG_PRINT("target set: %s\n", temp);
        }
        fclose(fd);
        free(buff);
        DEBUG_PRINT("Reading success\n");
        return;
    }while(0);
    if(fd)
        fclose(fd);
    if(buff)
        free(buff);
    DEBUG_PRINT("Reading failed\n");
}
void spy_zygote_init()
{
    spy_create_socket();
    read_config();

}

void spy_init()
{
    // DEBUG_PRINT("Im in\n");
    char buffer[256];

    getProcessName(buffer);
    DEBUG_PRINT("package [ %s ] is starting.\n", buffer);

    if(!is_spy_target(buffer))
        return;

    DEBUG_PRINT("package [ %s ] is our target.\n", buffer);

    DEBUG_PRINT("add hooks\n");
    spy_hook_init();
    DEBUG_PRINT("add hooks ok.\n");

    if(dalvikWatcherInit()==true)
        enable_spy_invoke();

    DEBUG_PRINT("connecting to spy server.\n");
    spy_client_init();
}