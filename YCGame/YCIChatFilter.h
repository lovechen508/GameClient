#ifndef _INCLUDE_YCICHATFILTER_H_
#define _INCLUDE_YCICHATFILTER_H_

struct NET_CHAT_MSG_NTF_T;

/*
 * YCIChatFilter : ����������ӿ���
 * 
 * ˵�����ڽ��յ���Ϣʱ�ж���Ϣ�Ƿ���Ҫ����
 */
class YCAPI YCIChatFilter
{
public:

	YCIChatFilter(unsigned int id, YCIChatFilter *next);
	
	virtual ~YCIChatFilter(void);

	//
	// ������id()
	//
	// Ŀ�ģ�ȡ�ù�����Id
	//
	unsigned int id() const;

	//
	// ������setNext(YCIChatFilter* next)
	//
	// Ŀ�ģ��������ú�̹����������ؾ�
	//
	YCIChatFilter* setNext(YCIChatFilter* next);

	//
	// ������setNext(YCIChatFilter* next)
	//
	// Ŀ�ģ���ȡ���ú�̹�����
	//
	YCIChatFilter* getNext();

	//
	// ������validate(const NET_CHAT_MSG_NTF_T* msg)
	//
	// Ŀ�ģ�У����Ϣ�Ƿ�Ϸ����Ϸ�����true
	//
	bool validate(const NET_CHAT_MSG_NTF_T* msg);

private:

	//
	// ������selfValidate(const NET_CHAT_MSG_T* msg)
	//
	// Ŀ�ģ�����У����Ϣ�Ƿ�Ϸ����Ϸ�����true
	//
	virtual bool selfValidate(const NET_CHAT_MSG_NTF_T* msg) = 0;

private:

	//
	// ������Id
	//
	unsigned int myId;

	//
	// ��̹�����
	//
	YCIChatFilter *myNext;
};

#endif

