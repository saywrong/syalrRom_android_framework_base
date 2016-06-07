#include "dalvikWatcher.h"
#include "spy.h"
#include "utils.h"

typedef bool (*PMTERPWATCHERINIT)();


PSYLARJSTRING2CSTR sylarJstring2Cstr = NULL;

bool dalvikWatcherInit()
{
    DEBUG_PRINT("dalvikWatcherInit initing...");
    do{
        void *lib = dlopen("libdvm.so", RTLD_NOW);
        if(lib==NULL)
        {
            DEBUG_PRINT("dlopen failed!\n");
            break;
        }
        PMTERPWATCHERINIT mterpWatcherInit = \
                (PMTERPWATCHERINIT)dlsym(lib, "mterpWatcherInit");
        if(mterpWatcherInit==NULL)
        {
            DEBUG_PRINT("dlsym mterpWatcherInit failed!\n");
            break;
        }
        sylarJstring2Cstr = (PSYLARJSTRING2CSTR)dlsym(lib, "sylarJstring2Cstr");
        if(sylarJstring2Cstr==NULL)
        {
            DEBUG_PRINT("dlsym sylarJstring2Cstr failed!\n");
            break;
        }
        if(mterpWatcherInit()==false)
        {
            DEBUG_PRINT("mterpWatcherInit failed!\n");
            break;
        }
        DEBUG_PRINT("dalvikWatcherInit success");
        return true;
    }while(0);
    
    DEBUG_PRINT("dalvikWatcherInit fail");
    return false;
}