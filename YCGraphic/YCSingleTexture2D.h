#ifndef _INCLUDE_YCSINGLETEXTURE2D_H_
#define _INCLUDE_YCSINGLETEXTURE2D_H_

#include "YCITexture2D.h"

/*
 * YCSingleTexture2D : 2D 单张纹理只含一张图片类
 */

class YCSingleTexture2D : public YCITexture2D
{
public:

	//
	// 函数：YCSingleTexture2D(void)
	//
	// 目的：构造函数，转发基类
	//
	YCSingleTexture2D(YCITextureFile *texture, int status, int uniqueId);

	//
	// 函数：~YCSingleTexture2D(void)
	//
	// 目的：析构函数，释放纹理
	//
	virtual ~YCSingleTexture2D(void);

	//
	// 函数：hasTexture(unsigned int uniqueId)
	//
	// 目的：测试纹理中是否包含图片
	//
	// 注释：
	//     1. YCSingleTexture2D 只包含一张图片
	//     2. YCAtlasTexture2D 可能包含1..n张图片
	//
	virtual bool hasTexture(unsigned int uniqueId) const;

	//
	// 函数：getRect(unsigned int uniqueId, FloatRect& rect)
	//
	// 目的：返回纹理所在区域
	//
	virtual void getRect(unsigned int uniqueId, FloatRect& rect) const;
	
private:

	unsigned int myUniqueId;
};

#endif