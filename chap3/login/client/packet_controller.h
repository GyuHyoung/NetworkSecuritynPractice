
#ifndef _PACKET_CONTROLLER_H_
#define _PACKET_CONTROLLER_H_
#include <stddef.h>

int Packet_Handler(unsigned char *src, unsigned char **dst, int msgType, int *dst_len);
#endif /* PACKET_FACTORY_H_ */
