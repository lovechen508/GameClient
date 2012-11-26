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
// 正则表达式匹配器设为全局变量，加快构造速度
//
const static std::regex reTagClass("^(.*)\\.(.*)$", std::regex::icase);   // 匹配 p.title 含标签名
const static std::regex reClassOnly("^\\.(.*)$", std::regex::icase);      // 匹配 .title 无标签名
const static std::regex reTagId("^(.*)#(.*)$", std::regex::icase);        // 匹配 p#avatar 窗口Id带标签
const static std::regex reIdOnly("^#(.*)$", std::regex::icase);           // 匹配 #avatar 窗口Id不带标签

//
// 函数：matchSelector(const char* selector, YCIUITag* tag)
//
// 目的：该标签TAG是否匹配该selector
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

// 表达式选项 - 忽略大小写   
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
	// 函数：
	//
	// 目的：重新构造CSS属性列表
	//
	void clearProperty()
	{
		myProperties.clear();
	}

	//
	// 函数：setProperty(YCUITagMeta::E_UITAG_CSS property, int priority, bool bValue)
	//
	// 目的：设置 int 类型属性
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
	// 函数：setProperty(E_UITAG_CSS property, int priority, int iValue)
	//
	// 目的：设置 int 类型属性
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
	// 函数：setProperty(E_UITAG_CSS property, int priority, double fValue)
	//
	// 目的：设置 float 类型属性
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
	// 函数：setProperty(E_UITAG_CSS property, int priority, const char* strValue)
	//
	// 目的：设置 字符串 类型属性
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
	// 函数：hasProperty(E_UITAG_CSS property)
	//
	// 目的：是否已设置该属性
	//
	bool hasProperty(E_UITAG_CSS property)
	{
		return (myProperties.find(property) != myProperties.end());
	}

	//
	// 函数：getProperty(E_UITAG_CSS property, bool& bValue)
	//
	// 目的：获取 int 类型属性
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
			throw YCException(2002, "YCIUITag::getProperty属性值不为Integer类型");
		}
		return false;
	}

	//
	// 函数：getProperty(E_UITAG_CSS property, int iValue)
	//
	// 目的：获取 int 类型属性
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
			throw YCException(2002, "YCIUITag::getProperty属性值不为Integer类型");
		}
		return false;
	}

	//
	// 函数：getProperty(E_UITAG_CSS property, double fValue)
	//
	// 目的：获取 float 类型属性
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
			throw YCException(2002, "YCIUITag::getProperty属性值不为float类型");
		}
		return false;
	}

	//
	// 函数：getProperty(E_UITAG_CSS property, char* strValue, unsigned int length)
	//
	// 目的：获取 字符串 类型属性
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
					throw YCException(2002, "YCIUITag::getProperty字符串类型属性值太长");
				}
				memcpy(strValue, cssValue.c_str(), cssValue.length());
				strValue[cssValue.length()] = '\0';
				return true;
			}
			throw YCException(2002, "YCIUITag::getProperty属性值不为string类型");
		}
		return false;
	}

	//
	// 函数：getClass()
	//
	// 目的：获取类名
	//
	const char* getClass() const
	{
		return myClass.c_str();
	}

	//
	// 函数：hasClass(const char* name)
	//
	// 目的：获取类名
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
	// 函数：addClass(const char* clazz)
	//
	// 目的：添加类到标签
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
	// 函数：removeClass(const char* clazz)
	//
	// 目的：移除类从标签
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
	// Hover伪类
	//
	myHoverState = NULL;

	//
	// 宽高
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
	// 位置
	//
	myPositionX = INT_MIN;
	myPositionY = INT_MIN;

	myScreenPositionX = 0;
	myScreenPositionY = 0;

	//
	// 属性：外边距
	//
	myMarginTop = 0;
	myMarginLeft = 0;
	myMarginRight = 0;
	myMarginBottom = 0;

	//
	// 属性：边框宽
	//
	myBorderTop = 0;
	myBorderLeft = 0;
	myBorderRight = 0;
	myBorderBottom = 0;

	//
	// 属性：边框颜色
	//
	myBorderTopColor = 0;
	myBorderLeftColor = 0;
	myBorderRightColor = 0;
	myBorderBottomColor = 0;

	//
	// 属性：内填充
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
// 函数：initialTagAttribute()
//
// 目的：设置标签属性
//
bool YCIUITag::initialTagAttribute()
{	
	//////////////////////////////////////////////
	//注册基本事件
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
// 函数：isVisible()
//
// 目的：元件是否可见
//
bool YCIUITag::isVisible()
{
	return myVisiable;
}

//
// 函数：hasStatus(E_UITAG_STATUS status)
//
// 目的：取得元件的状态
//
bool YCIUITag::hasStatus(E_UITAG_STATUS status)
{
	return (myStatus & status) != 0;
}

//
// 函数：setStatus(E_UITAG_STATUS status)
//
// 目的：取得元件的状态
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
// 函数：removeStatus(E_UITAG_STATUS status)
//
// 目的：取消元件的状态
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
// 函数：setPosition(int x, int y);
//
// 目的：设置元件的位置，屏幕坐标
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
// 函数：setScreenPosition(int x, int y);
//
// 目的：设置元件的绝对位置，屏幕坐标
//
void YCIUITag::setScreenPosition(int x, int y)
{
	myScreenPositionX = x;
	myScreenPositionY = y;
}

//
// 函数：getPositionX()
//       getPositionY()
//
// 目的：
//      取得元件位置属性
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
// 函数：getWidth();
//		 getHeight();
//		 getOriginalWidth();
//		 getOriginalHeight();
//
// 目的：
//      取得元件的宽高
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
// 函数：getFloatMode()
//
// 目的：
//      取得元件的浮动模式
//
E_UITAG_FLOAT_MODE YCIUITag::getFloatMode()
{
	return myFloatMode;
}

//
// 函数：getDisplayMode()
//
// 目的：
//      取得元件的显示模式inline, block
//
E_UITAG_DISPLAY_MODE YCIUITag::getDisplayMode()
{
	return myDisplayMode;
}

//
// 函数：getPositionMode()
//
// 目的：
//      取得元件的定位模式relative, absolute
//
E_UITAG_POSITION_MODE YCIUITag::getPositionMode()
{
	return myPositionMode;
}

//
// 函数：render()
//
// 目的：在指定处绘制该UI元件及其子标签
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
// 函数：name() const
//
// 目的：获取标签名
//
const char* YCIUITag::name() const
{
	return myTagMeta->name();
}

//
// 函数：id() const
//
// 目的：获取id名
//
const char* YCIUITag::id() const
{
	return myDynamicUIDOM->getAttribute("id");
}

//
// 函数：parent() const
//
// 目的：获取父节点
//
YCIUITag* YCIUITag::parent() const
{
	return myParent;
}

//
// 函数：getClass() const
//
// 目的：获取类名
//      通过CSS的addClass、removeClass操作
//      类名可能发生变化
//
const char* YCIUITag::getClass() const
{
	return myUITagImpl->getClass();
}

//
// 函数：bind2DynamicDOM(const YCDynamicUIDOM* dom)
//
// 目的：绑定到DOM文档对象模型
//
void YCIUITag::bind2DynamicDOM(YCDynamicUIDOM* dom)
{
	SMART_ASSERT(myDynamicUIDOM == NULL);

	if (dom == NULL)
	{
		throw YCException(2002, "YCIUITag::bind2DynamicDOM参数dom为NULL");
	}

	myDynamicUIDOM = dom;
}


//
// 函数：bind2CSS(const YCCSSContext* css)
//
// 目的：绑定到DOM文档对象模型
//
void YCIUITag::bind2CSS(const YCCSSContext* css)
{
	if (css == NULL)
	{
		throw YCException(2002, "YCIUITag::bind2CSS参数css为NULL");
	}

	myLayoutContext = css;

	myUITagImpl->clearProperty();

	/////////////////////////////////////////////////
	// 获取自身CSS属性
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

		// 1. 是否hover伪类
		bool isHover = (YCCSSNode::NODE_HOVER == node->getNodeType());
		const std::string& leaf = isHover
                    			? hovertest.substr(0, hovertest.length()-6)
								: hovertest;

		// 2. 匹配叶节点元素
		if (!matchSelector(leaf, this))
		{
			continue;
		}
		
		// 3. 匹配限制路径
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
				//目前一个标签只支持一个hover节点
				SMART_ASSERT(myHoverState == NULL); 
				myHoverState = new YCUIHoverState(myTagMeta, this);
				myHoverState->bindCSSNode(node);
				LOG_TRACE("[" << name() << "] YCIUITag::bind2CSS 匹配到 Hover伪类：" << strSelector);
			}
			else
			{
				node->apply(this);
				LOG_TRACE("[" << name() << "] YCIUITag::bind2CSS 匹配到节点：" << strSelector);
			}
		}
	}

	//////////////////////////////////////////////
	// 取得浮动模式
	//////////////////////////////////////////////
	fetchFloatMode();

	//////////////////////////////////////////////
	// 取得定位模式
	//////////////////////////////////////////////
	fetchPositionMode();

	//////////////////////////////////////////////
	// 取得窗口宽，高
	//////////////////////////////////////////////
	fetchWidthAndHeight();

	//////////////////////////////////////////////
	// 确定窗口起始位置
	//////////////////////////////////////////////
	fetchPosition();

	//////////////////////////////////////////////
	// 确定显示模式
	//////////////////////////////////////////////
	fetchDisplayMode();

	//////////////////////////////////////////////
	// 处理自定义CSS属性
	//////////////////////////////////////////////
	extraCSS();
	
	//////////////////////////////////////////////
	// 可见性处理
	//////////////////////////////////////////////
	if (myDisplayMode == NONE_MODE)
	{
		myVisiable = false;
	}
	
	//////////////////////////////////////////////
	// 处理子节点
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
// 函数：setContainer(YCIContainer* container)
//
// 目的：设置容器
//
void YCIUITag::setContainer(YCIContainer* container)
{
	SMART_ASSERT(container != NULL);
	myContainer = container;
}

