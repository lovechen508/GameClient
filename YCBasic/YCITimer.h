#ifndef _INCLUDE_YCITIMER_H_
#define _INCLUDE_YCITIMER_H_

class YCDList;

/*
 * YCITimer : 定时器接口类
 */
class YCAPI YCITimer
{
public:

	/*
	 * YCITimerCallback : 定时器回调接口类
	 */
	class YCITimerCallback 
	{
	public:

		YCITimerCallback() {}

		virtual ~YCITimerCallback() {}

		//
		// 函数：onTimer()
		//
		// 目的：定时器回调函数
		//
		// 注释：msgId回调的消息号
		//      times 回调的次数
		//
		virtual void onTimer(unsigned int msgId, int times) = 0;
	};

public:

	//
	// 函数：start()
	//
	// 目的：启动定时器
	//
	virtual bool start() = 0;
	
	//
	// 函数：registCallback(unsigned int msgId, 
	//	                unsigned int intervalInMillisecond, 
	//					YCITimerCallback* callback,
	//					int times);
	//
	// 目的：注册定时器回调
	//
	// 注释：msgId和callback 两者唯一确定了一个注册
	//      intervalInMillisecond 回调时间间隔，必须为10ms的倍数
	//      times 回调的次数
	//
	void registCallback(unsigned int msgId, 
		                YCITimerCallback* callback, 
						unsigned int intervalInMillisecond);

	//
	// 函数：unregistCallback(unsigned int msgId, YCITimerCallback* callback)
	//
	// 目的：移除定时器回调
	//
	void unregistCallback(unsigned int msgId, YCITimerCallback* callback);

	//
	// 函数：stop()
	//
	// 目的：启动定时器
	//
	virtual void stop() = 0;

	//
	// 函数：trigger(unsigned int now)
	//
	// 目的：继承实现类必须定时10ms间隔调用此函数
	//
	void trigger(unsigned int now);

protected:

	YCITimer();

	virtual ~YCITimer();
	
private:

	bool myIsTriggering;

	YCDList * myCallbacks;
};

#endif