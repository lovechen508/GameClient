#ifndef _INCLUDE_YCUITAGDEF_H_
#define _INCLUDE_YCUITAGDEF_H_


#define MAX_ACCELERATE_KEYS 2

/*
 * YCUITagDef : 标签定义文件
 */

#define TAG_NAME_LENGTH				20
#define GROUP_NAME_LENGTH			20
#define ATTRIBUTE_NAME_LENGTH		20
#define MAX_CONTAINER_NAME			20


//
// Core Tag 标签名
//
#define UITAG_STR_WINDOW			"window"
#define UITAG_STR_DIALOG			"dialog"
#define UITAG_STR_TITLE				"title"
#define UITAG_STR_SCRIPT			"script"
#define UITAG_STR_LAYOUT			"layout"
#define UITAG_STR_DIV				"div"
#define UITAG_STR_SPAN				"span"
#define UITAG_STR_A					"a"
#define UITAG_STR_P					"p"
#define UITAG_STR_IMG				"img"
#define UITAG_STR_TABLE				"table"
#define UITAG_STR_TABLE_TH			"th"
#define UITAG_STR_TABLE_TR			"tr"
#define UITAG_STR_TABLE_TD			"td"
#define UITAG_STR_INPUT_BUTTON		"button"
#define UITAG_STR_INPUT_RADIO		"radio"
#define UITAG_STR_INPUT_EDIT		"edit"
#define UITAG_STR_INPUT_MULTIEDIT	"multiedit"
#define UITAG_STR_INPUT_CHECKBOX	"checkbox"
#define UITAG_STR_INPUT_COMBOBOX	"combobox"
#define UITAG_STR_CODE              "code"

//
// Core Tag 核心标签
//
enum E_UITAG_CORE
{
	UITAG_CORE_WINDOW				=  1,     // 窗口
	UITAG_CORE_DIALOG				=  2,     // 对话框

	UITAG_CORE_TITLE                =  3,     // 窗口名
	UITAG_CORE_SCRIPT               =  4,     // 引用Lua文件
	UITAG_CORE_LAYOUT               =  5,     // 引用CSS文件

	UITAG_CORE_DIV					=  6,     // div定位
	UITAG_CORE_SPAN					=  7,     // span定位

	UITAG_CORE_A                    =  8,     // a
	UITAG_CORE_P                    =  9,     // p
	UITAG_CORE_IMG                  = 10,     // img

	UITAG_CORE_TABLE                = 11,     // table
	UITAG_CORE_TABLE_TH             = 12,     // th
	UITAG_CORE_TABLE_TR             = 13,     // tr
	UITAG_CORE_TABLE_TD             = 14,     // td

	UITAG_CORE_INPUT_BUTTON         = 15,     // button
	UITAG_CORE_INPUT_RADIO          = 16,     // radio
	UITAG_CORE_INPUT_EDIT           = 17,     // edit 单行文本框
	UITAG_CORE_INPUT_MULTIEDIT      = 18,     // multiedit 多行文本框
	UITAG_CORE_INPUT_CHECKBOX       = 19,     // checkbox 复选框
	UITAG_CORE_INPUT_COMBOBOX       = 20,     // 下拉框
	
	UITAG_CORE_CODE                 = 21,     // lua代码
};

//
// 标签基本属性名
//
#define UITAG_PROPERTY_ID                   "id"
#define UITAG_PROPERTY_CLASS                "class"
#define UITAG_PROPERTY_ACCELERATEKEY        "accelerateKey"
#define UITAG_PROPERTY_ONOPEN               "onOpen"
#define UITAG_PROPERTY_ONCLOSE              "onClose"
#define UITAG_PROPERTY_ONCLICK              "onClick"

