#ifndef _INCLUDE_YCICOMPONENT_H_
#define _INCLUDE_YCICOMPONENT_H_

#include "YCEvent.h"
#include "YCUITagDef.h"
#include "YCGraphic.h"

#include "YCBasic\YCDef.h"
#include "YCInput\YCInput.h"

/*
 *    YCIComponent 窗口UI部件基类
 *
 * 
 *     ----------------------------------------------------------------------------
 *    |                                                                            |
 *    |                                Margin-top                                  |
 *    |                                                                            |
 *    |                --------------------------------------------                |
 *    |               |                                            |               |
 *    |               |                padding-top                 |               |
 *    |               |                                            |               |
 *    |               |                -----------                 |               |
 *    |               |               |           |                |               |
 *    |               |               |           |                |               |
 *    |               |               |           |                |               |
 *    | margin-left   |  padding-left |  Texture  | padding-right  | margin-right  |
 *    |               |               |           |                |               |
 *    |               |               |           |                |               |
 *    |               |               |           |                |               |
 *    |               |                -----------                 |               |
 *    |               |                                            |               |
 *    |               |               padding-bottom               |               |
 *    |               |                                            |               |
 *    |                --------------------------------------------                |
 *    |                                                                            |
 *    |                                Margin-bottom                               |
 *    |                                                                            |
 *     ----------------------------------------------------------------------------
 *
 */

class YCDList;
class YCIModel;
class YCIBackground;
class YCTagFactory;
class YCLuaParameter;
class YCIInputListener;
class YCIEventListener;

class YCAPI YCIComponent
{
public:

	/*
	 * 元件的四个方向定义
	 */
	enum E_COMPONENT_DIRECTION {
		DIRECTION_TOP    = 1,
		DIRECTION_LEFT   = 2,
		DIRECTION_RIGHT  = 3,
		DIRECTION_BOTTOM = 4,
	};

	enum E_COMPONENT_STATUS {
		STATUS_CLOSE  = 0,        // 元件打开
		STATUS_OPEN   = 1,        // 元件关闭
		STATUS_HIDE   = 2,        // 元件隐藏
	};

	// 快速访问鼠标键盘
	static YCInput * gInput;

	// 快速访问绘制精灵对象
	static YCGraphic::YCD3DSprite * gSprite;

	// 快速访问基本图形绘制对象
	static YCGraphic::YCD3DPrimitive * gPrimitive;

	// 快速访问绘制字符对象
	static YCGraphic::YCFont * gFont;

	// 快速访问标签工厂
	static YCTagFactory* gTagFactory;

public:

	YCIComponent(void);

	virtual ~YCIComponent(void);
	
	//
	// 函数：modelUpdate(YCEvent& ev)
	//
	// 目的：通知数据模型更新
	//
	virtual void modelUpdate(YCEvent& ev);

	//
	// 函数：render()
	//
	// 目的：在指定处绘制该UI元件及其子控件
	//
	virtual void render() = 0;
		
	//
	// 函数：draw()
	//
	// 目的：在指定处绘制该UI元件自身
	//
	virtual void draw() = 0;

	//
	// 函数：setValue(const char* strValue)
	//
	// 目的：设置元件的值
	//
	void setValue(const char* strValue);

	//
	// 函数：getValue()
	//
	// 目的：获取元件的值
	//
	const char* getValue();

	//
	// 函数：setOrder(unsigned int order)
	//
	// 目的：设置当前元件在的UILayer中的order
	//
	void setOrder(unsigned int order);

	//
	// 函数：getOrder()
	//
	// 目的：得到当前元件在的UILayer中的order
	//       order值越大，距用户越近
	//
	// 注释：在YCIComponent中无设置order接口
	//       所有元件的order默认为0，按照添加到
	//       UILayer的先后顺序逆序画
	//
	unsigned int getOrder();
	
	//
	// 函数：E_COMPONENT_STATUS()
	//
	// 目的：取得窗口名称
	//
	E_COMPONENT_STATUS getStatus();

	//
	// 函数：matchAccelerateKey()
	//
	// 目的：是否匹配快捷键
	//
	virtual bool matchAccelerateKey() = 0;

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
	virtual bool open(YCLuaParameter* context) = 0;

	//
	// 函数：close()
	//
	// 目的： 关闭一个元件实例
	//
	virtual void close() = 0;


	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	//
	// 函数：addListener(E_UITAG_EVENT event, const char* scripts)
	//
	// 目的：添加消息监听器
	//
	void addListener(E_UITAG_EVENT event, const char* scripts);

	//
	// 函数：getListener(E_UITAG_EVENT event)
	//
	// 目的：获取消息监听器
	//
	const char* getListener(E_UITAG_EVENT event);

	//
	// 函数：removeListener(E_UITAG_EVENT event)
	//
	// 目的：删除消息监听器
	//
	bool removeListener(E_UITAG_EVENT event);

private:

	//
	// 函数：fetchPosition()
	//
	// 目的：从CSS的top、left来确定位置
	//
	virtual void fetchPosition() = 0;

protected:
	
	//
	// 属性：myStrValue
	//
	// 说明：元件的值
	//
	char * myStrValue;

	//
	//在UILayer上的顺序，值越大越近
	//
	unsigned int myOrder;            

	//
	// 属性：myWindowStatus
	//
	// 目的：窗口打开状态
	//
	E_COMPONENT_STATUS myComponentStatus;

	//
	//不持有生命周期
	//
	YCDList* myListeners;
};

#endif
