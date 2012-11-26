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
// 函数：setGroup(const char* group)
//
// 目的：设置窗口组别
//
void YCWindow::setGroup(const char* group)
{
	SMART_ASSERT(group == NULL);
	if (group == NULL)
	{
		throw YCException(2002, "YCWindow::setGroup设置group为NULL");
	}

	if (strlen(group) >= GROUP_NAME_LENGTH)
	{
		throw YCException(2002, "YCWindow::setGroup设置group值过长");
	}

	memcpy(myGroup, group, strlen(group));
}

//
// 函数：getGroup()
//
// 目的：获取窗口组别
//
const char* YCWindow::getGroup()
{
	return myGroup;
}

//
// 函数：setWeight(int weight)
//
// 目的：设置窗口权重
//
void YCWindow::setWeight(int weight)
{
	myWeight = weight;
}

//
// 函数：getWeight()
//
// 目的：获取窗口权重
//
int YCWindow::getWeight()
{
	return myWeight;
}

//
// 函数：matchAccelerateKey()
//
// 目的：是否匹配快捷键
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
// 函数：clone()
//
// 目的：克隆当前类
//
YCWindow* YCWindow::clone()
{
	return NULL;
}

//////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////

//
// 函数：preload()
//
// 目的：准备数据等，每次打开窗口前调用
//
bool YCWindow::preload()
{
	return true;
}
	
//
// 函数：postunload()
//
// 目的：准备数据等，每次关闭窗口前调用
//
bool YCWindow::postunload()
{
	return true;
}

//
// 函数：initAccelerateKey()
//
// 目的：处理快捷键
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
			throw YCException(2002, "YCWindow::initAccelerateKey 快捷键组合数目过多!");
		}

		for (unsigned i = 0; i < splited.size(); ++i)
		{
			myAccelerateKey[i] = YCUITagHelper::Instance().Key2Char(splited[i].c_str());
		}
	}
}

