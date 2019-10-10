
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include "packet.h"


#ifndef _HEADER_H_

#define _HEADER_H_

int encode_PacketHeader(PACKET_HEADER *src, unsigned char **dst);
int decode_PacketHeader(unsigned char *src, PACKET_HEADER **dst);

int encode_LoginReq(LOGIN_REQ *src, unsigned char **dst);
int decode_LoginReq(unsigned char *src, LOGIN_REQ **dst);

int encode_LoginAck(LOGIN_ACK *src, unsigned char **dst);
int decode_LoginAck(unsigned char *src, LOGIN_ACK **dst);

int encode_packet(int msgType, void *msg, unsigned char **dst);

#endif
