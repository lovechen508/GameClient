#ifndef _INCLUDE_YCRWLOCK_H_
#define _INCLUDE_YCRWLOCK_H_

/*
 * YCRWLock : 读写锁
 *
 * 注释：支持两种加锁方式
 *       读锁，写锁
 *       读锁升级为写锁必须是先释放读锁，再获取写锁
 */
class YCAPI YCRWLock
{
public:
	
	YCRWLock(void);
	
	~YCRWLock(void);

	//
	// 函数：read()
	//
	// 目的：只读加锁函数
	//
	virtual void read();

	//
	// 函数：write()
	//
	// 目的：只读加锁函数
	//
	virtual void write();

	//
	// 函数：release()
	//
	// 目的：解锁函数
	//
	virtual void release();
};

#endif

