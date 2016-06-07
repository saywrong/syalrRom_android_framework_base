#include "utils.h"

#include <android_runtime/AndroidRuntime.h>

JavaVM *g_vm = NULL;
JNIEnv *g_env = NULL;


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

bool isStartWith(const char *str,const char *start)
{
    if(strlen(start)>strlen(str))
        return false;
    size_t len = strlen(start);
    if(!memcmp(start,str,len))
        return true;
    return false;
}

bool isIgnoreClass(const char *str)
{
    if(isStartWith(str,"Ljava/"))
        return true;
    if(isStartWith(str,"Ljavax/"))
        return true;

    if(isStartWith(str,"Landroid/"))
        return true;
    if(isStartWith(str,"Landroidx/"))
        return true;
    if(isStartWith(str,"Lcom/android/"))
        return true;
    if(isStartWith(str,"Lorg/apache/"))
        return true;
    if(isStartWith(str,"Lcom/json/"))
        return true;
    if(isStartWith(str,"Llibcore/"))
        return true;
    if(isStartWith(str,"Ldalvik/"))
        return true;
    if(isStartWith(str,"Lcom/google/"))
        return true;
    return false;
}

char* strstr_reverse(const char* str, char* substr)
{
    char *iter,*tmp=NULL;
    iter = strstr(str,substr);
    while(iter)
    {
      tmp = iter;
      iter = strstr(iter+1,substr);
    }
    return tmp;
}

int touchFile(char *filename)
{
    char cmd[512];

    sprintf(cmd, "touch %s", filename);
    return system(cmd);
}

bool dumpMemToFile(void *start, size_t len,char *filepath)
{
    touchFile(filepath);
    DEBUG_PRINT("dumping to %s\n", filepath);
    int fd = open(filepath, O_WRONLY|O_CREAT,0777);

    if(fd!=-1)
    {
        void *buf = (void*)start;
        
        write(fd,buf,len);
        close(fd);
        DEBUG_PRINT("dumping dex success\n");
        return true;
    }
    DEBUG_PRINT("dumping dex failed errno %d\nopen file error\n", errno);
    return false;
}

char jstring2string_buff[512];

char* jstring2string(void *jstr)
{
    if(!g_env)
        getEnv();
    // DEBUG_PRINT("11111");
    char *ret = NULL;
    jclass clsstring = g_env->FindClass("java/lang/String");
    jstring strencode = g_env->NewStringUTF("utf-8");
    // DEBUG_PRINT("4444");
    jmethodID mid = g_env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    DEBUG_PRINT("%p", jstr);
    jbyteArray barr= (jbyteArray)g_env->CallObjectMethod( (jobject)jstr, mid, strencode);
    // DEBUG_PRINT("22222");
    DEBUG_PRINT("%p", jstr);
    jsize alen = g_env->GetArrayLength(barr);
    DEBUG_PRINT("%p", jstr);
    jbyte* ba = g_env->GetByteArrayElements(barr, JNI_FALSE);
    DEBUG_PRINT("%p", jstr);
    // DEBUG_PRINT("22222");
    if (alen > 0)
    {
        if(alen > 512)
          alen = 511;

        memcpy(jstring2string_buff, ba, alen);
        jstring2string_buff[511] = 0;

          
        ret = jstring2string_buff;
    }
   
    DEBUG_PRINT("%p", jstr);
    g_env->ReleaseByteArrayElements( barr, ba, 0);
    DEBUG_PRINT("%p", jstr);
    return ret;
} 

JNIEnv *getEnv()
{
    g_vm = android::AndroidRuntime::getJavaVM();
    int status = g_vm->GetEnv((void **)&g_env, JNI_VERSION_1_4);
    if(status<0)
    {
      status = g_vm->AttachCurrentThread(&g_env, NULL);
      if(status<0)
        return NULL;
    }
    return g_env;
}