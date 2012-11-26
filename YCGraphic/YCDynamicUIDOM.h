#ifndef _INCLUDE_YCDYNAMICUIDOM_H_
#define _INCLUDE_YCDYNAMICUIDOM_H_

#include "YCUITagDef.h"

class YCUIDOM;
class YCDList;
class YCIUITag;

/*
 * YCDynamicUIDOM : UI 界面文档对象内存模型
 *
 * 注释：从YCUIDOM在内存中构建，YCUIDOM是ui.xml
 *      配置文件的真实反映，YCDynimicUIDOM无
 *      filename，layout, script, code节点。。。
 */
class YCDynamicUIDOM
{
public:

	//
	// 函数：YCDynamicUIDOM* build(const YCUIDOM* uiDom)
	//
	// 目的：
	//
	static YCDynamicUIDOM* build(const YCUIDOM* uiDom);

public:

	YCDynamicUIDOM(void);

	virtual ~YCDynamicUIDOM(void);

	//
	// 函数：bind2DOM(YCUIDOM* uiDom)
	//
	// 目的：绑定到uiDOM, 不持有生命周期
	//
	void bind2DOM(const YCUIDOM* uiDom);

	//
	// 函数：getUIDOM()
	//
	// 目的：取得uiDOM
	//
	const YCUIDOM* getUIDOM() const;
	
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
	// 函数：getParent()
	//
	// 目的：获取父DOM节点名
	//
	YCDynamicUIDOM* getParent();

	//
	// 函数：setParent(YCUIDOM * parent)
	//
	// 目的：设置父DOM节点名
	//
	void setParent(YCDynamicUIDOM * parent);

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
	YCDList * getSubDom();

	//
	// 函数：addSubDom(YCDynamicUIDOM* dom)
	//
	// 目的：添加SubDom子节点
	//
	void addSubDom(YCDynamicUIDOM* dom);

	//
	// 函数：removeSubDom(YCDynamicUIDOM* dom)
	//
	// 目的：删除SubDom子节点
	//
	YCDynamicUIDOM * removeSubDom(YCDynamicUIDOM* dom);

	//
	// 函数：updateSubDom(YCDynamicUIDOM* dom)
	//
	// 目的：替换SubDom子节点
	//
	bool updateSubDom(YCDynamicUIDOM* old, YCDynamicUIDOM* add);

	//
	// 函数：removeSubDom(const char* name)
	//
	// 目的：删除SubDom子节点
	//
	YCDynamicUIDOM * removeSubDom(const char* name);

private:

	char myTagName[TAG_NAME_LENGTH];

	//
	// 属性：myOriginalDOM
	// 
	// 目的：设置原生UIDOM
	//
	const YCUIDOM* myOriginalDOM;

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
	YCDynamicUIDOM * myParent;

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
};

#endif

