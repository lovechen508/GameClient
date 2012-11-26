#include "YCWindow.h"

#include "YCUIDOM.h"
#include "YCUITagHelper.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCStringHelper.h"

YCWindow::YCWindow(const YCUITagMeta* tagMeta)
	: YCIContainer(tagMeta)
{
	memset(myGroup, 0, GROUP_NAME_LENGTH);
	memset(myAccelerateKey, 0, MAX_ACCELERATE_KEYS);
}

YCWindow::~YCWindow(void)
{
}

//
// ������setGroup(const char* group)
//
// Ŀ�ģ����ô������
//
void YCWindow::setGroup(const char* group)
{
	SMART_ASSERT(group == NULL);
	if (group == NULL)
	{
		throw YCException(2002, "YCWindow::setGroup����groupΪNULL");
	}

	if (strlen(group) >= GROUP_NAME_LENGTH)
	{
		throw YCException(2002, "YCWindow::setGroup����groupֵ����");
	}

	memcpy(myGroup, group, strlen(group));
}

//
// ������getGroup()
//
// Ŀ�ģ���ȡ�������
//
const char* YCWindow::getGroup()
{
	return myGroup;
}

//
// ������setWeight(int weight)
//
// Ŀ�ģ����ô���Ȩ��
//
void YCWindow::setWeight(int weight)
{
	myWeight = weight;
}

//
// ������getWeight()
//
// Ŀ�ģ���ȡ����Ȩ��
//
int YCWindow::getWeight()
{
	return myWeight;
}

//
// ������matchAccelerateKey()
//
// Ŀ�ģ��Ƿ�ƥ���ݼ�
//
bool YCWindow::matchAccelerateKey()
{
	bool hasAccelerate = false;
	for (int i = 0; i < MAX_ACCELERATE_KEYS; ++i)
	{
		if (myAccelerateKey[i] != 0)
		{
			hasAccelerate = true;
			if (!gInput->isKeyDown(myAccelerateKey[i]))
			{
				return false;
			}
		}
	}
	return (hasAccelerate ? true : false);
}

//
// ������clone()
//
// Ŀ�ģ���¡��ǰ��
//
YCWindow* YCWindow::clone()
{
	return NULL;
}

//////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////

//
// ������preload()
//
// Ŀ�ģ�׼�����ݵȣ�ÿ�δ򿪴���ǰ����
//
bool YCWindow::preload()
{
	return true;
}
	
//
// ������postunload()
//
// Ŀ�ģ�׼�����ݵȣ�ÿ�ιرմ���ǰ����
//
bool YCWindow::postunload()
{
	return true;
}

//
// ������initAccelerateKey()
//
// Ŀ�ģ������ݼ�
//
void YCWindow::initAccelerateKey()
{
	const char* accelerateKey = myUIDOM->getAttribute("accelerateKey");
	if (accelerateKey != NULL)
	{
		std::vector<std::string> splited;
		YCStringHelper::split(accelerateKey, "+", & splited);
		SMART_ASSERT(splited.size() >= MAX_ACCELERATE_KEYS).warn(accelerateKey);
		if (splited.size() > MAX_ACCELERATE_KEYS)
		{
			throw YCException(2002, "YCWindow::initAccelerateKey ��ݼ������Ŀ����!");
		}

		for (unsigned i = 0; i < splited.size(); ++i)
		{
			myAccelerateKey[i] = YCUITagHelper::Instance().Key2Char(splited[i].c_str());
		}
	}
}

