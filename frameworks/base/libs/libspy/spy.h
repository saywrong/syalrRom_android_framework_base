#include <utils/Log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ENABLE_DEBUG 1
    
#if ENABLE_DEBUG    
#define  SYLAR_LOG_TAG "SYLAR_SPY"    
#define  LOGD(fmt, args...)  __android_log_print(ANDROID_LOG_DEBUG,SYLAR_LOG_TAG, fmt, ##args)    
#define  LOGE(fmt, args...)  __android_log_print(ANDROID_LOG_ERROR,SYLAR_LOG_TAG, fmt, ##args)    

#define DEBUG_PRINT(format,args...) \
    LOGD(format, ##args)    
#else    
#define DEBUG_PRINT(format,args...)    
#endif  

#define ERROR_PRINT(format,args...) \
    LOGE(format, ##args)    

int getProcessName(char * buffer);

int spy_client_init();
void spy_create_socket();

void spy_zygote_init();
void spy_init();


#ifdef __cplusplus
}
#endif