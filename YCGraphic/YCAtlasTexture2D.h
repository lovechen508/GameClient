#ifndef _INCLUDE_YCALTASTEXTURE_H_
#define _INCLUDE_YCALTASTEXTURE_H_

class YCDList;

#include "YCITexture2D.h"

struct ATLAS_TEXTURE_INFO_T {
	unsigned int id;  // uniqueId
	int x;   // x坐标
	int y;   // y坐标
	int w;   // width
	int h;   // height
};

/*
 * YCAltasTexture : 多张图片压缩至一张纹理抽象类
 *
 * 注释：代表其中的某一张图片
 */
class YCAtlasTexture2D :
	public YCITexture2D
{
public:

	YCAtlasTexture2D(YCDList *idList, YCITextureFile *texture, int status);

	virtual ~YCAtlasTexture2D(void);
	
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

	YCDList* myIdList;
};

#endif


