#include <stdio.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 9190


int main(void)
{
    int sd;
    struct sockaddr_in sa;
    int size;
    char buf[4096];
    char* serv_ip = "192.168.152.180";
    char* msg = "Hello World!!";
  
    sd = socket(AF_INET, SOCK_STREAM, 0);
   
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(serv_ip);  
    sa.sin_port = htons(PORT);                
   
    if(connect(sd, (struct sockaddr*)&sa, sizeof(sa)) == -1){
        return -1;
    }

    if(write(sd, msg, strlen(msg)) == -1){
        return -1;
    }
   
    if((size = read(sd, buf, sizeof(buf) - 1)) == -1){
        return -1;
    }

    buf[size] = 0;
    printf("\n%s\n\n", buf);
   
    close(sd);
   
    return 0;
}

