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
// 函数：bindCSSNode(const YCCSSNode* node)
//
// 目的：绑定到DOM文档对象模型
//
void YCUIDisableState::bindCSSNode(const YCCSSNode* node)
{

}

//
// 函数：getBackground()
//
// 目的：绑定到DOM文档对象模型
//
YCIBackground* YCUIDisableState::getBackground()
{
	return NULL;
}

//
// 函数：getFontColor(unsigned int &hoverColor)
//
// 目的：取得Hover状态文字颜色
//
bool YCUIDisableState::getFontColor(unsigned int &hoverColor)
{
	return false;
}

//
// 函数：draw()
//
// 目的：完成正式的绘制
//       自己有新的状态就绘制
//       否则就交回元件自身绘制
//
void YCUIDisableState::draw()
{

}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// 函数：validateSubTag(const char* name)
//
// 目的：校验子控件
//
bool YCUIDisableState::validateSubTag(const char* name)
{
	return false;
}