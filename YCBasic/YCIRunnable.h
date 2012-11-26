#ifndef _INCLUDE_YCIRUNNABLE_H_
#define _INCLUDE_YCIRUNNABLE_H_

class YCAPI YCIRunnable
{
public:
	
	YCIRunnable(void);
	
	virtual ~YCIRunnable(void);

	// 
	// 函数：run()
	//
	// 目的：线程实现函数
	//
	// 实现模板：
	//           int run()
	//           {
	//				 while (!isInterrupted())
	//				 {
	//					 try {
	//						 //do stuff ...
	//					 } catch (...) {
	//						 // handle your exception
	//					 }
	//				 }
	//			 }
	//
	virtual int run() = 0;

	//
	// 函数：stop()
	//
	// 目的：中断一个线程的执行
	//
	void stop();

protected:

	//
	// 函数：isStopped()
	//
	// 目的：检查线程是否被中断
	//
	bool isStopped();

private:

	volatile bool myStopped;
};

#endif

