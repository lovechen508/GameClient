#ifndef _INCLUDE_YCMSGDEFINE_H_
#define _INCLUDE_YCMSGDEFINE_H_

#include "YCDataHolder.h"

#include "YCIPackageRequest.h"
#include "YCIPackageResponse.h"

#define MAX_PROTOCOL 8192

//
// ���ͽ��ջ���������
//
#define BUFFER_SIZE 10240

typedef bool (*ENCODE)(YCIPackageRequest*, YCDataHolder*);
typedef bool (*DECODE)(YCIPackageResponse*, YCDataHolder*);

#endif