//
// 函数：setParent(YCIUITag* parent)
//
// 目的：设置父标签
//
void YCIUITag::setParent(YCIUITag* parent)
{
	SMART_ASSERT(parent != NULL);
	myParent = parent;
}
	
//
// 函数：hoverOn()
//
// 目的：鼠标移上根据hover伪类修改CSS属性
//       在HOVERON_EVENT事件处理器之前执行
//
void YCIUITag::hoverOn()
{

}

//
// 函数：hoverOff()
//
// 目的：鼠标移出，移除hover伪类修改的CSS属性
//       在HOVEROFF_EVENT事件处理器之后执行
//
void YCIUITag::hoverOff()
{

}

//
// 函数：disable()
//
// 目的：使元件失效
//
void YCIUITag::disable()
{
	setStatus(UITAG_STATUS_DISABLED);
}

//
// 函数：enable()
//
// 目的：使元件有效
//
void YCIUITag::enable()
{
	removeStatus(UITAG_STATUS_DISABLED);
}

//
// 函数：show()
//
// 目的：使元件可见
//
void YCIUITag::show()
{
	myVisiable = false;
	myWidth = myOriginalWidth;
	myHeight = myOriginalHeight;
}
	
//
// 函数：hide()
//
// 目的：使元件不可见
//
void YCIUITag::hide()
{
	myVisiable = false;
	myWidth = 0;
	myHeight = 0;
}

