#include "YCUIDisableState.h"


YCUIDisableState::YCUIDisableState(const YCUITagMeta* tagMeta, YCIUITag* host)
	: YCUITag(tagMeta)
	, myHost(host)
{
}


YCUIDisableState::~YCUIDisableState(void)
{
}

//
// ������bindCSSNode(const YCCSSNode* node)
//
// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
//
void YCUIDisableState::bindCSSNode(const YCCSSNode* node)
{

}

//
// ������getBackground()
//
// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
//
YCIBackground* YCUIDisableState::getBackground()
{
	return NULL;
}

//
// ������getFontColor(unsigned int &hoverColor)
//
// Ŀ�ģ�ȡ��Hover״̬������ɫ
//
bool YCUIDisableState::getFontColor(unsigned int &hoverColor)
{
	return false;
}

//
// ������draw()
//
// Ŀ�ģ������ʽ�Ļ���
//       �Լ����µ�״̬�ͻ���
//       ����ͽ���Ԫ���������
//
void YCUIDisableState::draw()
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
bool YCUIDisableState::validateSubTag(const char* name)
{
	return false;
}