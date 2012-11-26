#ifndef _INCLUDE_YCMSGDEFINE_H_
#define _INCLUDE_YCMSGDEFINE_H_

#include "YCIPackageRequest.h"
#include "YCIPackageResponse.h"

#define MAX_PROTOCOL 8192

//
// ���ͽ��ջ���������
//
#define BUFFER_SIZE 10240

typedef bool (*ENCODE)(YCIPackageRequest*, char*, unsigned int);
typedef bool (*DECODE)(YCIPackageResponse*, char*, unsigned int);

#endif