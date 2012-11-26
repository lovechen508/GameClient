#ifndef _INCLUDE_YCCHATMANAGER_H_
#define _INCLUDE_YCCHATMANAGER_H_

#include "YCChatMsgDefine.h"

#include "YCGraphic/YCIModel.h"

#include "YCBasic\YCDef.h"

class YCQueue;
class YCDList;
class YCConfig;
class YCChatMessage;
class YCIChatFilter;
class YCIChatInterceptor;

struct NET_CHAT_MSG_NTF_T;

/*
 * YCChatManager : 聊天管理器
 *
 *
 *             --------------------   No    --------------------   No    --------------------                    ------------
 * message -> | YCIChatInterceptor | ----> | YCIChatInterceptor | ----> | YCIChatInterceptor |----------------->|            |
 *             --------------------		    --------------------  	     --------------------					|            |
 *																												|            |
 *																												|   Server   |
 *			   --------------------   Yes   --------------------   Yes   --------------------					|            |
 *		  <-  |   YCIChatFilter    | <---- |   YCIChatFilter    | <---- |   YCIChatFilter    |<-----------------|            |
 *			   --------------------		    --------------------  	     --------------------					 ------------
 *    
 */
class YCAPI YCChatManager : public YCIModel
{
public:
	
	//
	// 全频道
	//
	static const int CH_FILTER_FULL;

	//
	// 系统
	//
	static const int CH_FILTER_SYSTEM;

public:

	YCChatManager(void);

	~YCChatManager(void);

	//
	// 函数：initialize(YCConfig& config)
	//
	// 目的：初始化消息管理器
	//
	bool initialize(YCConfig& config);

	//
	// 函数：setCustomChannel(int channels)
	//
	// 目的：设置自定义频道
	//
	void setCustomChannel(int channels);

	//
	// 函数：addFilter(YCIChatFilter* filter)
	//
	// 目的：添加过滤器, 如果有同Id的则抛出异常
	//
	void addFilter(YCIChatFilter* filter);

	//
	// 函数：removeFilter(unsigned int id)
	//
	// 目的：移除过滤器
	//
	YCIChatFilter * removeFilter(unsigned int id);

	//
	// 函数：addInterceptor(YCIChatInterceptor* interceptor)
	//
	// 目的：添加拦截器, 如果有同Id的则抛出异常
	//
	void addInterceptor(YCIChatInterceptor* interceptor);

	//
	// 函数：removeInterceptor(unsigned int id)
	//
	// 目的：移除过滤器
	//
	YCIChatInterceptor * removeInterceptor(unsigned int id);

	//
	// 函数：send(const YCChatMessage* msg)
	//
	// 目的：发送消息
	//
	void send(const YCChatMessage* msg);

	//
	// 函数：recv(const NET_CHAT_MSG_T* msg)
	//
	// 目的：消息到达
	//
	void recv(const NET_CHAT_MSG_NTF_T* msg);

	//
	// 函数：addBlackRole(const char* role)
	//
	// 目的：添加角色到黑名单
	//
	void addBlackRole(const char* role);

	//
	// 函数：removeBlackRole(const char* role)
	//
	// 目的：将role移除黑名单
	//       如role为NULL，清空黑名单
	//
	void removeBlackRole(const char* role);

	//
	// 函数：finalize()
	//
	// 目的：释放资源
	//
	void finalize();

private:

	//
	// 本地存储文件名
	//
	char myChatConfigFile[MAX_PATH];

	//
	// 最大消息数量
	//
	int myMaxMessage;

	//
	// 缓存的消息对列
	//
	YCQueue * myMessages;

	//
	// 彩世消息
	//
	YCQueue * myGlobalMessages;

	//
	// 自定义频道
	//
	int myCustomizeChannels;

	//
	// 拦截器链
	//
	YCIChatInterceptor *myInterceptors;

	//
	// 过滤器链
	//
	YCIChatFilter* myFilters;

	//
	// 黑名单
	//
	YCDList * myBlackList;

};

#endif