//
// 函数：move(E_UITAG_MOVESPEED speed, int targetX, int targetY)
//
// 目的：元件以指定速度移动到指定位置
//
void YCIUITag::move(E_UITAG_MOVESPEED speed, int targetX, int targetY)
{

}

//
// 函数：hasClass(const char* clazz)
//
// 目的：该标签是否含有该clazz
//
bool YCIUITag::hasClass(const char* clazz)
{
	SMART_ASSERT(clazz != NULL);
	return myUITagImpl->hasClass(clazz);  // todo...
}

//
// 函数：addClass(const char* clazz)
//
// 目的：给该标签增加class
//
void YCIUITag::addClass(const char* clazz)
{
	SMART_ASSERT(clazz != NULL);
	myUITagImpl->addClass(clazz);
	bind2CSS(myLayoutContext);
}

//
// 函数：removeClass(const char* clazz)
//
// 目的：从该标签移除class
//
void YCIUITag::removeClass(const char* clazz)
{
	SMART_ASSERT(clazz != NULL);
	myUITagImpl->removeClass(clazz);
	bind2CSS(myLayoutContext);
}

//
// 函数：css(const char* property, const char* value)
//
// 目的：设置CSS属性值
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
// 函数：attr(const char* attribute, const char* value)
//
// 目的：设置属性值
//
void YCIUITag::attr(const char* attribute, const char* value)
{
	throw YCException(2002, "YCIUITag::attr尚未实现！");
}

//
// 函数：html(const char* html)
//
// 目的：设置新值和子标签
//
void YCIUITag::html(const char* html)
{
	YCUIDOM* newDOM = YCUIDomBuilder::Parse(html);
}

