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
 * YCUIDOM : UI 界面文档对象模型
 */
class YCUIDOM
{
public:

	YCUIDOM();

	~YCUIDOM();
	
	//
	// 函数：setFilename(const char* filename)
	//
	// 目的：设置文件名
	//
	void setFilename(const char* filename);

	//
	// 函数：getFilename()
	//
	// 目的：获取文件名
	//
	const char* getFilename() const;

	//
	// 函数：setName(const char* name)
	//
	// 目的：设置DOM节点名
	//
	void setName(const char* name);

	//
	// 函数：getName()
	//
	// 目的：获取DOM节点名
	//
	const char* getName() const;

	//
	// 函数：setValue(const char* value)
	//
	// 目的：设置DOM节点值
	//
	void setValue(const char* value);

	//
	// 函数：getValue()
	//
	// 目的：获取DOM节点值
	//
	const char* getValue() const;

	//
	// 函数：setParent(YCUIDOM * parent)
	//
	// 目的：设置父DOM节点名
	//
	void setParent(YCUIDOM * parent);

	//
	// 函数：setScript(const char* filename)
	//
	// 目的：设置窗口附加的Lua脚本
	//
	void setScript(const char* filename);

	//
	// 函数：getScripts() const
	//
	// 目的：获取窗口附加的Lua脚本
	//
	const YCDList* getScripts() const;

	//
	// 函数：setLayput(const char* filename)
	//
	// 目的：设置窗口附加的Layout样式表
	//
	void setLayout(const char* filename);

	//
	// 函数：getLayouts() const
	//
	// 目的：获取窗口附加的Layout样式表
	//
	const YCDList* getLayouts() const;

	//
	// 函数：setAttribute(const char* name, const char* value)
	//
	// 目的：设置属性
	//
	void setAttribute(const char* name, const char* value);

	//
	// 函数：getAttribute(const char* name)
	//
	// 目的：获取属性值
	//
	const char* getAttribute(const char* name) const;

	//
	// 函数：getAttributes() const
	//
	// 目的：获取属性列表
	//
	const YCDList * getAttributes() const;

	//
	// 函数：bind2UITag(YCIUITag* uiTag)
	//
	// 目的：将DOM绑定到UI元件
	// 
	void bind2UITag(YCIUITag* uiTag);

	//
	// 函数：getSubDom()
	//
	// 目的：取得子节点集合
	//
	const YCDList * getSubDom() const;

	//
	// 函数：addSubDom(YCUIDOM* dom)
	//
	// 目的：添加SubDom子节点
	//
	void addSubDom(YCUIDOM* dom);

	//
	// 函数：removeSubDom(YCUIDOM* dom)
	//
	// 目的：删除SubDom子节点
	//
	YCUIDOM * removeSubDom(YCUIDOM* dom);

	//
	// 函数：updateSubDom(YCUIDOM* dom)
	//
	// 目的：替换SubDom子节点
	//
	bool updateSubDom(YCUIDOM* old, YCUIDOM* add);

	//
	// 函数：removeSubDom(const char* name)
	//
	// 目的：删除SubDom子节点
	//
	YCUIDOM * removeSubDom(const char* name);

private:

	char * myFilename;

	char myTagName[TAG_NAME_LENGTH];

	//
	// 属性：myTagValue
	//
	// 目的：设置标签值
	//
	char * myTagValue;

	//
	// 属性：myParent;
	//
	// 目的：父Dom节点
	//
	YCUIDOM * myParent;

	//
	// 属性：myAttributes
	//
	// 目的：属性列表
	//
	YCDList * myAttributes;

	//
	// 属性：mySubUIDOM
	//
	// 目的：子节点
	//
	YCDList * mySubUIDOM;
	
	//
	// 属性：myScriptFiles
	//
	// 说明：Script 标签
	//
	YCDList * myScriptFiles;

	//
	// 属性：myLayoutFiles
	//
	// 说明：Layout 标签
	//
	YCDList * myLayoutFiles;
};

#endif