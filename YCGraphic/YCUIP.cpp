#include "YCUIP.h"

#include "YCFontDef.h"
#include "YCUIHoverState.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCStringHelper.h"

YCUIP::YCUIP(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
	, myFontSize(UITAG_P_SIZE)
	, myColor(0xFFFFFF)
	, myTextAlign(TEXT_ALIGN_LEFT)
	, myFontWeight(FONT_WEIGHT_NORMAL)
	, myFontType(FONT_YAHEI)
	, myLetterSpacing(0)
	, myStrTextureWidth(-1)
	, myStrTextureHeight(-1)
{
}


YCUIP::~YCUIP(void)
{
}

//////////////////////////////////////////////////////////////////
// Protected Part
//////////////////////////////////////////////////////////////////

//
// 函数：extraCSS()
//
// 目的：处理标签特定的CSS属性
//
void YCUIP::extraCSS()
{
	/*
	 UITAG_FONT_COLOR						,    //颜色
	 UITAG_FONT_SIZE						,    //大小
	 UITAG_FONT_WEIGHT						,    //粗细
	 UITAG_FONT_FAMILY						,    //字体
	 UITAG_FONT_LETTER_SPACING				,    //字间距
	 UITAG_FONT_STROKE_COLOR				,    //描边色
	 */
	char buf[BUFFER_16] = {0};
	if (getProperty(UITAG_FONT_COLOR, buf, BUFFER_16))
	{
		if (!YCStringHelper::parseColor(buf, myColor))
		{
			LOG_WARNING("YCUIP::extraCSS无效颜色值：" << myColor);
		}
	}

	if (getProperty(UITAG_TEXT_ALIGN, buf, BUFFER_16))
	{
		if (strcmp("middle", buf) == 0)
		{
			myTextAlign = TEXT_ALIGN_MIDDLE;
		}
		else if (strcmp("right", buf) == 0)
		{
			myTextAlign = TEXT_ALIGN_RIGHT;
		}
		else
		{
			myTextAlign = TEXT_ALIGN_LEFT;
		}
	}

	if (getProperty(UITAG_FONT_WEIGHT, buf, BUFFER_16))
	{
		if (strcmp("normal", buf) == 0)
		{
			myFontWeight = FONT_WEIGHT_NORMAL;
		}
		else if (strcmp("bold", buf) == 0)
		{
			myFontWeight = FONT_WEIGHT_BOLD;
		}
		else if (strcmp("bolder", buf) == 0)
		{
			myFontWeight = FONT_WEIGHT_BOLDER;
		}
	}

	if (getProperty(UITAG_FONT_FAMILY, buf, BUFFER_16))
	{
		if (strcmp("宋体", buf) == 0)
		{
			myFontType = FONT_SONG;
		}
		else if (strcmp("黑体", buf) == 0)
		{
			myFontType = FONT_HEI;
		}
		else if (strcmp("楷体", buf) == 0)
		{
			myFontType = FONT_KAI;
		}
		else if (strcmp("隶书", buf) == 0)
		{
			myFontType = FONT_LISHU;
		}
		else if (strcmp("微软雅黑", buf) == 0)
		{
			myFontType = FONT_YAHEI;
		}
	}

	//字间距
	getProperty(UITAG_FONT_LETTER_SPACING, myLetterSpacing);

	//字符大小
	getProperty(UITAG_FONT_SIZE, myFontSize);
}

//
// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：窗口消息处理函数
//
// 返回值：已处理，返回true，否则返回false
//
//bool YCUIP::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	return false;
//}

//
// 函数：extraDraw()
//
// 目的：在指定处绘制该UI元件
//
void YCUIP::extraDraw()
{
	if (myStrValue != NULL)
	{	
		// 文字颜色
		unsigned int color = myColor;
		if (hasStatus(UITAG_STATUS_HOVER) && (myHoverState != NULL))
		{
			myHoverState->getFontColor(color);
		}
	
		if (myTextAlign == TEXT_ALIGN_LEFT)
		{
			gFont->draw(myFontType, myStrValue, myFontSize, color, myScreenPositionX, myScreenPositionY);
		}
		else
		{
			if (myStrTextureWidth < 0)
			{
				if (!gFont->getFontWidthAndHeight(myFontType, FONT_HORIZONTAL, myFontSize, myStrValue, myStrTextureWidth, myStrTextureHeight))
				{
					return;
				}
			}

			if (myTextAlign == TEXT_ALIGN_RIGHT)
			{
				gFont->draw(myFontType, myStrValue, myFontSize, color, 
					myScreenPositionX+myPaddingLeft+myClientWidth-myStrTextureWidth, myScreenPositionY+myPaddingTop);
			}
			else
			{
				gFont->draw(myFontType, myStrValue, myFontSize, color, 
					myScreenPositionX+myPaddingLeft+(myClientWidth-myStrTextureWidth)/2, myScreenPositionY+myPaddingTop);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// 函数：validateSubTag(const char* name)
//
// 目的：校验子控件
//
bool YCUIP::validateSubTag(const char* name)
{
	SMART_ASSERT(name != NULL);
	return (strcmp("p", name) == 0 || strcmp("span", name) == 0);
}
