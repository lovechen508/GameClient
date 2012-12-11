#ifndef _INCLUDE_YCMSGDEFINE_H_
#define _INCLUDE_YCMSGDEFINE_H_

#include "YCDataHolder.h"

#define MAX_PROTOCOL 8192

//
// 发送接收缓冲区长度
//
#define BUFFER_SIZE 10240

typedef int (*ENCODE)(void*, YCDataHolder*);
typedef int (*DECODE)(void*, YCDataHolder*);

#endif