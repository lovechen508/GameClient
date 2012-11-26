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
// ������raw()
//
// Ŀ�ģ�������ʵ����LPDIRECT3DTEXTURE9ָ�룬��YCGraphicʹ��
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
// ������draw()
//
// Ŀ�ģ�������ʵ����ָ�룬��YCGraphicʹ��
//
void YCITexture2D::draw()
{
	
}

//
// ������status()
//
// Ŀ�ģ���ȡ����ǰ״̬
//
int YCITexture2D::status()
{
	return myStatus;
}

//
// ������increaseReference()
//
// Ŀ�ģ��������ü��������������Ӻ��ֵ
//
int YCITexture2D::increaseReference()
{
	return InterlockedExchangeAdd(&myRefCount, 1);
}

//
// ������decreaseReference()
//
// Ŀ�ģ��������ü����������ؼ��ٺ��ֵ
//
int YCITexture2D::decreaseReference()
{
	return InterlockedExchangeAdd(&myRefCount, -1);
}

//
// ������referenceCount()
//
// Ŀ�ģ����ص�ǰ�����ü���ֵ
//
int YCITexture2D::referenceCount()
{
	return myRefCount;
}

//
// ������isTransparent(unsigned int x, unsigned int y)
//
// Ŀ�ģ����Ե�ǰ���Ƿ�Ϊ�գ�������ֵΪ��
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
// ������width()
//
// Ŀ�ģ�����������ʵ���
//
unsigned int YCITexture2D::width()
{
	return myTexture->width();
}

//
// ������height()
//
// Ŀ�ģ�����������ʵ�߶�
//
unsigned int YCITexture2D::height()
{
	return myTexture->height();
}

//
// ������setLatestAccess(time_t timestamp)
//
// Ŀ�ģ�����������ʱ��
//
void YCITexture2D::setLatestAccess(time_t timestamp)
{
	myLatestAccess = timestamp;
}

//
// ������getLatestAccess()
//
// Ŀ�ģ���ȡ������ʱ��
//
time_t YCITexture2D::getLatestAccess()
{
	return myLatestAccess;
}