//
// 函数：append(const char* html)
//
// 目的：附加新子标签
//
void YCIUITag::append(const char* html)
{
	std::auto_ptr<YCUIDOM> appendDom(YCUIDomBuilder::Parse(html));
	if (appendDom.get() != NULL)
	{
		////////////////////////////////
		// 构建 Tag
		////////////////////////////////
		const char* tagName = appendDom->getName();
		LOG_DEBUG("YCIUITag::append处理节点: " << tagName);
		if (strcmp(tagName, "code") == 0)
		{
			try 
			{
				YCLua* lua = (YCLua*)YCRegistry::get("lua");
				if (lua == NULL)
				{
					LOG_ERROR("YCIContainer::createSubTags查询YCLua环境失败，请确认注册YCLua！");
					throw YCException(2002, "YCIContainer::createSubTags查询YCLua环境失败，请确认注册YCLua！");
				}

				// 构建Lua上下文
				YCLuaContext context;
				context.addContext("container", myContainer);
				context.addContext("this", this);
				lua->execute(&context, appendDom->getValue(), appendDom->getName());
			}
			catch (...)
			{
				throw YCException(2002, "YCIUITag::append运行Lua脚本出现未知异常！");
			}
		}
		else
		{
			try
			{
				if (!validateSubTag(tagName))
				{
					LOG_WARNING("YCIUITag::append " << name() << " 未知子标签："<< tagName);
				}
				else
				{
					YCIUITag* tag = gTagFactory->create(tagName);
					if (tag == NULL)
					{
						LOG_FATAL("YCIUITag::append未知标签名：" << tagName);
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
				LOG_WARNING("YCIUITag::createSubTags创建子标签失败：" << e.what());
			}
		}		

		////////////////////////////////
		// 是否需要重排
		////////////////////////////////
		if (myLayoutContext != NULL)
		{
			bind2CSS(myLayoutContext);
			layout(true);
		}
	}
}

// 
// 函数：bind(E_UITAG_EVENT ev, const char* scripts)
//
// 目的：绑定事件处理器
//
// 注释：
//      handler LUA脚本
//
void YCIUITag::bind(E_UITAG_EVENT ev, const char* scripts)
{
	SMART_ASSERT(scripts != NULL);
	addListener(ev, scripts);
}

// 
// 函数：unbind(E_UITAG_EVENT ev, void* handler)
//
// 目的：移除事件处理器
//
void YCIUITag::unbind(E_UITAG_EVENT ev)
{
	removeListener(ev);
}

//
// 函数：toggle(void* odd, void *even)
//
// 目的：toggle事件处理器
//
void YCIUITag::toggle(const char* odd, const char* even)
{

}

//
// 函数：move(int deltaX, int deltaY)
//
// 目的：移动窗口
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
// 函数：setProperty(YCUITagMeta::E_UITAG_CSS property, int priority, bool bValue)
//
// 目的：设置 int 类型属性
//
void YCIUITag::setProperty(E_UITAG_CSS property, int priority, bool bValue)
{
	if (validateProperty(property))
	{
		myUITagImpl->setProperty(property, priority, bValue);
	}
}

//
// 函数：setProperty(E_UITAG_CSS property, int priority, int iValue)
//
// 目的：设置 int 类型属性
//
void YCIUITag::setProperty(E_UITAG_CSS property, int priority, int iValue)
{
	if (validateProperty(property))
	{
		myUITagImpl->setProperty(property, priority, iValue);
	}
}

//
// 函数：setProperty(E_UITAG_CSS property, int priority, double fValue)
//
// 目的：设置 float 类型属性
//
void YCIUITag::setProperty(E_UITAG_CSS property, int priority, double fValue)
{
	if (validateProperty(property))
	{
		myUITagImpl->setProperty(property, priority, fValue);
	}
}

//
// 函数：setProperty(E_UITAG_CSS property, int priority, const char* strValue)
//
// 目的：设置 字符串 类型属性
//
void YCIUITag::setProperty(E_UITAG_CSS property, int priority, const char* strValue)
{
	if (validateProperty(property))
	{
		myUITagImpl->setProperty(property, priority, strValue);
	}
}

//
// 函数：hasProperty(E_UITAG_CSS property)
//
// 目的：是否设置该属性
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
// 函数：getProperty(E_UITAG_CSS property, bool& bValue)
//
// 目的：获取 int 类型属性
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
// 函数：getProperty(E_UITAG_CSS property, int iValue)
//
// 目的：获取 int 类型属性
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
// 函数：getProperty(E_UITAG_CSS property, double fValue)
//
// 目的：获取 float 类型属性
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
// 函数：getProperty(E_UITAG_CSS property, char* strValue, int length)
//
// 目的：获取 字符串 类型属性
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
// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：窗口消息处理函数
//
// 返回值：已处理，返回true，否则返回false
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

				//处理了鼠标点击事件
				const char* click = getListener(UITAG_CLICK_EVENT);
				if (click != NULL)
				{
					YCLua* lua = (YCLua*)YCRegistry::get("lua");
					if (lua == NULL)
					{
						LOG_ERROR("YCIContainer::initialize查询YCLua环境失败，请确认注册YCLua！");
						throw YCException(2002, "YCIUITag::onMessage查询YCLua环境失败，请确认注册YCLua！");
					}

					// 构建Lua上下文
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

				// 处理鼠标移入事件
				const char* hoverOn = getListener(UITAG_HOVERON_EVENT);
				if (hoverOn != NULL)
				{
					YCLua* lua = (YCLua*)YCRegistry::get("lua");
					if (lua == NULL)
					{
						LOG_ERROR("YCIContainer::onMessage查询YCLua环境失败，请确认注册YCLua！");
						throw YCException(2002, "YCIUITag::onMessage查询YCLua环境失败，请确认注册YCLua！");
					}

					// 构建Lua上下文
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

				// 处理鼠标移出事件
				const char* hoverOff = getListener(UITAG_HOVERON_EVENT);
				if (hoverOff != NULL)
				{
					YCLua* lua = (YCLua*)YCRegistry::get("lua");
					if (lua == NULL)
					{
						LOG_ERROR("YCIContainer::initialize查询YCLua环境失败，请确认注册YCLua！");
						throw YCException(2002, "YCIUITag::onMessage查询YCLua环境失败，请确认注册YCLua！");
					}

					// 构建Lua上下文
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
	
	// 让子控件处理消息
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
// 函数：isMouseIn(int mouseX, int mouseY)
// 
// 目的：鼠标是否在标签内部
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
// 函数：createDynamicUIDOM(const YCUIDOM* uiDom)
//
// 目的：创建子节点内存Dynamic UIDOM
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
		LOG_DEBUG("YCIContainer::bindContext处理子节点: " << uiDom->getName());
		if (strcmp(tagName, "code") == 0)
		{
			if (codeDom != NULL)
			{
				throw YCException(2002, "YCIContainer::bindContext子节点下有多个code节点！", uiDom->getName());
			}

			codeDom = subDom;
		}
		else
		{
			try
			{
				if (!validateSubTag(tagName))
				{
					LOG_WARNING("YCIUITag::createSubTags " << name() << " 未知子标签："<< tagName);
				}
				else
				{
					YCDynamicUIDOM* dynamicUIDOM = YCDynamicUIDOM::build(subDom);
					if (dynamicUIDOM == NULL)
					{
						LOG_WARNING("YCIUITag::createSubTags " << name() << " 构建DynamicUIDOM失败："<< tagName);
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
				LOG_WARNING("YCIUITag::createSubTags创建DynamicUIDOM失败：" << e.what());
			}
		}
	}

	return codeDom;
}

//
// 函数：createSubTags(const YCUIDOM *uiDom)
//
// 目的：创建子标签
//
bool YCIUITag::createSubTags(const YCUIDOM *uiDom)
{
	if (uiDom == NULL)
	{
		return true;
	}
	
	////////////////////////////////////
	// 移除以前的节点，如果有的话
	////////////////////////////////////
	deleteSubTags();

	////////////////////////////////////
	// 根据myUIDOM创建子节点的dynamicUIDOM
	////////////////////////////////////
	const YCUIDOM* codeDom = createDynamicUIDOM(uiDom);

	////////////////////////////////////
	// 根据myDynamicUIDOM创建标签
	////////////////////////////////////
	YCDList * subUIDom = myDynamicUIDOM->getSubDom();
	for (Item_List_T* item = subUIDom->begin();
		 item != subUIDom->end();
		 item = subUIDom->next(item))
	{
		YCDynamicUIDOM* dynamicUIDom = (YCDynamicUIDOM*)subUIDom->payload(item);
		const char* nodeName = uiDom->getName();
		LOG_DEBUG("YCIContainer::bindContext处理子节点: " << dynamicUIDom->getName());
		if (strcmp(nodeName, "code") == 0)
		{
			throw YCException(2002, "YCIUITag::createSubTags中myDynamicUIDOM出现code节点！");
		}
		else
		{
			try
			{
				const char* tagName = dynamicUIDom->getName();
				if (!validateSubTag(tagName))
				{
					LOG_WARNING("YCIUITag::createSubTags " << name() << " 未知子标签："<< tagName);
				}
				else
				{
					YCIUITag* tag = gTagFactory->create(tagName);
					if (tag == NULL)
					{
						LOG_FATAL("YCIUITag::createSubTags未知标签名：" << tagName);
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
				LOG_WARNING("YCIUITag::createSubTags创建子标签失败：" << e.what());
			}
		}
	}

	////////////////////////////////////
	// 最后处理code节点
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
				LOG_ERROR("YCIContainer::createSubTags查询YCLua环境失败，请确认注册YCLua！");
				throw YCException(2002, "YCIContainer::createSubTags查询YCLua环境失败，请确认注册YCLua！");
			}

			// 构建Lua上下文
			YCLuaContext context;
			context.addContext("container", myContainer);
			context.addContext("this", this);
			lua->execute(&context, codeDom->getValue(), name());
		}
		catch (...)
		{
			throw YCException(2002, "YCIUITag::createSubTags运行Lua脚本出现未知异常！");
		}
	}

	return true;
}

//
// 函数：deleteSubTags()
//
// 目的：删除所有子控件
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
// 函数：fetchFloatMode()
//
// 目的：取得元件浮动模式
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
// 函数：fetchPositionMode()
//
// 目的： 取得元件定位模式
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
				LOG_WARNING("YCIUITag::fetchPositionMode元件设置为决对定位模式，但未设置(top,left) 或者(right, bottom)" << name());
			}
			else
			{
				myPositionMode = OBSOLUTE_MODE;
			}
		}
	}
}

