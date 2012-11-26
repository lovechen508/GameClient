#ifndef _INCLUDE_YCTEXTURE_H_
#define _INCLUDE_YCTEXTURE_H_

#include "YCITexture.h"

class YCITexture2D;

/*
 * YCTexture : 纹理单元具体类
 *             不可继承
 */
class YCAPI YCTexture : public YCITexture
{
public:

	//
	// 函数：YCITexture()
	//
	// 目的：构造函数，同时YCITexture2D纹理引用计数加1
	//
	YCTexture(unsigned int uniqueId);

	//
	// 函数：~YCTexture(void)
	//
	// 目的：释放纹理，将YCITexture2D纹理引用计数减1
	//
	virtual ~YCTexture(void);

	//
	// 函数：bind(YCITexture2D* texture)
	//
	// 目的：绑定真实的纹理
	//
	void bind(YCITexture2D* texture);

	//
	// 函数：isTransparent(unsigned int x, unsigned int y)
	//
	// 目的：测试某个点是否透明
	//
	bool isTransparent(unsigned int x, unsigned int y);
			
	//
	// 函数：id()
	//
	// 目的：返回纹理唯一ID
	//
	unsigned int unique();

	//
	// 函数：raw()
	//
	// 目的：返回真实纹理LPDIRECT3DTEXTURE9指针，供YCGraphic使用
	//
	virtual void* raw();

	//
	// 函数：draw()
	//
	// 目的：返回真实纹理LPDIRECT3DTEXTURE9指针，供YCGraphic使用
	//
	void draw();
	
	//
	// 函数：globalWidth()
	// 
	// 目的：整张Altas纹理的宽度
	//
	virtual unsigned int globalWidth();

	//
	// 函数：globalHeight()
	// 
	// 目的：整张Altas纹理的高度
	//
	virtual unsigned int globalHeight();

	//
	// 函数：color()
	//
	// 目的：返回透明色
	//
	virtual unsigned int color() {return 0xFFFFFFFF;};

protected:

	unsigned int myUniqueId;
	
	//位图矩形的纹理对象包装
	YCITexture2D* myLPTexture; 
};

#endif