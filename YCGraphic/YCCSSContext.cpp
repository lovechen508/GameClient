#include "YCCSSContext.h"

#include "YCCSSNode.h"
#include "YCIUITag.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCException.h"

YCCSSContext::YCCSSContext(void)
	: myDList(new YCDList())
{
}


YCCSSContext::~YCCSSContext(void)
{
	delete myDList;
}

//
// 函数：addNode(YCCSSNode* node)
//
// 目的：添加YCCSSNode节点
//
void YCCSSContext::addNode(YCCSSNode* node)
{
	if (node == NULL)
	{
		throw YCException(2002, "YCCSSContext::addNode添加YCCSSNode节点为空");
	}

	// 计算优先级
	node->calculatePriority();

	myDList->append(node, NULL);
}

//
// 函数：clearWithoutDelete()
//
// 目的：YCIContainer才持有生命周期
//      YCUITag及其子节点不持有生命周期
//
void YCCSSContext::clearWithoutDelete()
{
	if (myDList != NULL)
	{
		myDList->clear();
	}
}

//
// 函数：nodes() const
//
// 目的：暴露所有YCCSSNode以供YCIUITag遍历
//
const YCDList* YCCSSContext::nodes() const
{
	return myDList;
}

//
// 函数：filter(const YCIUITag* parent, const YCIUITag* child)
//
// 目的：narrow down CSS上下文
//
YCCSSContext* YCCSSContext::filter(const YCIUITag* parent, const YCIUITag* child) const
{
	return NULL;
}

