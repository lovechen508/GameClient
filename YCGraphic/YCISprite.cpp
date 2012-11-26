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
// ������setId(unsigned long long id)
//
// Ŀ�ģ�����ΨһID
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
// ������draw(unsigned int timestamp)
//
// Ŀ�ģ��ڵ�ͼ���������Ļ���
//
void YCISprite::draw(unsigned int timestamp)
{
	if (myActions->size() > 0)
	{

		
	}
}

//
// ������insertAction(YCIAction *action)
//
// Ŀ�ģ����õ�ǰ����������¶���������ͷ��
//
void YCISprite::insertAction(YCIAction *action)
{

}

//
// ������appendAction(YCIAction *action)
//
// Ŀ�ģ������¶���������β
//
void appendAction(YCIAction *action)
{
	
}