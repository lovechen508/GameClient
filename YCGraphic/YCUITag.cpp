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
// 函数：extraDraw()
//
// 目的：UI元件的自定义绘制
//
void YCUITag::extraDraw()
{

}

//
// 函数：matchAccelerateKey()
//
// 目的：是否匹配快捷键
//
bool YCUITag::matchAccelerateKey()
{
	return false;
}

//
// 函数：open(YCLuaParameter* context)
//
// 目的： 创建一个窗口新实例
//        同时绑定窗口打开的数据上下文
//
// 注释：
//       每次从YCUIManager请求一个窗口，都创建一个窗口新实例
//       保证YCUIDOM的初始不变性
//
bool YCUITag::open(YCLuaParameter* context)
{
	return true;
}

//
// 函数：close()
//
// 目的： 关闭一个元件实例
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
// 函数：fetchPosition()
//
// 目的：从CSS的top、left来确定位置
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
// 函数：extraCSS()
//
// 目的：处理标签特定的CSS属性
//
void YCUITag::extraCSS()
{

}


