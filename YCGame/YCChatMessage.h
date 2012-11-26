#ifndef _INCLUDE_YCCHATMESSAGE_H_
#define _INCLUDE_YCCHATMESSAGE_H_

class YCIMessageUnit;

/*
 * YCChatMessage : ������Ϣ������
 */
class YCChatMessage
{
public:

	//
	// ���캯��
	//
	YCChatMessage(int type, const char* name);

	~YCChatMessage(void);

	//
	// ������setUnit(YCIMessageUnit* unit)
	//
	// Ŀ�ģ�������Ϣ��Ԫ���ӹ���������
	//
	void setUnit(YCIMessageUnit* unit);

	//
	// ������getUnit()
	//
	// Ŀ�ģ� ȡ����Ϣ��Ԫ
	//
	YCIMessageUnit* getUnit();

private:

	//
	// ��Ϣ���� CH_LIGHT ...
	//
	int myType;

	//
	// ���ͽ�ɫ��
	//
	char* mySender;

	//
	// ��Ϣ��Ԫ����
	//
	YCIMessageUnit* myUnits;
};

#endif