#include "YCSingleTexture2D.h"

#include "YCBasic\YCAssert.h"

#include "YCITextureFile.h"

#include "YCBasic\YCException.h"

YCSingleTexture2D::YCSingleTexture2D(YCITextureFile *texture, int status, int uniqueId)
	: YCITexture2D(texture, status)
	, myUniqueId(uniqueId)
{
	SMART_ASSERT(uniqueId != 0);
}

YCSingleTexture2D::~YCSingleTexture2D(void)
{
}

//
// ������hasTexture(unsigned int uniqueId)
//
// Ŀ�ģ������������Ƿ����ͼƬ
//
// ע�ͣ�
//     1. YCSingleTexture2D ֻ����һ��ͼƬ
//     2. YCAtlasTexture2D ���ܰ���1..n��ͼƬ
//
bool YCSingleTexture2D::hasTexture(unsigned int uniqueId) const
{
	return (uniqueId == myUniqueId);
}

//
// ������getRect(unsigned int uniqueId, FloatRect& rect)
//
// Ŀ�ģ�����������������
//
void YCSingleTexture2D::getRect(unsigned int uniqueId, FloatRect& rect) const
{
	if (!hasTexture(uniqueId))
	{
		throw YCException(2002, "YCSingleTexture2D::rect��Ӧ������δ�ҵ�!");
	}

	if (myStatus == SUCC)
	{
		rect.top = 0;
		rect.left = 0;
		rect.right = (float)myTexture->width();
		rect.bottom = (float)myTexture->height();
	}
	else
	{
		rect.top = 0;
		rect.left = 0;
		rect.right = 0;
		rect.bottom = 0;
	}
}
