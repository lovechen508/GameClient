#include "YCUIRadio.h"

YCUIRadio::YCUIRadio(YCUITagMeta* tagMeta)
	: YCIUIToggleButton(tagMeta)
{
}

YCUIRadio::~YCUIRadio(void)
{
}

//
// ������draw()
//
// Ŀ�ģ���ָ�������Ƹ�UIԪ��
//
void YCUIRadio::draw()
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
bool YCUIRadio::validateSubTag(const char* name)
{
	return false;
}