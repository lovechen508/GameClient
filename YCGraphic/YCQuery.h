#ifndef _INCLUDE_YCQUERY_H_
#define _INCLUDE_YCQUERY_H_

#include "YCLua\YCIQuery.h"

class YCLuaContext;

class YCQuery : public YCIQuery
{
public:
	
	//
	// 函数：YCQuery()
	//
	// 目的：构造函数，绑定到窗口
	//
	YCQuery();

	virtual ~YCQuery();

	//
	// 函数：bind(YCLuaContext * context)
	//
	// 目的：绑定上下文
	//
	void bind(YCLuaContext * context);

	//
	// 函数：$(const char* selector)
	//
	// 目的：根据selector选择器取得YCIUITag列表
	//
	virtual YCQuery& $(const char* selector);

	//
	// 函数：css(const char *attr, const char *value)
	//
	// 目的：修改CSS属性
	//
	virtual YCQuery& css(const char *css, const char *value);

	//
	// 函数：attr(const char* attr, const char *value)
	//
	// 目的：
	//
	virtual YCQuery& attr(const char* attr, const char *value);

	//
	// 函数：value(const char* value)
	//
	// 目的：获取元件的值
	//
	virtual YCIQuery& value(const char* value);

	//
	// 函数：value()
	//
	// 目的：获取元件的值
	//
	virtual const char* value();

	//
	// 函数：html(const char *value)
	//
	// 目的：
	//
	virtual YCQuery& html(const char *html);

	//
	// 函数：append()
	//
	// 目的：
	//
	virtual YCQuery& append(const char* html);

	//
	// 函数：each()
	//
	// 目的：
	//
	virtual YCQuery& each();

	//
	// 函数：hover()
	//
	// 目的：
	//
	virtual YCQuery& hover();

	//
	// 函数：toggle()
	//
	// 目的：
	//
	virtual YCQuery& toggle();

	//
	// 函数：hide()
	//
	// 目的：
	//
	virtual YCQuery& hide();

	//
	// 函数：show()
	//
	// 目的：
	//
	virtual YCQuery& show();

	//
	// 函数：fadeIn()
	//
	// 目的：
	//
	virtual YCQuery& fadeIn();

	//
	// 函数：fadeOut()
	//
	// 目的：
	//
	virtual YCQuery& fadeOut();

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCQuery& animation();

	//
	// 函数：move(const char* speed, int x, int y)
	//
	// 目的：移动元件中心点到(x, y)
	//
	virtual YCQuery& move(const char* speed, int x, int y);

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCQuery& parent();

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCQuery& child();

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCQuery& click();

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCQuery& submit();

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCQuery& slideUp();

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCQuery& slideDown();

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCQuery& addClass();

	//
	// 函数：
	//
	// 目的：
	//
	virtual YCQuery& removeClass();

	//
	// 函数：end()
	//
	// 目的：清除selector
	//
	virtual YCQuery& end();

private:

	//
	// 属性：myContext
	//
	// 目的：当前YCQuery执行的上下文
	//       当前调用序列中由选择器匹配到的Tag列表
	//       实际上以selector为Key保存在myContext中
	//
	YCLuaContext * myContext;
};

#endif