//
// 函数：fetchPosition()
//
// 目的：从CSS的top、left来确定位置
//
void YCIUITag::fetchPosition()
{
	char position[MAX_PATH] = {0};

	int x = INT_MIN;
	int y = INT_MIN;

	// 左上绝对定位
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
					LOG_WARNING("YCIUITag::fetchPosition无效UITAG_LEFT值" << e.what());
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
					LOG_WARNING("YCIUITag::fetchPosition无效UITAG_TOP值" << e.what());
				}
			}
		}
	}

	//右下绝对定位
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
					LOG_WARNING("YCIUITag::fetchPosition无效UITAG_RIGHT值" << e.what());
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
					LOG_WARNING("YCIUITag::fetchPosition无效UITAG_BOTTOM值" << e.what());
				}
			}
		}
	}

	setPosition(x + myMarginLeft, y + myMarginTop);	
}

//
// 函数：calculateViewport()
//
// 目的：计算视口裁剪区
//
void YCIUITag::calculateViewport()
{
	myViewPort.left = myScreenPositionX + myMarginLeft + myPaddingLeft;
	myViewPort.top = myScreenPositionY + myMarginTop + myPaddingTop;
	myViewPort.right = myViewPort.left + myClientWidth;
	myViewPort.bottom = myViewPort.top + myClientHeight;
}

