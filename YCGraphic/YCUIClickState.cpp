#include "YCUIClickState.h"


YCUIClickState::YCUIClickState(const YCUITagMeta* tagMeta, YCIUITag* host)
	: YCUITag(tagMeta)
	, myHost(host)
{
}


YCUIClickState::~YCUIClickState(void)
{
}

//
// ������bindCSSNode(const YCCSSNode* node)
//
// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
//
void YCUIClickState::bindCSSNode(const YCCSSNode* node)
{

}

//
// ������getBackground()
//
// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
//
YCIBackground* YCUIClickState::getBackground()
{
	return NULL;
}

//
// ������getFontColor(unsigned int &hoverColor)
//
// Ŀ�ģ�ȡ��Hover״̬������ɫ
//
bool YCUIClickState::getFontColor(unsigned int &hoverColor)
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
void YCUIClickState::draw()
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
bool YCUIClickState::validateSubTag(const char* name)
{
	return false;
}