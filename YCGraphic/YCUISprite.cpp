#include "YCUISprite.h"


YCUISprite::YCUISprite(YCUITagMeta* tagMeta)
	: YCUIImg(tagMeta)
{
}


YCUISprite::~YCUISprite(void)
{
}

//
// ������draw()
//
// Ŀ�ģ���ָ�������Ƹ�UIԪ��
//
void YCUISprite::draw()
{

}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// ������validateSubTag(const char* name)
//
// Ŀ�ģ�У���ӿؼ�
//
bool YCUISprite::validateSubTag(const char* name)
{
	return false;
}
