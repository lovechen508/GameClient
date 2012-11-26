#ifndef _INCLUDE_YCTHREAD_H_
#define _INCLUDE_YCTHREAD_H_

class YCIRunnable;
class YCThreadImpl;

class YCAPI YCThread
{
public:

	typedef enum YC_THREAD_PRIORITY{LOW, NORMAL, HIGH};
	
	YCThread(YCIRunnable* runnable);

	~YCThread(void);

	//
	// 函数：start()
	//
	// 目的：启动线程
	//
	void start(YC_THREAD_PRIORITY priority);

	//
	// 函数：join()
	//
	// 目的：发送通知并等待线程结束
	//
	void join();

private:

	YCThreadImpl *myImpl;
};

#endif

