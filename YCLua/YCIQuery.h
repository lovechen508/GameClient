#ifndef _INCLUDE_YCIQUERY_H_
#define _INCLUDE_YCIQUERY_H_

/*
 * YCIQuery : 类JQuery接口实现
 */

class YCAPI YCIQuery
{
public:

	//
	// 函数：YCIQuery()
	//
	// 目的：构造函数，绑定到窗口
	//
	YCIQuery() {}

	virtual ~YCIQuery() {}

	//
	// 函数：$(const char* selector)
	//
	// 目的：根据selector选择器取得YCIUITag列表
	//
	virtual YCIQuery& $(const char* selector) = 0;

	//
	// 函数：css(const char *attr, const char *value)
	//
	// 目的：修改CSS属性
	//
	virtual YCIQuery& css(const char *css, const char *value) = 0;

	//
	// 函数：attr(const char* attr, const char *value)
	//
	// 目的：
	//
	virtual YCIQuery& attr(const char* attr, const char *value) = 0;

	//
	// 函数：value(const char* value)
	//
	// 目的：获取元件的值
	//
	virtual YCIQuery& value(const char* value) = 0;

	//
	// 函数：value()
	//
	// 目的：获取元件的值
	//
	virtual const char* value() = 0;

	//
	// 函数：html(const char *value)
	//
	// 目的：
	//
	virtual YCIQuery& html(const char *html) = 0;

	//
	// 函数：append()
	//
	// 目的：
	//
	virtual YCIQuery& append(const char* html) = 0;

	//
	// 函数：each()
	//
	// 目的：
	//
	virtual YCIQuery& each() = 0;

	//
	// 函数：hover()
	//
	// 目的：
	//
	virtual YCIQuery& hover() = 0;

	//
	// 函数：toggle()
	//
	// 目的：
	//
	virtual YCIQuery& toggle() = 0;

	//
	// 函数：hide()
	//
	// 目的：
	//
	virtual YCIQuery& hide() = 0;

	//
	// 函数：show()
	//
	// 目的：
	//
	virtual YCIQuery& show() = 0;

	//
	// 函数：fadeIn()
	//
	// 目的：
	//
	virtual YCIQuery& fadeIn() = 0;

	//
	// 函数：fadeOut()
	//
	// 目的：
	//
	virtual YCIQuery& fadeOut() = 0;

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCIQuery& animation() = 0;

	//
	// 函数：move(const char* speed, int x, int y)
	//
	// 目的：移动元件中心点到(x, y)
	//
	virtual YCIQuery& move(const char* speed, int x, int y) = 0;

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCIQuery& parent() = 0;

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCIQuery& child() = 0;

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCIQuery& click() = 0;

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCIQuery& submit() = 0;

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCIQuery& slideUp() = 0;

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCIQuery& slideDown() = 0;

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCIQuery& addClass() = 0;

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCIQuery& removeClass() = 0;

	//
	// 函数：end()
	//
	// 目的：清除selector
	//
	virtual YCIQuery& end() = 0;
};

#endif