//
// 函数：fetchDisplayMode()
//
// 目的： 取得元件显示模式
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
			LOG_WARNING("YCIUITag::fetchDisplayMode元件错误显示模式[" << name() << "] display = " << display);
		}
	}
}

//
// 函数：fetchWidthAndHeight()
//
// 目的：取得元件的宽和高，不包含margin区域
//      border和padding区域在width和height之内
//
void YCIUITag::fetchWidthAndHeight()
{
	if (!getProperty(UITAG_WIDTH, myOriginalWidth))
	{
		throw YCException(2002, "YCIUITag::getWidthAndHeight未设置width属性", name());
	}

	if (!getProperty(UITAG_HEIGHT, myOriginalHeight))
	{
		//throw YCException(2002, "YCIUITag::getWidthAndHeight未设置height属性：", name());
		LOG_INFO("YCIUITag::getWidthAndHeight未设置height属性：" << name());
	}
	
	// 检查 margin
	myUITagImpl->getProperty(UITAG_MARGIN_TOP, myMarginTop);
	myUITagImpl->getProperty(UITAG_MARGIN_LEFT, myMarginLeft);
	myUITagImpl->getProperty(UITAG_MARGIN_RIGHT, myMarginRight);
	myUITagImpl->getProperty(UITAG_MARGIN_BOTTOM, myMarginBottom);
	
	myWidthWithMargin = myOriginalWidth + myMarginLeft + myMarginRight;
	myHeightWithMargin = myOriginalHeight + myMarginTop + myMarginBottom;

	myWidth = myOriginalWidth;
	myHeight = myOriginalHeight;
	
	// 检查 border
	handleBorder(UITAG_BORDER_TOP, myBorderTop, myBorderTopColor);
	handleBorder(UITAG_BORDER_LEFT, myBorderLeft, myBorderLeftColor);
	handleBorder(UITAG_BORDER_RIGHT, myBorderRight, myBorderRightColor);
	handleBorder(UITAG_BORDER_BOTTOM, myBorderBottom, myBorderBottomColor);

	// 检查 padding
	myUITagImpl->getProperty(UITAG_PADDING_TOP, myPaddingTop);
	myUITagImpl->getProperty(UITAG_PADDING_LEFT, myPaddingLeft);
	myUITagImpl->getProperty(UITAG_PADDING_RIGHT, myPaddingRight);
	myUITagImpl->getProperty(UITAG_PADDING_BOTTOM, myPaddingBottom);

	// 计算内部可画子控件的宽，高度
	myClientWidth = myOriginalWidth - myPaddingLeft - myPaddingRight;
	myClientHeight = myOriginalHeight - myPaddingTop - myPaddingBottom;
}

