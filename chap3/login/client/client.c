#include "codec.h"
#include "packet.h"
#include "packet_controller.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int read_msg(int sock, unsigned char **msgBuf, PACKET_HEADER **header);
ssize_t readn(int sockfd, unsigned char *buf, size_t nbytes);
ssize_t writen(int sockfd, unsigned char *buf, size_t nbytes);
void error_handling(char *message);


int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_adr;
	int length, recv_len = 0;
	unsigned char *message = NULL, *sendBuf = NULL;
	char id[MAX_ID_LEN] = {0}, password[MAX_PWD_LEN] = {0};

	PACKET_HEADER *header;
	LOGIN_REQ *loginReq;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");

	while(1)
	{
		fputs("Input ID(Q to quit): ", stdout);
		fgets(id, MAX_ID_LEN, stdin);
		fflush(stdin);

		id[strlen(id)-1] = '\0';

		if(!strcmp(id,"q\n") || !strcmp(id,"Q\n"))
			exit(0);

		fputs("Input Password: ", stdout);
		fgets(password, MAX_PWD_LEN, stdin);
		fflush(stdin);

		password[strlen(password)-1] = '\0';

		break;
	}

	//Generating Login Request Message
	loginReq = (LOGIN_REQ *)calloc(1, sizeof(LOGIN_REQ));
	strcpy(loginReq->id, id);
	strcpy(loginReq->passwd, password);
	//Encoding Packet
	length = encode_packet(MT_LOGIN_REQ, (void *)loginReq, &sendBuf);
	//Sending Login Request Packet
	length = writen(sock, sendBuf, length);
	printf("\nClient Sent %d bytes\n", length);
	free(sendBuf); sendBuf = NULL;

	//Receiving Login Ack packet
	recv_len = read_msg(sock, &message, &header);
	Packet_Handler(message,&sendBuf, header->msgType, &length);
	
	free(header);
	free(message);
	close(sock);
	return 0;
}

int read_msg(int sock, unsigned char **msgBuf, PACKET_HEADER **header)
{
	size_t msgLength = 0;

	int headerLength = sizeof(PACKET_HEADER);
	unsigned char *buf = (unsigned char *)calloc(1, headerLength);

	readn(sock, buf, headerLength);

	decode_PacketHeader(buf, header);
	msgLength = (*header)->length;

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


