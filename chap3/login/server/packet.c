#include "packet.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void printPacketInformation(PACKET_HEADER *head, void *msg)
{
	printf("Msgtype Value  : 0x%x\n", head->msgType);
	printf("Datalength     : %d bytes\n", head->length);

	switch(head->msgType){
	case MT_LOGIN_REQ:
		printf("ID : %s\n", ((LOGIN_REQ *)msg)->id);
		printf("PASSWORD : %s\n", ((LOGIN_REQ *)msg)->passwd);
		break;
	case MT_LOGIN_ACK:
		printf("LOGIN RESULT : 0x%x\n", ((LOGIN_ACK *)msg)->result);
		printf("RESULT MSG : %s\n", ((LOGIN_ACK *)msg)->res_msg);
		break;
	default:
		printf("Packet printing error!\n");
	}
	return;
}
