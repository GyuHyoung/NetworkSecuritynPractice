#ifndef PACKET_FACTORY_H_
#define PACKET_FACTORY_H_
#include <stddef.h>

int Packet_Handler(unsigned char *src, unsigned char **dst, int msgType, size_t* dst_len);
#endif /* PACKET_FACTORY_H_ */
