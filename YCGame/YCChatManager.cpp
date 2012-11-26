#include "YCChatManager.h"

#include <map>
#include <string>

#include "YCIChatFilter.h"
#include "YCChatMessage.h"
#include "YCChatMsgDefine.h"
#include "YCIChatInterceptor.h"

#include "YCBasic\YCQueue.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"

#include "YCConfig\YCConfig.h"
#include "YCConfig\YCXmlReader.h"
#include "YCConfig\YCXmlWriter.h"

//
// 全频道
//
const int YCChatManager::CH_FILTER_FULL = (CH_LIGHT|CH_PRIVATE|CH_FRIEND|CH_TEAM|CH_FAMILY|CH_COUNTRY|CH_WORLD|CH_SYSTEM|CH_GLOBAL);

//
// 系统
//
const int YCChatManager::CH_FILTER_SYSTEM = CH_SYSTEM;

YCChatManager::YCChatManager(void)
	: myFilters(NULL)
	, myInterceptors(NULL)
	, myCustomizeChannels(CH_FILTER_FULL)
	, myBlackList(NULL)
	, myMessages(NULL)
	, myGlobalMessages(NULL)
{
}


YCChatManager::~YCChatManager(void)
{
}

//
// 函数：initialize(YCConfig& config)
//
// 目的：初始化消息管理器
//
bool YCChatManager::initialize(YCConfig& config)
{
	const char* chatFile = config.chatCfgPath();
	if (NULL == chatFile)
	{
		throw YCException(2002, "YCChatManager::initialize无聊天本地存储文件！");
	}

	if (strlen(chatFile) >= MAX_PATH)
	{
		throw YCException(2002, "YCChatManager::initialize聊天本地存储文件路径过长！");
	}

	memset(myChatConfigFile, 0, MAX_PATH);
	char theExePath[MAX_PATH] = {0};
	if (!YCFileUtil::GetExePath(theExePath, MAX_PATH))
	{
		throw YCException(2002, "YCChatManager::initialize无法取得可执行文件路径！");
	}
	sprintf(myChatConfigFile, "%s\\..\\%s", theExePath, chatFile);
	LOG_DEBUG("聊天本地存储文件路径：" << myChatConfigFile);

	if (!YCFileUtil::IsFileExist(myChatConfigFile))
	{
		YCXmlWriter writer(myChatConfigFile);
		writer.enter("chat", NULL).flush();
	}

	return true;
}

//
// 函数：setCustomChannel(int channels)
//
// 目的：设置自定义频道
//
void YCChatManager::setCustomChannel(int channels)
{

}

//
// 函数：addFilter(YCIChatFilter* filter)
//
// 目的：添加过滤器, 如果有同名的则替换并返回
//
void YCChatManager::addFilter(YCIChatFilter* filter)
{
	if (filter == NULL)
	{
		throw YCException(2002, "YCChatManager::addFilter添加空过滤器");
	}

	for (YCIChatFilter* curr = myFilters; 
		curr != NULL; 
		curr = curr->getNext())
	{
		if (filter->id() == curr->id())
		{
			throw YCException(2002, "YCChatManager::addFilter过滤器重复");
		}
	}

	filter->setNext(myFilters);
	myFilters = filter;
}

//
// 函数：removeFilter(unsigned int id)
//
// 目的：移除过滤器
//
YCIChatFilter * YCChatManager::removeFilter(unsigned int id)
{
	YCIChatFilter* result = NULL;

	if (myFilters != NULL)
	{
		if (myFilters->id() == id)
		{
			result = myFilters;
			myFilters = myFilters->getNext();
			result->setNext(NULL);
		}
		else
		{
			YCIChatFilter* prev = myFilters;
			for (YCIChatFilter* curr = myFilters->getNext(); 
				(curr != NULL) && (result == NULL); 
				prev = curr, curr = curr->getNext())
			{
				if (curr->id() == id)
				{
					result = curr;
					prev->setNext(curr->getNext());
					result->setNext(NULL);
				}
			}
		}
	}

	return result;
}

//
// 函数：addInterceptor(YCIChatInterceptor* interceptor)
//
// 目的：添加拦截器, 如果有同名的则替换并返回
//
void YCChatManager::addInterceptor(YCIChatInterceptor* interceptor)
{
	if (interceptor == NULL)
	{
		throw YCException(2002, "YCChatManager::addInterceptor添加空拦截器");
	}

	for (YCIChatInterceptor* curr = myInterceptors; 
		curr != NULL; 
		curr = curr->getNext())
	{
		if (interceptor->id() == curr->id())
		{
			throw YCException(2002, "YCChatManager::addInterceptor过滤器重复");
		}
	}

	interceptor->setNext(myInterceptors);
	myInterceptors = interceptor;
}

//
// 函数：removeInterceptor(unsigned int id)
//
// 目的：移除拦截器
//
YCIChatInterceptor * YCChatManager::removeInterceptor(unsigned int id)
{
	YCIChatInterceptor* result = NULL;	

	if (myInterceptors != NULL)
	{
		if (myInterceptors->id() == id)
		{
			result = myInterceptors;
			myInterceptors = myInterceptors->getNext();
			result->setNext(NULL);
		}
		else
		{
			YCIChatInterceptor* prev = myInterceptors;
			for (YCIChatInterceptor* curr = myInterceptors->getNext(); 
				(curr != NULL) && (result == NULL); 
				prev = curr, curr = curr->getNext())
			{
				if (curr->id() == id)
				{
					result = curr;
					prev->setNext(curr->getNext());
					result->setNext(NULL);
				}
			}
		}
	}
	
	return result;
}

//
// 函数：send(const YCChatMessage* msg)
//
// 目的：发送消息
//
void YCChatManager::send(const YCChatMessage* msg)
{
	if ((myInterceptors != NULL) && (myInterceptors->intercept(msg)))
	{
		LOG_DEBUG("YCChatManager::send消息被拦截");
	}

	// TODO ... send to server
}

//
// 函数：recv(const NET_CHAT_MSG_NTF_T* msg)
//
// 目的：消息到达
//
void YCChatManager::recv(const NET_CHAT_MSG_NTF_T* msg)
{
	if ((myFilters == NULL) || (myFilters->validate(msg)))
	{
		
	}
}

//
// 函数：addBlackRole(const char* role)
//
// 目的：添加角色到黑名单
//
void YCChatManager::addBlackRole(const char* role)
{
	
}

//
// 函数：removeBlackRole(const char* role)
//
// 目的：将role移除黑名单
//       如role为NULL，清空黑名单
//
void YCChatManager::removeBlackRole(const char* role)
{

}

//
// 函数：finalize()
//
// 目的：释放资源
//
void YCChatManager::finalize()
{
	//
	// 删除拦截器
	//
	SAFE_DELETE(myInterceptors);

	//
	// 删除过滤器
	//
	SAFE_DELETE(myFilters);

	//
	// 清除缓存消息
	//
	if (NULL != myMessages)
	{
		myMessages->clear(&YCDListClearHelper<YCChatMessage>);
		SAFE_DELETE(myMessages);
	}

	//
	// 清除彩世消息
	//
	if (NULL != myGlobalMessages)
	{
		myGlobalMessages->clear(&YCDListClearHelper<YCChatMessage>);
		SAFE_DELETE(myGlobalMessages);
	}

	//
	// 清除黑名单
	//
	if (NULL != myBlackList)
	{
		myBlackList->clear(&YCDListClearHelper<char>);
		SAFE_DELETE(myBlackList);
	}
}

////////////////////////////////////////////////////////////
// Private Part
////////////////////////////////////////////////////////////

