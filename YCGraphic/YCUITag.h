#ifndef _INCLUDE_YCUITAG_H_
#define _INCLUDE_YCUITAG_H_

#include "YCIUITag.h"
#include "YCUITagDef.h"

/*
 * YCUITag : UI 标签的缺省实现
 */
class YCUITag :
	public YCIUITag
{
public:
	
	YCUITag(const YCUITagMeta* tagMeta);

	virtual ~YCUITag(void);

	//
	// 函数：matchAccelerateKey()
	//
	// 目的：是否匹配快捷键
	//
	virtual bool matchAccelerateKey();

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
	virtual bool open(YCLuaParameter* context);

	//
	// 函数：close()
	//
	// 目的： 关闭一个元件实例
	//
	virtual void close();

protected:

	//
	// 函数：extraCSS()
	//
	// 目的：处理标签特定的CSS属性
	//
	virtual void extraCSS();

	//
	// 函数：extraDraw()
	//
	// 目的：UI元件的自定义绘制
	//
	virtual void extraDraw();

private:

	//
	// 函数：fetchPosition()
	//
	// 目的：从CSS的top、left来确定位置
	//
	virtual void fetchPosition();
};

#endif
