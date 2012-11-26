#include "YCIUITag.h"

#include <map>
#include <regex>
#include <vector>

#include "YCUIDOM.h"
#include "YCCSSNode.h"
#include "YCTagFactory.h"
#include "YCCSSContext.h"
#include "YCUIHoverState.h"
#include "YCUIDomBuilder.h"
#include "YCDynamicUIDOM.h"
#include "YCTextureManager.h"
#include "YCColorBackground.h"
#include "YCImageBackground.h"
#include "YCSplitBackground.h"
#include "YCAnimationManager.h"
#include "YCAnimationBackground.h"

#include "YCLua\YCLua.h"
#include "YCLua\YCLuaContext.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCAny.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCStringHelper.h"

//
// ������ʽƥ������Ϊȫ�ֱ������ӿ칹���ٶ�
//
const static std::regex reTagClass("^(.*)\\.(.*)$", std::regex::icase);   // ƥ�� p.title ����ǩ��
const static std::regex reClassOnly("^\\.(.*)$", std::regex::icase);      // ƥ�� .title �ޱ�ǩ��
const static std::regex reTagId("^(.*)#(.*)$", std::regex::icase);        // ƥ�� p#avatar ����Id����ǩ
const static std::regex reIdOnly("^#(.*)$", std::regex::icase);           // ƥ�� #avatar ����Id������ǩ

//
// ������matchSelector(const char* selector, YCIUITag* tag)
//
// Ŀ�ģ��ñ�ǩTAG�Ƿ�ƥ���selector
//
bool matchSelector(const std::string& leaf, YCIUITag* tag)
{
	SMART_ASSERT(tag != NULL);

	std::string id;
	std::string name;
	std::string clazz;

	if (std::regex_match(leaf, reIdOnly)) 
	{
		id = leaf.substr(1);
		const char* theId = tag->id();
		if (theId == NULL || strcmp(theId, id.c_str()) != 0)
		{
			return false;
		}
	}
	else if (std::regex_match(leaf, reTagId)) 
	{
		int pos = leaf.find_first_of('#');
		name = leaf.substr(0, pos);
		id = leaf.substr(pos+1);
		const char* theId = tag->id();
		if (strcmp(name.c_str(), tag->name()) != 0 
		 || theId == NULL 
		 || strcmp(theId, id.c_str()) != 0)
		{
			return false;
		}
	}
	else if (std::regex_match(leaf, reClassOnly))
	{
		clazz = leaf.substr(1);
		if (!tag->hasClass(clazz.c_str()))
		{
			return false;
		}
	}
	else if (std::regex_match(leaf, reTagClass))
	{
		int pos = leaf.find_first_of('.');
		name = leaf.substr(0, pos);
		clazz = leaf.substr(pos+1);
		if (strcmp(name.c_str(), tag->name()) != 0
			|| !tag->hasClass(clazz.c_str()))
		{
			return false;
		}
	}
	else if (strcmp(leaf.c_str(), tag->name()) != 0)
	{
		return false;
	}
	return true;
}

// ���ʽѡ�� - ���Դ�Сд   
const static std::regex_constants::syntax_option_type fl = std::regex_constants::icase;  

#pragma region YCIUITagImpl

class YCIUITagImpl
{
public:

	YCIUITagImpl()
	{
	}

	~YCIUITagImpl()
	{
	}

	//
	// ������
	//
	// Ŀ�ģ����¹���CSS�����б�
	//
	void clearProperty()
	{
		myProperties.clear();
	}

	//
	// ������setProperty(YCUITagMeta::E_UITAG_CSS property, int priority, bool bValue)
	//
	// Ŀ�ģ����� int ��������
	//
	void setProperty(E_UITAG_CSS property, int priority, bool bValue)
	{
		std::map<E_UITAG_CSS, std::pair<int, YCAny> >::iterator at = myProperties.find(property);
		if (at == myProperties.end())
		{
			myProperties[property] = std::pair<int, YCAny>(priority, bValue);
		}
		else
		{
			std::pair<int, YCAny>& second = at->second;
			if (second.first < priority)
			{
				second.second = bValue;
			}
		}
	}

	//
	// ������setProperty(E_UITAG_CSS property, int priority, int iValue)
	//
	// Ŀ�ģ����� int ��������
	//
	void setProperty(E_UITAG_CSS property, int priority, int iValue)
	{
		std::map<E_UITAG_CSS, std::pair<int, YCAny> >::iterator at = myProperties.find(property);
		if (at == myProperties.end())
		{
			myProperties[property] = std::pair<int, YCAny>(priority, iValue);
		}
		else
		{
			std::pair<int, YCAny>& second = at->second;
			if (second.first < priority)
			{
				second.second = iValue;
			}
		}
	}

	//
	// ������setProperty(E_UITAG_CSS property, int priority, double fValue)
	//
	// Ŀ�ģ����� float ��������
	//
	void setProperty(E_UITAG_CSS property, int priority, double fValue)
	{
		std::map<E_UITAG_CSS, std::pair<int, YCAny> >::iterator at = myProperties.find(property);
		if (at == myProperties.end())
		{
			myProperties[property] = std::pair<int, YCAny>(priority, fValue);
		}
		else
		{
			std::pair<int, YCAny>& second = at->second;
			if (second.first < priority)
			{
				second.second = fValue;
			}
		}
	}

	//
	// ������setProperty(E_UITAG_CSS property, int priority, const char* strValue)
	//
	// Ŀ�ģ����� �ַ��� ��������
	//
	void setProperty(E_UITAG_CSS property, int priority, const char* strValue)
	{
		std::map<E_UITAG_CSS, std::pair<int, YCAny> >::iterator at = myProperties.find(property);
		if (at == myProperties.end())
		{
			myProperties[property] = std::pair<int, YCAny>(priority, std::string(strValue));
		}
		else
		{
			std::pair<int, YCAny>& second = at->second;
			if (second.first < priority)
			{
				second.second = std::string(strValue);
			}
		}
	}

	//
	// ������hasProperty(E_UITAG_CSS property)
	//
	// Ŀ�ģ��Ƿ������ø�����
	//
	bool hasProperty(E_UITAG_CSS property)
	{
		return (myProperties.find(property) != myProperties.end());
	}

	//
	// ������getProperty(E_UITAG_CSS property, bool& bValue)
	//
	// Ŀ�ģ���ȡ int ��������
	//
	bool getProperty(E_UITAG_CSS property, bool& bValue)
	{
		std::map<E_UITAG_CSS, std::pair<int, YCAny> >::iterator at = myProperties.find(property);
		if (at != myProperties.end())
		{
			std::pair<int, YCAny>& pair = at->second;
			const YCAny& anyone = pair.second;
			if (anyone.type() == typeid(bool))
			{
				bValue = any_cast<bool>(anyone);
				return true;
			}
			throw YCException(2002, "YCIUITag::getProperty����ֵ��ΪInteger����");
		}
		return false;
	}

	//
	// ������getProperty(E_UITAG_CSS property, int iValue)
	//
	// Ŀ�ģ���ȡ int ��������
	//
	bool getProperty(E_UITAG_CSS property, int& iValue)
	{
		std::map<E_UITAG_CSS, std::pair<int, YCAny> >::iterator at = myProperties.find(property);
		if (at != myProperties.end())
		{
			std::pair<int, YCAny>& pair = at->second;
			const YCAny& anyone = pair.second;
			if (anyone.type() == typeid(int))
			{
				iValue = any_cast<int>(anyone);
				return true;
			}
			throw YCException(2002, "YCIUITag::getProperty����ֵ��ΪInteger����");
		}
		return false;
	}

	//
	// ������getProperty(E_UITAG_CSS property, double fValue)
	//
	// Ŀ�ģ���ȡ float ��������
	//
	bool getProperty(E_UITAG_CSS property, double& fValue)
	{
		std::map<E_UITAG_CSS, std::pair<int, YCAny> >::iterator at = myProperties.find(property);
		if (at != myProperties.end())
		{
			std::pair<int, YCAny>& pair = at->second;
			const YCAny& anyone = pair.second;
			if (anyone.type() == typeid(float))
			{
				fValue = any_cast<float>(anyone);
				return true;
			}
			throw YCException(2002, "YCIUITag::getProperty����ֵ��Ϊfloat����");
		}
		return false;
	}

	//
	// ������getProperty(E_UITAG_CSS property, char* strValue, unsigned int length)
	//
	// Ŀ�ģ���ȡ �ַ��� ��������
	//
	bool getProperty(E_UITAG_CSS property, char* strValue, unsigned int length)
	{
		std::map<E_UITAG_CSS, std::pair<int, YCAny> >::iterator at = myProperties.find(property);
		if (at != myProperties.end())
		{
			std::pair<int, YCAny>& pair = at->second;
			const YCAny& anyone = pair.second;
			if (anyone.type() == typeid(std::string))
			{
				const std::string& cssValue = any_cast<std::string>(anyone);
				if (cssValue.length() >= length)
				{
					throw YCException(2002, "YCIUITag::getProperty�ַ�����������ֵ̫��");
				}
				memcpy(strValue, cssValue.c_str(), cssValue.length());
				strValue[cssValue.length()] = '\0';
				return true;
			}
			throw YCException(2002, "YCIUITag::getProperty����ֵ��Ϊstring����");
		}
		return false;
	}

	//
	// ������getClass()
	//
	// Ŀ�ģ���ȡ����
	//
	const char* getClass() const
	{
		return myClass.c_str();
	}

