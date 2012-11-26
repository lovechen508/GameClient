#include "YCAnimationBackground.h"

#include "YCIAnimation.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

//
// ������YCAnimationBackground(YCIUITag* host, YCIAnimation* animation)
//
// Ŀ�ģ����캯������Ԫ���Ͷ���
//
YCAnimationBackground::YCAnimationBackground(YCIUITag* host, YCIAnimation* animation)
	: YCIBackground(host)
	, myAnimation(animation)
	, myDrawer(NULL)
{
	SMART_ASSERT(myAnimation != NULL);
	YCGraphic* graphic = (YCGraphic*)YCRegistry::get("graphic");
	if (graphic == NULL)
	{
		throw YCException(2002, "YCAnimationBackground�޷���ȡYCGraphic�����");
	}
	myDrawer = graphic->createD3DSprite();
}


YCAnimationBackground::~YCAnimationBackground(void)
{
	SAFE_DELETE(myAnimation);
}

//
// ������draw()
//
// Ŀ�ģ���ָ��host.(myPositionX, myPositionY)��
//      ��͸����alpha����host.(myWidth, myHeight)�ı���
//
void YCAnimationBackground::draw(float alpha, int left, int top, int width, int height)
{

}