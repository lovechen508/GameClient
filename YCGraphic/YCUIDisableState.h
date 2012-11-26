#ifndef _INCLUDE_YCUIDISABLESTATE_H_
#define _INCLUDE_YCUIDISABLESTATE_H_

#include "YCUITag.h"

/* 
 * YCUIDisableState : α�࣬ʧЧ״̬
 */
class YCUIDisableState :
	public YCUITag
{
public:

	YCUIDisableState(const YCUITagMeta* tagMeta, YCIUITag* host);

	virtual ~YCUIDisableState(void);

	//
	// ������bindCSSNode(const YCCSSNode* node)
	//
	// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
	//
	virtual void bindCSSNode(const YCCSSNode* node);

	//
	// ������getBackground()
	//
	// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
	//
	YCIBackground* getBackground();

	//
	// ������getFontColor(unsigned int &hoverColor)
	//
	// Ŀ�ģ�ȡ��Hover״̬������ɫ
	//
	bool getFontColor(unsigned int &hoverColor);

	//
	// ������draw()
	//
	// Ŀ�ģ������ʽ�Ļ���
	//       �Լ����µ�״̬�ͻ���
	//       ����ͽ���Ԫ���������
	//
	virtual void draw();

private:

	//
	// ������validateSubTag(const char* name)
	//
	// Ŀ�ģ�У���ӿؼ�
	//
	virtual bool validateSubTag(const char* name);

private:

	//
	// ���ԣ�myContext
	//
	// ˵����Disabled CSS �ڵ�
	//
	const YCCSSNode *myContext;

	//
	// ���ԣ�myHost
	//
	// ˵��������Ԫ��
	//
	YCIUITag* myHost;

	//
	// ���ԣ�myHoverColorEnable
	//
	// Ŀ�ģ��Ƿ�������Hover��ɫ
	//
	bool myColorEnable;

	//
	// ���ԣ�myHoverColor
	//
	// Ŀ�ģ��������������ɫ
	//
	unsigned int myColor;
};

#endif