//
// 标签CSS属性名
//
#define UITAG_CSS_TOP                       "top"
#define UITAG_CSS_LEFT                      "left"
#define UITAG_CSS_RIGHT                     "right"
#define UITAG_CSS_BOTTOM                    "bottom"
#define UITAG_CSS_WIDTH						"width"
#define UITAG_CSS_HEIGHT					"height"
#define UITAG_CSS_ALPHA                     "alpha"
#define UITAG_CSS_POSITION					"position"
#define UITAG_CSS_MARGIN_TOP				"margin-top"
#define UITAG_CSS_MARGIN_LEFT				"margin-left"
#define UITAG_CSS_MARGIN_RIGHT				"margin-right"
#define UITAG_CSS_MARGIN_BOTTOM				"margin-bottom"
#define UITAG_CSS_BORDER_TOP				"border-top"
#define UITAG_CSS_BORDER_LEFT				"border-left"
#define UITAG_CSS_BORDER_RIGHT				"border-right"
#define UITAG_CSS_BORDER_BOTTOM				"border-bottom"
#define UITAG_CSS_PADDING_TOP				"padding-top"
#define UITAG_CSS_PADDING_LEFT				"padding-left"
#define UITAG_CSS_PADDING_RIGHT				"padding-right"
#define UITAG_CSS_PADDING_BOTTOM			"padding-bottom"
#define UITAG_CSS_BACKGROUND_ALPHA			"background-alpha"
#define UITAG_CSS_BACKGROUND_COLOR			"background-color"
#define UITAG_CSS_BACKGROUND_IMAGE			"background-image"
#define UITAG_CSS_BACKGROUND_ANIMATION		"background-animation"
#define UITAG_CSS_BACKGROUND_TOP_LEFT		"background-top-left"
#define UITAG_CSS_BACKGROUND_TOP			"background-top"
#define UITAG_CSS_BACKGROUND_TOP_RIGHT		"background-top-right"
#define UITAG_CSS_BACKGROUND_MIDDLE_LEFT	"background-middle-left"
#define UITAG_CSS_BACKGROUND_MIDDLE			"background-middle"
#define UITAG_CSS_BACKGROUND_MIDDLE_RIGHT	"background-middle-right"
#define UITAG_CSS_BACKGROUND_BOTTOM_LEFT	"background-bottom-left"
#define UITAG_CSS_BACKGROUND_BOTTOM			"background-bottom"
#define UITAG_CSS_BACKGROUND_BOTTOM_RIGHT	"background-bottom-right"
#define UITAG_CSS_DISPLAY					"display"
#define UITAG_CSS_FLOAT                     "float"
#define UITAG_CSS_FONT_COLOR				"font-color"
#define UITAG_CSS_FONT_SIZE					"font-size"
#define UITAG_CSS_FONT_WEIGHT				"font-weight"
#define UITAG_CSS_FONT_FAMILY				"font-family"
#define UITAG_CSS_FONT_LETTER_SPACING		"font-letter-spacing"
#define UITAG_CSS_FONT_STROKE_COLOR			"font-stroke-color"
#define UITAG_CSS_TEXT_INDENT				"text-indent"
#define UITAG_CSS_TEXT_ALIGN				"text-align"
#define UITAG_CSS_TEXT_DECORATION			"text-decoration"
#define UITAG_CSS_STYLE_IMAGE				"list-image"
#define UITAG_CSS_VERTICAL_ALIGN			"valign"
#define UITAG_CSS_RADIO_UNCHECKED			"radio-unchecked"
#define UITAG_CSS_RADIO_CHECKED				"radio-checked"
#define UITAG_CSS_CHECKBOX_UNCHECKED		"checkbox-unchecked"
#define UITAG_CSS_CHECKBOX_CHECKED			"checkbox-checked"
#define UITAG_CSS_COMBO_ARROW				"combo-arrow"
#define UITAG_CSS_SCROLL_BEGIN				"scroll-begin"
#define UITAG_CSS_SCROLL_MOVEUP				"scroll-moveup"
#define UITAG_CSS_SCROLL_SLIDER				"scroll-slider"
#define UITAG_CSS_SCROLL_MOVEDOWN			"scroll-movedown"
#define UITAG_CSS_SCROLL_END				"scroll-end"
#define UITAG_CSS_OVERFLOW					"overflow"
#define UITAG_CSS_FILE                      "file"
#define UITAG_CSS_MULTILINE					"multilines"
#define UITAG_CSS_LETTER_LIMIT              "letter-limit"
#define UITAG_CSS_INPUT_VALIDATE            "input-validate"
#define UITAG_CSS_GROUP                     "group"
#define UITAG_CSS_WEIGHT                    "weight"
#define UITAG_CSS_MODE                      "mode"
#define UITAG_CSS_REPEAT                    "repeat"
#define UITAG_CSS_TABLE_DELIMITER           "table-delimiter"

