#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "packet.h"
#include "codec.h"
#include "packet_controller.h"

int read_msg(int sock, unsigned char **msgBuf, int *msgType);
ssize_t readn(int sockfd, unsigned char *buf, size_t nbytes);
ssize_t writen(int sockfd, unsigned char *buf, size_t nbytes);
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	unsigned char *message, *sendBuf;
	size_t recv_len = 0;
	int i = 0, msgType;
	size_t length;
	int optVal = 1;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock==-1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&optVal, sizeof(int));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");

	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	clnt_adr_sz=sizeof(clnt_adr);

	while(1)
	{
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock==-1)
			continue;
		else
			printf("\nConnected client %d \n", ++i);

		pid_t pid = fork();
		if(pid == 0)
		{
			close(serv_sock);

			while(1)
			{
				recv_len = read_msg(clnt_sock, &message, &msgType);//데이터 수신
				if(recv_len != 0){
					if(Packet_Handler(message, &sendBuf, msgType, &length) != -1) {
						write(clnt_sock, sendBuf, length); // 데이터 송신
						free(sendBuf); free(message);
						continue;
					}
				}
				break;
			}
			free(message);
			close(clnt_sock);
		}
		else if(pid < 0)
		{
			fprintf(stderr, "Fork() Failed\n");
			close(clnt_sock);
		}
		else
			close(clnt_sock);
	
	}

	close(serv_sock);
	return 0;
}

int read_msg(int sock, unsigned char **msgBuf, int *msgType)
{
	size_t msgLength = 0;
	PACKET_HEADER *header = NULL;
	int headerLength = sizeof(PACKET_HEADER);
	unsigned char *buf = (unsigned char *)calloc(1, headerLength);

	readn(sock, buf, headerLength);

	decode_PacketHeader(buf, &header);
	msgLength = header->length;
	*msgType = header->msgType;

	*msgBuf = (unsigned char *)calloc(1, msgLength);

	readn(sock, *msgBuf, msgLength);

	free(buf);
	return msgLength;
}

ssize_t readn(int sockfd, unsigned char *buf, size_t nbytes)
{
	size_t nleft;
	ssize_t nread;
	unsigned char *ptr;

	ptr = buf;
	nleft = nbytes;

	while(nleft > 0){
		nread = read(sockfd, ptr, nleft);
		if(nread == 0)
			break;
		ptr += nread;
		nleft -= nread;
	}
	return (nbytes - nleft);
}

ssize_t writen(int sockfd, unsigned char *buf, size_t nbytes)
{
	size_t nleft;
	ssize_t nwritten;
	unsigned char *ptr;

	ptr = buf;
	nleft = nbytes;

	while(nleft > 0){
		nwritten = write(sockfd, ptr, nleft);
		if(nwritten == 0)
			break;
		ptr += nwritten;
		nleft -= nwritten;
	}
	return (nbytes - nleft);
}



void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
