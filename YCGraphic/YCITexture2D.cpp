#include "YCITexture2D.h"

#include "YCBasic\YCAssert.h"

#include "YCBasic\YCDef.h"

#include "YCITextureFile.h"

YCITexture2D::YCITexture2D(YCITextureFile* texture, int status)
	: myTexture(texture), myStatus(status), myRefCount(0)
{

}

YCITexture2D::~YCITexture2D(void)
{
	SMART_ASSERT(myRefCount == 0);
	SAFE_DELETE(myTexture);
}

//
// 函数：raw()
//
// 目的：返回真实纹理LPDIRECT3DTEXTURE9指针，供YCGraphic使用
//
void* YCITexture2D::raw()
{
	if (myStatus == SUCC && myTexture != NULL)
	{
		return myTexture->raw();
	}
	return NULL;
}

//
// 函数：draw()
//
// 目的：返回真实纹理指针，供YCGraphic使用
//
void YCITexture2D::draw()
{
	
}

//
// 函数：status()
//
// 目的：获取纹理当前状态
//
int YCITexture2D::status()
{
	return myStatus;
}

//
// 函数：increaseReference()
//
// 目的：增加引用计数，并返回增加后的值
//
int YCITexture2D::increaseReference()
{
	return InterlockedExchangeAdd(&myRefCount, 1);
}

//
// 函数：decreaseReference()
//
// 目的：减少引用计数，并返回减少后的值
//
int YCITexture2D::decreaseReference()
{
	return InterlockedExchangeAdd(&myRefCount, -1);
}

//
// 函数：referenceCount()
//
// 目的：返回当前的引用计数值
//
int YCITexture2D::referenceCount()
{
	return myRefCount;
}

//
// 函数：isTransparent(unsigned int x, unsigned int y)
//
// 目的：测试当前点是否为空，即像素值为零
//
bool YCITexture2D::isTransparent(unsigned int x, unsigned int y)
{
	if (myTexture == NULL)
	{
		return true;
	}
	return myTexture->isTransparent(x, y);
}

//
// 函数：width()
//
// 目的：返回纹理真实宽度
//
unsigned int YCITexture2D::width()
{
	return myTexture->width();
}

//
// 函数：height()
//
// 目的：返回纹理真实高度
//
unsigned int YCITexture2D::height()
{
	return myTexture->height();
}

//
// 函数：setLatestAccess(time_t timestamp)
//
// 目的：更新最后访问时间
//
void YCITexture2D::setLatestAccess(time_t timestamp)
{
	myLatestAccess = timestamp;
}

//
// 函数：getLatestAccess()
//
// 目的：获取最后访问时间
//
time_t YCITexture2D::getLatestAccess()
{
	return myLatestAccess;
}

