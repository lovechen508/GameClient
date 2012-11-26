#ifndef _INCLUDE_YCUIRADIO_H_
#define _INCLUDE_YCUIRADIO_H_

#include "YCIUIToggleButton.h"

class YCUIRadio :
	public YCIUIToggleButton
{
public:

	YCUIRadio(YCUITagMeta* tagMeta);

	virtual ~YCUIRadio(void);

	//
	// ������draw()
	//
	// Ŀ�ģ���ָ�������Ƹ�UIԪ��
	//
	void draw();
		
private:

	//
	// ������validateSubTag(const char* name)
	//
	// Ŀ�ģ�У���ӿؼ�
	//
	virtual bool validateSubTag(const char* name);
};

#endif
