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
// ������bind(YCITexture2D* texture)
//
// Ŀ�ģ�����ʵ������
//
void YCTexture::bind(YCITexture2D* texture)
{
	if (texture == NULL)
	{
		throw YCException(2002, "YCTexture::bind���캯����YCITexture2DΪ��");
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
// ������isTransparent(unsigned int x, unsigned int y)
//
// Ŀ�ģ�����ĳ�����Ƿ�͸��
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
// ������raw()
//
// Ŀ�ģ�������ʵ����LPDIRECT3DTEXTURE9ָ�룬��YCGraphicʹ��
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
// ������draw()
//
// Ŀ�ģ�������ʵ����LPDIRECT3DTEXTURE9ָ�룬��YCGraphicʹ��
//
void YCTexture::draw()
{
	if (myLPTexture != NULL)
	{
		myLPTexture->draw();
	}
}

//
// ������globalWidth()
// 
// Ŀ�ģ�����Altas����Ŀ��
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
// ������globalHeight()
// 
// Ŀ�ģ�����Altas����ĸ߶�
//
unsigned int YCTexture::globalHeight()
{
	if (myLPTexture != NULL)
	{
		return myLPTexture->height();
	}
	return 0;
}
