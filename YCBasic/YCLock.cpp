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
// ������acquire()
//
// Ŀ�ģ���������
//
void YCLock::acquire()
{
	EnterCriticalSection(&myMutex);
}

//
// ������release()
//
// Ŀ�ģ���������
//
void YCLock::release()
{
	LeaveCriticalSection(&myMutex);
}