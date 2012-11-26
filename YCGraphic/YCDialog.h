#ifndef _INCLUDE_YCDIALOG_H_
#define _INCLUDE_YCDIALOG_H_

#include "YCIContainer.h"

class YCDialog :
	public YCIContainer
{
public:

	YCDialog(const YCUITagMeta* tagMeta);

	virtual ~YCDialog(void);

private:

	//
	// ������preload()
	//
	// Ŀ�ģ�׼�����ݵȣ�ÿ�δ򿪴���ǰ����
	//
	virtual bool preload();
	
	//
	// ������postunload()
	//
	// Ŀ�ģ�׼�����ݵȣ�ÿ�ιرմ���ǰ����
	//
	virtual bool postunload();

	//
	// ������clone()
	//
	// Ŀ�ģ���¡��ǰ��
	//
	virtual YCDialog* clone();
};

#endif