	//
	// ������hasClass(const char* name)
	//
	// Ŀ�ģ���ȡ����
	//
	bool hasClass(const char* name) const
	{
		std::vector<std::string> classVector;
		YCStringHelper::split(myClass.c_str(), " ", &classVector);
		for (unsigned int i = 0; i < classVector.size(); ++i)
		{
			if (strcmp(name, classVector[i].c_str()) == 0)
			{
				return true;
			}
		}
		return false;
	}

	//
	// ������addClass(const char* clazz)
	//
	// Ŀ�ģ�����ൽ��ǩ
	//
	void addClass(const char* clazz)
	{
		SMART_ASSERT(clazz != NULL);
		if (myClass.size() > 0)
		{
			myClass += " ";
		}
		myClass += clazz;
	}

	//
	// ������removeClass(const char* clazz)
	//
	// Ŀ�ģ��Ƴ���ӱ�ǩ
	//
	void removeClass(const char* clazz)
	{
		SMART_ASSERT(clazz != NULL);
		int pos = myClass.find_first_of(clazz);
		if (pos != std::string::npos)
		{
			if (pos == 0 || myClass[pos-1] == ' ') //head
			{
				int end = pos+strlen(clazz);
				if (end == myClass.length() || myClass[end] == ' ')
				{
					myClass.replace(pos, end, "");
				}
			}
		}
	}

private:

	std::string myClass;
	
	std::map<E_UITAG_CSS, std::pair<int, YCAny>> myProperties;

};

#pragma endregion YCIUITagImpl

YCIUITag::YCIUITag(const YCUITagMeta* tagMeta)
	: myUITagImpl(new YCIUITagImpl())
	, myContainer(NULL)
	, myParent(NULL)
	, myStatus(UITAG_STATUS_NORMAL)
	, myLayoutContext(NULL)
	, myTagMeta(tagMeta)
	, myFloatMode(FLOAT_NONE)
	, myPositionMode(RELATION_MODE)
	, myDisplayMode(BLOCK_MODE)
	, myBackground(NULL)
	, myAlphaValue(1.0)
	, myVisiable(true)
	, mySubTags(new YCDList())
	, myDynamicUIDOM(NULL)
{
	//
	// Hoverα��
	//
	myHoverState = NULL;

	//
	// ���
	//
	myOriginalWidth = 0;
	myOriginalHeight = -1;
	
	myWidthWithMargin = 0;
	myHeightWithMargin = -1;
	myWidth = 0;			
	myHeight = -1;			
	myClientWidth = 0;    
	myClientHeight = -1;   

	//
	// λ��
	//
	myPositionX = INT_MIN;
	myPositionY = INT_MIN;

	myScreenPositionX = 0;
	myScreenPositionY = 0;

	//
	// ���ԣ���߾�
	//
	myMarginTop = 0;
	myMarginLeft = 0;
	myMarginRight = 0;
	myMarginBottom = 0;

	//
	// ���ԣ��߿��
	//
	myBorderTop = 0;
	myBorderLeft = 0;
	myBorderRight = 0;
	myBorderBottom = 0;

	//
	// ���ԣ��߿���ɫ
	//
	myBorderTopColor = 0;
	myBorderLeftColor = 0;
	myBorderRightColor = 0;
	myBorderBottomColor = 0;

	//
	// ���ԣ������
	//
	myPaddingTop = 0;
	myPaddingLeft = 0;
	myPaddingRight = 0;
	myPaddingBottom = 0;
}

YCIUITag::~YCIUITag()
{
	SAFE_DELETE(myUITagImpl);
	SAFE_DELETE(myBackground);
	SAFE_DELETE(myHoverState);

	deleteSubTags();
	SAFE_DELETE(mySubTags);
	
	if (myDynamicUIDOM != NULL && 
		myDynamicUIDOM->getParent() == NULL)
	{
		SAFE_DELETE(myDynamicUIDOM);
	}
}

//
// ������initialTagAttribute()
//
// Ŀ�ģ����ñ�ǩ����
//
bool YCIUITag::initialTagAttribute()
{	
	//////////////////////////////////////////////
	//ע������¼�
	//////////////////////////////////////////////
	const char* onOpen = myDynamicUIDOM->getAttribute("onOpen");
	if (onOpen != NULL)
	{
		addListener(UITAG_OPEN_EVENT, onOpen);
	}

	const char* onClose = myDynamicUIDOM->getAttribute("onClose");
	if (onClose != NULL)
	{
		addListener(UITAG_CLOSE_EVENT, onClose);
	}

	const char* onClick = myDynamicUIDOM->getAttribute("onClick");
	if (onClick != NULL)
	{
		addListener(UITAG_CLICK_EVENT, onClick);
	}

	const char* clazz = myDynamicUIDOM->getAttribute("class");
	if (clazz != NULL)
	{
		myUITagImpl->addClass(clazz);
	}

	return true;
}

//
// ������isVisible()
//
// Ŀ�ģ�Ԫ���Ƿ�ɼ�
//
bool YCIUITag::isVisible()
{
	return myVisiable;
}

//
// ������hasStatus(E_UITAG_STATUS status)
//
// Ŀ�ģ�ȡ��Ԫ����״̬
//
bool YCIUITag::hasStatus(E_UITAG_STATUS status)
{
	return (myStatus & status) != 0;
}

//
// ������setStatus(E_UITAG_STATUS status)
//
// Ŀ�ģ�ȡ��Ԫ����״̬
//
void YCIUITag::setStatus(E_UITAG_STATUS status)
{
	if (myStatus == UITAG_STATUS_DISABLED)
	{
		if (status == UITAG_STATUS_NORMAL)
		{
			myStatus = UITAG_STATUS_NORMAL;
		}
	}
	else
	{
		if (status == UITAG_STATUS_DISABLED)
		{
			myStatus = UITAG_STATUS_DISABLED;
		}
		else
		{
			myStatus |= status;
		}
	}
}

//
// ������removeStatus(E_UITAG_STATUS status)
//
// Ŀ�ģ�ȡ��Ԫ����״̬
//
void YCIUITag::removeStatus(E_UITAG_STATUS status)
{
	if (status == UITAG_STATUS_DISABLED)
	{
		myStatus = UITAG_STATUS_DISABLED;
	}
	else
	{
		myStatus ^= status;
	}
}

//
// ������setPosition(int x, int y);
//
// Ŀ�ģ�����Ԫ����λ�ã���Ļ����
//
void YCIUITag::setPosition(int x, int y)
{
	myPositionX = x;
	myPositionY = y;

	setScreenPosition((myParent != NULL) ? myParent->getScreenPositionX() + myPositionX : myPositionX,
		              (myParent != NULL) ? myParent->getScreenPositionY() + myPositionY : myPositionY);

	calculateViewport();
}

//
// ������setScreenPosition(int x, int y);
//
// Ŀ�ģ�����Ԫ���ľ���λ�ã���Ļ����
//
void YCIUITag::setScreenPosition(int x, int y)
{
	myScreenPositionX = x;
	myScreenPositionY = y;
}

//
// ������getPositionX()
//       getPositionY()
//
// Ŀ�ģ�
//      ȡ��Ԫ��λ������
//
int YCIUITag::getPositionX()
{
	return myPositionX;
}

int YCIUITag::getPositionY()
{
	return myPositionY;
}

int YCIUITag::getScreenPositionX()
{
	return myScreenPositionX;
}
	
int YCIUITag::getScreenPositionY()
{
	return myScreenPositionY;
}

//
// ������getWidth();
//		 getHeight();
//		 getOriginalWidth();
//		 getOriginalHeight();
//
// Ŀ�ģ�
//      ȡ��Ԫ���Ŀ��
//
int YCIUITag::getWidthWithMargin()
{
	return myWidthWithMargin;
}

int YCIUITag::getHeightWithMargin()
{
	return myHeightWithMargin;
}

int YCIUITag::getWidth()
{
	return myWidth;
}

int YCIUITag::getHeight()
{
	return myHeight;
}

int YCIUITag::getOriginalWidth()
{
	return myOriginalWidth;
}

int YCIUITag::getClientWidth()
{
	return myClientWidth;
}

int YCIUITag::getClientHeight()
{
	return myClientHeight;
}

int YCIUITag::getOriginalHeight()
{
	return myOriginalHeight;
}

//
// ������getFloatMode()
//
// Ŀ�ģ�
//      ȡ��Ԫ���ĸ���ģʽ
//
E_UITAG_FLOAT_MODE YCIUITag::getFloatMode()
{
	return myFloatMode;
}

//
// ������getDisplayMode()
//
// Ŀ�ģ�
//      ȡ��Ԫ������ʾģʽinline, block
//
E_UITAG_DISPLAY_MODE YCIUITag::getDisplayMode()
{
	return myDisplayMode;
}

//
// ������getPositionMode()
//
// Ŀ�ģ�
//      ȡ��Ԫ���Ķ�λģʽrelative, absolute
//
E_UITAG_POSITION_MODE YCIUITag::getPositionMode()
{
	return myPositionMode;
}

//
// ������render()
//
// Ŀ�ģ���ָ�������Ƹ�UIԪ�������ӱ�ǩ
//
void YCIUITag::render()
{
	if ((myStatus & UITAG_STATUS_NORMAL) != 0)
	{
		draw();
				
		const IntRect& old = gSprite->narrowViewport(myViewPort);
			for (Item_List_T* item = mySubTags->begin();
				 item != mySubTags->end();
				 item = mySubTags->next(item))
			{
				((YCIUITag*)mySubTags->payload(item))->render();
			}
		gSprite->recoverViewport(old);
	}
}

