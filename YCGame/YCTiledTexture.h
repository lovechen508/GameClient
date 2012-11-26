#ifndef _INCLUDE_YCTILEDTEXTURE_H_
#define _INCLUDE_YCTILEDTEXTURE_H_

#include "YCGraphic\YCITexture.h"
#include "YCGraphic\YCIMapObject.h"

class YCTileSet;

/*
 * YCTiledTexture : 分块地图纹理
 *
 * 说明：YCTexture 适配器模式
 *      如果图片的大小就是一个地图块，就是YCTexture挂接到地图上
 *      如果图片的大小大于一个地图块，就是YCTiledTexture挂接到地图上
 *      YCTiledTexture只是提供合适的UV值
 */
class YCTiledTexture 
	: public YCITexture
	, public YCIMapObject
{
public:

	YCTiledTexture(YCTileSet* tileSet, YCITexture* texture);

	virtual ~YCTiledTexture(void);

	//
	// 函数：draw()
	//
	// 目的：在地图上完成自身的绘制
	//
	virtual void draw();

	//
	// 函数：rebindTexture(YCITexture* texture)
	//
	// 目的：重新绑定纹理
	//
	void rebindTexture(YCITexture* texture);

	//
	// 函数：setRect(unsigned int left, unsigned int top)
	//
	// 目的：设置地图块对应位置
	//
	void setRect(unsigned int left, unsigned int top);

	//
	// 函数：raw()
	//
	// 目的：返回真实纹理LPDIRECT3DTEXTURE9指针，供YCGraphic使用
	//
	virtual void* raw();	

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
	virtual unsigned int color();

private:

	//
	// 属性：myTileSet
	//
	// 目的：纹理来源的地图块
	//
	YCTileSet * myTileSet;

	//
	// 属性：myTexture
	//
	// 目的：背后的真实纹理，不持有生命周期
	//
	YCITexture * myTexture;

	//
	// 属性：myLeft, myTop
	//
	// 目的：左上角，长宽由宏定义决定
	//
	unsigned int myLeft;
	unsigned int myTop;

};

#endif

