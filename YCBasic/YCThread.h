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
	// ������start()
	//
	// Ŀ�ģ������߳�
	//
	void start(YC_THREAD_PRIORITY priority);

	//
	// ������join()
	//
	// Ŀ�ģ�����֪ͨ���ȴ��߳̽���
	//
	void join();

private:

	YCThreadImpl *myImpl;
};

#endif

