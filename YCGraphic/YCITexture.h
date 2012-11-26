#ifndef _INCLUDE_YCITEXTURE_H_
#define _INCLUDE_YCITEXTURE_H_

#include "YCBasic\YCDef.h"

#include "YCRect.h"

/*
 * YCITexture : 纹理抽象类
 */
class YCAPI YCITexture
{
public:

	YCITexture(void);

	virtual ~YCITexture(void);

	//
	// 函数：fetchUV()
	//
	// 目的：得到UV值
	//
	void fetchUV(float& leftTopU,     float& leftTopV,          // V0         V0 ------ V1
		         float& rightTopU,    float& rightTopV,         // V1           |      |
		         float& leftBottomU,  float& leftBottomV,       // V2           |      |
		         float& rightBottomU, float& rightBottomV);     // V3         V2 ------ V3

	//
	// 函数：rect()
	//
	// 目的：返回纹理所在区域
	//
	const FloatRect* rect() const;

	//
	// 函数：raw()
	//
	// 目的：返回真实纹理LPDIRECT3DTEXTURE9指针，供YCGraphic使用
	//
	virtual void* raw() = 0;	

	//
	// 函数：globalWidth()
	// 
	// 目的：整张Altas纹理的宽度
	//
	virtual unsigned int globalWidth() = 0;

	//
	// 函数：globalHeight()
	// 
	// 目的：整张Altas纹理的高度
	//
	virtual unsigned int globalHeight() = 0;

	//
	// 函数：color()
	//
	// 目的：返回透明色
	//
	virtual unsigned int color() = 0;

protected:	
	
	FloatRect myRect; //图片区域

	float myUV[8]; // UV值  V0,V1,V2,V3
};

#endif