//
// 函数：updateHeight(int height)
//
// 目的：设置元件的高
//
void YCIUITag::updateHeight(int height)
{
	myOriginalHeight = height;
	myHeight = myOriginalHeight;	
	myHeightWithMargin = myOriginalHeight + myMarginTop + myMarginBottom;
	myClientHeight = myOriginalHeight - myPaddingTop - myPaddingBottom;

	//Y方向重新定位
	fetchPosition();
}

//
// 函数：buildBackground()
//
// 目的：构建元素背景
//
// 注释：
//      目前有四种背景模式, 绘制背景优先级从上到下
//         YCColorBackground
//         YCAnimationBackground
//         YCImageBackground
//         YCSplitBackground  -> 九块
//                            -> x方向平铺 3块
//                            -> y方向平铺 3块
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
			LOG_WARNING("YCIUITag::buildBackground构建颜色背景失败, 无法解析配置值！");
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
				LOG_WARNING("YCIUITag::buildBackground构建动画背景失败, 无法获取YCAnimationManager句柄！");
			}
			else
			{
				YCIAnimation* animation = animationManager->getAnimation(background);
				if (animation == NULL)
				{
					LOG_WARNING("YCIUITag::buildBackground构建动画背景失败：" << background);
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
				LOG_WARNING("YCIUITag::buildBackground构建单一图片背景失败, 无法获取YCTextureManager句柄！");
			}
			else
			{
				YCTexture* texture = textureManager->fetch(TEXTURE_UI, background);
				if (texture == NULL)
				{
					LOG_WARNING("YCIUITag::buildBackground构建单一图片背景失败：" << background);
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
			LOG_WARNING("YCIUITag::buildBackground构建Split图片背景失败, 无法获取YCTextureManager句柄！");
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
// 函数：layout()
//
// 目的：进行元件自身的布局
//
// 注释：
//      开始子元素的布局
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
//        |------------------------ 。[leftTopX,leftTopY]         。(rightTopX, rightTopY)     |
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
//        |------------------------ 。[leftBottomX, leftBottomY]                               |
//        |                                                                                    |
//        |                                                                                    |
//         ------------------------------------------------------------------------------------
//
void YCIUITag::layout(bool resizable)
{
	// 检查 background
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
	// 在布局过程中
	// 按照一行一行的布局方式从上到下进行
	// 每一行的布局为
	//    FLOAT_LEFT_1， FLOAT_LEFT_2，... FLOAT_LEFT_n ..., 
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

			// 是否绝对布局
			if (tag->getPositionMode() == OBSOLUTE_MODE)
			{
				
			}
			else
			{
				bool newLine = false;

				if ((rightTopX-leftTopX)<widthWithMargin)
				{
					newLine = true;

					// 开始新一行
					line.clear();

					leftTopX = myMarginLeft + myPaddingLeft; //回到最左边
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
							// 如果当前元件是BLOCK_MODE，则判断
							// 当前line行中的元件是否都是FLOAT_LEFT,FLOAT_RIGHT或者是FLOAT_NONE,INLINE_MODE
							//  是 则安插在当前行
							//  否 则启用新行
							for (std::list<YCIUITag*>::iterator it = line.begin(); it != line.end(); ++it)
							{
								YCIUITag* current = (YCIUITag*)*it;
								if (current->getFloatMode() == FLOAT_NONE
								 && current->getDisplayMode() == BLOCK_MODE)
								{
									newLine = true;
									break; //不浮动，块模式
								}
							}

							if (newLine)
							{
								line.clear();

								leftTopX = myMarginLeft + myPaddingLeft; //回到最左边
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
								//找到插入点，在最后一个FLOAT_NONE之后
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
							// 插入到该行的FLOAT_LEFT之后，FLOAT_NONE之前
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
								// 还没有FLOAT_NONE标签
								tag->setPosition(leftTopX, leftTopY);
								line.push_back(tag);
							}
							else
							{
								//存在FLOAT_NONE标签
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
							// 插入到该行的FLOAT_NONE之后，FLOAT_RIGHT之前
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
								// 还没有FLOAT_RIGHT标签						
								line.push_back(tag);
							}
							else
							{
								//存在FLOAT_RIGHT标签
								line.insert(it, tag);
							}
							//leftBottomY = max(leftBottomY, (leftTopY+heightWithMargin));
						}					
					}
					// 继续现在行
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
							// 插入到该行的FLOAT_LEFT之后，FLOAT_NONE之前
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
								// 还没有FLOAT_NONE标签
								tag->setPosition(leftTopX, leftTopY);
								line.push_back(tag);
							}
							else
							{
								//存在FLOAT_NONE标签
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
							// 插入到该行的FLOAT_NONE之后，FLOAT_RIGHT之前
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
								// 还没有FLOAT_RIGHT标签						
								line.push_back(tag);
							}
							else
							{
								//存在FLOAT_RIGHT标签
								line.insert(it, tag);
							}
							//leftBottomY = max(leftBottomY, (leftTopY+heightWithMargin));
						}
					}
				}

				// 布局子元件
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

		// 如果未设置高度发生了变化
		if (myOriginalHeight != newHeight)
		{
			updateHeight(newHeight);
		}
	}
	else
	{
		// 如果未设置高度，则根据子控件计算
		if (myOriginalHeight < 0)
		{
			updateHeight(leftBottomY-myMarginTop+myPaddingTop);
		}
	}
}

