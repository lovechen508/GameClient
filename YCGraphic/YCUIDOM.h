#ifndef _INCLUDE_YCUIDOM_H_
#define _INCLUDE_YCUIDOM_H_

#include <string>

#include "YCUITagDef.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"

class YCDList;
class YCIUITag;

#pragma region YCUIAttribute

struct YCUIAttribute
{
	std::string name;
	std::string value;

	YCUIAttribute(const char* n, const char* v)
		: name(n), value(v)
	{
		SMART_ASSERT(n != NULL);
	}
};

#pragma endregion YCUIAttribute

/*
 * YCUIDOM : UI �����ĵ�����ģ��
 */
class YCUIDOM
{
public:

	YCUIDOM();

	~YCUIDOM();
	
	//
	// ������setFilename(const char* filename)
	//
	// Ŀ�ģ������ļ���
	//
	void setFilename(const char* filename);

	//
	// ������getFilename()
	//
	// Ŀ�ģ���ȡ�ļ���
	//
	const char* getFilename() const;

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
	// ������setParent(YCUIDOM * parent)
	//
	// Ŀ�ģ����ø�DOM�ڵ���
	//
	void setParent(YCUIDOM * parent);

	//
	// ������setScript(const char* filename)
	//
	// Ŀ�ģ����ô��ڸ��ӵ�Lua�ű�
	//
	void setScript(const char* filename);

	//
	// ������getScripts() const
	//
	// Ŀ�ģ���ȡ���ڸ��ӵ�Lua�ű�
	//
	const YCDList* getScripts() const;

	//
	// ������setLayput(const char* filename)
	//
	// Ŀ�ģ����ô��ڸ��ӵ�Layout��ʽ��
	//
	void setLayout(const char* filename);

	//
	// ������getLayouts() const
	//
	// Ŀ�ģ���ȡ���ڸ��ӵ�Layout��ʽ��
	//
	const YCDList* getLayouts() const;

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

	//
	// ������addSubDom(YCUIDOM* dom)
	//
	// Ŀ�ģ����SubDom�ӽڵ�
	//
	void addSubDom(YCUIDOM* dom);

	//
	// ������removeSubDom(YCUIDOM* dom)
	//
	// Ŀ�ģ�ɾ��SubDom�ӽڵ�
	//
	YCUIDOM * removeSubDom(YCUIDOM* dom);

	//
	// ������updateSubDom(YCUIDOM* dom)
	//
	// Ŀ�ģ��滻SubDom�ӽڵ�
	//
	bool updateSubDom(YCUIDOM* old, YCUIDOM* add);

	//
	// ������removeSubDom(const char* name)
	//
	// Ŀ�ģ�ɾ��SubDom�ӽڵ�
	//
	YCUIDOM * removeSubDom(const char* name);

private:

	char * myFilename;

	char myTagName[TAG_NAME_LENGTH];

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
	YCUIDOM * myParent;

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
	
	//
	// ���ԣ�myScriptFiles
	//
	// ˵����Script ��ǩ
	//
	YCDList * myScriptFiles;

	//
	// ���ԣ�myLayoutFiles
	//
	// ˵����Layout ��ǩ
	//
	YCDList * myLayoutFiles;
};

#endif