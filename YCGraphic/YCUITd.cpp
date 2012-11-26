#include "YCUITd.h"

#include <vector>

#include "YCFontDef.h"
#include "YCUIHoverState.h"

#include "YCBasic\YCLogger.h"
#include "YCBasic\YCStringHelper.h"

YCUITd::YCUITd(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
	, myDelimiterEnabled(false)
	, myDelimiterWidth(0)
{
}

YCUITd::~YCUITd(void)
{
}

//
// 函数：setDelimiterEnable(bool enable)
//
// 目的：是否绘制分隔符
//
void YCUITd::setDelimiterEnable(bool enable)
{
	myDelimiterEnabled = enable;
}

//
// 函数：extraDraw()
//
// 目的：UI元件的自定义绘制
//
void YCUITd::extraDraw()
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

	// 分隔符
	if (myDelimiterEnabled && (myDelimiterWidth > 0))
	{
		gPrimitive->drawRetangle(myScreenPositionX+myMarginLeft+myWidth-myDelimiterWidth, 
			                     myScreenPositionY+myMarginTop, 
								 myScreenPositionX+myMarginLeft+myWidth, 
								 myScreenPositionY+myMarginTop+myHeight, 
			                     myDelimiterColor, myDelimiterColor, myDelimiterColor, myDelimiterColor);
	}
}

//
// 函数：extraCSS()
//
// 目的：处理标签特定的CSS属性
//
void YCUITd::extraCSS()
{
	// 字符相关
	handleFont();

	// 分隔符
	handleDelimiter();
}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// 函数：validateSubTag(const char* name)
//
// 目的：校验子控件
//
bool YCUITd::validateSubTag(const char* name)
{
	return true;
}

//
// 函数：handleFont()
//
// 目的：处理字符
//
void YCUITd::handleFont()
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
// 函数：handleDelimiter()
//
// 目的：处理分隔符
//
void YCUITd::handleDelimiter()
{
	if (hasProperty(UITAG_TABLE_DELIMITER))
	{	
		char propertyValue[MAX_PATH] = {0};
		std::vector<std::string> delimiterVector;

		getProperty(UITAG_TABLE_DELIMITER, propertyValue, MAX_PATH);
		YCStringHelper::split(propertyValue, " ", &delimiterVector);
		if (delimiterVector.size() != 2)
		{
			LOG_WARNING("YCIUITag::layout CSS 属性 " << UITAG_CSS_TABLE_DELIMITER << " 配置错误：" << propertyValue);
		}
		else
		{
			if (!YCStringHelper::isType(delimiterVector[0].c_str(), YCStringHelper::INTEGER))
			{
				LOG_WARNING("YCIUITag::layout CSS 属性 " << UITAG_CSS_TABLE_DELIMITER << " 配置错误：" << propertyValue);
			}
			else
			{				
				if (!YCStringHelper::parseColor(delimiterVector[1].c_str(), myDelimiterColor))
				{
					LOG_WARNING("YCIUITag::layout CSS 属性 " << UITAG_CSS_TABLE_DELIMITER << " 配置错误：" << propertyValue);
				}
				else
				{
					myDelimiterColor += 0xFF000000;
					myDelimiterWidth = atoi(delimiterVector[0].c_str());
				}
			}
		}
	}
}