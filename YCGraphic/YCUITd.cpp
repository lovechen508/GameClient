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
// ������setDelimiterEnable(bool enable)
//
// Ŀ�ģ��Ƿ���Ʒָ���
//
void YCUITd::setDelimiterEnable(bool enable)
{
	myDelimiterEnabled = enable;
}

//
// ������extraDraw()
//
// Ŀ�ģ�UIԪ�����Զ������
//
void YCUITd::extraDraw()
{
	if (myStrValue != NULL)
	{	
		// ������ɫ
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

	// �ָ���
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
// ������extraCSS()
//
// Ŀ�ģ������ǩ�ض���CSS����
//
void YCUITd::extraCSS()
{
	// �ַ����
	handleFont();

	// �ָ���
	handleDelimiter();
}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// ������validateSubTag(const char* name)
//
// Ŀ�ģ�У���ӿؼ�
//
bool YCUITd::validateSubTag(const char* name)
{
	return true;
}

//
// ������handleFont()
//
// Ŀ�ģ������ַ�
//
void YCUITd::handleFont()
{
	/*
	 UITAG_FONT_COLOR						,    //��ɫ
	 UITAG_FONT_SIZE						,    //��С
	 UITAG_FONT_WEIGHT						,    //��ϸ
	 UITAG_FONT_FAMILY						,    //����
	 UITAG_FONT_LETTER_SPACING				,    //�ּ��
	 UITAG_FONT_STROKE_COLOR				,    //���ɫ
	 */
	char buf[BUFFER_16] = {0};
	if (getProperty(UITAG_FONT_COLOR, buf, BUFFER_16))
	{
		if (!YCStringHelper::parseColor(buf, myColor))
		{
			LOG_WARNING("YCUIP::extraCSS��Ч��ɫֵ��" << myColor);
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
		if (strcmp("����", buf) == 0)
		{
			myFontType = FONT_SONG;
		}
		else if (strcmp("����", buf) == 0)
		{
			myFontType = FONT_HEI;
		}
		else if (strcmp("����", buf) == 0)
		{
			myFontType = FONT_KAI;
		}
		else if (strcmp("����", buf) == 0)
		{
			myFontType = FONT_LISHU;
		}
		else if (strcmp("΢���ź�", buf) == 0)
		{
			myFontType = FONT_YAHEI;
		}
	}

	//�ּ��
	getProperty(UITAG_FONT_LETTER_SPACING, myLetterSpacing);

	//�ַ���С
	getProperty(UITAG_FONT_SIZE, myFontSize);
}

//
// ������handleDelimiter()
//
// Ŀ�ģ�����ָ���
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
			LOG_WARNING("YCIUITag::layout CSS ���� " << UITAG_CSS_TABLE_DELIMITER << " ���ô���" << propertyValue);
		}
		else
		{
			if (!YCStringHelper::isType(delimiterVector[0].c_str(), YCStringHelper::INTEGER))
			{
				LOG_WARNING("YCIUITag::layout CSS ���� " << UITAG_CSS_TABLE_DELIMITER << " ���ô���" << propertyValue);
			}
			else
			{				
				if (!YCStringHelper::parseColor(delimiterVector[1].c_str(), myDelimiterColor))
				{
					LOG_WARNING("YCIUITag::layout CSS ���� " << UITAG_CSS_TABLE_DELIMITER << " ���ô���" << propertyValue);
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