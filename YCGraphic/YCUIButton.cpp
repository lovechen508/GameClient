#include "YCUIButton.h"

YCUIButton::YCUIButton(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
{
}

YCUIButton::~YCUIButton()
{
}

//
// ������draw()
//
// Ŀ�ģ���ָ�������Ƹ�UIԪ��
//
void YCUIButton::draw()
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
bool YCUIButton::validateSubTag(const char* name)
{
	return false;
}