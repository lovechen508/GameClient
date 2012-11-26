#ifndef _INCLUDE_YCCONDITION_H_
#define _INCLUDE_YCCONDITION_H_

#include "YCDef.h"

class YCILock;

/*
 * YCCondition : 条件变量抽象类
 *
 * 注释：
 */
class YCAPI YCCondition
{
public:
	
	YCCondition(void);
	
	~YCCondition(void);

	//
	// 函数：wait(unsigned int dwMilliseconds)
	//
	// 目的：释放锁，等待notify，返回前重新获得锁
	//
	void wait(unsigned int dwMilliseconds = INFINITE);

	//
	// 函数：wait(YCILock* lock, unsigned int dwMilliseconds)
	//
	// 目的：释放锁，等待notify，返回前重新获得锁
	//
	void wait(YCILock* lock, unsigned int dwMilliseconds = INFINITE);

	//
	// 函数：notify()
	//
	// 目的：通知所有线程
	//
	void notify();

private:

	HANDLE myEvent;

};

#endif

