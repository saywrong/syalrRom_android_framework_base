#include <stdio.h>    
#include <stdlib.h>    
#include <fcntl.h>
#include <pthread.h>
#include <dlfcn.h>    
#include <dirent.h>    
#include <unistd.h>    
#include <string.h>    
#include <elf.h>   
#include <errno.h> 

#include <asm/user.h>    
#include <asm/ptrace.h>    
#include <sys/ptrace.h>    
#include <sys/wait.h>    
#include <sys/mman.h>
#include <sys/socket.h>  
#include <sys/endian.h>
#include <sys/types.h>
#include <linux/in.h>
#include <arpa/inet.h>

#include "spy.h"

#define PORT 8919
int client_sockfd;

int run_command(char* command, void* args, char *result)
{
    DEBUG_PRINT("run command ok");
    if (!strcmp(command,"pkg_name"))
        getProcessName(result);
    else
        strcpy(result,command);
    return 0;
}

void* connect_sylar_server(void *data)
{
    // int client_sockfd;
    int len;
    struct sockaddr_in remote_addr; //服务器端网络地址结构体
    char buf[BUFSIZ];  //数据传送的缓冲区
    char result[BUFSIZ];

    memset(&remote_addr,0,sizeof(remote_addr)); //数据初始化--清零
    remote_addr.sin_family=AF_INET; //设置为IP通信
    remote_addr.sin_addr.s_addr=inet_addr("127.0.0.1");//服务器IP地址
    remote_addr.sin_port=htons(PORT); //服务器端口号

    // /*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
    // if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
    // {
    //     DEBUG_PRINT("socket error: %s", strerror(errno));
    //     return NULL;
    // }

    /*将套接字绑定到服务器的网络地址上*/
    if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
    {
        DEBUG_PRINT("connect error: %s", strerror(errno));
        return NULL;
    }
    DEBUG_PRINT("connected to server\n");
    len=recv(client_sockfd,buf,BUFSIZ,0);//接收服务器端信息
    buf[len]='\0';
    DEBUG_PRINT("%s",buf); //打印服务器端信息

    /*循环接收信息并打印接收信息--recv返回接收到的字节数，send返回发送的字节数*/
    while((len=recv(client_sockfd, buf, BUFSIZ, 0))>0)
    {
        
        buf[len]='\0';
        DEBUG_PRINT("recieve command:%s\n", buf);
        if(!strcmp(buf,"quit"))
        {
            break;
        }
        run_command(buf, NULL, result);

        len=send(client_sockfd,result,strlen(result),0);
    }
    close(client_sockfd);//关闭套接字
    return NULL;
}

void spy_create_socket()
{
    /*创建客户端套接字--IPv4协议，面向连接通信，TCP协议*/
    if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        DEBUG_PRINT("socket error: %s", strerror(errno));
        return;
    }
}

int spy_client_init() {
    DEBUG_PRINT("spy client init...\n");
    pthread_t th;

    int ret;
    ret = pthread_create(&th, NULL, connect_sylar_server, NULL);  
    DEBUG_PRINT("thread started\n");
    // void *status;
    // ret = pthread_join(th, &status);
    return ret;
}
