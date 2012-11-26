#ifndef _INCLUDE_YCICHATINTERCEPTOR_H_
#define _INCLUDE_YCICHATINTERCEPTOR_H_

class YCChatMessage;

/*
 * YCIChatInterceptor : ��Ϣ������
 *
 * ˵��������Ϣ���͵�������ǰ���������л���
 *       ������Ϣ������ȡ����Ϣ
 */
class YCIChatInterceptor
{
public:

	YCIChatInterceptor(unsigned int id, YCIChatInterceptor *next);

	virtual ~YCIChatInterceptor(void);

	//
	// ������id()
	//
	// Ŀ�ģ�ȡ��������Id
	//
	unsigned int id() const;

	//
	// ������setNext(YCIChatInterceptor* next)
	//
	// Ŀ�ģ��������ú�̹����������ؾ�
	//
	YCIChatInterceptor* setNext(YCIChatInterceptor* next);

	//
	// ������setNext(YCIChatInterceptor* next)
	//
	// Ŀ�ģ���ȡ���ú�̹�����
	//
	YCIChatInterceptor* getNext();

	//
	// ������intercept(const YCChatMessage* msg)
	//
	// Ŀ�ģ������Ϣ�Ƿ�����, �����ط���true
	//
	bool intercept(const YCChatMessage* msg);

private:

	//
	// ������selfIntercept(const YCChatMessage* msg)
	//
	// Ŀ�ģ���������Ϣ�Ƿ�����, �����ط���true
	//
	virtual bool selfIntercept(const YCChatMessage* msg) = 0;

private:

	//
	// ������Id
	//
	unsigned int myId;

	//
	// ��̹�����
	//
	YCIChatInterceptor *myNext;
};

#endif