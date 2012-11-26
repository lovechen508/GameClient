#include "YCUITagHelper.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

YCUITagHelper YCUITagHelper::gInstance;

//
// 函数：YCUITagHelper& Instance()
//
// 目的：取得唯一实例
//
YCUITagHelper& YCUITagHelper::Instance()
{
	return gInstance;
}

#pragma region string_vk_mapping

#define KEY_CHAR_MAP(key, vk) if (strcmp(input, key) == 0) return vk; //return ::MapVirtualKey(vk, MAPVK_VK_TO_CHAR)

char YCUITagHelper::Key2Char(const char* input)
{
	SMART_ASSERT(input != NULL);

	KEY_CHAR_MAP("ctrl", VK_CONTROL);
	KEY_CHAR_MAP("tab",  VK_TAB);
	KEY_CHAR_MAP("alt",  VK_MENU);
	KEY_CHAR_MAP("shift",VK_SHIFT);

	KEY_CHAR_MAP("f1",   VK_F1);
	KEY_CHAR_MAP("f2",   VK_F2);
	KEY_CHAR_MAP("f3",   VK_F3);
	KEY_CHAR_MAP("f4",   VK_F4);
	KEY_CHAR_MAP("f5",   VK_F5);
	KEY_CHAR_MAP("f6",   VK_F6);
	KEY_CHAR_MAP("f7",   VK_F7);
	KEY_CHAR_MAP("f8",   VK_F8);
	KEY_CHAR_MAP("f9",   VK_F9);
	KEY_CHAR_MAP("f10",  VK_F10);
	KEY_CHAR_MAP("f11",  VK_F11);
	KEY_CHAR_MAP("f12",  VK_F12);

	KEY_CHAR_MAP("a",    'A');
	KEY_CHAR_MAP("b",    'B');
	KEY_CHAR_MAP("c",    'C');
	KEY_CHAR_MAP("d",    'D');
	KEY_CHAR_MAP("e",    'E');
	KEY_CHAR_MAP("f",    'F');
	KEY_CHAR_MAP("g",    'G');
	KEY_CHAR_MAP("h",    'H');
	KEY_CHAR_MAP("i",    'I');
	KEY_CHAR_MAP("j",    'J');
	KEY_CHAR_MAP("k",    'K');
	KEY_CHAR_MAP("l",    'L');
	KEY_CHAR_MAP("m",    'M');
	KEY_CHAR_MAP("n",    'N');
	KEY_CHAR_MAP("o",    'O');
	KEY_CHAR_MAP("p",    'P');
	KEY_CHAR_MAP("q",    'Q');
	KEY_CHAR_MAP("r",    'R');
	KEY_CHAR_MAP("s",    'S');
	KEY_CHAR_MAP("t",    'T');
	KEY_CHAR_MAP("u",    'U');
	KEY_CHAR_MAP("v",    'V');
	KEY_CHAR_MAP("w",    'W');
	KEY_CHAR_MAP("x",    'X');
	KEY_CHAR_MAP("y",    'Y');
	KEY_CHAR_MAP("z",    'Z');

	KEY_CHAR_MAP("0",    '0');
	KEY_CHAR_MAP("1",    '1');
	KEY_CHAR_MAP("2",    '2');
	KEY_CHAR_MAP("3",    '3');
	KEY_CHAR_MAP("4",    '4');
	KEY_CHAR_MAP("5",    '5');
	KEY_CHAR_MAP("6",    '6');
	KEY_CHAR_MAP("7",    '7');
	KEY_CHAR_MAP("8",    '8');
	KEY_CHAR_MAP("9",    '9');

	throw YCException(2002, "YCWindow::initAccelerateKey不支持的快捷键");
	return 0;
}

#undef KEY_CHAR_MAP

#pragma endregion string_vk_mapping

