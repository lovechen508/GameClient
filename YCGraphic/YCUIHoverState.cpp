#include "YCUIHoverState.h"

#include <vector>

#include "YCIUITag.h"
#include "YCCSSNode.h"
#include "YCUITagDef.h"
#include "YCCSSContext.h"
#include "YCIBackground.h"
#include "YCTextureManager.h"
#include "YCColorBackground.h"
#include "YCImageBackground.h"
#include "YCSplitBackground.h"
#include "YCAnimationManager.h"
#include "YCAnimationBackground.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCStringHelper.h"

YCUIHoverState::YCUIHoverState(const YCUITagMeta* tagMeta, YCIUITag* host)
	: YCUITag(tagMeta)
	, myHost(host)
	, myContext(NULL)
	, myColorEnable(false)
{
	SMART_ASSERT(host != NULL);
}


YCUIHoverState::~YCUIHoverState(void)
{
}

//
// 函数：bindCSSNode(const YCCSSNode* node)
//
// 目的：绑定到DOM文档对象模型
//
void YCUIHoverState::bindCSSNode(const YCCSSNode* node)
{	
	if (node == NULL)
	{
		throw YCException(2002, "YCUIHoverState::bindHoverNode参数node为NULL");
	}

	//
	// 再次检查是否hover伪类,并解析CSS属性
	// 目前只支持改变backgound和font-color
	//
	if (YCCSSNode::NODE_HOVER == node->getNodeType())
	{
		SMART_ASSERT(myContext == NULL);
		myContext = node;
		LOG_TRACE("[" << myHost->name() << "] YCIUITag::bind2CSS 匹配到 Hover伪类：" << node->getSelector());

		myContext->apply(this);
	}

	buildBackground();

	char buf[BUFFER_16] = {0};
	if (getProperty(UITAG_FONT_COLOR, buf, BUFFER_16))
	{
		if (!YCStringHelper::parseColor(buf, myColor))
		{
			LOG_WARNING("YCUIP::extraCSS无效颜色值：" << myColor);
		}
		else
		{
			myColorEnable = true;
		}
	}
}

//
// 函数：getBackground()
//
// 目的：绑定到DOM文档对象模型
//
YCIBackground* YCUIHoverState::getBackground()
{
	return myBackground;
}

//
// 函数：getFontColor(unsigned int &hoverColor)
//
// 目的：绑定到DOM文档对象模型
//
bool YCUIHoverState::getFontColor(unsigned int &hoverColor)
{
	if (myColorEnable)
	{
		hoverColor = myColor;
	}
	return myColorEnable;
}

//
// 函数：draw()
//
// 目的：完成正式的绘制
//       自己有新的状态就绘制
//       否则就交回元件自身绘制
//
void YCUIHoverState::draw()
{
	throw YCException(2002, "YCUIHoverState::draw 不能直接调用！");
}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// 函数：validateSubTag(const char* name)
//
// 目的：校验子控件
//
bool YCUIHoverState::validateSubTag(const char* name)
{
	return false;
}
