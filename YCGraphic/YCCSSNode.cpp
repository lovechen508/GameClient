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
	// ������setProperty(YCUITagMeta::E_UITAG_CSS property, YCCSSItem *cssItem)
	//
	// Ŀ�ģ�����CSS����
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
	// ������getProperty(E_UITAG_CSS property) const
	//
	// Ŀ�ģ���ȡ int ��������
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
	// ������apply(int priority, YCIUITag* tag) const
	//
	// Ŀ�ģ�Ӧ��CSS���򵽱�ǩ
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
				throw YCException(2002, "YCCSSNode::applyδ֪��ǩ��");
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

	// �ڵ�����
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
// ������getNodeType()
//
// Ŀ�ģ��ýڵ��Ƿ���α������
//
YCCSSNode::CSS_NODE_TYPE YCCSSNode::getNodeType() const
{
	return myType;
}

//
// ������getPriority()
//
// Ŀ�ģ�ȡ�øýڵ��CSS���ȼ�
//
int YCCSSNode::getPriority() const
{
	return myPriority;
}

//
// ������getSelector()
//
// Ŀ�ģ�ȡ�øýڵ��ѡ���
//
const char* YCCSSNode::getSelector() const
{
	return mySelector;
}

//
// ������apply(YCIUITag* tag) const 
//
// Ŀ�ģ�Ӧ��CSS���򵽱�ǩ
//
void YCCSSNode::apply(YCIUITag* tag) const
{
	SMART_ASSERT(tag != NULL);
	myNodeImpl->apply(myPriority, tag);
}

//
// ������setProperty(E_UITAG_CSS property, YCCSSItem& cssItem)
//
// Ŀ�ģ����� int ��������
//
void YCCSSNode::setProperty(E_UITAG_CSS property, YCCSSItem* cssItem)
{
	myNodeImpl->setProperty(property, cssItem);
}

//
// ������getProperty(E_UITAG_CSS property)
//
// Ŀ�ģ���ȡ int ��������
//
const YCCSSItem* YCCSSNode::getProperty(E_UITAG_CSS property) const
{
	return myNodeImpl->getProperty(property);
}

//
// ������calculatePriority()
//
// Ŀ�ģ�����ýڵ�����ȼ�ֵ
//
// ע�ͣ�
//       ÿ��IDѡ���(�� #id),��0,1,0,0
//       ÿ��Classѡ���(�� .class)��ÿ������ѡ���(�� [attribute=])��ÿ��α��(�� :hover)��0,0,1,0
//       ÿ��Ԫ��ѡ�������p����αԪ��ѡ���(�� :firstchild)�ȣ���0,0,0,1
//
void YCCSSNode::calculatePriority()
{
	//
	// ������ʽƥ������Ϊȫ�ֱ������ӿ칹���ٶ�
	//
	const std::regex reTagClass("^(.*)\\.(.*)$", std::regex::icase);   // ƥ�� p.title ����ǩ��
	const std::regex reClassOnly("^\\.(.*)$", std::regex::icase);      // ƥ�� .title �ޱ�ǩ��
	const std::regex reTagId("^(.*)#(.*)$", std::regex::icase);        // ƥ�� p#avatar ����Id����ǩ
	const std::regex reIdOnly("^#(.*)$", std::regex::icase);           // ƥ�� #avatar ����Id������ǩ

	std::vector<std::string> strVector;
	YCStringHelper::split(mySelector, " ", &strVector);
	if (strVector.size() == 0)
	{
		throw YCException(2002, "YCCSSNode::calculatePriority��Чѡ�����", mySelector);
	}

	if (strVector.size() > 2)
	{
		throw YCException(2002, "YCCSSNode::calculatePriorityĿǰֻ֧������ѡ���", mySelector);
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
// ������isPseudoType(CSS_NODE_TYPE type) const
//
// Ŀ�ģ��ýڵ��Ƿ���ĳα��
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
