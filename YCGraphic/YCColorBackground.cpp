#include "YCColorBackground.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

YCColorBackground::YCColorBackground(YCIUITag *host,      //����UIԪ��
		                             unsigned int color)  //����͸����
	: YCIBackground(host)
	, myColor(color)
	, myDrawer(NULL)
{
	myColor += 0xFF000000;
	YCGraphic* graphic = (YCGraphic*)YCRegistry::get("graphic");
	if (graphic == NULL)
	{
		throw YCException(2002, "YCColorBackground�޷���ȡYCGraphic�����");
	}
	myDrawer = graphic->createPrimitive();
}

YCColorBackground::~YCColorBackground(void)
{
}

//
// ������draw()
//
// Ŀ�ģ���ָ��host.(myPositionX, myPositionY)��
//      ��͸����alpha����host.(myWidth, myHeight)�ı���
//
void YCColorBackground::draw(float alpha, int left, int top, int width, int height)
{
	myDrawer->drawRetangle(left, top, left+width, top+height, myColor, myColor, myColor, myColor);
}