//
// 函数：getSpecialValue(const char* input)
//
// 目的：取得特殊字符串值
//
//       SCREEN_WIDTH
//       SCREEN_HEIGHT
//       HALF_SCREEN_WIDTH
//       HALF_SCREEN_HEIGHT
//       ...
//
//       如果没有对应的字符串值，将抛出异常
//
int YCUITagHelper::getSpecialValue(const char* input)
{
	SMART_ASSERT(input != NULL);

	if (strcmp(input, "SCREEN_WIDTH") == 0)
	{
		int *pWidth = (int*)YCRegistry::get("width");
		if (pWidth == NULL)
		{
			throw YCException(2002, "YCUITagHelper::getSpecialValue无法取得屏幕宽度！");
		}
		return *pWidth;
	}
	else if (strcmp(input, "SCREEN_HEIGHT") == 0)
	{
		int *pHeight = (int*)YCRegistry::get("height");
		if (pHeight == NULL)
		{
			throw YCException(2002, "YCUITagHelper::getSpecialValue无法取得屏幕高度！");
		}
		return *pHeight;
	}
	else if (strcmp(input, "HALF_SCREEN_WIDTH") == 0)
	{
		int *pWidth = (int*)YCRegistry::get("width");
		if (pWidth == NULL)
		{
			throw YCException(2002, "YCUITagHelper::getSpecialValue无法取得屏幕宽度！");
		}
		return (*pWidth)/2;
	}
	else if (strcmp(input, "HALF_SCREEN_HEIGHT") == 0)
	{
		int *pHeight = (int*)YCRegistry::get("height");
		if (pHeight == NULL)
		{
			throw YCException(2002, "YCUITagHelper::getSpecialValue无法取得屏幕高度！");
		}
		return (*pHeight)/2;
	}
	
	LOG_WARNING("YCUITagHelper::getSpecialValue无效特殊值：" << input);
	throw YCException(2002, "YCUITagHelper::getSpecialValue无效特殊值");

	return 0;
}

//
// 函数：getPropertyEnum(const char* name)
//
// 目的：由css文件的属性字符串名称转换为枚举值
//
E_UITAG_CSS YCUITagHelper::getPropertyEnum(const char* name) const
{
	std::map<std::string, E_UITAG_CSS>::const_iterator at = myCSSProperties.find(name);
	if (at == myCSSProperties.end())
	{
		return UITAG_INVALID;
	}
	return at->second;
}

//
// 函数：getCSSType(E_UITAG_CSS property)
//
// 目的：由css文件的属性字符串类型
//
E_UITAG_PROPERTY_TYPE YCUITagHelper::getCSSType(E_UITAG_CSS property) const
{
	std::map<E_UITAG_CSS, E_UITAG_PROPERTY_TYPE>::const_iterator at = myCSSPropertyType.find(property);
	if (at == myCSSPropertyType.end())
	{
		LOG_WARNING("YCUITagHelper::getCSSType 未知属性类型：" << property);
		throw YCException(2002, "YCUITagHelper::getCSSType未知属性类型");
	}
	return at->second;
}

//
// 函数：getPropertyType(const char* name)
//
// 目的：取得该属性值的类型
//
E_UITAG_PROPERTY_TYPE YCUITagHelper::getPropertyType(const char* name) const
{
	SMART_ASSERT(name != NULL);
	std::map<std::string, E_UITAG_PROPERTY_TYPE>::const_iterator at = myPropertyType.find(name);
	if (at == myPropertyType.end())
	{
		LOG_WARNING("YCUITagHelper::getPropertyType 未知属性类型：" << name);
		throw YCException(2002, "YCUITagHelper::getPropertyType未知属性类型");
	}
	return at->second;
}

/////////////////////////////////////////////////////////////////
// Private Part
/////////////////////////////////////////////////////////////////