//
// ������name() const
//
// Ŀ�ģ���ȡ��ǩ��
//
const char* YCIUITag::name() const
{
	return myTagMeta->name();
}

//
// ������id() const
//
// Ŀ�ģ���ȡid��
//
const char* YCIUITag::id() const
{
	return myDynamicUIDOM->getAttribute("id");
}

//
// ������parent() const
//
// Ŀ�ģ���ȡ���ڵ�
//
YCIUITag* YCIUITag::parent() const
{
	return myParent;
}

//
// ������getClass() const
//
// Ŀ�ģ���ȡ����
//      ͨ��CSS��addClass��removeClass����
//      �������ܷ����仯
//
const char* YCIUITag::getClass() const
{
	return myUITagImpl->getClass();
}

//
// ������bind2DynamicDOM(const YCDynamicUIDOM* dom)
//
// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
//
void YCIUITag::bind2DynamicDOM(YCDynamicUIDOM* dom)
{
	SMART_ASSERT(myDynamicUIDOM == NULL);

	if (dom == NULL)
	{
		throw YCException(2002, "YCIUITag::bind2DynamicDOM����domΪNULL");
	}

	myDynamicUIDOM = dom;
}


//
// ������bind2CSS(const YCCSSContext* css)
//
// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
//
void YCIUITag::bind2CSS(const YCCSSContext* css)
{
	if (css == NULL)
	{
		throw YCException(2002, "YCIUITag::bind2CSS����cssΪNULL");
	}

	myLayoutContext = css;

	myUITagImpl->clearProperty();

	/////////////////////////////////////////////////
	// ��ȡ����CSS����
	/////////////////////////////////////////////////
	const YCDList* nodes = myLayoutContext->nodes();
	for (const Item_List_T* item = nodes->begin();
		 item != nodes->end();
		 item = nodes->next(item))
	{
		const YCCSSNode* node = (const YCCSSNode*)nodes->payload(item);
		std::vector<std::string> selectors;
		const char* strSelector = node->getSelector();
		YCStringHelper::split(strSelector, " ", &selectors);

		const std::string& hovertest = selectors.back();

		// 1. �Ƿ�hoverα��
		bool isHover = (YCCSSNode::NODE_HOVER == node->getNodeType());
		const std::string& leaf = isHover
                    			? hovertest.substr(0, hovertest.length()-6)
								: hovertest;

		// 2. ƥ��Ҷ�ڵ�Ԫ��
		if (!matchSelector(leaf, this))
		{
			continue;
		}
		
		// 3. ƥ������·��
		bool matchPath = true;
		if (selectors.size() > 1)
		{
			YCIUITag *ancestor = myParent;
			for (int i = (selectors.size()-2); i>=0 && matchPath; --i)
			{
				const std::string& selector = selectors[i];
				if (ancestor == NULL)
				{
					matchPath = false;
				}
				while (matchPath && !matchSelector(selector, ancestor))
				{
					ancestor = ancestor->parent();
					if (ancestor == NULL)
					{
						matchPath = false;
					}
				}
			}
		}

		if (matchPath)
		{
			if (isHover)
			{
				//Ŀǰһ����ǩֻ֧��һ��hover�ڵ�
				SMART_ASSERT(myHoverState == NULL); 
				myHoverState = new YCUIHoverState(myTagMeta, this);
				myHoverState->bindCSSNode(node);
				LOG_TRACE("[" << name() << "] YCIUITag::bind2CSS ƥ�䵽 Hoverα�ࣺ" << strSelector);
			}
			else
			{
				node->apply(this);
				LOG_TRACE("[" << name() << "] YCIUITag::bind2CSS ƥ�䵽�ڵ㣺" << strSelector);
			}
		}
	}

	//////////////////////////////////////////////
	// ȡ�ø���ģʽ
	//////////////////////////////////////////////
	fetchFloatMode();

	//////////////////////////////////////////////
	// ȡ�ö�λģʽ
	//////////////////////////////////////////////
	fetchPositionMode();

	//////////////////////////////////////////////
	// ȡ�ô��ڿ���
	//////////////////////////////////////////////
	fetchWidthAndHeight();

	//////////////////////////////////////////////
	// ȷ��������ʼλ��
	//////////////////////////////////////////////
	fetchPosition();

	//////////////////////////////////////////////
	// ȷ����ʾģʽ
	//////////////////////////////////////////////
	fetchDisplayMode();

	//////////////////////////////////////////////
	// �����Զ���CSS����
	//////////////////////////////////////////////
	extraCSS();
	
	//////////////////////////////////////////////
	// �ɼ��Դ���
	//////////////////////////////////////////////
	if (myDisplayMode == NONE_MODE)
	{
		myVisiable = false;
	}
	
	//////////////////////////////////////////////
	// �����ӽڵ�
	//////////////////////////////////////////////
	for (Item_List_T* item = mySubTags->begin();
			item != mySubTags->end();
			item = mySubTags->next(item))
	{
		YCIUITag* subTag = (YCIUITag*)mySubTags->payload(item);
		subTag->bind2CSS(myLayoutContext);
	}
}

//
// ������setContainer(YCIContainer* container)
//
// Ŀ�ģ���������
//
void YCIUITag::setContainer(YCIContainer* container)
{
	SMART_ASSERT(container != NULL);
	myContainer = container;
}

//
// ������setParent(YCIUITag* parent)
//
// Ŀ�ģ����ø���ǩ
//
void YCIUITag::setParent(YCIUITag* parent)
{
	SMART_ASSERT(parent != NULL);
	myParent = parent;
}
	
//
// ������hoverOn()
//
// Ŀ�ģ�������ϸ���hoverα���޸�CSS����
//       ��HOVERON_EVENT�¼�������֮ǰִ��
//
void YCIUITag::hoverOn()
{

}

//
// ������hoverOff()
//
// Ŀ�ģ�����Ƴ����Ƴ�hoverα���޸ĵ�CSS����
//       ��HOVEROFF_EVENT�¼�������֮��ִ��
//
void YCIUITag::hoverOff()
{

}

//
// ������disable()
//
// Ŀ�ģ�ʹԪ��ʧЧ
//
void YCIUITag::disable()
{
	setStatus(UITAG_STATUS_DISABLED);
}

//
// ������enable()
//
// Ŀ�ģ�ʹԪ����Ч
//
void YCIUITag::enable()
{
	removeStatus(UITAG_STATUS_DISABLED);
}

//
// ������show()
//
// Ŀ�ģ�ʹԪ���ɼ�
//
void YCIUITag::show()
{
	myVisiable = false;
	myWidth = myOriginalWidth;
	myHeight = myOriginalHeight;
}
	
//
// ������hide()
//
// Ŀ�ģ�ʹԪ�����ɼ�
//
void YCIUITag::hide()
{
	myVisiable = false;
	myWidth = 0;
	myHeight = 0;
}

//
// ������move(E_UITAG_MOVESPEED speed, int targetX, int targetY)
//
// Ŀ�ģ�Ԫ����ָ���ٶ��ƶ���ָ��λ��
//
void YCIUITag::move(E_UITAG_MOVESPEED speed, int targetX, int targetY)
{

}

//
// ������hasClass(const char* clazz)
//
// Ŀ�ģ��ñ�ǩ�Ƿ��и�clazz
//
bool YCIUITag::hasClass(const char* clazz)
{
	SMART_ASSERT(clazz != NULL);
	return myUITagImpl->hasClass(clazz);  // todo...
}

//
// ������addClass(const char* clazz)
//
// Ŀ�ģ����ñ�ǩ����class
//
void YCIUITag::addClass(const char* clazz)
{
	SMART_ASSERT(clazz != NULL);
	myUITagImpl->addClass(clazz);
	bind2CSS(myLayoutContext);
}

//
// ������removeClass(const char* clazz)
//
// Ŀ�ģ��Ӹñ�ǩ�Ƴ�class
//
void YCIUITag::removeClass(const char* clazz)
{
	SMART_ASSERT(clazz != NULL);
	myUITagImpl->removeClass(clazz);
	bind2CSS(myLayoutContext);
}

//
// ������css(const char* property, const char* value)
//
// Ŀ�ģ�����CSS����ֵ
//
void YCIUITag::css(const char* property, const char* value)
{
	SMART_ASSERT(property != NULL);
	SMART_ASSERT(value != NULL);
	E_UITAG_CSS css = YCUITagHelper::Instance().getPropertyEnum(property);
	if (validateProperty(css))
	{
		E_UITAG_PROPERTY_TYPE type = YCUITagHelper::Instance().getCSSType(css);
		if      (type == TAG_INT) 	  setProperty(css, INLINE_PRIORITY, atoi(value));
		else if (type == TAG_DOUBLE)  setProperty(css, INLINE_PRIORITY, atof(value));
		else if (type == TAG_STRING)  setProperty(css, INLINE_PRIORITY, value);
		else if (type == TAG_BOOLEAN) setProperty(css, INLINE_PRIORITY, strcmp("true", value) == 0 ? true : false);
	}
}

//
// ������attr(const char* attribute, const char* value)
//
// Ŀ�ģ���������ֵ
//
void YCIUITag::attr(const char* attribute, const char* value)
{
	throw YCException(2002, "YCIUITag::attr��δʵ�֣�");
}

//
// ������html(const char* html)
//
// Ŀ�ģ�������ֵ���ӱ�ǩ
//
void YCIUITag::html(const char* html)
{
	YCUIDOM* newDOM = YCUIDomBuilder::Parse(html);
}

