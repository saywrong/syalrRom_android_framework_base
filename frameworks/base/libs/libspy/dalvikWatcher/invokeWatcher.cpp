#include "invokeWatcher.h"
#include "spy.h"
#include "../utils.h"
#include "dalvikWatcher.h"



extern PSYLARJSTRING2CSTR sylarJstring2Cstr;

char* methodDescriptorNextArgType(char* descriptor, char *buf) {
    // Skip any array references.

    // ERROR_PRINT("methodDescriptorNextArgType 1");
    if(descriptor==NULL)
        return NULL;

    while (*descriptor == '(') {
        descriptor++;
    }


    if(*descriptor == ')')
        return NULL;
    while(*descriptor == ' ')
        descriptor++;  

    memset(buf,0,64);
    char *start=NULL,*end=NULL;
    // ERROR_PRINT("methodDescriptorNextArgType 2");
    switch (*descriptor) {
        case '[':{
            start = start ? start:descriptor;
            descriptor++;
        }

        case 'B': case 'C': case 'D': case 'F':
        case 'I': case 'J': case 'S': case 'Z': {

            start = start ? start : descriptor;
            end = descriptor + 1;
            break;
        }
        case 'L': {
            start = start ? start:descriptor;
            end = strchr(descriptor + 1, ';');
            if(end != NULL)
            {
                end++;  
            }
            else
                ERROR_PRINT("error look L end\n");
            break;
        }
        default:
        {
            ERROR_PRINT("unknown type %c", *descriptor);
            break;
        }
    }
    // ERROR_PRINT("methodDescriptorNextArgType 3");
    if(start&&end)
    {
        uint len = end - start;
        len = len>=64? 63:len;
        memcpy(buf, start, len);
    }
    // ERROR_PRINT("methodDescriptorNextArgType 4");
    return end;
}
int temp = 0;
void dalvik_invoke_watcher(Method *curMethod, Method *callMethod, _u4 *outs, char *desc, void *thread)
{
    ClassObject* curclazz = curMethod->clazz;
    ClassObject* callClazz = callMethod->clazz;

    if(isIgnoreClass(curclazz->descriptor) && isIgnoreClass(callClazz->descriptor))
        return;
    DEBUG_PRINT("%s.%s calling %s.%s\n",curclazz->descriptor, curMethod->name,
                                  callClazz->descriptor, callMethod->name);
    DEBUG_PRINT("method desc:%s", desc);
    // DEBUG_PRINT("method accessFlags:%x", callMethod->accessFlags);
    
    // char argtype_buff[64];
    // char *iter = methodDescriptorNextArgType(desc, argtype_buff);
    int arg_ount=0;
    u4 accessFlags = callMethod->accessFlags;

    if( (accessFlags & ACC_STATIC) == 0)
        //not static method, skip *this
        arg_ount++;
    char *iter = callMethod->shorty;

    // ERROR_PRINT("shorty-> %s\n",iter);
    while((*++iter) != '\0')
    {   
        // ERROR_PRINT("arg %d type: %s iter:%p ", arg_ount, argtype_buff, iter);
        // if(!strcmp(argtype_buff, "Ljava/lang/String;"))
        // {
            // ERROR_PRINT("jstring out[%d]: %x \n",arg_ount, outs[arg_ount]);
        switch(*iter){
            case 'L':
            {
                // ERROR_PRINT("argcount %d \n", arg_ount);
                char *str = sylarJstring2Cstr((void*)(outs[arg_ount]));
                if(str!=NULL)
                {
                    ERROR_PRINT("arg %d is JavaString: %s", arg_ount, str);
                    // ERROR_PRINT("jstring out[%d]: %x \n",arg_ount, outs[arg_ount]);
                    free(str);
                }
                
                break;
            }
            case 'D':
            case 'J':
                arg_ount++;
                break;
            default:
                /* Z B C S I -- do nothing */
                break;
        }
        arg_ount++;
        
        
    }

}