//
// 函数：onCSSUpdated()
//
// 目的：当CSS发生变化时
//
void YCIUITag::onCSSUpdated()
{

}

//
// 函数：onDOMUpdated()
//
// 目的：当DOM发生变化时
//
void YCIUITag::onDOMUpdated()
{
	////////////////////////////////
	// 重新构建标签
	////////////////////////////////
	// TODO ...

	////////////////////////////////
	// 绑定 CSS
	////////////////////////////////
	bind2CSS(myLayoutContext);

	////////////////////////////////
	// 布局
	////////////////////////////////
	layout(true);
}

//
// 函数：draw()
//
// 目的：在指定处绘制该UI元件自身
//
void YCIUITag::draw()
{
	// 画背景
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

	// 画边框
	drawBorder();

	// 自定义绘制
	extraDraw();
}

//
// 函数：handleBorder(E_UITAG_CSS property, int& border, unsigned int& color)
//
// 目的：处理CSS边框宽度
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
			LOG_WARNING("YCIUITag::layout CSS 属性 " << property << " 边框配置错误：" << border);
		}
		else
		{
			if (!YCStringHelper::isType(borderVector[0].c_str(), YCStringHelper::INTEGER))
			{
				LOG_WARNING("YCIUITag::layout CSS 属性 " << property << " 边框配置错误：" << border);
			}
			else
			{				
				if (!YCStringHelper::parseColor(borderVector[1].c_str(), color))
				{
					LOG_WARNING("YCIUITag::layout CSS 属性 " << property << " 边框配置错误：" << border);
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
// 函数：drawBorder()
//
// 目的：画边框，继承类可以覆盖，但不能调用基类的实现
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
// 函数：validateProperty(E_UITAG_CSS property)
//
// 目的：校验属性是否针对此Tag有效
//
bool YCIUITag::validateProperty(E_UITAG_CSS property)
{
	if (property != UITAG_INVALID && myTagMeta->isSupportedCSS(property))
	{
		return true;
	}

	LOG_DEBUG("YCIUITag::validateProperty 标签[" << name() << "]不支持属性 ：" << property);
	return false;
}

