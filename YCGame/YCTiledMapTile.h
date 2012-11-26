#ifndef _INCLUDE_YCTILEDMAPTILE_H_
#define _INCLUDE_YCTILEDMAPTILE_H_

#include "YCGraphic\YCIMapTile.h"

/*
 * YCTiledMapTile: 地图块抽象
 *
 * 注释：
 *
 */
class YCTiledMapTile :
	public YCIMapTile
{
public:

	YCTiledMapTile(void);

	virtual ~YCTiledMapTile(void);
	
	//
	// 函数：setFlag(unsigned int flag) const
	//
	// 目的：设置标志位
	//
	virtual void setFlag(unsigned int flag) const;

	//
	// 函数：isEnabled(unsigned int flag)
	//
	// 目的：测试标志位是否设置
	//
	virtual bool isEnabled(unsigned int flag) const;

	
	//
	// 函数：getIntProperty(unsigned int property) const
	//
	// 目的：取得Integer类型属性值
	//
	virtual int getIntProperty(unsigned int property) const;
	
	//
	// 函数：getFloatProperty(unsigned int property) const
	//
	// 目的：取得Float类型属性值
	//
	virtual float getFloatProperty(unsigned int property) const;
		
	//
	// 函数：getStringProperty(unsigned int property) const
	//
	// 目的：取得字符串类型属性值
	//
	virtual const char* getStringProperty(unsigned int property) const;

private:


};

#endif



