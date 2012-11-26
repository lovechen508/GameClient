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
// 函数：draw()
//
// 目的：在地图上完成自身的绘制
//
void YCTiledTexture::draw()
{

}

//
// 函数：rebindTexture(YCITexture* texture)
//
// 目的：重新绑定纹理
//
void YCTiledTexture::rebindTexture(YCITexture* texture)
{
	myTexture = texture;
}

//
// 函数：setRect(unsigned int left, unsigned int top)
//
// 目的：设置地图块对应位置UV
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
// 函数：raw()
//
// 目的：返回真实纹理LPDIRECT3DTEXTURE9指针，供YCGraphic使用
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
// 函数：globalWidth()
// 
// 目的：整张Altas纹理的宽度
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
// 函数：globalHeight()
// 
// 目的：整张Altas纹理的高度
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
// 函数：color()
//
// 目的：返回透明色
//
unsigned int YCTiledTexture::color()
{
	return 0xFFFFFFFF;
}