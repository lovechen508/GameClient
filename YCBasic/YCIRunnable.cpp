#include "YCIRunnable.h"

YCIRunnable::YCIRunnable(void)
	:myStopped(false)
{
}


YCIRunnable::~YCIRunnable(void)
{
}

//
// 函数：stop()
//
// 目的：中断一个线程的执行
//
void YCIRunnable::stop()
{
	myStopped = true;
}

//
// 函数：isInterrupted()
//
// 目的：检查线程是否被中断
//
bool YCIRunnable::isStopped()
{
	return myStopped;
}

