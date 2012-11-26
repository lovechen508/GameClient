#ifndef _INCLUDE_YCITIMER_H_
#define _INCLUDE_YCITIMER_H_

class YCDList;

/*
 * YCITimer : ��ʱ���ӿ���
 */
class YCAPI YCITimer
{
public:

	/*
	 * YCITimerCallback : ��ʱ���ص��ӿ���
	 */
	class YCITimerCallback 
	{
	public:

		YCITimerCallback() {}

		virtual ~YCITimerCallback() {}

		//
		// ������onTimer()
		//
		// Ŀ�ģ���ʱ���ص�����
		//
		// ע�ͣ�msgId�ص�����Ϣ��
		//      times �ص��Ĵ���
		//
		virtual void onTimer(unsigned int msgId, int times) = 0;
	};

public:

	//
	// ������start()
	//
	// Ŀ�ģ�������ʱ��
	//
	virtual bool start() = 0;
	
	//
	// ������registCallback(unsigned int msgId, 
	//	                unsigned int intervalInMillisecond, 
	//					YCITimerCallback* callback,
	//					int times);
	//
	// Ŀ�ģ�ע�ᶨʱ���ص�
	//
	// ע�ͣ�msgId��callback ����Ψһȷ����һ��ע��
	//      intervalInMillisecond �ص�ʱ����������Ϊ10ms�ı���
	//      times �ص��Ĵ���
	//
	void registCallback(unsigned int msgId, 
		                YCITimerCallback* callback, 
						unsigned int intervalInMillisecond);

	//
	// ������unregistCallback(unsigned int msgId, YCITimerCallback* callback)
	//
	// Ŀ�ģ��Ƴ���ʱ���ص�
	//
	void unregistCallback(unsigned int msgId, YCITimerCallback* callback);

	//
	// ������stop()
	//
	// Ŀ�ģ�������ʱ��
	//
	virtual void stop() = 0;

	//
	// ������trigger(unsigned int now)
	//
	// Ŀ�ģ��̳�ʵ������붨ʱ10ms������ô˺���
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