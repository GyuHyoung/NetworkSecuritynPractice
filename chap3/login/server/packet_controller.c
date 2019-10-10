
#include "packet_controller.h"
#include "packet.h"
#include "codec.h"
#include <string.h>

static const char *ID = "csec";
static const char *PWD = "1q2w3e4r";

int Packet_Handler(unsigned char *src, unsigned char **dst, int msgType, size_t* dst_len)
{
	LOGIN_REQ *loginReq;
	LOGIN_ACK *loginAck;

	char *id ,*passwd;
	int rv;

	switch(msgType){
	case MT_LOGIN_REQ:
		printf("\nLogin Request received\n");
		decode_LoginReq(src, &loginReq); 	//Decoding Message
		id = loginReq->id;
		passwd = loginReq->passwd;
		//Checking ID/PWD and Generating Packet Message
		if(strcmp(id, ID) != 0 || strcmp(passwd, PWD) != 0)
		{
			char *res_msg = "Login Fail!";
			loginAck = (LOGIN_ACK *)calloc(1, sizeof(LOGIN_ACK));
			loginAck->result = R_FAIL;
			strncpy(loginAck->res_msg, res_msg, strlen(res_msg));
		}
		else
		{
			char *res_msg = "Login Success!";
			loginAck = (LOGIN_ACK *)calloc(1, sizeof(LOGIN_ACK));
			loginAck->result = R_SUCCESS;
			strncpy(loginAck->res_msg, res_msg, strlen(res_msg));
		}
		//Encoding Packet
		*dst_len = encode_packet(MT_LOGIN_ACK, (void *)loginAck, dst);
		printf("Sending... Login Ack Message to client\n");
		free(loginReq); free(loginAck);
		rv = 0;
		break;

	default:
		rv = -1;
		break;
	}

	return rv;
}
