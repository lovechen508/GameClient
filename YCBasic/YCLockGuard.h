#ifndef _INCLUDE_YCLOCKGUARD_H_
#define _INCLUDE_YCLOCKGUARD_H_

class YCILock;

/*
 * YCLockGuard: YCILock加解锁守卫器
 *
 * 注释：在构造函数中加锁
 *       在析构函数中解锁
 */
class YCAPI YCLockGuard
{
public:

	//
	// 函数：YCLockGuard(YCILock* lock)
	//
	// 目的：加锁
	//
	YCLockGuard(YCILock* lock);

	//
	// 函数：~YCLockGuard(void)
	//
	// 目的：解锁
	//
	~YCLockGuard(void);

private:

	YCILock *myLock;
};

#endif

