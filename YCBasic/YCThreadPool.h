#ifndef _INCLUDE_YCTHREADPOOL_H_
#define _INCLUDE_YCTHREADPOOL_H_

class YCThreadPoolImpl;

/*
 * YCThreadPool : �̳߳�ȱʡʵ����
 *                ���Լ̳���ʵ���Լ����Ƶ��̳߳�
 */
class YCAPI YCThreadPool
{
public:

	/*
	 * WorkItem : �̳߳ع�����
	 *            ���еĹ��������Ӵ˼̳�
	 */
	class WorkItem {

	public:

		WorkItem() {}
		virtual ~WorkItem() {}

		//
		// ������void run();
		//
		// Ŀ�ģ����ÿ���������ʵ������
		//
		virtual void run() = 0;
	};

public:

	//
	// ������YCThreadPool(int noOfThreads, int maxItems)
	//
	// Ŀ�ģ����캯���������̳߳�
	//
	// ������
	//      noOfThreads : �����߳���
	//      maxItems : ���ȴ����У�����������enqueue������
	//
	YCThreadPool(int noOfThreads, int maxItems);

	virtual ~YCThreadPool(void);

	//
	// ������enqueue(WorkItem* item)
	//
	// Ŀ�ģ����������в��빤����
	//
	void enqueue(WorkItem* item);

	//
	// ������abort()
	//
	// Ŀ�ģ�ֹͣ�̳߳أ�����δ��ɵĹ�����
	//
	void abort();

	//
	// ������stop()
	//
	// Ŀ�ģ����ٽ��ܹ�����Ĳ��룬��ʱ����enqueue���׳�YCException
	//      ��ɶ��������й����ֹͣ�̳߳�
	//
	void stop();

	//
	// ������entries()
	//
	// Ŀ�ģ����صȴ������еĹ���������
	//
	int entries(); 

private:
	
	YCThreadPoolImpl * myThreadPool;
};

#endif
