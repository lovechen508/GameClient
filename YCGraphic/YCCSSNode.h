#ifndef _INCLUDE_YCCSSNODE_H_
#define _INCLUDE_YCCSSNODE_H_

#include "YCUITagDef.h"

class YCIUITag;
class YCCSSItem;
class YCCSSNodeImpl;

/*
 * YCCSSNode : CSS节点
 *
 *  说明：代表css文件一个{}包围的节点
 *
 *        div {      
 *			width : 100;
 *          height : 200;
 *			background-color : 0xFFCCCCCC;
 *        }
 */ 
class YCCSSNode
{
public:

	//  node 节点伪类
	enum CSS_NODE_TYPE
	{
		NODE_NORMAL  = 1,  //正常
		NODE_HOVER   = 2,  //鼠标进入
		NODE_CLICK   = 3,  //鼠标按下
		NODE_DISABLE = 4,  //失效
	};

public:
	
	//
	// 函数：YCCSSNode(const char* sel)
	//
	// 目的：构造器
	//
	YCCSSNode(const char* sel);
	
	//
	// 函数：~YCCSSNode(void)
	//
	// 目的：析构器
	//
	~YCCSSNode(void);

	//
	// 函数：getNodeType()
	//
	// 目的：该节点是否是伪类类型
	//
	CSS_NODE_TYPE getNodeType() const;
	
	//
	// 函数：getPriority()
	//
	// 目的：取得该节点的CSS优先级
	//
	int getPriority() const;

	//
	// 函数：getSelector()
	//
	// 目的：取得该节点的选择符
	//
	const char* getSelector() const;

	//
	// 函数：apply(YCIUITag* tag) const
	//
	// 目的：应用CSS规则到标签
	//
	void apply(YCIUITag* tag) const;
	
	//
	// 函数：setProperty(YCUITagMeta::E_UITAG_CSS property, YCCSSItem *cssItem)
	//
	// 目的：设置CSS属性
	//
	void setProperty(E_UITAG_CSS property, YCCSSItem* cssItem);

	//
	// 函数：getProperty(E_UITAG_CSS property) const
	//
	// 目的：获取 int 类型属性
	//
	const YCCSSItem* getProperty(E_UITAG_CSS property) const;
	
	//
	// 函数：calculatePriority()
	//
	// 目的：计算该节点的优先级值
	//
	// 注释：
	//       每个ID选择符(如 #id),加0,1,0,0
	//       每个Class选择符(如 .class)、每个属性选择符(如 [attribute=])、每个伪类(如 :hover)加0,0,1,0
	//       每个元素选择符（如p）或伪元素选择符(如 :firstchild)等，加0,0,0,1
	//
	void calculatePriority();

private:

	//
	// 函数：isPseudoType(CSS_NODE_TYPE type) const
	//
	// 目的：该节点是否是某伪类
	//
	bool isPseudoType(CSS_NODE_TYPE type) const;
	
private:

	char* mySelector;    //选择器 div

	CSS_NODE_TYPE myType; //节点类型

	int myPriority;
	
	YCCSSNodeImpl *myNodeImpl;
};

#endif

