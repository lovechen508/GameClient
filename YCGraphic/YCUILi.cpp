#include "YCUILi.h"

#include "YCFontDef.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCStringHelper.h"

YCUILi::YCUILi(YCUITagMeta* tagMeta)
	: YCUITag(tagMeta)
	, myFontSize(UITAG_LI_SIZE)
	, myColor(0xFFFFFF)
	, myTextAlign(TEXT_ALIGN_LEFT)
	, myFontWeight(FONT_WEIGHT_NORMAL)
	, myFontType(FONT_YAHEI)
	, myLetterSpacing(0)
	, myStrTextureWidth(-1)
	, myStrTextureHeight(-1)
{
}


YCUILi::~YCUILi(void)
{
}

//
// ������draw()
//
// Ŀ�ģ���ָ�������Ƹ�UIԪ��
//
//void YCUILi::draw()
//{

//}

//
// ������extraCSS()
//
// Ŀ�ģ������ǩ�ض���CSS����
//
void YCUILi::extraCSS()
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
// ������extraDraw()
//
// Ŀ�ģ���ָ�������Ƹ�UIԪ��
//
void YCUILi::extraDraw()
{
	if (myStrValue != NULL)
	{		
		if (myTextAlign == TEXT_ALIGN_LEFT)
		{
			gFont->draw(myFontType, myStrValue, myFontSize, myColor, myScreenPositionX+myPaddingLeft, myScreenPositionY+myPaddingTop);
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

			if (myTextAlign == TEXT_ALIGN_MIDDLE)
			{
				gFont->draw(myFontType, myStrValue, myFontSize, myColor, 
					myScreenPositionX+myPaddingLeft+myClientWidth-myStrTextureWidth, myScreenPositionY+myPaddingTop);
			}
			else
			{
				gFont->draw(myFontType, myStrValue, myFontSize, myColor, 
					myScreenPositionX+myPaddingLeft+(myClientWidth-myStrTextureWidth)/2, myScreenPositionY+myPaddingTop);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// ������validateSubTag(const char* name)
//
// Ŀ�ģ�У���ӿؼ�
//
bool YCUILi::validateSubTag(const char* name)
{
	return false;
}
