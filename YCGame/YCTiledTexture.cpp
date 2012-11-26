#include "YCTiledTexture.h"

#include "YCTileSet.h"

#include "YCGraphic\YCITexture.h"
#include "YCGraphic\YCMapDef.h"

#include "YCBasic\YCAssert.h"

YCTiledTexture::YCTiledTexture(YCTileSet* tileSet, YCITexture* texture)
	: myTileSet(tileSet)
	, myTexture(texture)
	, myLeft(0)
	, myTop(0)
{
	SMART_ASSERT(myTileSet != NULL);

	memset(myUV, 0, 8);
}


YCTiledTexture::~YCTiledTexture(void)
{
}

//
// ������draw()
//
// Ŀ�ģ��ڵ�ͼ���������Ļ���
//
void YCTiledTexture::draw()
{

}

//
// ������rebindTexture(YCITexture* texture)
//
// Ŀ�ģ����°�����
//
void YCTiledTexture::rebindTexture(YCITexture* texture)
{
	myTexture = texture;
}

//
// ������setRect(unsigned int left, unsigned int top)
//
// Ŀ�ģ����õ�ͼ���Ӧλ��UV
//
void YCTiledTexture::setRect(unsigned int left, unsigned int top)
{
	myLeft = left;
	myTop = top;

	if (myTexture != NULL)
	{
		int globalWidth = myTexture->globalWidth();
		int globalHeight = myTexture->globalHeight();

		myUV[0] = (float)left/globalWidth;
		myUV[1] = (float)top/globalHeight;

		myUV[2] = (float)(left+TILE_WIDTH)/globalWidth;
		myUV[3] = (float)top/globalHeight;

		myUV[4] = (float)left/globalWidth;
		myUV[5] = (float)(top+TILE_HEIGHT)/globalHeight; 

		myUV[6] = (float)(left+TILE_WIDTH)/globalWidth;
		myUV[7] = (float)(top+TILE_HEIGHT)/globalHeight; 
	}
}

//
// ������raw()
//
// Ŀ�ģ�������ʵ����LPDIRECT3DTEXTURE9ָ�룬��YCGraphicʹ��
//
void* YCTiledTexture::raw()
{
	if (myTexture != NULL)
	{
		return myTexture->raw();
	}
	return NULL;
}

//
// ������globalWidth()
// 
// Ŀ�ģ�����Altas����Ŀ��
//
unsigned int YCTiledTexture::globalWidth()
{
	if (myTexture != NULL)
	{
		return myTexture->globalWidth();
	}
	return 0;
}

//
// ������globalHeight()
// 
// Ŀ�ģ�����Altas����ĸ߶�
//
unsigned int YCTiledTexture::globalHeight()
{
	if (myTexture != NULL)
	{
		return myTexture->globalHeight();
	}
	return 0;
}

//
// ������color()
//
// Ŀ�ģ�����͸��ɫ
//
unsigned int YCTiledTexture::color()
{
	return 0xFFFFFFFF;
}