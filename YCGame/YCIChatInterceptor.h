#ifndef _INCLUDE_YCICHATINTERCEPTOR_H_
#define _INCLUDE_YCICHATINTERCEPTOR_H_

class YCChatMessage;

/*
 * YCIChatInterceptor : 消息拦截器
 *
 * 说明：在消息发送到服务器前，拦截器有机会
 *       处理消息，甚至取消消息
 */
class YCIChatInterceptor
{
public:

	YCIChatInterceptor(unsigned int id, YCIChatInterceptor *next);

	virtual ~YCIChatInterceptor(void);

	//
	// 函数：id()
	//
	// 目的：取得拦截器Id
	//
	unsigned int id() const;

	//
	// 函数：setNext(YCIChatInterceptor* next)
	//
	// 目的：重新设置后继过滤器，返回旧
	//
	YCIChatInterceptor* setNext(YCIChatInterceptor* next);

	//
	// 函数：setNext(YCIChatInterceptor* next)
	//
	// 目的：获取设置后继过滤器
	//
	YCIChatInterceptor* getNext();

	//
	// 函数：intercept(const YCChatMessage* msg)
	//
	// 目的：检查消息是否拦截, 被拦截返回true
	//
	bool intercept(const YCChatMessage* msg);

private:

	//
	// 函数：selfIntercept(const YCChatMessage* msg)
	//
	// 目的：自身检查消息是否拦截, 被拦截返回true
	//
	virtual bool selfIntercept(const YCChatMessage* msg) = 0;

private:

	//
	// 过滤器Id
	//
	unsigned int myId;

	//
	// 后继过滤器
	//
	YCIChatInterceptor *myNext;
};

#endif