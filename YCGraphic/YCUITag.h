#ifndef _INCLUDE_YCUITAG_H_
#define _INCLUDE_YCUITAG_H_

#include "YCIUITag.h"
#include "YCUITagDef.h"

/*
 * YCUITag : UI ��ǩ��ȱʡʵ��
 */
class YCUITag :
	public YCIUITag
{
public:
	
	YCUITag(const YCUITagMeta* tagMeta);

	virtual ~YCUITag(void);

	//
	// ������matchAccelerateKey()
	//
	// Ŀ�ģ��Ƿ�ƥ���ݼ�
	//
	virtual bool matchAccelerateKey();

	//
	// ������open(YCLuaParameter* context)
	//
	// Ŀ�ģ� ����һ��������ʵ��
	//        ͬʱ�󶨴��ڴ򿪵�����������
	//
	// ע�ͣ�
	//       ÿ�δ�YCUIManager����һ�����ڣ�������һ��������ʵ��
	//       ��֤YCUIDOM�ĳ�ʼ������
	//
	virtual bool open(YCLuaParameter* context);

	//
	// ������close()
	//
	// Ŀ�ģ� �ر�һ��Ԫ��ʵ��
	//
	virtual void close();

protected:

	//
	// ������extraCSS()
	//
	// Ŀ�ģ������ǩ�ض���CSS����
	//
	virtual void extraCSS();

	//
	// ������extraDraw()
	//
	// Ŀ�ģ�UIԪ�����Զ������
	//
	virtual void extraDraw();

private:

	//
	// ������fetchPosition()
	//
	// Ŀ�ģ���CSS��top��left��ȷ��λ��
	//
	virtual void fetchPosition();
};

#endif
