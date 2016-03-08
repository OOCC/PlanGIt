#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "type_def.h"

/* SOCKET通信：客户端与服务器传输的数据包结构体 */
typedef struct package
{
	UCHAR index;
	ULONG length;
	ULONG body;
} SPKG;

#endif