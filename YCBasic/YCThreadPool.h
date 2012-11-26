#ifndef _INCLUDE_YCTHREADPOOL_H_
#define _INCLUDE_YCTHREADPOOL_H_

class YCThreadPoolImpl;

/*
 * YCThreadPool : 线程池缺省实现类
 *                可以继承以实现自己定制的线程池
 */
class YCAPI YCThreadPool
{
public:

	/*
	 * WorkItem : 线程池工作项
	 *            所有的工作项必须从此继承
	 */
	class WorkItem {

	public:

		WorkItem() {}
		virtual ~WorkItem() {}

		//
		// 函数：void run();
		//
		// 目的：完成每个工作项的实际内容
		//
		virtual void run() = 0;
	};

public:

	//
	// 函数：YCThreadPool(int noOfThreads, int maxItems)
	//
	// 目的：构造函数，创建线程池
	//
	// 参数：
	//      noOfThreads : 池中线程数
	//      maxItems : 最大等待队列，超过此上限enqueue将阻塞
	//
	YCThreadPool(int noOfThreads, int maxItems);

	virtual ~YCThreadPool(void);

	//
	// 函数：enqueue(WorkItem* item)
	//
	// 目的：向工作队列中插入工作项
	//
	void enqueue(WorkItem* item);

	//
	// 函数：abort()
	//
	// 目的：停止线程池，放弃未完成的工作项
	//
	void abort();

	//
	// 函数：stop()
	//
	// 目的：不再接受工作项的插入，此时调用enqueue将抛出YCException
	//      完成队列中所有工作项，停止线程池
	//
	void stop();

	//
	// 函数：entries()
	//
	// 目的：返回等待队列中的工作项数量
	//
	int entries(); 

private:
	
	YCThreadPoolImpl * myThreadPool;
};

#endif
