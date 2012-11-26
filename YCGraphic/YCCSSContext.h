#ifndef _INCLUDE_YCCSSCONTEXT_H_
#define _INCLUDE_YCCSSCONTEXT_H_

class YCDList;
class YCIUITag;
class YCCSSNode;

/*
 * YCCSSContext : 针对YCIContainer的CSS上下文
 */
class YCCSSContext
{

public:

	YCCSSContext(void);

	~YCCSSContext(void);

	//
	// 函数：addNode(YCCSSNode* node)
	//
	// 目的：添加YCCSSNode节点
	//
	void addNode(YCCSSNode* node);

	//
	// 函数：clearWithoutDelete()
	//
	// 目的：YCIContainer才持有生命周期
	//      YCUITag及其子节点不持有生命周期
	//
	void clearWithoutDelete();

	//
	// 函数：nodes() const
	//
	// 目的：暴露所有YCCSSNode以供YCIUITag遍历
	//
	const YCDList* nodes() const;

	//
	// 函数：filter(const YCIUITag* parent, const YCIUITag* child)
	//
	// 目的：narrow down CSS上下文
	//
	YCCSSContext* filter(const YCIUITag* parent, const YCIUITag* child) const;

private:

	YCDList *myDList;
};

#endif