//
// 标签伪类
//
enum E_UITAG_STATUS {
	UITAG_STATUS_NORMAL     = 0x00000001,
	UITAG_STATUS_HOVER      = 0x00000002,
	UITAG_STATUS_MOUSEDOWN  = 0x00000004,
	UITAG_STATUS_DISABLED   = 0x00000008,
};

//
// 标签CSS属性
//
enum E_UITAG_CSS {
	UITAG_INVALID							= -1,
	
	/* top&left */
	UITAG_TOP                               =  1,
	UITAG_LEFT                              ,
	UITAG_RIGHT                             ,
	UITAG_BOTTOM                            ,

	/* width&height */
	UITAG_WIDTH								,
	UITAG_HEIGHT							,

	/* alpha 透明度 */
	UITAG_ALPHA                             ,

	/* position */
	UITAG_POSITION							,    //relative, absolute

	/* margin */
	UITAG_MARGIN_TOP						,    // 3px
	UITAG_MARGIN_LEFT						,
	UITAG_MARGIN_RIGHT						,
	UITAG_MARGIN_BOTTOM						,

	/* border */
	UITAG_BORDER_TOP						,    // 1px solid 0xFFFF0000
	UITAG_BORDER_LEFT						,
	UITAG_BORDER_RIGHT						,
	UITAG_BORDER_BOTTOM						,

	/* padding */
	UITAG_PADDING_TOP						,    // 3px
	UITAG_PADDING_LEFT						,
	UITAG_PADDING_RIGHT						,
	UITAG_PADDING_BOTTOM					,

	/* background 9块*/
	UITAG_BACKGROUND_ALPHA					,    //背景透明度    [0.0f, 1.0f]
	UITAG_BACKGROUND_COLOR					,    //背景色        0xFFCCCCCC
	UITAG_BACKGROUND_IMAGE					,    //单块背景图    ..\..\image\background.png
	UITAG_BACKGROUND_ANIMATION				,    //动画背景      ..\..\..\ani.xml
	UITAG_BACKGROUND_TOP_LEFT				,    //9块           ..\..\image\bg_tl.png
	UITAG_BACKGROUND_TOP					,
	UITAG_BACKGROUND_TOP_RIGHT				,
	UITAG_BACKGROUND_MIDDLE_LEFT			,
	UITAG_BACKGROUND_MIDDLE					,
	UITAG_BACKGROUND_MIDDLE_RIGHT			,
	UITAG_BACKGROUND_BOTTOM_LEFT			,
	UITAG_BACKGROUND_BOTTOM					,
	UITAG_BACKGROUND_BOTTOM_RIGHT			,

	/* display */
	UITAG_DISPLAY							,

	/* float */
	UITAG_FLOAT								,

	/* font */
	UITAG_FONT_COLOR						,    //颜色
	UITAG_FONT_SIZE							,    //大小
	UITAG_FONT_WEIGHT						,    //粗细
	UITAG_FONT_FAMILY						,    //字体
	UITAG_FONT_LETTER_SPACING				,    //字间距
	UITAG_FONT_STROKE_COLOR					,    //描边色

	/* text */
	UITAG_TEXT_INDENT						,    //首行缩进
	UITAG_TEXT_ALIGN						,    //字符水平对齐
	UITAG_TEXT_DECORATION					,	 //文本装饰

	/* ul, li */
	UITAG_STYLE_IMAGE						,    //列表图片

	/* table */
	UITAG_VERTICAL_ALIGN					,    //td纵向对齐方式

	/* button */

	/* radio */
	UITAG_RADIO_UNCHECKED                   ,   //未选中图片
	UITAG_RADIO_CHECKED                     ,   //已选中图片

	/* checkbox */
	UITAG_CHECKBOX_UNCHECKED                ,   //未选中图片
	UITAG_CHECKBOX_CHECKED                  ,   //已选中图片

	/* combo */
	UITAG_COMBO_ARROW                       ,   //向下箭头图片

	/* scroll */
	UITAG_SCROLL_BEGIN                      ,   //回到顶部
	UITAG_SCROLL_MOVEUP                     ,   //向上移动
	UITAG_SCROLL_SLIDER                     ,   //滑块图标
	UITAG_SCROLL_MOVEDOWN                   ,   //向下移动
	UITAG_SCROLL_END                        ,   //回到底部

