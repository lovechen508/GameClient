#ifndef _INCLUDE_YCIRUNNABLE_H_
#define _INCLUDE_YCIRUNNABLE_H_

class YCAPI YCIRunnable
{
public:
	
	YCIRunnable(void);
	
	virtual ~YCIRunnable(void);

	// 
	// ������run()
	//
	// Ŀ�ģ��߳�ʵ�ֺ���
	//
	// ʵ��ģ�壺
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
	// ������stop()
	//
	// Ŀ�ģ��ж�һ���̵߳�ִ��
	//
	void stop();

protected:

	//
	// ������isStopped()
	//
	// Ŀ�ģ�����߳��Ƿ��ж�
	//
	bool isStopped();

private:

	volatile bool myStopped;
};

#endif

