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
 * YCChatManager : ���������
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
	// ȫƵ��
	//
	static const int CH_FILTER_FULL;

	//
	// ϵͳ
	//
	static const int CH_FILTER_SYSTEM;

public:

	YCChatManager(void);

	~YCChatManager(void);

	//
	// ������initialize(YCConfig& config)
	//
	// Ŀ�ģ���ʼ����Ϣ������
	//
	bool initialize(YCConfig& config);

	//
	// ������setCustomChannel(int channels)
	//
	// Ŀ�ģ������Զ���Ƶ��
	//
	void setCustomChannel(int channels);

	//
	// ������addFilter(YCIChatFilter* filter)
	//
	// Ŀ�ģ���ӹ�����, �����ͬId�����׳��쳣
	//
	void addFilter(YCIChatFilter* filter);

	//
	// ������removeFilter(unsigned int id)
	//
	// Ŀ�ģ��Ƴ�������
	//
	YCIChatFilter * removeFilter(unsigned int id);

	//
	// ������addInterceptor(YCIChatInterceptor* interceptor)
	//
	// Ŀ�ģ����������, �����ͬId�����׳��쳣
	//
	void addInterceptor(YCIChatInterceptor* interceptor);

	//
	// ������removeInterceptor(unsigned int id)
	//
	// Ŀ�ģ��Ƴ�������
	//
	YCIChatInterceptor * removeInterceptor(unsigned int id);

	//
	// ������send(const YCChatMessage* msg)
	//
	// Ŀ�ģ�������Ϣ
	//
	void send(const YCChatMessage* msg);

	//
	// ������recv(const NET_CHAT_MSG_T* msg)
	//
	// Ŀ�ģ���Ϣ����
	//
	void recv(const NET_CHAT_MSG_NTF_T* msg);

	//
	// ������addBlackRole(const char* role)
	//
	// Ŀ�ģ���ӽ�ɫ��������
	//
	void addBlackRole(const char* role);

	//
	// ������removeBlackRole(const char* role)
	//
	// Ŀ�ģ���role�Ƴ�������
	//       ��roleΪNULL����պ�����
	//
	void removeBlackRole(const char* role);

	//
	// ������finalize()
	//
	// Ŀ�ģ��ͷ���Դ
	//
	void finalize();

private:

	//
	// ���ش洢�ļ���
	//
	char myChatConfigFile[MAX_PATH];

	//
	// �����Ϣ����
	//
	int myMaxMessage;

	//
	// �������Ϣ����
	//
	YCQueue * myMessages;

	//
	// ������Ϣ
	//
	YCQueue * myGlobalMessages;

	//
	// �Զ���Ƶ��
	//
	int myCustomizeChannels;

	//
	// ��������
	//
	YCIChatInterceptor *myInterceptors;

	//
	// ��������
	//
	YCIChatFilter* myFilters;

	//
	// ������
	//
	YCDList * myBlackList;

};

#endif

