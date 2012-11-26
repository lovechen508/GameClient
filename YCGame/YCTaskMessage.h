#ifndef _INCLUDE_YCTASKMESSAGE_H_
#define _INCLUDE_YCTASKMESSAGE_H_

#include <string>

#include "YCTaskDefine.h"

class YCIMessageUnit;
struct YCIUnitAction;

//
// YCTaskMessage : ������Ϣ��
//     ���ݷ���������Ϣ�������ӳ�����ṹ
//
class YCTaskMessage
{
public:

	YCTaskMessage(const char* sectionName);

	~YCTaskMessage(void);

	//
	// ȡ����Ϣ������
	//
	const char* getSectionName() const;

	//
	// ע�ͣ������������һ����Ϣ
	//
	void setNext(YCTaskMessage* next);

	//
	// ע�ͣ���ȡ�������һ����Ϣ
	//
	YCTaskMessage* getNext();

	//
	// ע�ͣ�����NPC��Ϣ��Ԫ
	//
	void setTalk(YCIMessageUnit* unit);

	//
	// ע�ͣ�ȡ��NPC��Ϣ��Ԫ
	//
	const YCIMessageUnit* getTalk() const;

	//
	// ����������Ϣ�����е�������Ϣ���һ������
	// һ�����������һ�У�����ʹͼƬ���ӣ�Ҳ����ʹ��������
	//
	void setCommand(YCIMessageUnit* current);

	//
	// ��ȡ������Ϣ�������������һ����Ϣ��Ԫ
	// ���currentΪ�գ���������ĵ�һ����Ϣ��Ԫ
	//
	const YCIMessageUnit* getCommand() const;
		
public:

	//
	// ������onClickCommand(YCTaskMessage* message, const std::string& action, const std::string& command)
	//
	// Ŀ�ģ��󶨵���Ϣ��Ԫ������Ϣ��Ԫ�����ʱ��ص�
	//
	// ������
	//      Message : ��Ϣ��Ԫ����������ͷ����[start], ����command=dlg11ʱ��YCTaskMessage�����в���
	//		Action	: 'link' 'server_cmd'
	//      command	: 'dlg11' 23 ...
	//
	static void onClickCommand(YCTaskMessage* message, const std::string& action, const std::string& command);

private:

	char mySectionName[TASK_SECTION_NAME];

	YCIMessageUnit* myTalk;

	YCIMessageUnit* myCommands;

	YCTaskMessage* myNext;
};

#endif

