#include "YCUITextArea.h"

YCUITextArea::YCUITextArea(YCUITagMeta* tagMeta)
	: YCUITextField(tagMeta)
{
}


YCUITextArea::~YCUITextArea(void)
{
}

//
// ������draw()
//
// Ŀ�ģ���ָ�������Ƹ�UIԪ��
//
void YCUITextArea::draw()
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
bool YCUITextArea::validateSubTag(const char* name)
{
	return false;
}