	UITAG_OVERFLOW                          ,   //内容溢出
	UITAG_FILE                              ,   //文件引用

	UITAG_MULTILINE                         ,   //框内显示的行数
	UITAG_LETTER_LIMIT                      ,   //字符数限制
	UITAG_INPUT_VALIDATE                    ,   //输入校验

	UITAG_GROUP                             ,   //窗口组
	UITAG_WEIGHT                            ,   //窗口权重
	UITAG_MODE                              ,   //模态窗口

	UITAG_REPEAT                            ,   //动画是否重复
	UITAG_TABLE_DELIMITER                   ,   //表格分隔线
};

//
// CSS 选择器优先级
//
enum E_UITAG_CSS_PRIORITY {
	INLINE_PRIORITY = 1000,
	ID_PRIORITY		= 100,
	CLASS_PRIORITY	= 10,
	PARENT_PRIORITY = 5,
	TAG_PRIORITY	= 1,
};

//
// 属性值类型
//
enum E_UITAG_PROPERTY_TYPE {
	TAG_INT     = 1,
	TAG_DOUBLE  = 2,
	TAG_BOOLEAN = 3,
	TAG_STRING  = 4,
};

//
// 元件浮动模式：不浮动，浮动到右边，浮动到左边
//
enum E_UITAG_FLOAT_MODE {
	FLOAT_NONE  = 0,
	FLOAT_LEFT  = 1,
	FLOAT_RIGHT = 2
};

//
// 元件显示模式：行内模式，块模式
//
enum E_UITAG_DISPLAY_MODE {
	NONE_MODE   = 0,
	INLINE_MODE = 1,
	BLOCK_MODE  = 2,	
};

//
// 元件定位模式：相对模式，绝对定位模式
//
enum E_UITAG_POSITION_MODE {
	RELATION_MODE = 1,
	OBSOLUTE_MODE = 2,
};

//
// 元件响应消息枚举
//
enum E_UITAG_EVENT {
	UITAG_OPEN_EVENT		= 1,        //打开事件
	UITAG_CLOSE_EVENT		= 2,        //关闭事件
	UITAG_CLICK_EVENT		= 3,        //鼠标点击
	UITAG_BLUR_EVENT		= 4,		//失去焦点
	UITAG_FOCUS_EVENT		= 5,        //获得焦点
	UITAG_HOVERON_EVENT		= 6,        //鼠标移入
	UITAG_HOVEROFF_EVENT	= 7,		//鼠标移出
	UITAG_RESIZE_EVENT		= 8,        //更改大小
};

#define STR_FONT_YAHEI "微软雅黑"
#define STR_FONT_SONG  "宋体"
#define STR_FONT_HEI   "黑体"
#define STR_FONT_LISHU "隶书"
#define STR_FONT_KAI   "楷体"

//
// 字体枚举
//
enum E_FONT_TYPE {
	FONT_SONG				= 1,		//宋体
	FONT_YAHEI				= 2,		//雅黑
	FONT_HEI				= 3,		//黑体
	FONT_LISHU				= 4,		//隶书
	FONT_KAI				= 5,		//楷体
};

//
// 字符对齐模式枚举
//
enum E_TEXT_ALIGN {
	TEXT_ALIGN_LEFT		   	= 1,         //左对齐
	TEXT_ALIGN_MIDDLE		= 2,         //居中
	TEXT_ALIGN_RIGHT		= 3,         //右对齐
};

//
// 字体粗细枚举
//
enum E_FONT_WEIGHT {
	FONT_WEIGHT_NORMAL		= 1,        //正常
	FONT_WEIGHT_BOLD		= 2,		//加粗
	FONT_WEIGHT_BOLDER      = 3,        //超粗
};

//
// 字体方向
//
enum E_FONT_DIRECTION {
	FONT_HORIZONTAL         = 1,        //水平
	FONT_VERTICAL           = 2,        //竖直
};

// 
// 元件移动速度枚举
//
enum E_UITAG_MOVESPEED {
	UITAG_MOVE_SLOW			= 1,        //慢
	UITAG_MOVE_NORMAL		= 2,        //中
	UITAG_MOVE_FAST         = 3,        //快
};

#endif