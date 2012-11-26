#ifndef _INCLUDE_YCIMAPTILE_H_
#define _INCLUDE_YCIMAPTILE_H_

class YCDList;
class YCIMapObject;

/*
 * YCIMapTile : 地图块抽象类
 *
 * 注释：
 *
 */
class YCAPI YCIMapTile
{
public:

	YCIMapTile(void);

	virtual ~YCIMapTile(void);

	//
	// 函数：draw(unsigned int layer, unsigned int timestamp)
	//
	// 目的：画某一层
	//
	void draw(unsigned int layer, unsigned int timestamp);

	//
	// 函数：addObject(YCIMapObject* object)
	//
	// 目的：挂接对象到地图块
	//
	void addObject(YCIMapObject* object);
	
	//
	// 函数：setFlag(unsigned int flag) const
	//
	// 目的：设置标志位
	//
	virtual void setFlag(unsigned int flag) const = 0;

	//
	// 函数：isEnabled(unsigned int flag)
	//
	// 目的：测试标志位是否设置
	//
	virtual bool isEnabled(unsigned int flag) const = 0;
	
	//
	// 函数：getIntProperty(unsigned int property) const
	//
	// 目的：取得Integer类型属性值
	//
	virtual int getIntProperty(unsigned int property) const = 0;
	
	//
	// 函数：getFloatProperty(unsigned int property) const
	//
	// 目的：取得Float类型属性值
	//
	virtual float getFloatProperty(unsigned int property) const = 0;
		
	//
	// 函数：getStringProperty(unsigned int property) const
	//
	// 目的：取得字符串类型属性值
	//
	virtual const char* getStringProperty(unsigned int property) const = 0;

private:

	//
	// 属性：myObjects
	//
	// 目的：挂接到该地图块上的对象，不持有生命周期
	//
	YCDList * myObjects;

};

#endif

