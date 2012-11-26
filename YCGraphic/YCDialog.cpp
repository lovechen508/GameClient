#include "YCDialog.h"

YCDialog::YCDialog(const YCUITagMeta* tagMeta)
	: YCIContainer(tagMeta)
{
}

YCDialog::~YCDialog(void)
{
}

//
// 函数：clone()
//
// 目的：克隆当前类
//
YCDialog* YCDialog::clone()
{
	return NULL;
}

//////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////

//
// 函数：preload()
//
// 目的：准备数据等，每次打开窗口前调用
//
bool YCDialog::preload()
{
	return true;
}
	
//
// 函数：postunload()
//
// 目的：准备数据等，每次关闭窗口前调用
//
bool YCDialog::postunload()
{
	return true;
}
