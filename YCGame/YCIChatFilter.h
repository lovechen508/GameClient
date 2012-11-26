#ifndef _INCLUDE_YCICHATFILTER_H_
#define _INCLUDE_YCICHATFILTER_H_

struct NET_CHAT_MSG_NTF_T;

/*
 * YCIChatFilter : 聊天过滤器接口类
 * 
 * 说明：在接收到消息时判断消息是否需要过滤
 */
class YCAPI YCIChatFilter
{
public:

	YCIChatFilter(unsigned int id, YCIChatFilter *next);
	
	virtual ~YCIChatFilter(void);

	//
	// 函数：id()
	//
	// 目的：取得过滤器Id
	//
	unsigned int id() const;

	//
	// 函数：setNext(YCIChatFilter* next)
	//
	// 目的：重新设置后继过滤器，返回旧
	//
	YCIChatFilter* setNext(YCIChatFilter* next);

	//
	// 函数：setNext(YCIChatFilter* next)
	//
	// 目的：获取设置后继过滤器
	//
	YCIChatFilter* getNext();

	//
	// 函数：validate(const NET_CHAT_MSG_NTF_T* msg)
	//
	// 目的：校验消息是否合法，合法返回true
	//
	bool validate(const NET_CHAT_MSG_NTF_T* msg);

private:

	//
	// 函数：selfValidate(const NET_CHAT_MSG_T* msg)
	//
	// 目的：自身校验消息是否合法，合法返回true
	//
	virtual bool selfValidate(const NET_CHAT_MSG_NTF_T* msg) = 0;

private:

	//
	// 过滤器Id
	//
	unsigned int myId;

	//
	// 后继过滤器
	//
	YCIChatFilter *myNext;
};

#endif

