#ifndef _INCLUDE_YCWINDOW_H_
#define _INCLUDE_YCWINDOW_H_

#include "YCUITagDef.h"
#include "YCIContainer.h"

class YCWindow :
	public YCIContainer
{
public:

	YCWindow(const YCUITagMeta* tagMeta);

	virtual ~YCWindow(void);
	
	//
	// ������setGroup(const char* group)
	//
	// Ŀ�ģ����ô������
	//
	void setGroup(const char* group);

	//
	// ������getGroup()
	//
	// Ŀ�ģ���ȡ�������
	//
	const char* getGroup();

	//
	// ������setWeight(int weight)
	//
	// Ŀ�ģ����ô���Ȩ��
	//
	void setWeight(int weight);

	//
	// ������getWeight()
	//
	// Ŀ�ģ���ȡ����Ȩ��
	//
	int getWeight();

	//
	// ������matchAccelerateKey()
	//
	// Ŀ�ģ��Ƿ�ƥ���ݼ�
	//
	virtual bool matchAccelerateKey();

	//
	// ������clone()
	//
	// Ŀ�ģ���¡��ǰ��
	//
	virtual YCWindow* clone();
	
private:
	
	//
	// ������preload()
	//
	// Ŀ�ģ�׼�����ݵȣ�ÿ�δ򿪴���ǰ����
	//
	virtual bool preload();
	
	//
	// ������postunload()
	//
	// Ŀ�ģ�׼�����ݵȣ�ÿ�ιرմ���ǰ����
	//
	virtual bool postunload();

	//
	// ������initAccelerateKey()
	//
	// Ŀ�ģ������ݼ�
	//
	virtual void initAccelerateKey();

private:

	// Ŀǰֻ֧���������ϳ�һ����ݼ�
	BYTE myAccelerateKey[MAX_ACCELERATE_KEYS];

	int myWeight;

	char myGroup[GROUP_NAME_LENGTH];
};

#endif