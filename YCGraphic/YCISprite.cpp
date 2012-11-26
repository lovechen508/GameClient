#include "YCISprite.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"

YCISprite::YCISprite(void)
	: myActions(new YCDList())
{
}

YCISprite::~YCISprite(void)
{
	SAFE_DELETE(myActions);
}

//
// 函数：setId(unsigned long long id)
//
// 目的：设置唯一ID
//
void YCISprite::setId(unsigned long long id)
{
	myID = id;
}

unsigned long long YCISprite::getId() const
{
	return myID;
}

//
// 函数：draw(unsigned int timestamp)
//
// 目的：在地图上完成自身的绘制
//
void YCISprite::draw(unsigned int timestamp)
{
	if (myActions->size() > 0)
	{

		
	}
}

//
// 函数：insertAction(YCIAction *action)
//
// 目的：重置当前动画，添加新动画到队列头部
//
void YCISprite::insertAction(YCIAction *action)
{

}

//
// 函数：appendAction(YCIAction *action)
//
// 目的：增加新动画到队列尾
//
void appendAction(YCIAction *action)
{
	
}