YCUITagHelper::YCUITagHelper(void)
{
	// 属性值映射
	myCSSProperties[UITAG_CSS_TOP]						= UITAG_TOP;
	myCSSProperties[UITAG_CSS_LEFT]						= UITAG_LEFT;
	myCSSProperties[UITAG_CSS_RIGHT]					= UITAG_RIGHT;
	myCSSProperties[UITAG_CSS_BOTTOM]					= UITAG_BOTTOM;
	myCSSProperties[UITAG_CSS_WIDTH]					= UITAG_WIDTH;
	myCSSProperties[UITAG_CSS_HEIGHT]					= UITAG_HEIGHT;
	myCSSProperties[UITAG_CSS_ALPHA]					= UITAG_ALPHA;
	myCSSProperties[UITAG_CSS_POSITION]				    = UITAG_POSITION;
	myCSSProperties[UITAG_CSS_MARGIN_TOP]				= UITAG_MARGIN_TOP;
	myCSSProperties[UITAG_CSS_MARGIN_LEFT]				= UITAG_MARGIN_LEFT;
	myCSSProperties[UITAG_CSS_MARGIN_RIGHT]			    = UITAG_MARGIN_RIGHT;
	myCSSProperties[UITAG_CSS_MARGIN_BOTTOM]			= UITAG_MARGIN_BOTTOM;
	myCSSProperties[UITAG_CSS_BORDER_TOP]				= UITAG_BORDER_TOP;
	myCSSProperties[UITAG_CSS_BORDER_LEFT]				= UITAG_BORDER_LEFT;
	myCSSProperties[UITAG_CSS_BORDER_RIGHT]			    = UITAG_BORDER_RIGHT;
	myCSSProperties[UITAG_CSS_BORDER_BOTTOM]			= UITAG_BORDER_BOTTOM;
	myCSSProperties[UITAG_CSS_PADDING_TOP]				= UITAG_PADDING_TOP;
	myCSSProperties[UITAG_CSS_PADDING_LEFT]			    = UITAG_PADDING_LEFT;
	myCSSProperties[UITAG_CSS_PADDING_RIGHT]			= UITAG_PADDING_RIGHT;
	myCSSProperties[UITAG_CSS_PADDING_BOTTOM]			= UITAG_PADDING_BOTTOM;
	myCSSProperties[UITAG_CSS_BACKGROUND_ALPHA]		    = UITAG_BACKGROUND_ALPHA;
	myCSSProperties[UITAG_CSS_BACKGROUND_COLOR]		    = UITAG_BACKGROUND_COLOR;
	myCSSProperties[UITAG_CSS_BACKGROUND_IMAGE]		    = UITAG_BACKGROUND_IMAGE;
	myCSSProperties[UITAG_CSS_BACKGROUND_ANIMATION]	    = UITAG_BACKGROUND_ANIMATION;
	myCSSProperties[UITAG_CSS_BACKGROUND_TOP_LEFT]		= UITAG_BACKGROUND_TOP_LEFT;
	myCSSProperties[UITAG_CSS_BACKGROUND_TOP]			= UITAG_BACKGROUND_TOP;
	myCSSProperties[UITAG_CSS_BACKGROUND_TOP_RIGHT]	    = UITAG_BACKGROUND_TOP_RIGHT;
	myCSSProperties[UITAG_CSS_BACKGROUND_MIDDLE_LEFT]	= UITAG_BACKGROUND_MIDDLE_LEFT;
	myCSSProperties[UITAG_CSS_BACKGROUND_MIDDLE]		= UITAG_BACKGROUND_MIDDLE;
	myCSSProperties[UITAG_CSS_BACKGROUND_MIDDLE_RIGHT]	= UITAG_BACKGROUND_MIDDLE_RIGHT;
	myCSSProperties[UITAG_CSS_BACKGROUND_BOTTOM_LEFT]	= UITAG_BACKGROUND_BOTTOM_LEFT;
	myCSSProperties[UITAG_CSS_BACKGROUND_BOTTOM]		= UITAG_BACKGROUND_BOTTOM;
	myCSSProperties[UITAG_CSS_BACKGROUND_BOTTOM_RIGHT]	= UITAG_BACKGROUND_BOTTOM_RIGHT;
	myCSSProperties[UITAG_CSS_DISPLAY]					= UITAG_DISPLAY;
	myCSSProperties[UITAG_CSS_FLOAT]					= UITAG_FLOAT;
	myCSSProperties[UITAG_CSS_FONT_COLOR]				= UITAG_FONT_COLOR;
	myCSSProperties[UITAG_CSS_FONT_SIZE]				= UITAG_FONT_SIZE;
	myCSSProperties[UITAG_CSS_FONT_WEIGHT]				= UITAG_FONT_WEIGHT;
	myCSSProperties[UITAG_CSS_FONT_FAMILY]				= UITAG_FONT_FAMILY;
	myCSSProperties[UITAG_CSS_FONT_LETTER_SPACING]		= UITAG_FONT_LETTER_SPACING;
	myCSSProperties[UITAG_CSS_FONT_STROKE_COLOR]		= UITAG_FONT_STROKE_COLOR;
	myCSSProperties[UITAG_CSS_TEXT_INDENT]				= UITAG_TEXT_INDENT;
	myCSSProperties[UITAG_CSS_TEXT_ALIGN]				= UITAG_TEXT_ALIGN;
	myCSSProperties[UITAG_CSS_TEXT_DECORATION]			= UITAG_TEXT_DECORATION;
	myCSSProperties[UITAG_CSS_STYLE_IMAGE]				= UITAG_STYLE_IMAGE;
	myCSSProperties[UITAG_CSS_VERTICAL_ALIGN]			= UITAG_VERTICAL_ALIGN;
	myCSSProperties[UITAG_CSS_RADIO_UNCHECKED]			= UITAG_RADIO_UNCHECKED;
	myCSSProperties[UITAG_CSS_RADIO_CHECKED]			= UITAG_RADIO_CHECKED;
	myCSSProperties[UITAG_CSS_CHECKBOX_UNCHECKED]		= UITAG_CHECKBOX_UNCHECKED;
	myCSSProperties[UITAG_CSS_CHECKBOX_CHECKED]			= UITAG_CHECKBOX_CHECKED;
	myCSSProperties[UITAG_CSS_COMBO_ARROW]				= UITAG_COMBO_ARROW;
	myCSSProperties[UITAG_CSS_SCROLL_BEGIN]			    = UITAG_SCROLL_BEGIN;
	myCSSProperties[UITAG_CSS_SCROLL_MOVEUP]			= UITAG_SCROLL_MOVEUP;
	myCSSProperties[UITAG_CSS_SCROLL_SLIDER]			= UITAG_SCROLL_SLIDER;
	myCSSProperties[UITAG_CSS_SCROLL_MOVEDOWN]			= UITAG_SCROLL_MOVEDOWN;
	myCSSProperties[UITAG_CSS_SCROLL_END]				= UITAG_SCROLL_END;
	myCSSProperties[UITAG_CSS_OVERFLOW]				    = UITAG_OVERFLOW;
	myCSSProperties[UITAG_CSS_FILE]					    = UITAG_FILE;
	myCSSProperties[UITAG_CSS_MULTILINE]				= UITAG_MULTILINE;
	myCSSProperties[UITAG_CSS_LETTER_LIMIT]			    = UITAG_LETTER_LIMIT;
	myCSSProperties[UITAG_CSS_INPUT_VALIDATE]			= UITAG_INPUT_VALIDATE;
	myCSSProperties[UITAG_CSS_GROUP]					= UITAG_GROUP;
	myCSSProperties[UITAG_CSS_WEIGHT]					= UITAG_WEIGHT;
	myCSSProperties[UITAG_CSS_MODE]						= UITAG_MODE;
	myCSSProperties[UITAG_CSS_REPEAT]					= UITAG_REPEAT;
	myCSSProperties[UITAG_CSS_TABLE_DELIMITER]          = UITAG_TABLE_DELIMITER;

	//CSS属性类型映射
	myCSSPropertyType[UITAG_TOP]						= TAG_STRING;
	myCSSPropertyType[UITAG_LEFT]						= TAG_STRING;
	myCSSPropertyType[UITAG_RIGHT]						= TAG_STRING;
	myCSSPropertyType[UITAG_BOTTOM]						= TAG_STRING;
	myCSSPropertyType[UITAG_WIDTH]						= TAG_INT;
	myCSSPropertyType[UITAG_HEIGHT]						= TAG_INT;
	myCSSPropertyType[UITAG_ALPHA]						= TAG_DOUBLE;
	myCSSPropertyType[UITAG_POSITION]					= TAG_STRING;
	myCSSPropertyType[UITAG_MARGIN_TOP]					= TAG_INT;
	myCSSPropertyType[UITAG_MARGIN_LEFT]				= TAG_INT;
	myCSSPropertyType[UITAG_MARGIN_RIGHT]				= TAG_INT;
	myCSSPropertyType[UITAG_MARGIN_BOTTOM]				= TAG_INT;
	myCSSPropertyType[UITAG_BORDER_TOP]					= TAG_STRING;
	myCSSPropertyType[UITAG_BORDER_LEFT]				= TAG_STRING;
	myCSSPropertyType[UITAG_BORDER_RIGHT]				= TAG_STRING;
	myCSSPropertyType[UITAG_BORDER_BOTTOM]				= TAG_STRING;
	myCSSPropertyType[UITAG_PADDING_TOP]				= TAG_INT;
	myCSSPropertyType[UITAG_PADDING_LEFT]				= TAG_INT;
	myCSSPropertyType[UITAG_PADDING_RIGHT]				= TAG_INT;
	myCSSPropertyType[UITAG_PADDING_BOTTOM]				= TAG_INT;
	myCSSPropertyType[UITAG_BACKGROUND_ALPHA]			= TAG_DOUBLE;
	myCSSPropertyType[UITAG_BACKGROUND_COLOR]			= TAG_STRING;
	myCSSPropertyType[UITAG_BACKGROUND_IMAGE]			= TAG_STRING;
	myCSSPropertyType[UITAG_BACKGROUND_ANIMATION]		= TAG_STRING;
	myCSSPropertyType[UITAG_BACKGROUND_TOP_LEFT]		= TAG_STRING;
	myCSSPropertyType[UITAG_BACKGROUND_TOP]				= TAG_STRING;
	myCSSPropertyType[UITAG_BACKGROUND_TOP_RIGHT]		= TAG_STRING;
	myCSSPropertyType[UITAG_BACKGROUND_MIDDLE_LEFT]		= TAG_STRING;
	myCSSPropertyType[UITAG_BACKGROUND_MIDDLE]			= TAG_STRING;
	myCSSPropertyType[UITAG_BACKGROUND_MIDDLE_RIGHT]	= TAG_STRING;
	myCSSPropertyType[UITAG_BACKGROUND_BOTTOM_LEFT]		= TAG_STRING;
	myCSSPropertyType[UITAG_BACKGROUND_BOTTOM]			= TAG_STRING;
	myCSSPropertyType[UITAG_BACKGROUND_BOTTOM_RIGHT]	= TAG_STRING;
	myCSSPropertyType[UITAG_DISPLAY]					= TAG_STRING;
	myCSSPropertyType[UITAG_FLOAT]						= TAG_STRING;
	myCSSPropertyType[UITAG_FONT_COLOR]					= TAG_STRING;
	myCSSPropertyType[UITAG_FONT_SIZE]					= TAG_INT;
	myCSSPropertyType[UITAG_FONT_WEIGHT]				= TAG_INT;
	myCSSPropertyType[UITAG_FONT_FAMILY]				= TAG_STRING;
	myCSSPropertyType[UITAG_FONT_LETTER_SPACING]		= TAG_INT;
	myCSSPropertyType[UITAG_FONT_STROKE_COLOR]			= TAG_STRING;
	myCSSPropertyType[UITAG_TEXT_INDENT]				= TAG_INT;
	myCSSPropertyType[UITAG_TEXT_ALIGN]					= TAG_STRING;
	myCSSPropertyType[UITAG_TEXT_DECORATION]			= TAG_STRING;
	myCSSPropertyType[UITAG_STYLE_IMAGE]				= TAG_STRING;
	myCSSPropertyType[UITAG_VERTICAL_ALIGN]				= TAG_STRING;
	myCSSPropertyType[UITAG_RADIO_UNCHECKED]			= TAG_STRING;
	myCSSPropertyType[UITAG_RADIO_CHECKED]				= TAG_STRING;
	myCSSPropertyType[UITAG_CHECKBOX_UNCHECKED]			= TAG_STRING;
	myCSSPropertyType[UITAG_CHECKBOX_CHECKED]			= TAG_STRING;
	myCSSPropertyType[UITAG_COMBO_ARROW]				= TAG_STRING;
	myCSSPropertyType[UITAG_SCROLL_BEGIN]				= TAG_STRING;
	myCSSPropertyType[UITAG_SCROLL_MOVEUP]				= TAG_STRING;
	myCSSPropertyType[UITAG_SCROLL_SLIDER]				= TAG_STRING;
	myCSSPropertyType[UITAG_SCROLL_MOVEDOWN]			= TAG_STRING;
	myCSSPropertyType[UITAG_SCROLL_END]					= TAG_STRING;
	myCSSPropertyType[UITAG_OVERFLOW]					= TAG_STRING;
	myCSSPropertyType[UITAG_FILE]						= TAG_STRING;
	myCSSPropertyType[UITAG_MULTILINE]					= TAG_STRING;
	myCSSPropertyType[UITAG_LETTER_LIMIT]				= TAG_INT;
	myCSSPropertyType[UITAG_INPUT_VALIDATE]				= TAG_STRING;
	myCSSPropertyType[UITAG_GROUP]                      = TAG_STRING;
	myCSSPropertyType[UITAG_WEIGHT]                     = TAG_INT;
	myCSSPropertyType[UITAG_MODE]                       = TAG_INT;
	myCSSPropertyType[UITAG_REPEAT]                     = TAG_BOOLEAN;
	myCSSPropertyType[UITAG_TABLE_DELIMITER]            = TAG_STRING;

	myPropertyType["id"]                                = TAG_STRING;
	myPropertyType["class"]								= TAG_STRING;
	myPropertyType["accelerateKey"]						= TAG_STRING;
	myPropertyType["onClick"]							= TAG_STRING;
	myPropertyType["onOpen"]							= TAG_STRING;
	myPropertyType["onClose"]							= TAG_STRING;
}


YCUITagHelper::~YCUITagHelper(void)
{

}
