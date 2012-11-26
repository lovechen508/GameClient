#ifndef _INCLUDE_YCDYNAMICUIDOM_H_
#define _INCLUDE_YCDYNAMICUIDOM_H_

#include "YCUITagDef.h"

class YCUIDOM;
class YCDList;
class YCIUITag;

/*
 * YCDynamicUIDOM : UI �����ĵ������ڴ�ģ��
 *
 * ע�ͣ���YCUIDOM���ڴ��й�����YCUIDOM��ui.xml
 *      �����ļ�����ʵ��ӳ��YCDynimicUIDOM��
 *      filename��layout, script, code�ڵ㡣����
 */
class YCDynamicUIDOM
{
public:

	//
	// ������YCDynamicUIDOM* build(const YCUIDOM* uiDom)
	//
	// Ŀ�ģ�
	//
	static YCDynamicUIDOM* build(const YCUIDOM* uiDom);

public:

	YCDynamicUIDOM(void);

	virtual ~YCDynamicUIDOM(void);

	//
	// ������bind2DOM(YCUIDOM* uiDom)
	//
	// Ŀ�ģ��󶨵�uiDOM, ��������������
	//
	void bind2DOM(const YCUIDOM* uiDom);

	//
	// ������getUIDOM()
	//
	// Ŀ�ģ�ȡ��uiDOM
	//
	const YCUIDOM* getUIDOM() const;
	
	//
	// ������setName(const char* name)
	//
	// Ŀ�ģ�����DOM�ڵ���
	//
	void setName(const char* name);

	//
	// ������getName()
	//
	// Ŀ�ģ���ȡDOM�ڵ���
	//
	const char* getName() const;

	//
	// ������setValue(const char* value)
	//
	// Ŀ�ģ�����DOM�ڵ�ֵ
	//
	void setValue(const char* value);

	//
	// ������getValue()
	//
	// Ŀ�ģ���ȡDOM�ڵ�ֵ
	//
	const char* getValue() const;

	//
	// ������getParent()
	//
	// Ŀ�ģ���ȡ��DOM�ڵ���
	//
	YCDynamicUIDOM* getParent();

	//
	// ������setParent(YCUIDOM * parent)
	//
	// Ŀ�ģ����ø�DOM�ڵ���
	//
	void setParent(YCDynamicUIDOM * parent);

	//
	// ������setAttribute(const char* name, const char* value)
	//
	// Ŀ�ģ���������
	//
	void setAttribute(const char* name, const char* value);

	//
	// ������getAttribute(const char* name)
	//
	// Ŀ�ģ���ȡ����ֵ
	//
	const char* getAttribute(const char* name) const;

	//
	// ������getAttributes() const
	//
	// Ŀ�ģ���ȡ�����б�
	//
	const YCDList * getAttributes() const;

	//
	// ������bind2UITag(YCIUITag* uiTag)
	//
	// Ŀ�ģ���DOM�󶨵�UIԪ��
	// 
	void bind2UITag(YCIUITag* uiTag);

	//
	// ������getSubDom()
	//
	// Ŀ�ģ�ȡ���ӽڵ㼯��
	//
	const YCDList * getSubDom() const;
	YCDList * getSubDom();

	//
	// ������addSubDom(YCDynamicUIDOM* dom)
	//
	// Ŀ�ģ����SubDom�ӽڵ�
	//
	void addSubDom(YCDynamicUIDOM* dom);

	//
	// ������removeSubDom(YCDynamicUIDOM* dom)
	//
	// Ŀ�ģ�ɾ��SubDom�ӽڵ�
	//
	YCDynamicUIDOM * removeSubDom(YCDynamicUIDOM* dom);

	//
	// ������updateSubDom(YCDynamicUIDOM* dom)
	//
	// Ŀ�ģ��滻SubDom�ӽڵ�
	//
	bool updateSubDom(YCDynamicUIDOM* old, YCDynamicUIDOM* add);

	//
	// ������removeSubDom(const char* name)
	//
	// Ŀ�ģ�ɾ��SubDom�ӽڵ�
	//
	YCDynamicUIDOM * removeSubDom(const char* name);

private:

	char myTagName[TAG_NAME_LENGTH];

	//
	// ���ԣ�myOriginalDOM
	// 
	// Ŀ�ģ�����ԭ��UIDOM
	//
	const YCUIDOM* myOriginalDOM;

	//
	// ���ԣ�myTagValue
	//
	// Ŀ�ģ����ñ�ǩֵ
	//
	char * myTagValue;

	//
	// ���ԣ�myParent;
	//
	// Ŀ�ģ���Dom�ڵ�
	//
	YCDynamicUIDOM * myParent;

	//
	// ���ԣ�myAttributes
	//
	// Ŀ�ģ������б�
	//
	YCDList * myAttributes;

	//
	// ���ԣ�mySubUIDOM
	//
	// Ŀ�ģ��ӽڵ�
	//
	YCDList * mySubUIDOM;
};

#endif

