#include "YCTexture.h"

#include "YCBasic\YCAssert.h"

#include "YCBasic\YCException.h"

#include "YCITexture2D.h"

YCTexture::YCTexture(unsigned int uniqueId)
	: YCITexture()
	, myLPTexture(NULL)
	, myUniqueId(uniqueId)
{
}

YCTexture::~YCTexture()
{
	if (myLPTexture != NULL)
	{
		myLPTexture->decreaseReference();
	}
}

//
// 函数：bind(YCITexture2D* texture)
//
// 目的：绑定真实的纹理
//
void YCTexture::bind(YCITexture2D* texture)
{
	if (texture == NULL)
	{
		throw YCException(2002, "YCTexture::bind构造函数中YCITexture2D为空");
	}
	
	myLPTexture = texture;
	myLPTexture->increaseReference();
	myLPTexture->getRect(myUniqueId, myRect);
		
	// Altas Texture 
	int globalWidth = myLPTexture->width();
	int globalHeight = myLPTexture->height();

	myUV[0] = (float)myRect.left/globalWidth;
	myUV[1] = (float)myRect.top/globalHeight;

	myUV[2] = (float)myRect.right/globalWidth;
	myUV[3] = (float)myRect.top/globalHeight;

	myUV[4] = (float)myRect.left/globalWidth;
	myUV[5] = (float)myRect.bottom/globalHeight; 

	myUV[6] = (float)myRect.right/globalWidth;
	myUV[7] = (float)myRect.bottom/globalHeight; 
}

//
// 函数：isTransparent(unsigned int x, unsigned int y)
//
// 目的：测试某个点是否透明
//
bool YCTexture::isTransparent(unsigned int x, unsigned int y)
{
	if (myLPTexture == NULL)
	{
		return true;
	}
	return myLPTexture->isTransparent(x+myRect.left, y+myRect.top);
}

//
// 函数：raw()
//
// 目的：返回真实纹理LPDIRECT3DTEXTURE9指针，供YCGraphic使用
//
void* YCTexture::raw()
{
	if (myLPTexture != NULL)
	{
		return myLPTexture->raw();
	}
	return NULL;
}

//
// 函数：draw()
//
// 目的：返回真实纹理LPDIRECT3DTEXTURE9指针，供YCGraphic使用
//
void YCTexture::draw()
{
	if (myLPTexture != NULL)
	{
		myLPTexture->draw();
	}
}

//
// 函数：globalWidth()
// 
// 目的：整张Altas纹理的宽度
//
unsigned int YCTexture::globalWidth()
{
	if (myLPTexture != NULL)
	{
		return myLPTexture->width();
	}
	return 0;
}

//
// 函数：globalHeight()
// 
// 目的：整张Altas纹理的高度
//
unsigned int YCTexture::globalHeight()
{
	if (myLPTexture != NULL)
	{
		return myLPTexture->height();
	}
	return 0;
}
