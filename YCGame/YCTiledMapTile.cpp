#include "YCTiledMapTile.h"


YCTiledMapTile::YCTiledMapTile(void)
{
}


YCTiledMapTile::~YCTiledMapTile(void)
{
}

//
// ������setFlag(unsigned int flag) const
//
// Ŀ�ģ����ñ�־λ
//
void YCTiledMapTile::setFlag(unsigned int flag) const
{

}

//
// ������isEnabled(unsigned int flag)
//
// Ŀ�ģ����Ա�־λ�Ƿ�����
//
bool YCTiledMapTile::isEnabled(unsigned int flag) const
{
	return true;
}
	
//
// ������getIntProperty(unsigned int property) const
//
// Ŀ�ģ�ȡ��Integer��������ֵ
//
int YCTiledMapTile::getIntProperty(unsigned int property) const
{
	return 0;
}
	
//
// ������getFloatProperty(unsigned int property) const
//
// Ŀ�ģ�ȡ��Float��������ֵ
//
float YCTiledMapTile::getFloatProperty(unsigned int property) const
{
	return 0.0f;
}
		
//
// ������getStringProperty(unsigned int property) const
//
// Ŀ�ģ�ȡ���ַ�����������ֵ
//
const char* YCTiledMapTile::getStringProperty(unsigned int property) const
{
	return "";
}
