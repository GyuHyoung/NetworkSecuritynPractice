
#include "packet_controller.h"
#include "packet.h"
#include "codec.h"
#include <string.h>

int Packet_Handler(unsigned char *src, unsigned char **dst, int msgType, int *dst_len)
{
	LOGIN_ACK *loginAck;
	unsigned char *imgBuf;
	FILE *fp;
	int rv;

	switch(msgType){

	case MT_LOGIN_ACK:
		printf("\nClient received LOGIN_ACK packet.\n");
		decode_LoginAck(src, &loginAck);
		printf("\nLogin Result Msg: %s\n", loginAck->res_msg);
		free(loginAck);
		rv = 0;
		break;
	default:
		rv = -1;
		break;
	}
	return rv;
}
