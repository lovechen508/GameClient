#include "YCDialog.h"

YCDialog::YCDialog(const YCUITagMeta* tagMeta)
	: YCIContainer(tagMeta)
{
}

YCDialog::~YCDialog(void)
{
}

//
// ������clone()
//
// Ŀ�ģ���¡��ǰ��
//
YCDialog* YCDialog::clone()
{
	return NULL;
}

//////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////

//
// ������preload()
//
// Ŀ�ģ�׼�����ݵȣ�ÿ�δ򿪴���ǰ����
//
bool YCDialog::preload()
{
	return true;
}
	
//
// ������postunload()
//
// Ŀ�ģ�׼�����ݵȣ�ÿ�ιرմ���ǰ����
//
bool YCDialog::postunload()
{
	return true;
}
