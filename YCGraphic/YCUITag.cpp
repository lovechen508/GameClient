#include "YCUITag.h"

#include "YCCSSContext.h"
#include "YCTagFactory.h"

#include "YCLua\YCLuaParameter.h"

#include "YCBasic\YCException.h"
#include "YCBasic\YCStringHelper.h"

YCUITag::YCUITag(const YCUITagMeta* tagMeta)
	: YCIUITag(tagMeta)
{
}


YCUITag::~YCUITag(void)
{
	if (myLayoutContext != NULL)
	{
		const_cast<YCCSSContext*>(myLayoutContext)->clearWithoutDelete();
	}
}

	
//
// ������extraDraw()
//
// Ŀ�ģ�UIԪ�����Զ������
//
void YCUITag::extraDraw()
{

}

//
// ������matchAccelerateKey()
//
// Ŀ�ģ��Ƿ�ƥ���ݼ�
//
bool YCUITag::matchAccelerateKey()
{
	return false;
}

//
// ������open(YCLuaParameter* context)
//
// Ŀ�ģ� ����һ��������ʵ��
//        ͬʱ�󶨴��ڴ򿪵�����������
//
// ע�ͣ�
//       ÿ�δ�YCUIManager����һ�����ڣ�������һ��������ʵ��
//       ��֤YCUIDOM�ĳ�ʼ������
//
bool YCUITag::open(YCLuaParameter* context)
{
	return true;
}

//
// ������close()
//
// Ŀ�ģ� �ر�һ��Ԫ��ʵ��
//
void YCUITag::close()
{

}

////////////////////////////////////////////////////////////////
// Protected Part
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
// Private Part
////////////////////////////////////////////////////////////////

//
// ������fetchPosition()
//
// Ŀ�ģ���CSS��top��left��ȷ��λ��
//
void YCUITag::fetchPosition()
{
	char position[MAX_PATH] = {0};
	if (getProperty(UITAG_LEFT, position, MAX_PATH))
	{
		if (YCStringHelper::isType(position, YCStringHelper::INTEGER))
		{
			myPositionX = atoi(position);				
		}
		else
		{
			try
			{
				myPositionX = YCUITagHelper::getSpecialValue(position);
			}
			catch (YCException& e)
			{
				myPositionX = YCUITagHelper::getSpecialValue("HALF_SCREEN_WIDTH");
			}
			myPositionX -= myOriginalWidth/2;
		}
	}

	if (getProperty(UITAG_TOP, position, MAX_PATH))
	{
		if (YCStringHelper::isType(position, YCStringHelper::INTEGER))
		{
			myPositionY =  atoi(position);				
		}
		else
		{
			try
			{
				myPositionY = YCUITagHelper::getSpecialValue(position);
			}
			catch (YCException& e)
			{
				myPositionY = YCUITagHelper::getSpecialValue("HALF_SCREEN_HEIGHT");
			}
			myPositionY -= myOriginalHeight/2;
		}
	}
}

//
// ������extraCSS()
//
// Ŀ�ģ������ǩ�ض���CSS����
//
void YCUITag::extraCSS()
{

}


