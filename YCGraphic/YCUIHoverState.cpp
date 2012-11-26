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
// ������bindCSSNode(const YCCSSNode* node)
//
// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
//
void YCUIHoverState::bindCSSNode(const YCCSSNode* node)
{	
	if (node == NULL)
	{
		throw YCException(2002, "YCUIHoverState::bindHoverNode����nodeΪNULL");
	}

	//
	// �ٴμ���Ƿ�hoverα��,������CSS����
	// Ŀǰֻ֧�ָı�backgound��font-color
	//
	if (YCCSSNode::NODE_HOVER == node->getNodeType())
	{
		SMART_ASSERT(myContext == NULL);
		myContext = node;
		LOG_TRACE("[" << myHost->name() << "] YCIUITag::bind2CSS ƥ�䵽 Hoverα�ࣺ" << node->getSelector());

		myContext->apply(this);
	}

	buildBackground();

	char buf[BUFFER_16] = {0};
	if (getProperty(UITAG_FONT_COLOR, buf, BUFFER_16))
	{
		if (!YCStringHelper::parseColor(buf, myColor))
		{
			LOG_WARNING("YCUIP::extraCSS��Ч��ɫֵ��" << myColor);
		}
		else
		{
			myColorEnable = true;
		}
	}
}

//
// ������getBackground()
//
// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
//
YCIBackground* YCUIHoverState::getBackground()
{
	return myBackground;
}

//
// ������getFontColor(unsigned int &hoverColor)
//
// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
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
// ������draw()
//
// Ŀ�ģ������ʽ�Ļ���
//       �Լ����µ�״̬�ͻ���
//       ����ͽ���Ԫ���������
//
void YCUIHoverState::draw()
{
	throw YCException(2002, "YCUIHoverState::draw ����ֱ�ӵ��ã�");
}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// ������validateSubTag(const char* name)
//
// Ŀ�ģ�У���ӿؼ�
//
bool YCUIHoverState::validateSubTag(const char* name)
{
	return false;
}
