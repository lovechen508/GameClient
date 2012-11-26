#include "YCTiledMapTile.h"


YCTiledMapTile::YCTiledMapTile(void)
{
}


YCTiledMapTile::~YCTiledMapTile(void)
{
}

//
// 函数：setFlag(unsigned int flag) const
//
// 目的：设置标志位
//
void YCTiledMapTile::setFlag(unsigned int flag) const
{

}

//
// 函数：isEnabled(unsigned int flag)
//
// 目的：测试标志位是否设置
//
bool YCTiledMapTile::isEnabled(unsigned int flag) const
{
	return true;
}
	
//
// 函数：getIntProperty(unsigned int property) const
//
// 目的：取得Integer类型属性值
//
int YCTiledMapTile::getIntProperty(unsigned int property) const
{
	return 0;
}
	
//
// 函数：getFloatProperty(unsigned int property) const
//
// 目的：取得Float类型属性值
//
float YCTiledMapTile::getFloatProperty(unsigned int property) const
{
	return 0.0f;
}
		
//
// 函数：getStringProperty(unsigned int property) const
//
// 目的：取得字符串类型属性值
//
const char* YCTiledMapTile::getStringProperty(unsigned int property) const
{
	return "";
}
