#include "YCUIDiv.h"


YCUIDiv::YCUIDiv(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
{
}


YCUIDiv::~YCUIDiv(void)
{
}
/*
//
// ������draw()
//
// Ŀ�ģ���ָ�������Ƹ�UIԪ��
//
void YCUIDiv::draw()
{

}
*/
//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// ������validateSubTag(const char* name)
//
// Ŀ�ģ�У���ӿؼ�
//
bool YCUIDiv::validateSubTag(const char* name)
{
	return true;
}