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
// 函数：hasTexture(unsigned int uniqueId)
//
// 目的：测试纹理中是否包含图片
//
// 注释：
//     1. YCSingleTexture2D 只包含一张图片
//     2. YCAtlasTexture2D 可能包含1..n张图片
//
bool YCSingleTexture2D::hasTexture(unsigned int uniqueId) const
{
	return (uniqueId == myUniqueId);
}

//
// 函数：getRect(unsigned int uniqueId, FloatRect& rect)
//
// 目的：返回纹理所在区域
//
void YCSingleTexture2D::getRect(unsigned int uniqueId, FloatRect& rect) const
{
	if (!hasTexture(uniqueId))
	{
		throw YCException(2002, "YCSingleTexture2D::rect对应的纹理未找到!");
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
