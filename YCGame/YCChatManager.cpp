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
// ȫƵ��
//
const int YCChatManager::CH_FILTER_FULL = (CH_LIGHT|CH_PRIVATE|CH_FRIEND|CH_TEAM|CH_FAMILY|CH_COUNTRY|CH_WORLD|CH_SYSTEM|CH_GLOBAL);

//
// ϵͳ
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
// ������initialize(YCConfig& config)
//
// Ŀ�ģ���ʼ����Ϣ������
//
bool YCChatManager::initialize(YCConfig& config)
{
	const char* chatFile = config.chatCfgPath();
	if (NULL == chatFile)
	{
		throw YCException(2002, "YCChatManager::initialize�����챾�ش洢�ļ���");
	}

	if (strlen(chatFile) >= MAX_PATH)
	{
		throw YCException(2002, "YCChatManager::initialize���챾�ش洢�ļ�·��������");
	}

	memset(myChatConfigFile, 0, MAX_PATH);
	char theExePath[MAX_PATH] = {0};
	if (!YCFileUtil::GetExePath(theExePath, MAX_PATH))
	{
		throw YCException(2002, "YCChatManager::initialize�޷�ȡ�ÿ�ִ���ļ�·����");
	}
	sprintf(myChatConfigFile, "%s\\..\\%s", theExePath, chatFile);
	LOG_DEBUG("���챾�ش洢�ļ�·����" << myChatConfigFile);

	if (!YCFileUtil::IsFileExist(myChatConfigFile))
	{
		YCXmlWriter writer(myChatConfigFile);
		writer.enter("chat", NULL).flush();
	}

	return true;
}

//
// ������setCustomChannel(int channels)
//
// Ŀ�ģ������Զ���Ƶ��
//
void YCChatManager::setCustomChannel(int channels)
{

}

//
// ������addFilter(YCIChatFilter* filter)
//
// Ŀ�ģ���ӹ�����, �����ͬ�������滻������
//
void YCChatManager::addFilter(YCIChatFilter* filter)
{
	if (filter == NULL)
	{
		throw YCException(2002, "YCChatManager::addFilter��ӿչ�����");
	}

	for (YCIChatFilter* curr = myFilters; 
		curr != NULL; 
		curr = curr->getNext())
	{
		if (filter->id() == curr->id())
		{
			throw YCException(2002, "YCChatManager::addFilter�������ظ�");
		}
	}

	filter->setNext(myFilters);
	myFilters = filter;
}

//
// ������removeFilter(unsigned int id)
//
// Ŀ�ģ��Ƴ�������
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
// ������addInterceptor(YCIChatInterceptor* interceptor)
//
// Ŀ�ģ����������, �����ͬ�������滻������
//
void YCChatManager::addInterceptor(YCIChatInterceptor* interceptor)
{
	if (interceptor == NULL)
	{
		throw YCException(2002, "YCChatManager::addInterceptor��ӿ�������");
	}

	for (YCIChatInterceptor* curr = myInterceptors; 
		curr != NULL; 
		curr = curr->getNext())
	{
		if (interceptor->id() == curr->id())
		{
			throw YCException(2002, "YCChatManager::addInterceptor�������ظ�");
		}
	}

	interceptor->setNext(myInterceptors);
	myInterceptors = interceptor;
}

//
// ������removeInterceptor(unsigned int id)
//
// Ŀ�ģ��Ƴ�������
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
// ������send(const YCChatMessage* msg)
//
// Ŀ�ģ�������Ϣ
//
void YCChatManager::send(const YCChatMessage* msg)
{
	if ((myInterceptors != NULL) && (myInterceptors->intercept(msg)))
	{
		LOG_DEBUG("YCChatManager::send��Ϣ������");
	}

	// TODO ... send to server
}

//
// ������recv(const NET_CHAT_MSG_NTF_T* msg)
//
// Ŀ�ģ���Ϣ����
//
void YCChatManager::recv(const NET_CHAT_MSG_NTF_T* msg)
{
	if ((myFilters == NULL) || (myFilters->validate(msg)))
	{
		
	}
}

//
// ������addBlackRole(const char* role)
//
// Ŀ�ģ���ӽ�ɫ��������
//
void YCChatManager::addBlackRole(const char* role)
{
	
}

//
// ������removeBlackRole(const char* role)
//
// Ŀ�ģ���role�Ƴ�������
//       ��roleΪNULL����պ�����
//
void YCChatManager::removeBlackRole(const char* role)
{

}

//
// ������finalize()
//
// Ŀ�ģ��ͷ���Դ
//
void YCChatManager::finalize()
{
	//
	// ɾ��������
	//
	SAFE_DELETE(myInterceptors);

	//
	// ɾ��������
	//
	SAFE_DELETE(myFilters);

	//
	// ���������Ϣ
	//
	if (NULL != myMessages)
	{
		myMessages->clear(&YCDListClearHelper<YCChatMessage>);
		SAFE_DELETE(myMessages);
	}

	//
	// ���������Ϣ
	//
	if (NULL != myGlobalMessages)
	{
		myGlobalMessages->clear(&YCDListClearHelper<YCChatMessage>);
		SAFE_DELETE(myGlobalMessages);
	}

	//
	// ���������
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

