#include "YCUICheckBox.h"

YCUICheckBox::YCUICheckBox(YCUITagMeta* tagMeta)
	: YCIUIToggleButton(tagMeta)
{
}


YCUICheckBox::~YCUICheckBox(void)
{
}

//
// ������draw()
//
// Ŀ�ģ���ָ�������Ƹ�UIԪ��
//
void YCUICheckBox::draw()
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
bool YCUICheckBox::validateSubTag(const char* name)
{
	return false;
}