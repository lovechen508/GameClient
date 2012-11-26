#ifndef _INCLUDE_YCILOCK_H_
#define _INCLUDE_YCILOCK_H_

class YCAPI YCILock
{
public:
	YCILock(void);

	virtual ~YCILock(void);

	//
	// 函数：acquire()
	//
	// 目的：加锁函数
	//
	virtual void acquire() = 0;

	//
	// 函数：release()
	//
	// 目的：解锁函数
	//
	virtual void release() = 0;
};

#endif

