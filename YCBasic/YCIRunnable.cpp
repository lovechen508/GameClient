#include "YCIRunnable.h"

YCIRunnable::YCIRunnable(void)
	:myStopped(false)
{
}


YCIRunnable::~YCIRunnable(void)
{
}

//
// ������stop()
//
// Ŀ�ģ��ж�һ���̵߳�ִ��
//
void YCIRunnable::stop()
{
	myStopped = true;
}

//
// ������isInterrupted()
//
// Ŀ�ģ�����߳��Ƿ��ж�
//
bool YCIRunnable::isStopped()
{
	return myStopped;
}