//
// ������append(const char* html)
//
// Ŀ�ģ��������ӱ�ǩ
//
void YCIUITag::append(const char* html)
{
	std::auto_ptr<YCUIDOM> appendDom(YCUIDomBuilder::Parse(html));
	if (appendDom.get() != NULL)
	{
		////////////////////////////////
		// ���� Tag
		////////////////////////////////
		const char* tagName = appendDom->getName();
		LOG_DEBUG("YCIUITag::append����ڵ�: " << tagName);
		if (strcmp(tagName, "code") == 0)
		{
			try 
			{
				YCLua* lua = (YCLua*)YCRegistry::get("lua");
				if (lua == NULL)
				{
					LOG_ERROR("YCIContainer::createSubTags��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
					throw YCException(2002, "YCIContainer::createSubTags��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
				}

				// ����Lua������
				YCLuaContext context;
				context.addContext("container", myContainer);
				context.addContext("this", this);
				lua->execute(&context, appendDom->getValue(), appendDom->getName());
			}
			catch (...)
			{
				throw YCException(2002, "YCIUITag::append����Lua�ű�����δ֪�쳣��");
			}
		}
		else
		{
			try
			{
				if (!validateSubTag(tagName))
				{
					LOG_WARNING("YCIUITag::append " << name() << " δ֪�ӱ�ǩ��"<< tagName);
				}
				else
				{
					YCIUITag* tag = gTagFactory->create(tagName);
					if (tag == NULL)
					{
						LOG_FATAL("YCIUITag::appendδ֪��ǩ����" << tagName);
					}
					else
					{
						tag->setContainer(myContainer);
						tag->setParent(this);
						tag->setValue(appendDom->getValue());

						YCDynamicUIDOM* dynamicUIDOM = YCDynamicUIDOM::build(appendDom.get());
						tag->bind2DynamicDOM(dynamicUIDOM);
						myDynamicUIDOM->addSubDom(dynamicUIDOM);
						tag->initialTagAttribute();
						if (tag->createSubTags(appendDom.get()))
						{ 
							mySubTags->append(tag, NULL);
						}
						else
						{
							SAFE_DELETE(tag);
						}
					}
				}
			}
			catch (YCException& e)
			{
				LOG_WARNING("YCIUITag::createSubTags�����ӱ�ǩʧ�ܣ�" << e.what());
			}
		}		

		////////////////////////////////
		// �Ƿ���Ҫ����
		////////////////////////////////
		if (myLayoutContext != NULL)
		{
			bind2CSS(myLayoutContext);
			layout(true);
		}
	}
}

// 
// ������bind(E_UITAG_EVENT ev, const char* scripts)
//
// Ŀ�ģ����¼�������
//
// ע�ͣ�
//      handler LUA�ű�
//
void YCIUITag::bind(E_UITAG_EVENT ev, const char* scripts)
{
	SMART_ASSERT(scripts != NULL);
	addListener(ev, scripts);
}

// 
// ������unbind(E_UITAG_EVENT ev, void* handler)
//
// Ŀ�ģ��Ƴ��¼�������
//
void YCIUITag::unbind(E_UITAG_EVENT ev)
{
	removeListener(ev);
}

//
// ������toggle(void* odd, void *even)
//
// Ŀ�ģ�toggle�¼�������
//
void YCIUITag::toggle(const char* odd, const char* even)
{

}

//
// ������move(int deltaX, int deltaY)
//
// Ŀ�ģ��ƶ�����
//
void YCIUITag::move(int deltaX, int deltaY)
{
	setPosition(myPositionX + deltaX, myPositionY + deltaY);

	for (Item_List_T* item = mySubTags->begin();
		 item != mySubTags->end();
		 item = mySubTags->next(item))
	{
		YCIUITag* child = (YCIUITag*)mySubTags->payload(item);
		child->move(deltaX, deltaY);
	}
}

//
// ������setProperty(YCUITagMeta::E_UITAG_CSS property, int priority, bool bValue)
//
// Ŀ�ģ����� int ��������
//
void YCIUITag::setProperty(E_UITAG_CSS property, int priority, bool bValue)
{
	if (validateProperty(property))
	{
		myUITagImpl->setProperty(property, priority, bValue);
	}
}

//
// ������setProperty(E_UITAG_CSS property, int priority, int iValue)
//
// Ŀ�ģ����� int ��������
//
void YCIUITag::setProperty(E_UITAG_CSS property, int priority, int iValue)
{
	if (validateProperty(property))
	{
		myUITagImpl->setProperty(property, priority, iValue);
	}
}

//
// ������setProperty(E_UITAG_CSS property, int priority, double fValue)
//
// Ŀ�ģ����� float ��������
//
void YCIUITag::setProperty(E_UITAG_CSS property, int priority, double fValue)
{
	if (validateProperty(property))
	{
		myUITagImpl->setProperty(property, priority, fValue);
	}
}

//
// ������setProperty(E_UITAG_CSS property, int priority, const char* strValue)
//
// Ŀ�ģ����� �ַ��� ��������
//
void YCIUITag::setProperty(E_UITAG_CSS property, int priority, const char* strValue)
{
	if (validateProperty(property))
	{
		myUITagImpl->setProperty(property, priority, strValue);
	}
}

//
// ������hasProperty(E_UITAG_CSS property)
//
// Ŀ�ģ��Ƿ����ø�����
//
bool YCIUITag::hasProperty(E_UITAG_CSS property)
{
	if (validateProperty(property))
	{
		return myUITagImpl->hasProperty(property);
	}
	return false;
}

//
// ������getProperty(E_UITAG_CSS property, bool& bValue)
//
// Ŀ�ģ���ȡ int ��������
//
bool YCIUITag::getProperty(E_UITAG_CSS property, bool& bValue)
{
	if (validateProperty(property))
	{
		return myUITagImpl->getProperty(property, bValue);
	}
	return false;
}

//
// ������getProperty(E_UITAG_CSS property, int iValue)
//
// Ŀ�ģ���ȡ int ��������
//
bool YCIUITag::getProperty(E_UITAG_CSS property, int& iValue)
{
	if (validateProperty(property))
	{
		return myUITagImpl->getProperty(property, iValue);
	}
	return false;
}

//
// ������getProperty(E_UITAG_CSS property, double fValue)
//
// Ŀ�ģ���ȡ float ��������
//
bool YCIUITag::getProperty(E_UITAG_CSS property, double& fValue)
{
	if (validateProperty(property))
	{
		return myUITagImpl->getProperty(property, fValue);
	}
	return false;
}

//
// ������getProperty(E_UITAG_CSS property, char* strValue, int length)
//
// Ŀ�ģ���ȡ �ַ��� ��������
//
bool YCIUITag::getProperty(E_UITAG_CSS property, char* strValue, unsigned int length)
{
	if (validateProperty(property))
	{
		return myUITagImpl->getProperty(property, strValue, length);
	}
	return false;
}

//
// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ�������Ϣ������
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCIUITag::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;
	unsigned int mouseX;
	unsigned int mouseY;

	gInput->getMousePosition(mouseX, mouseY);

	if (isMouseIn(mouseX, mouseY))
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:
			{
				setStatus(UITAG_STATUS_MOUSEDOWN);

				//������������¼�
				const char* click = getListener(UITAG_CLICK_EVENT);
				if (click != NULL)
				{
					YCLua* lua = (YCLua*)YCRegistry::get("lua");
					if (lua == NULL)
					{
						LOG_ERROR("YCIContainer::initialize��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
						throw YCException(2002, "YCIUITag::onMessage��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
					}

					// ����Lua������
					YCLuaContext context;
					context.addContext("container", myContainer);
					context.addContext("this", this);
					lua->execute(&context, click, name());
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				removeStatus(UITAG_STATUS_MOUSEDOWN);
			}
			break;
		case WM_MOUSEMOVE:
			if (!hasStatus(UITAG_STATUS_HOVER))
			{
				setStatus(UITAG_STATUS_HOVER);

				// ������������¼�
				const char* hoverOn = getListener(UITAG_HOVERON_EVENT);
				if (hoverOn != NULL)
				{
					YCLua* lua = (YCLua*)YCRegistry::get("lua");
					if (lua == NULL)
					{
						LOG_ERROR("YCIContainer::onMessage��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
						throw YCException(2002, "YCIUITag::onMessage��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
					}

					// ����Lua������
					YCLuaContext context;
					context.addContext("container", myContainer);
					context.addContext("this", this);
					lua->execute(&context, hoverOn, name());
				}
			}
			break;
		default:
			break;
		}
	}
	else
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:
			{				
				
			}
			break;
		case WM_MOUSEMOVE:
			if (hasStatus(UITAG_STATUS_HOVER))
			{
				removeStatus(UITAG_STATUS_HOVER);

				// ��������Ƴ��¼�
				const char* hoverOff = getListener(UITAG_HOVERON_EVENT);
				if (hoverOff != NULL)
				{
					YCLua* lua = (YCLua*)YCRegistry::get("lua");
					if (lua == NULL)
					{
						LOG_ERROR("YCIContainer::initialize��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
						throw YCException(2002, "YCIUITag::onMessage��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
					}

					// ����Lua������
					YCLuaContext context;
					context.addContext("container", myContainer);
					context.addContext("this", this);
					lua->execute(&context, hoverOff, name());
				}
			}
			break;
		default:
			break;
		}

	}
	
	// ���ӿؼ�������Ϣ
	for (Item_List_T* item = mySubTags->begin();
			item != mySubTags->end();
			item = mySubTags->next(item))
	{
		YCIUITag* tag = (YCIUITag*)mySubTags->payload(item);
		handled |= tag->onMessage(message, wParam, lParam);
	}

	return handled;
}

//
// ������isMouseIn(int mouseX, int mouseY)
// 
// Ŀ�ģ�����Ƿ��ڱ�ǩ�ڲ�
//
bool YCIUITag::isMouseIn(int mouseX, int mouseY)
{
	return (mouseX >= myScreenPositionX && mouseY >= myScreenPositionY 
		&& mouseX <= (myScreenPositionX+myWidth) && mouseY <= (myScreenPositionY+myHeight));
}

///////////////////////////////////////////////////////////////////////////////
// Protected Part
///////////////////////////////////////////////////////////////////////////////

//
// ������createDynamicUIDOM(const YCUIDOM* uiDom)
//
// Ŀ�ģ������ӽڵ��ڴ�Dynamic UIDOM
//
const YCUIDOM* YCIUITag::createDynamicUIDOM(const YCUIDOM* uiDom)
{
	const YCUIDOM* codeDom = NULL;

	const YCDList * subUIDom = uiDom->getSubDom();
	for (const Item_List_T* item = subUIDom->begin();
		 item != subUIDom->end();
		 item = subUIDom->next(item))
	{
		const YCUIDOM* subDom = (const YCUIDOM*)subUIDom->payload(item);
		const char* tagName = subDom->getName();
		LOG_DEBUG("YCIContainer::bindContext�����ӽڵ�: " << uiDom->getName());
		if (strcmp(tagName, "code") == 0)
		{
			if (codeDom != NULL)
			{
				throw YCException(2002, "YCIContainer::bindContext�ӽڵ����ж��code�ڵ㣡", uiDom->getName());
			}

			codeDom = subDom;
		}
		else
		{
			try
			{
				if (!validateSubTag(tagName))
				{
					LOG_WARNING("YCIUITag::createSubTags " << name() << " δ֪�ӱ�ǩ��"<< tagName);
				}
				else
				{
					YCDynamicUIDOM* dynamicUIDOM = YCDynamicUIDOM::build(subDom);
					if (dynamicUIDOM == NULL)
					{
						LOG_WARNING("YCIUITag::createSubTags " << name() << " ����DynamicUIDOMʧ�ܣ�"<< tagName);
					}
					else
					{
						dynamicUIDOM->bind2DOM(subDom);
						myDynamicUIDOM->addSubDom(dynamicUIDOM);
					}
				}
			}
			catch (YCException& e)
			{
				LOG_WARNING("YCIUITag::createSubTags����DynamicUIDOMʧ�ܣ�" << e.what());
			}
		}
	}

	return codeDom;
}

//
// ������createSubTags(const YCUIDOM *uiDom)
//
// Ŀ�ģ������ӱ�ǩ
//
bool YCIUITag::createSubTags(const YCUIDOM *uiDom)
{
	if (uiDom == NULL)
	{
		return true;
	}
	
	////////////////////////////////////
	// �Ƴ���ǰ�Ľڵ㣬����еĻ�
	////////////////////////////////////
	deleteSubTags();

	////////////////////////////////////
	// ����myUIDOM�����ӽڵ��dynamicUIDOM
	////////////////////////////////////
	const YCUIDOM* codeDom = createDynamicUIDOM(uiDom);

	////////////////////////////////////
	// ����myDynamicUIDOM������ǩ
	////////////////////////////////////
	YCDList * subUIDom = myDynamicUIDOM->getSubDom();
	for (Item_List_T* item = subUIDom->begin();
		 item != subUIDom->end();
		 item = subUIDom->next(item))
	{
		YCDynamicUIDOM* dynamicUIDom = (YCDynamicUIDOM*)subUIDom->payload(item);
		const char* nodeName = uiDom->getName();
		LOG_DEBUG("YCIContainer::bindContext�����ӽڵ�: " << dynamicUIDom->getName());
		if (strcmp(nodeName, "code") == 0)
		{
			throw YCException(2002, "YCIUITag::createSubTags��myDynamicUIDOM����code�ڵ㣡");
		}
		else
		{
			try
			{
				const char* tagName = dynamicUIDom->getName();
				if (!validateSubTag(tagName))
				{
					LOG_WARNING("YCIUITag::createSubTags " << name() << " δ֪�ӱ�ǩ��"<< tagName);
				}
				else
				{
					YCIUITag* tag = gTagFactory->create(tagName);
					if (tag == NULL)
					{
						LOG_FATAL("YCIUITag::createSubTagsδ֪��ǩ����" << tagName);
					}
					else
					{
						tag->setContainer(myContainer);
						tag->setParent(this);
						tag->setValue(dynamicUIDom->getValue());

						tag->bind2DynamicDOM(dynamicUIDom);
						tag->initialTagAttribute();
						if (tag->createSubTags(dynamicUIDom->getUIDOM()))
						{ 
							mySubTags->append(tag, NULL);
						}
						else
						{
							SAFE_DELETE(tag);
						}
					}
				}
			}
			catch (YCException& e)
			{
				LOG_WARNING("YCIUITag::createSubTags�����ӱ�ǩʧ�ܣ�" << e.what());
			}
		}
	}

	////////////////////////////////////
	// �����code�ڵ�
	////////////////////////////////////
	if (codeDom != NULL)
	{
		const char* nodeName = codeDom->getName();
		SMART_ASSERT(strcmp(nodeName, "code") == 0);
		
		try 
		{
			YCLua* lua = (YCLua*)YCRegistry::get("lua");
			if (lua == NULL)
			{
				LOG_ERROR("YCIContainer::createSubTags��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
				throw YCException(2002, "YCIContainer::createSubTags��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
			}

			// ����Lua������
			YCLuaContext context;
			context.addContext("container", myContainer);
			context.addContext("this", this);
			lua->execute(&context, codeDom->getValue(), name());
		}
		catch (...)
		{
			throw YCException(2002, "YCIUITag::createSubTags����Lua�ű�����δ֪�쳣��");
		}
	}

	return true;
}

//
// ������deleteSubTags()
//
// Ŀ�ģ�ɾ�������ӿؼ�
//
void YCIUITag::deleteSubTags()
{	
	if (mySubTags != NULL && mySubTags->size() > 0)
	{
		for (Item_List_T *item = mySubTags->begin(); 
			 item != mySubTags->end();
			 item = mySubTags->next(item))
		{
			YCIUITag* tag = (YCIUITag*)mySubTags->payload(item);
			SAFE_DELETE(tag);
		}

		mySubTags->clear();
	}
}

//
// ������fetchFloatMode()
//
// Ŀ�ģ�ȡ��Ԫ������ģʽ
//
void YCIUITag::fetchFloatMode()
{
	char mode[MAX_PATH] = {0};
	if (getProperty(UITAG_FLOAT, mode, MAX_PATH))
	{
		if (strcmp("left", mode) == 0)
		{
			myFloatMode = FLOAT_LEFT;
		}
		else if (strcmp("right", mode) == 0)
		{
			myFloatMode = FLOAT_RIGHT;
		}
	}
}

//
// ������fetchPositionMode()
//
// Ŀ�ģ� ȡ��Ԫ����λģʽ
//
void YCIUITag::fetchPositionMode()
{
	char position[MAX_PATH] = {0};
	if (getProperty(UITAG_POSITION, position, MAX_PATH))
	{
		if (strcmp("absolute", position) == 0)
		{
			if ( (!hasProperty(UITAG_LEFT) || !hasProperty(UITAG_TOP))
		      || (!hasProperty(UITAG_RIGHT) || !hasProperty(UITAG_BOTTOM)) )
			{
				LOG_WARNING("YCIUITag::fetchPositionModeԪ������Ϊ���Զ�λģʽ����δ����(top,left) ����(right, bottom)" << name());
			}
			else
			{
				myPositionMode = OBSOLUTE_MODE;
			}
		}
	}
}

//
// ������fetchPosition()
//
// Ŀ�ģ���CSS��top��left��ȷ��λ��
//
void YCIUITag::fetchPosition()
{
	char position[MAX_PATH] = {0};

	int x = INT_MIN;
	int y = INT_MIN;

	// ���Ͼ��Զ�λ
	if (hasProperty(UITAG_LEFT) && hasProperty(UITAG_TOP))
	{
		if (getProperty(UITAG_LEFT, position, MAX_PATH))
		{
			if (YCStringHelper::isType(position, YCStringHelper::INTEGER))
			{
				x = atoi(position);				
			}
			else
			{
				try
				{
					x = YCUITagHelper::getSpecialValue(position) - myWidthWithMargin/2;
				}
				catch (YCException& e)
				{
					LOG_WARNING("YCIUITag::fetchPosition��ЧUITAG_LEFTֵ" << e.what());
				}
			}
		}

		if (getProperty(UITAG_TOP, position, MAX_PATH))
		{
			if (YCStringHelper::isType(position, YCStringHelper::INTEGER))
			{
				y = atoi(position);				
			}
			else
			{
				try
				{
					y = YCUITagHelper::getSpecialValue(position) - myHeightWithMargin/2;
				}
				catch (YCException& e)
				{
					LOG_WARNING("YCIUITag::fetchPosition��ЧUITAG_TOPֵ" << e.what());
				}
			}
		}
	}

	//���¾��Զ�λ
	else if (hasProperty(UITAG_RIGHT) && hasProperty(UITAG_BOTTOM))
	{
		if (getProperty(UITAG_RIGHT, position, MAX_PATH))
		{
			if (YCStringHelper::isType(position, YCStringHelper::INTEGER))
			{
				x = atoi(position);				
			}
			else
			{
				try
				{
					x = YCUITagHelper::getSpecialValue(position) - myWidthWithMargin/2;
				}
				catch (YCException& e)
				{
					LOG_WARNING("YCIUITag::fetchPosition��ЧUITAG_RIGHTֵ" << e.what());
				}
			}
		}

		if (getProperty(UITAG_BOTTOM, position, MAX_PATH))
		{
			if (YCStringHelper::isType(position, YCStringHelper::INTEGER))
			{
				y = atoi(position);				
			}
			else
			{
				try
				{
					y = YCUITagHelper::getSpecialValue(position) - myHeightWithMargin/2;
				}
				catch (YCException& e)
				{
					LOG_WARNING("YCIUITag::fetchPosition��ЧUITAG_BOTTOMֵ" << e.what());
				}
			}
		}
	}

	setPosition(x + myMarginLeft, y + myMarginTop);	
}

//
// ������calculateViewport()
//
// Ŀ�ģ������ӿڲü���
//
void YCIUITag::calculateViewport()
{
	myViewPort.left = myScreenPositionX + myMarginLeft + myPaddingLeft;
	myViewPort.top = myScreenPositionY + myMarginTop + myPaddingTop;
	myViewPort.right = myViewPort.left + myClientWidth;
	myViewPort.bottom = myViewPort.top + myClientHeight;
}

//
// ������fetchDisplayMode()
//
// Ŀ�ģ� ȡ��Ԫ����ʾģʽ
//
void YCIUITag::fetchDisplayMode()
{
	char display[MAX_PATH] = {0};
	if (getProperty(UITAG_DISPLAY, display, MAX_PATH))
	{
		if (strcmp("none", display) == 0)
		{
			myDisplayMode = NONE_MODE;
		}
		else if (strcmp("inline", display) == 0)
		{
			myDisplayMode = INLINE_MODE;
		}
		else if (strcmp("block", display) == 0)
		{
			myDisplayMode = BLOCK_MODE;
		}
		else
		{
			LOG_WARNING("YCIUITag::fetchDisplayModeԪ��������ʾģʽ[" << name() << "] display = " << display);
		}
	}
}

//
// ������fetchWidthAndHeight()
//
// Ŀ�ģ�ȡ��Ԫ���Ŀ�͸ߣ�������margin����
//      border��padding������width��height֮��
//
void YCIUITag::fetchWidthAndHeight()
{
	if (!getProperty(UITAG_WIDTH, myOriginalWidth))
	{
		throw YCException(2002, "YCIUITag::getWidthAndHeightδ����width����", name());
	}

	if (!getProperty(UITAG_HEIGHT, myOriginalHeight))
	{
		//throw YCException(2002, "YCIUITag::getWidthAndHeightδ����height���ԣ�", name());
		LOG_INFO("YCIUITag::getWidthAndHeightδ����height���ԣ�" << name());
	}
	
	// ��� margin
	myUITagImpl->getProperty(UITAG_MARGIN_TOP, myMarginTop);
	myUITagImpl->getProperty(UITAG_MARGIN_LEFT, myMarginLeft);
	myUITagImpl->getProperty(UITAG_MARGIN_RIGHT, myMarginRight);
	myUITagImpl->getProperty(UITAG_MARGIN_BOTTOM, myMarginBottom);
	
	myWidthWithMargin = myOriginalWidth + myMarginLeft + myMarginRight;
	myHeightWithMargin = myOriginalHeight + myMarginTop + myMarginBottom;

	myWidth = myOriginalWidth;
	myHeight = myOriginalHeight;
	
	// ��� border
	handleBorder(UITAG_BORDER_TOP, myBorderTop, myBorderTopColor);
	handleBorder(UITAG_BORDER_LEFT, myBorderLeft, myBorderLeftColor);
	handleBorder(UITAG_BORDER_RIGHT, myBorderRight, myBorderRightColor);
	handleBorder(UITAG_BORDER_BOTTOM, myBorderBottom, myBorderBottomColor);

	// ��� padding
	myUITagImpl->getProperty(UITAG_PADDING_TOP, myPaddingTop);
	myUITagImpl->getProperty(UITAG_PADDING_LEFT, myPaddingLeft);
	myUITagImpl->getProperty(UITAG_PADDING_RIGHT, myPaddingRight);
	myUITagImpl->getProperty(UITAG_PADDING_BOTTOM, myPaddingBottom);

	// �����ڲ��ɻ��ӿؼ��Ŀ��߶�
	myClientWidth = myOriginalWidth - myPaddingLeft - myPaddingRight;
	myClientHeight = myOriginalHeight - myPaddingTop - myPaddingBottom;
}

//
// ������updateHeight(int height)
//
// Ŀ�ģ�����Ԫ���ĸ�
//
void YCIUITag::updateHeight(int height)
{
	myOriginalHeight = height;
	myHeight = myOriginalHeight;	
	myHeightWithMargin = myOriginalHeight + myMarginTop + myMarginBottom;
	myClientHeight = myOriginalHeight - myPaddingTop - myPaddingBottom;

	//Y�������¶�λ
	fetchPosition();
}

//
// ������buildBackground()
//
// Ŀ�ģ�����Ԫ�ر���
//
// ע�ͣ�
//      Ŀǰ�����ֱ���ģʽ, ���Ʊ������ȼ����ϵ���
//         YCColorBackground
//         YCAnimationBackground
//         YCImageBackground
//         YCSplitBackground  -> �ſ�
//                            -> x����ƽ�� 3��
//                            -> y����ƽ�� 3��
//
void YCIUITag::buildBackground()
{
	YCIBackground* newBackground = NULL;
	
	char background[MAX_PATH] = {0};
	if (getProperty(UITAG_BACKGROUND_COLOR, background, MAX_PATH))
	{
		unsigned int color = 0;
		if (YCStringHelper::parseColor(background, color))
		{
			newBackground = new YCColorBackground(this, color);
		}
		else
		{
			LOG_WARNING("YCIUITag::buildBackground������ɫ����ʧ��, �޷���������ֵ��");
		}
	}
	else if (getProperty(UITAG_BACKGROUND_IMAGE, background, MAX_PATH))
	{
		const char * fileExt = YCFileUtil::GetFileExt(background);
		if (strcmp(fileExt, "ani") == 0)
		{
			YCAnimationManager* animationManager = (YCAnimationManager*)YCRegistry::get("animationManager");
			if (animationManager == NULL)
			{
				LOG_WARNING("YCIUITag::buildBackground������������ʧ��, �޷���ȡYCAnimationManager�����");
			}
			else
			{
				YCIAnimation* animation = animationManager->getAnimation(background);
				if (animation == NULL)
				{
					LOG_WARNING("YCIUITag::buildBackground������������ʧ�ܣ�" << background);
				}
				else
				{
					newBackground = new YCAnimationBackground(this, animation);
				}
			}
		}
		else
		{
			YCTextureManager* textureManager = (YCTextureManager*)YCRegistry::get("textureManager");
			if (textureManager == NULL)
			{
				LOG_WARNING("YCIUITag::buildBackground������һͼƬ����ʧ��, �޷���ȡYCTextureManager�����");
			}
			else
			{
				YCTexture* texture = textureManager->fetch(TEXTURE_UI, background);
				if (texture == NULL)
				{
					LOG_WARNING("YCIUITag::buildBackground������һͼƬ����ʧ�ܣ�" << background);
				}
				else
				{
					newBackground = new YCImageBackground(this, texture);
				}
			}
		}
	}
	else // YCSplitBackground
	{
		YCTextureManager* textureManager = (YCTextureManager*)YCRegistry::get("textureManager");
		if (textureManager == NULL)
		{
			LOG_WARNING("YCIUITag::buildBackground����SplitͼƬ����ʧ��, �޷���ȡYCTextureManager�����");
		}
		else
		{
			(newBackground = YCSplitBackground::Build_9_TileBG(textureManager, this))
			|| (newBackground = YCSplitBackground::Build_LMR3_TileBG(textureManager, this))
			|| (newBackground = YCSplitBackground::Build_TMB3_TileBG(textureManager, this));
		}
	}

	if (newBackground != NULL)
	{
		double alpha = 1.0f;
		if (getProperty(UITAG_BACKGROUND_ALPHA, alpha))
		{
			newBackground->setAlphaValue((float)alpha);
		}
		SAFE_DELETE(myBackground);
		myBackground = newBackground;
	}
}

//
// ������layout()
//
// Ŀ�ģ�����Ԫ������Ĳ���
//
// ע�ͣ�
//      ��ʼ��Ԫ�صĲ���
//
//         ------------------------------------------------------------------------------------
//        |                |                                   |                               |
//        |                |                                   |                               |
//        |                |                                   |                               |
//        |                |                                   |                               |
//        |   YCIUITAG1    |            YCIUITAG2              |          YCIUITAG3            |
//        |                |                                   |                               |
//        |                |                                   |                               |
//        |                |-----------------------------------|                               |
//        |                |                                   |                               |
//        |----------------                                    |                               |
//        |                                                    |                               |
//        |                                                     ------------------------------ |
//        |------------------------ ��[leftTopX,leftTopY]         ��(rightTopX, rightTopY)     |
//        |                        |                               |                           |
//        |                        |                               |                           |
//        |                        |                               |                           |
//        |                        |                               |                           |
//        |                        |                               |    YCIUITAG4              |
//        |      YCIUITAG4         |                               |       float:right         |
//        |                        |                               |                           |
//        |                        |                               |                           |
//        |                        |                                ---------------------------|
//        |                        |                                                           |
//        |------------------------ ��[leftBottomX, leftBottomY]                               |
//        |                                                                                    |
//        |                                                                                    |
//         ------------------------------------------------------------------------------------
//
void YCIUITag::layout(bool resizable)
{
	// ��� background
	buildBackground();
	
	//SMART_ASSERT(myPositionX != 0);
	//SMART_ASSERT(myPositionY != 0);
	
	bool leftFloat = true;
	bool rightFloat = true;
	int leftTopX = myMarginLeft + myPaddingLeft;
	int leftTopY = myMarginTop + myPaddingTop;
	int rightTopX = leftTopX + myClientWidth;
	int leftBottomY = leftTopY;

	//
	// �ڲ��ֹ�����
	// ����һ��һ�еĲ��ַ�ʽ���ϵ��½���
	// ÿһ�еĲ���Ϊ
	//    FLOAT_LEFT_1�� FLOAT_LEFT_2��... FLOAT_LEFT_n ..., 
	//           (FLOAT_NONE, INLINE_MODE), ..., (FLOAT_NONE, BLOCK_MODE), ..., (FLOAT_NONE, INLINE_MODE)
	//    FLOAT_RIGHT_n, ..., FLOAT_RIGHT_2, FLOAT_RIGHT_1
	//
	std::list<YCIUITag*> line;
	for (Item_List_T* item = mySubTags->begin();
		 item != mySubTags->end();
		 item = mySubTags->next(item))
	{
		YCIUITag* tag = (YCIUITag*)mySubTags->payload(item);
		if (tag->isVisible())
		{
			int widthWithMargin = tag->getWidthWithMargin();
			int heightWithMargin = tag->getHeightWithMargin();

			// �Ƿ���Բ���
			if (tag->getPositionMode() == OBSOLUTE_MODE)
			{
				
			}
			else
			{
				bool newLine = false;

				if ((rightTopX-leftTopX)<widthWithMargin)
				{
					newLine = true;

					// ��ʼ��һ��
					line.clear();

					leftTopX = myMarginLeft + myPaddingLeft; //�ص������
					leftTopY = leftBottomY;
					rightTopX = leftTopX + myClientWidth;
					leftBottomY = leftTopY;

					E_UITAG_FLOAT_MODE floatMode = tag->getFloatMode();
					if ( (floatMode == FLOAT_NONE) || (floatMode == FLOAT_LEFT) )
					{
						tag->setPosition(leftTopX, leftTopY);
						leftTopX += widthWithMargin;
					}
					else if (floatMode == FLOAT_RIGHT)
					{
						rightTopX -= widthWithMargin;
						tag->setPosition(rightTopX, leftTopY);
					}
				
					//leftBottomY += heightWithMargin;
					line.push_back(tag);
				}
				else
				{
					E_UITAG_FLOAT_MODE floatMode = tag->getFloatMode();
					E_UITAG_DISPLAY_MODE displayMode = tag->getDisplayMode();

					if (displayMode == BLOCK_MODE)
					{
						if (floatMode == FLOAT_NONE)
						{
							// �����ǰԪ����BLOCK_MODE�����ж�
							// ��ǰline���е�Ԫ���Ƿ���FLOAT_LEFT,FLOAT_RIGHT������FLOAT_NONE,INLINE_MODE
							//  �� �򰲲��ڵ�ǰ��
							//  �� ����������
							for (std::list<YCIUITag*>::iterator it = line.begin(); it != line.end(); ++it)
							{
								YCIUITag* current = (YCIUITag*)*it;
								if (current->getFloatMode() == FLOAT_NONE
								 && current->getDisplayMode() == BLOCK_MODE)
								{
									newLine = true;
									break; //����������ģʽ
								}
							}

							if (newLine)
							{
								line.clear();

								leftTopX = myMarginLeft + myPaddingLeft; //�ص������
								leftTopY = leftBottomY;
								rightTopX = leftTopX + myClientWidth;
								leftBottomY = leftTopY;

								E_UITAG_FLOAT_MODE floatMode = tag->getFloatMode();
								if ( (floatMode == FLOAT_NONE) || (floatMode == FLOAT_LEFT) )
								{
									tag->setPosition(leftTopX, leftTopY);
									leftTopX += widthWithMargin;
								}
								else if (floatMode == FLOAT_RIGHT)
								{
									rightTopX -= widthWithMargin;
									tag->setPosition(rightTopX, leftTopY);
								}
				
								//leftBottomY += heightWithMargin;
								line.push_back(tag);
							}
							else
							{
								tag->setPosition(leftTopX, leftTopY);
								leftTopX += widthWithMargin;
								leftBottomY = max(leftBottomY, (leftTopY+heightWithMargin));
								std::list<YCIUITag*>::iterator it = line.begin();
								//�ҵ�����㣬�����һ��FLOAT_NONE֮��
								for (; it != line.end(); ++it)
								{
									if (((YCIUITag*)*it)->getFloatMode() == FLOAT_RIGHT)
									{
										break;
									}
								}
								line.insert(it, tag);
							}
						}
						else if (floatMode == FLOAT_LEFT)
						{
							// ���뵽���е�FLOAT_LEFT֮��FLOAT_NONE֮ǰ
							std::list<YCIUITag*>::iterator it = line.begin();
							for (; it != line.end(); ++it)
							{
								if (((YCIUITag*)*it)->getFloatMode() != FLOAT_LEFT)
								{
									break;
								}
							}

							if (it == line.end())
							{
								// ��û��FLOAT_NONE��ǩ
								tag->setPosition(leftTopX, leftTopY);
								line.push_back(tag);
							}
							else
							{
								//����FLOAT_NONE��ǩ
								YCIUITag* insertPoint = (YCIUITag*)*it;
								tag->setPosition(insertPoint->getPositionX(), leftTopY);
								line.insert(it, tag);

								while (it != line.end())
								{
									YCIUITag* floatNone = (YCIUITag*)*it;
									floatNone->setPosition(widthWithMargin + floatNone->getPositionX(),
										                   floatNone->getPositionY());
								}						
							}

							leftTopX += widthWithMargin;
							//leftBottomY = max(leftBottomY, (leftTopY+heightWithMargin));

						}
						else if (floatMode == FLOAT_RIGHT)
						{
							// ���뵽���е�FLOAT_NONE֮��FLOAT_RIGHT֮ǰ
							std::list<YCIUITag*>::iterator it = line.begin();
							for (; it != line.end(); ++it)
							{
								if (((YCIUITag*)*it)->getFloatMode() == FLOAT_RIGHT)
								{
									break;
								}
							}

							rightTopX -= widthWithMargin;
							tag->setPosition(rightTopX, leftTopY);
							if (it == line.end())
							{
								// ��û��FLOAT_RIGHT��ǩ						
								line.push_back(tag);
							}
							else
							{
								//����FLOAT_RIGHT��ǩ
								line.insert(it, tag);
							}
							//leftBottomY = max(leftBottomY, (leftTopY+heightWithMargin));
						}					
					}
					// ����������
					else if (displayMode == INLINE_MODE)
					{
						if (floatMode == FLOAT_NONE)
						{
							std::list<YCIUITag*>::iterator it = line.begin();
							for (; it != line.end(); ++it)
							{
								if (((YCIUITag*)*it)->getFloatMode() == FLOAT_RIGHT)
								{
									break;
								}
							}
							tag->setPosition(leftTopX, leftTopY);
							leftTopX += widthWithMargin;
							line.insert(it, tag);							
							//leftBottomY = max(leftBottomY, (leftTopY+heightWithMargin));
						}
						else if (floatMode == FLOAT_LEFT)
						{
							// ���뵽���е�FLOAT_LEFT֮��FLOAT_NONE֮ǰ
							std::list<YCIUITag*>::iterator it = line.begin();
							for (; it != line.end(); ++it)
							{
								if (((YCIUITag*)*it)->getFloatMode() != FLOAT_LEFT)
								{
									break;
								}
							}

							if (it == line.end())
							{
								// ��û��FLOAT_NONE��ǩ
								tag->setPosition(leftTopX, leftTopY);
								line.push_back(tag);
							}
							else
							{
								//����FLOAT_NONE��ǩ
								YCIUITag* insertPoint = (YCIUITag*)*it;
								tag->setPosition(insertPoint->getPositionX(), leftTopY);
								line.insert(it, tag);

								while (it != line.end())
								{
									YCIUITag* floatNone = (YCIUITag*)*it;
									floatNone->setPosition(widthWithMargin + floatNone->getPositionX(),
										                   floatNone->getPositionY());
								}						
							}

							leftTopX += widthWithMargin;
							//leftBottomY = max(leftBottomY, (leftTopY+heightWithMargin));
						}
						else if (floatMode == FLOAT_RIGHT)
						{
							// ���뵽���е�FLOAT_NONE֮��FLOAT_RIGHT֮ǰ
							std::list<YCIUITag*>::iterator it = line.begin();
							for (; it != line.end(); ++it)
							{
								if (((YCIUITag*)*it)->getFloatMode() == FLOAT_RIGHT)
								{
									break;
								}
							}

							rightTopX -= widthWithMargin;
							tag->setPosition(rightTopX, leftTopY);
							if (it == line.end())
							{
								// ��û��FLOAT_RIGHT��ǩ						
								line.push_back(tag);
							}
							else
							{
								//����FLOAT_RIGHT��ǩ
								line.insert(it, tag);
							}
							//leftBottomY = max(leftBottomY, (leftTopY+heightWithMargin));
						}
					}
				}

				// ������Ԫ��
				tag->layout(resizable);

				if (newLine)
				{
					leftBottomY += tag->getHeightWithMargin();
				}
				else
				{
					leftBottomY = max(leftBottomY, (leftTopY+tag->getHeightWithMargin()));
				}
			}			
		}
	}

	if (resizable)
	{
		int newHeight = leftBottomY-myMarginTop+myPaddingTop;

		// ���δ���ø߶ȷ����˱仯
		if (myOriginalHeight != newHeight)
		{
			updateHeight(newHeight);
		}
	}
	else
	{
		// ���δ���ø߶ȣ�������ӿؼ�����
		if (myOriginalHeight < 0)
		{
			updateHeight(leftBottomY-myMarginTop+myPaddingTop);
		}
	}
}

//
// ������onCSSUpdated()
//
// Ŀ�ģ���CSS�����仯ʱ
//
void YCIUITag::onCSSUpdated()
{

}

//
// ������onDOMUpdated()
//
// Ŀ�ģ���DOM�����仯ʱ
//
void YCIUITag::onDOMUpdated()
{
	////////////////////////////////
	// ���¹�����ǩ
	////////////////////////////////
	// TODO ...

	////////////////////////////////
	// �� CSS
	////////////////////////////////
	bind2CSS(myLayoutContext);

	////////////////////////////////
	// ����
	////////////////////////////////
	layout(true);
}

//
// ������draw()
//
// Ŀ�ģ���ָ�������Ƹ�UIԪ������
//
void YCIUITag::draw()
{
	// ������
	YCIBackground* theBackground = myBackground;
	if (hasStatus(UITAG_STATUS_HOVER) && (myHoverState != NULL))
	{
		YCIBackground* hoverBackground = myHoverState->getBackground();
		if (hoverBackground != NULL)
		{
			theBackground = hoverBackground;
		}
	}
	if (theBackground != NULL)
	{
		theBackground->draw(myAlphaValue, 
			                myScreenPositionX, 
							myScreenPositionY, 
							myWidth, myHeight);
	}

	// ���߿�
	drawBorder();

	// �Զ������
	extraDraw();
}

//
// ������handleBorder(E_UITAG_CSS property, int& border, unsigned int& color)
//
// Ŀ�ģ�����CSS�߿���
//
void YCIUITag::handleBorder(E_UITAG_CSS property, int& border, unsigned int& color)
{
	if (property != UITAG_BORDER_TOP 
	 && property != UITAG_BORDER_LEFT
	 && property != UITAG_BORDER_RIGHT 
	 && property != UITAG_BORDER_BOTTOM)
	{
		return;
	}

	if (myUITagImpl->hasProperty(property))
	{	
		char propertyValue[MAX_PATH] = {0};
		std::vector<std::string> borderVector;

		myUITagImpl->getProperty(property, propertyValue, MAX_PATH);
		YCStringHelper::split(propertyValue, " ", &borderVector);
		if (borderVector.size() != 2)
		{
			LOG_WARNING("YCIUITag::layout CSS ���� " << property << " �߿����ô���" << border);
		}
		else
		{
			if (!YCStringHelper::isType(borderVector[0].c_str(), YCStringHelper::INTEGER))
			{
				LOG_WARNING("YCIUITag::layout CSS ���� " << property << " �߿����ô���" << border);
			}
			else
			{				
				if (!YCStringHelper::parseColor(borderVector[1].c_str(), color))
				{
					LOG_WARNING("YCIUITag::layout CSS ���� " << property << " �߿����ô���" << border);
				}
				else
				{
					color += 0xFF000000;
					border = atoi(borderVector[0].c_str());
				}
			}
		}
	}
}

//
// ������drawBorder()
//
// Ŀ�ģ����߿򣬼̳�����Ը��ǣ������ܵ��û����ʵ��
//
void YCIUITag::drawBorder()
{
#ifdef BORDER_TEST

	if (myBorderTop > 0)
	{
		gPrimitive->drawRetangle(myScreenPositionX+myMarginLeft, 
			                     myScreenPositionY+myMarginTop, 
								 myScreenPositionX+myMarginLeft+myWidth, 
								 myScreenPositionY+myMarginTop+myBorderTop, 
			                     myBorderTopColor, myBorderTopColor, myBorderTopColor, myBorderTopColor);
	}
	else
	{
		gPrimitive->drawLine(myScreenPositionX+myMarginLeft, 
			                 myScreenPositionY+myMarginTop, 
							 myScreenPositionX+myMarginLeft+myWidth, 
							 myScreenPositionY+myMarginTop, 
							 0xFFFF0000, 0xFFFF0000);
	}

	if (myBorderLeft > 0)
	{
		gPrimitive->drawRetangle(myScreenPositionX+myMarginLeft, 
			                     myScreenPositionY+myMarginTop, 
								 myScreenPositionX+myMarginLeft+myBorderLeft, 
								 myScreenPositionY+myMarginTop+myHeight, 
			                     myBorderLeftColor, myBorderLeftColor, myBorderLeftColor, myBorderLeftColor);
	}
	else
	{
		gPrimitive->drawLine(myScreenPositionX+myMarginLeft, 
			                 myScreenPositionY+myMarginTop, 
							 myScreenPositionX+myMarginLeft, 
							 myScreenPositionY+myMarginTop+myHeight, 
							 0xFFFF0000, 0xFFFF0000);
	}

	if (myBorderRight > 0)
	{
		gPrimitive->drawRetangle(myScreenPositionX+myMarginLeft+myWidth-myBorderRight, 
			                     myScreenPositionY+myMarginTop, 
								 myScreenPositionX+myMarginLeft+myWidth, 
								 myScreenPositionY+myMarginTop+myHeight, 
			                     myBorderRightColor, myBorderRightColor, myBorderRightColor, myBorderRightColor);
	}
	else
	{
		gPrimitive->drawLine(myScreenPositionX+myMarginLeft+myWidth-1, 
			                 myScreenPositionY+myMarginTop, 
							 myScreenPositionX+myMarginLeft+myWidth-1, 
							 myScreenPositionY+myMarginTop+myHeight, 
			                 0xFFFF0000, 0xFFFF0000);
	}

	if (myBorderBottom > 0)
	{
		gPrimitive->drawRetangle(myScreenPositionX+myMarginLeft, 
			                     myScreenPositionY+myMarginTop+myHeight-myBorderBottom, 
								 myScreenPositionX+myMarginLeft+myWidth, 
								 myScreenPositionY+myMarginTop+myHeight, 
								 myBorderBottomColor, myBorderBottomColor, myBorderBottomColor, myBorderBottomColor);
	}
	else
	{
		gPrimitive->drawLine(myScreenPositionX+myMarginLeft, 
			                 myScreenPositionY+myMarginTop+myHeight-1, 
							 myScreenPositionX+myMarginLeft+myWidth, 
							 myScreenPositionY+myMarginTop+myHeight-1, 
			                 0xFFFF0000, 0xFFFF0000);
	}

#else
	if (myBorderTop > 0)
	{
		gPrimitive->drawRetangle(myScreenPositionX+myMarginLeft, 
			                     myScreenPositionY+myMarginTop, 
								 myScreenPositionX+myMarginLeft+myWidth, 
								 myScreenPositionY+myMarginTop+myBorderTop, 
			                     myBorderTopColor, myBorderTopColor, myBorderTopColor, myBorderTopColor);
	}

	if (myBorderLeft > 0)
	{
		gPrimitive->drawRetangle(myScreenPositionX+myMarginLeft, 
			                     myScreenPositionY+myMarginTop, 
								 myScreenPositionX+myMarginLeft+myBorderLeft, 
								 myScreenPositionY+myMarginTop+myHeight, 
			                     myBorderLeftColor, myBorderLeftColor, myBorderLeftColor, myBorderLeftColor);
	}

	if (myBorderRight > 0)
	{
		gPrimitive->drawRetangle(myScreenPositionX+myMarginLeft+myWidth-myBorderRight, 
			                     myScreenPositionY+myMarginTop, 
								 myScreenPositionX+myMarginLeft+myWidth, 
								 myScreenPositionY+myMarginTop+myHeight, 
			                     myBorderRightColor, myBorderRightColor, myBorderRightColor, myBorderRightColor);
	}

	if (myBorderBottom > 0)
	{
		gPrimitive->drawRetangle(myScreenPositionX+myMarginLeft, 
			                     myScreenPositionY+myMarginTop+myHeight-myBorderBottom, 
								 myScreenPositionX+myMarginLeft+myWidth, 
								 myScreenPositionY+myMarginTop+myHeight, 
			                     myBorderBottomColor, myBorderBottomColor, myBorderBottomColor, myBorderBottomColor);
	}
#endif
}

//
// ������validateProperty(E_UITAG_CSS property)
//
// Ŀ�ģ�У�������Ƿ���Դ�Tag��Ч
//
bool YCIUITag::validateProperty(E_UITAG_CSS property)
{
	if (property != UITAG_INVALID && myTagMeta->isSupportedCSS(property))
	{
		return true;
	}

	LOG_DEBUG("YCIUITag::validateProperty ��ǩ[" << name() << "]��֧������ ��" << property);
	return false;
}

