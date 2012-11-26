#include "YCCSSNode.h"

#include <map>
#include <regex>

#include "YCIUITag.h"
#include "YCCSSItem.h"

#include "YCBasic\YCAny.h"
#include "YCBasic\YCStringHelper.h"
#include "YCBasic\YCException.h"

#pragma region YCCSSNodeImpl

class YCCSSNodeImpl
{
public:

	YCCSSNodeImpl()
	{
	}

	~YCCSSNodeImpl()
	{
		for (std::map<E_UITAG_CSS, YCCSSItem*>::iterator it = myProperties.begin();
			 it != myProperties.end();
			 ++it)
		{
			delete it->second;
		}
		myProperties.clear();
	}

	//
	// 函数：setProperty(YCUITagMeta::E_UITAG_CSS property, YCCSSItem *cssItem)
	//
	// 目的：设置CSS属性
	//
	void setProperty(E_UITAG_CSS property, YCCSSItem* cssItem)
	{
		std::map<E_UITAG_CSS, YCCSSItem*>::iterator at = myProperties.find(property);
		if (at == myProperties.end())
		{
			myProperties[property] = cssItem;
		}
	}

	//
	// 函数：getProperty(E_UITAG_CSS property) const
	//
	// 目的：获取 int 类型属性
	//
	const YCCSSItem* getProperty(E_UITAG_CSS property) const
	{
		std::map<E_UITAG_CSS, YCCSSItem*>::const_iterator at = myProperties.find(property);
		if (at != myProperties.end())
		{
			return at->second;
		}
		return NULL;
	}

	//
	// 函数：apply(int priority, YCIUITag* tag) const
	//
	// 目的：应用CSS规则到标签
	//
	void apply(int priority, YCIUITag* tag) const
	{
		for (std::map<E_UITAG_CSS, YCCSSItem*>::const_iterator it = myProperties.begin();
			 it != myProperties.end();
			 ++it)
		{
			E_UITAG_PROPERTY_TYPE type = YCUITagHelper::Instance().getCSSType(it->first);
			YCCSSItem *cssItem = (YCCSSItem*)it->second;

			switch (type)
			{
			case TAG_INT :
				tag->setProperty(it->first, priority, cssItem->toInt());
				break;
			case TAG_BOOLEAN :
				tag->setProperty(it->first, priority, cssItem->toBoolean());
				break;
			case TAG_STRING :
				tag->setProperty(it->first, priority, cssItem->toString().c_str());
				break;
			case TAG_DOUBLE :
				tag->setProperty(it->first, priority, (float)cssItem->toDouble());
				break;
			default:
				throw YCException(2002, "YCCSSNode::apply未知标签名");
			}
		}

	}

private:
	
	std::map<E_UITAG_CSS, YCCSSItem*> myProperties;

};

#pragma endregion YCCSSNodeImpl

YCCSSNode::YCCSSNode(const char* sel)
	: myNodeImpl(new YCCSSNodeImpl())
	, myPriority(-1)
	, mySelector(NULL)
	, myType(NODE_NORMAL)
{
	SMART_ASSERT(sel != NULL);
	mySelector = new char[strlen(sel)+1];
	memcpy(mySelector, sel, strlen(sel));
	mySelector[strlen(sel)] = '\0';

	// 节点类型
	if      (isPseudoType(NODE_HOVER)) myType = NODE_HOVER;
	else if (isPseudoType(NODE_CLICK)) myType = NODE_CLICK;
	else if (isPseudoType(NODE_DISABLE)) myType = NODE_DISABLE;
}


YCCSSNode::~YCCSSNode(void)
{
	SAFE_DELETE(mySelector);
	SAFE_DELETE(myNodeImpl);
}

//
// 函数：getNodeType()
//
// 目的：该节点是否是伪类类型
//
YCCSSNode::CSS_NODE_TYPE YCCSSNode::getNodeType() const
{
	return myType;
}

