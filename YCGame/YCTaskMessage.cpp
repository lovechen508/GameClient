#include "YCTaskMessage.h"

#include "YCIMessageUnit.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

YCTaskMessage::YCTaskMessage(const char* sectionName)
	: myNext(NULL)
	, myTalk(NULL)
	, myCommands(NULL)
{
	SMART_ASSERT(sectionName != NULL);

	int length = strlen(sectionName);
	if (length >= TASK_SECTION_NAME)
	{
		throw YCException(2002, "YCTaskMessage�����������ƹ���", length);
	}

	memcpy(mySectionName, sectionName, length);
	mySectionName[length] = '\0';
}


YCTaskMessage::~YCTaskMessage(void)
{
	SAFE_DELETE(myNext);
	SAFE_DELETE(myTalk);
	SAFE_DELETE(myCommands);
}

//
// ȡ����Ϣ������
//
const char* YCTaskMessage::getSectionName() const
{
	return mySectionName;
}

//
// ע�ͣ������������һ����Ϣ
//
void YCTaskMessage::setNext(YCTaskMessage* next)
{
	SMART_ASSERT(next != NULL);
	SMART_ASSERT(myNext == NULL);
	myNext = next;
}

//
// ע�ͣ���ȡ�������һ����Ϣ
//
YCTaskMessage* YCTaskMessage::getNext()
{
	return myNext;
}

//
// ע�ͣ�׷����Ϣ��Ԫ��һ��׷�Ӷ�Ӧһ����Ϣ
//
void YCTaskMessage::setTalk(YCIMessageUnit* unit)
{
	SMART_ASSERT(unit != NULL);
	SMART_ASSERT(myTalk == NULL);
	myTalk = unit;
}

//
// ע�ͣ�ȡ��������Ϣ��Ԫ
//
const YCIMessageUnit* YCTaskMessage::getTalk() const
{
	return myTalk;
}

//
// ����������Ϣ�����е�������Ϣ���һ������
// һ�����������һ�У�����ʹͼƬ���ӣ�Ҳ����ʹ��������
//
void YCTaskMessage::setCommand(YCIMessageUnit* current)
{
	SMART_ASSERT(current != NULL);
	if (myCommands == NULL)
	{
		myCommands = current;
	}
	else
	{
		YCIMessageUnit* last = myCommands;
		while (last->next() != NULL)
		{
			last = last->next();
		}
		last->append(current);
	}
}

//
// ��ȡ������Ϣ�������������һ����Ϣ��Ԫ
// ���currentΪ�գ���������ĵ�һ����Ϣ��Ԫ
//
const YCIMessageUnit* YCTaskMessage::getCommand() const
{
	return myCommands;
}

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
void YCTaskMessage::onClickCommand(YCTaskMessage* message, const std::string& action, const std::string& command)
{
	SMART_ASSERT(message != NULL);
	if (message == NULL)
	{
		throw YCException(1002, "YCTaskMessage::onClickCommand��Ч��Ϣ��Ӧ�ṹ");
	}

	if (action == "link")
	{
		return;
	}
	else if (action == "server_cmd")
	{
		// ����������Ϣ����...
		return;
	}
	else
	{
		throw YCException(1002, "YCTaskMessage::onClickCommandδ֪Action", action.c_str());
	}
}