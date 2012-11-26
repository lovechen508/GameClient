#include "YCImageBackground.h"

#include "YCTexture.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

YCImageBackground::YCImageBackground(YCIUITag *host, YCTexture* texture)
	: YCIBackground(host)
	, myTexture(texture)
	, myDrawer(NULL)
{
	SMART_ASSERT(texture != NULL);
	YCGraphic* graphic = (YCGraphic*)YCRegistry::get("graphic");
	if (graphic == NULL)
	{
		throw YCException(2002, "YCImageBackground�޷���ȡYCGraphic�����");
	}
	myDrawer = graphic->createD3DSprite();
}

YCImageBackground::~YCImageBackground(void)
{
	SAFE_DELETE(myTexture);
}

//
// ������draw()
//
// Ŀ�ģ���ָ��host.(myPositionX, myPositionY)��
//      ��͸����alpha����host.(myWidth, myHeight)�ı���
//
void YCImageBackground::draw(float alpha, int left, int top, int width, int height)
{
	myDrawer->draw(myTexture, left, top, 1.0f, 1.0f, alpha, 0);
}