//
// 函数：getPriority()
//
// 目的：取得该节点的CSS优先级
//
int YCCSSNode::getPriority() const
{
	return myPriority;
}

//
// 函数：getSelector()
//
// 目的：取得该节点的选择符
//
const char* YCCSSNode::getSelector() const
{
	return mySelector;
}

//
// 函数：apply(YCIUITag* tag) const 
//
// 目的：应用CSS规则到标签
//
void YCCSSNode::apply(YCIUITag* tag) const
{
	SMART_ASSERT(tag != NULL);
	myNodeImpl->apply(myPriority, tag);
}

//
// 函数：setProperty(E_UITAG_CSS property, YCCSSItem& cssItem)
//
// 目的：设置 int 类型属性
//
void YCCSSNode::setProperty(E_UITAG_CSS property, YCCSSItem* cssItem)
{
	myNodeImpl->setProperty(property, cssItem);
}

//
// 函数：getProperty(E_UITAG_CSS property)
//
// 目的：获取 int 类型属性
//
const YCCSSItem* YCCSSNode::getProperty(E_UITAG_CSS property) const
{
	return myNodeImpl->getProperty(property);
}

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
void YCCSSNode::calculatePriority()
{
	//
	// 正则表达式匹配器设为全局变量，加快构造速度
	//
	const std::regex reTagClass("^(.*)\\.(.*)$", std::regex::icase);   // 匹配 p.title 含标签名
	const std::regex reClassOnly("^\\.(.*)$", std::regex::icase);      // 匹配 .title 无标签名
	const std::regex reTagId("^(.*)#(.*)$", std::regex::icase);        // 匹配 p#avatar 窗口Id带标签
	const std::regex reIdOnly("^#(.*)$", std::regex::icase);           // 匹配 #avatar 窗口Id不带标签

	std::vector<std::string> strVector;
	YCStringHelper::split(mySelector, " ", &strVector);
	if (strVector.size() == 0)
	{
		throw YCException(2002, "YCCSSNode::calculatePriority无效选择符：", mySelector);
	}

	if (strVector.size() > 2)
	{
		throw YCException(2002, "YCCSSNode::calculatePriority目前只支持两级选择符", mySelector);
	}

	if (strVector.size() == 2)
	{
		myPriority = PARENT_PRIORITY;
	}

	const std::string& leaf = strVector.back();
	if (std::regex_match(leaf, reTagClass))
	{
		myPriority += CLASS_PRIORITY + TAG_PRIORITY;
	}
	else if (std::regex_match(leaf, reClassOnly))
	{
		myPriority += CLASS_PRIORITY;
	}
	else if (std::regex_match(leaf, reTagId))
	{
		myPriority += ID_PRIORITY + TAG_PRIORITY;
	}
	else if (std::regex_match(leaf, reIdOnly))
	{
		myPriority += ID_PRIORITY;
	}
	else
	{
		myPriority += TAG_PRIORITY;
	}
}

//////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////

//
// 函数：isPseudoType(CSS_NODE_TYPE type) const
//
// 目的：该节点是否是某伪类
//
bool YCCSSNode::isPseudoType(CSS_NODE_TYPE type) const
{
	bool result = false;
	if     (type == NODE_HOVER)
	{
		const char* pos = strstr(mySelector, ":hover");
		if (pos != NULL && pos == (mySelector+strlen(mySelector)-6))
		{
			result = true;
		}
	}
	else if (type == NODE_CLICK)
	{
		const char* pos = strstr(mySelector, ":click");
		if (pos != NULL && pos == (mySelector+strlen(mySelector)-6))
		{
			result = true;
		}
	}
	else if (type == NODE_DISABLE)
	{
		const char* pos = strstr(mySelector, ":disable");
		if (pos != NULL && pos == (mySelector+strlen(mySelector)-7))
		{
			result = true;
		}
	}

	return result;
}
