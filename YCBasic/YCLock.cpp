#include "YCLock.h"


YCLock::YCLock(void)
{
	InitializeCriticalSection(&myMutex);
}


YCLock::~YCLock(void)
{
	DeleteCriticalSection(&myMutex);
}

//
// 函数：acquire()
//
// 目的：加锁函数
//
void YCLock::acquire()
{
	EnterCriticalSection(&myMutex);
}

//
// 函数：release()
//
// 目的：解锁函数
//
void YCLock::release()
{
	LeaveCriticalSection(&myMutex);
}