#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
 
#define PORT 9190


int main(void)
{
    int size;
    int listen_sd;
    int sd;
    struct sockaddr_in sa_serv;
    struct sockaddr_in sa_cli;
    size_t client_len;
    char buf[4096];
    char* msg = "Welcome to Server!!";
  
    listen_sd = socket(AF_INET, SOCK_STREAM, 0);
 
    memset(&sa_serv, 0, sizeof(sa_serv));
    sa_serv.sin_family = AF_INET;
    sa_serv.sin_addr.s_addr = INADDR_ANY;
    sa_serv.sin_port = htons(PORT);
 
    if(bind(listen_sd, (struct sockaddr*)&sa_serv, sizeof(sa_serv)) == -1){
        return -1;
    }
   
    if(listen(listen_sd, 5) == -1){
        return -1;
    }

    client_len = sizeof(sa_cli);
    sd = accept(listen_sd, (struct sockaddr*)&sa_cli, &client_len);

    if((size = read(sd, buf, sizeof(buf)-1)) == -1){
        return -1;
    }
    
    buf[size] = 0;
    printf("\n%s\n\n", buf);
   
    if(write(sd, msg, strlen(msg)) == -1){
        return -1;
    }

    close(sd);
    close(listen_sd);
  
    return(0);
}

