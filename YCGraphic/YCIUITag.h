#ifndef _INCLUDE_YCIUITAG_H_
#define _INCLUDE_YCIUITAG_H_

#include "YCRect.h"
#include "YCIComponent.h"
#include "YCUITagDef.h"
#include "YCUITagMeta.h"

#include "YCBasic\YCQueue.h"

class YCUIDOM;
class YCCSSNode;
class YCIContainer;
class YCCSSContext;
class YCIUITagImpl;
class YCTagFactory;
class YCUIHoverState;
class YCDynamicUIDOM;

/*
 * 类名：YCIUITag
 *
 * 注释：UI部分窗口标签接口类
 *       继承自YCIComponent
 *                                                 
 *                                                -----------------------                                     
 *                                               |      YCIComponent     |                                 
 *                                                -----------------------       
 *                                                           |
 *                                                -----------------------  
 *                                               |        YCIUITag       |
 *                                                -----------------------                                 
 *                                                           |          
 *                              -----------------------------------------------------------
 *                             |                                                           |
 *                  -----------------------                                       --------------------                           
 *                 |      YCIContainer     |                                     |      YCUITag       |                               
 *                  -----------------------                                       --------------------       
 *                             |                                                           |
 *              ------------------------------                  ------------------------------------------------------------------------------------------
 *             |                              |                |         |         |        |          |            |                  |                  |
 *      ----------------              ----------------      -------   --------   -----   -------   ----------   ---------         ------------     ----------------
 *     |    YCWindow    |            |    YCDialog    |    | YCDiv | | YCSpan | | YCP | | YCImg | | YCButton | | YCRadio |  ...  | YCCheckbox |   | YCUIHoverState |    
 *      ----------------              ----------------      -------   --------   -----   -------   ----------   ---------         ------------     ----------------
 *
 */

class YCAPI YCIUITag 
	: public YCIComponent
{
public:

	YCIUITag(const YCUITagMeta* tagMeta);

	virtual ~YCIUITag();

	//
	// 函数：initialTagAttribute()
	//
	// 目的：设置标签属性
	//
	bool initialTagAttribute();

	//
	// 函数：isVisible()
	//
	// 目的：元件是否可见
	//
	bool isVisible();

	//
	// 函数：hasStatus(E_UITAG_STATUS status)
	//
	// 目的：取得元件的状态
	//
	bool hasStatus(E_UITAG_STATUS status);

	//
	// 函数：setStatus(E_UITAG_STATUS status)
	//
	// 目的：取得元件的状态
	//
	void setStatus(E_UITAG_STATUS status);

	//
	// 函数：removeStatus(E_UITAG_STATUS status)
	//
	// 目的：取消元件的状态
	//
	void removeStatus(E_UITAG_STATUS status);

	//
	// 函数：setPosition(int x, int y);
	//
	// 目的：设置元件的相对位置，相对父标签
	//
	void setPosition(int x, int y);

	//
	// 函数：setScreenPosition(int x, int y);
	//
	// 目的：设置元件的绝对位置，屏幕坐标
	//
	void setScreenPosition(int x, int y);

	//
	// 函数：getPositionX()
	//       getPositionY()
	//
	// 目的：
	//      取得元件位置属性
	//
	int getPositionX();
	int getPositionY();

	int getScreenPositionX();
	int getScreenPositionY();

	//
	// 函数：getWidth();
	//		 getHeight();
	//		 getOriginalWidth();
	//		 getOriginalHeight();
	//
	// 目的：
	//      取得元件的宽高
	//
	int getWidthWithMargin();
	int getHeightWithMargin();
	int getWidth();
	int getHeight();
	int getClientWidth();
	int getClientHeight();
	int getOriginalWidth();
	int getOriginalHeight();

	//
	// 函数：getFloatMode()
	//
	// 目的：
	//      取得元件的浮动模式
	//
	E_UITAG_FLOAT_MODE getFloatMode();

	//
	// 函数：getDisplayMode()
	//
	// 目的：
	//      取得元件的显示模式inline, block
	//
	E_UITAG_DISPLAY_MODE getDisplayMode();

	//
	// 函数：getPositionMode()
	//
	// 目的：
	//      取得元件的定位模式relative, absolute
	//
	E_UITAG_POSITION_MODE getPositionMode();

	//
	// 函数：render()
	//
	// 目的：在指定处绘制该UI元件及其子控件
	//
	virtual void render();
	
	//
	// 函数：draw()
	//
	// 目的：在指定处绘制该UI元件自身
	//
	virtual void draw();

	//
	// 函数：name() const
	//
	// 目的：获取标签名
	//
	const char* name() const;

	//
	// 函数：id() const
	//
	// 目的：获取id名
	//
	const char* id() const;

	//
	// 函数：parent() const
	//
	// 目的：获取父节点
	//
	YCIUITag* parent() const;

	//
	// 函数：getClass() const
	//
	// 目的：获取类名
	//      通过CSS的addClass、removeClass操作
	//      类名可能发生变化
	//
	const char* getClass() const;
		
	//
	// 函数：bind2CSS(const YCCSSContext* css)
	//
	// 目的：绑定到DOM文档对象模型
	//
	void bind2CSS(const YCCSSContext* css);
	
	//
	// 函数：bind2DynamicDOM(YCDynamicUIDOM* dom)
	//
	// 目的：绑定到DOM文档对象模型
	//
	void bind2DynamicDOM(YCDynamicUIDOM* dom);

	//
	// 函数：setContainer(YCIContainer* container)
	//
	// 目的：设置容器
	//
	void setContainer(YCIContainer* container);

	//
	// 函数：setParent(YCIUITag* parent)
	//
	// 目的：设置父控件
	//
	void setParent(YCIUITag* parent);
	
	//
	// 函数：hoverOn()
	//
	// 目的：鼠标移上根据hover伪类修改CSS属性
	//       在HOVERON_EVENT事件处理器之前执行
	//
	void hoverOn();

	//
	// 函数：hoverOff()
	//
	// 目的：鼠标移出，移除hover伪类修改的CSS属性
	//       在HOVEROFF_EVENT事件处理器之后执行
	//
	void hoverOff();

	//
	// 函数：disable()
	//
	// 目的：使元件失效
	//
	void disable();

	//
	// 函数：enable()
	//
	// 目的：使元件有效
	//
	void enable();

	//
	// 函数：show()
	//
	// 目的：使元件可见
	//
	void show();
	
	//
	// 函数：hide()
	//
	// 目的：使元件不可见
	//
	void hide();

	//
	// 函数：move(E_UITAG_MOVESPEED speed, int targetX, int targetY)
	//
	// 目的：元件以指定速度移动到指定位置
	//
	void move(E_UITAG_MOVESPEED speed, int targetX, int targetY);

	//
	// 函数：hasClass(const char* clazz)
	//
	// 目的：该标签是否含有该clazz
	//
	bool hasClass(const char* clazz);

	//
	// 函数：addClass(const char* clazz)
	//
	// 目的：给该标签增加class
	//
	void addClass(const char* clazz);

	//
	// 函数：removeClass(const char* clazz)
	//
	// 目的：从该标签移除class
	//
	void removeClass(const char* clazz);

	//
	// 函数：css(const char* property, const char* value)
	//
	// 目的：设置CSS属性值
	//
	void css(const char* property, const char* value);

	//
	// 函数：attr(const char* attribute, const char* value)
	//
	// 目的：设置属性值
	//
	void attr(const char* attribute, const char* value);

	//
	// 函数：html(const char* html)
	//
	// 目的：设置新子标签
	//
	void html(const char* html);

	//
	// 函数：append(const char* html)
	//
	// 目的：附加新子标签
	//
	void append(const char* html);
	
	// 
	// 函数：bind(E_UITAG_EVENT ev, const char* scripts)
	//
	// 目的：绑定事件处理器
	//
	// 注释：
	//      handler LUA脚本
	//
	void bind(E_UITAG_EVENT ev, const char* scripts);

	// 
	// 函数：unbind(E_UITAG_EVENT ev)
	//
	// 目的：移除事件处理器
	//
	void unbind(E_UITAG_EVENT ev);

	//
	// 函数：toggle(const char* odd, const char* even)
	//
	// 目的：toggle事件处理器
	//
	void toggle(const char* odd, const char* even);

	//
	// 函数：move(int deltaX, int deltaY)
	//
	// 目的：移动窗口
	//
	void move(int deltaX, int deltaY);

#pragma region property

	//
	// 函数：setProperty(YCUITagMeta::E_UITAG_CSS property, int priority, bool bValue)
	//
	// 目的：设置 bool 类型属性
	//
	void setProperty(E_UITAG_CSS property, int priority, bool bValue);

	//
	// 函数：setProperty(YCUITagMeta::E_UITAG_CSS property, int priority, int iValue)
	//
	// 目的：设置 int 类型属性
	//
	void setProperty(E_UITAG_CSS property, int priority, int iValue);

	//
	// 函数：setProperty(YCUITagMeta::E_UITAG_CSS property, int priority, float fValue)
	//
	// 目的：设置 float 类型属性
	//
	void setProperty(E_UITAG_CSS property, int priority, double fValue);

	//
	// 函数：setProperty(E_UITAG_CSS property, int priority, const char* strValue)
	//
	// 目的：设置 字符串 类型属性
	//
	void setProperty(E_UITAG_CSS property, int priority, const char* strValue);

	//
	// 函数：getProperty(E_UITAG_CSS property, bool& bValue)
	//
	// 目的：获取 int 类型属性
	//
	bool getProperty(E_UITAG_CSS property, bool& bValue);

	//
	// 函数：hasProperty(E_UITAG_CSS property)
	//
	// 目的：是否已设置该属性
	//
	bool hasProperty(E_UITAG_CSS property);

	//
	// 函数：getProperty(E_UITAG_CSS property, int& iValue)
	//
	// 目的：获取 int 类型属性
	//
	bool getProperty(E_UITAG_CSS property, int& iValue);

	//
	// 函数：getProperty(E_UITAG_CSS property, double& fValue)
	//
	// 目的：获取 float 类型属性
	//
	bool getProperty(E_UITAG_CSS property, double& fValue);

	//
	// 函数：getProperty(E_UITAG_CSS property, char* strValue, unsigned int length)
	//
	// 目的：获取 字符串 类型属性
	//
	bool getProperty(E_UITAG_CSS property, char* strValue, unsigned int length);

#pragma endregion property

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

	//
	// 函数：isMouseIn(int mouseX, int mouseY)
	// 
	// 目的：鼠标是否在标签内部
	//
	bool isMouseIn(int mouseX, int mouseY);

	//
	// 函数：layout(bool resizable)
	//
	// 目的：进行元件自身及子元件的布局
	//
	virtual void layout(bool resizable);

protected:

	//
	// 函数：createDynamicUIDOM(const YCUIDOM* uiDom)
	//
	// 目的：创建内存Dynamic UIDOM, 返回标签为code的代码节点
	//
	const YCUIDOM* createDynamicUIDOM(const YCUIDOM* uiDom);

	//
	// 函数：createSubTags(const YCUIDOM *uiDom)
	//
	// 目的：创建子控件
	//
	bool createSubTags(const YCUIDOM *uiDom);

	//
	// 函数：fetchAlpha()
	//
	// 目的：从CSS的alpha来确定透明度
	//
	void fetchAlpha();

	//
	// 函数：fetchFloatMode()
	//
	// 目的：取得元件浮动模式
	//
	void fetchFloatMode();

	//
	// 函数：fetchPositionMode()
	//
	// 目的： 取得元件定位模式
	//
	void fetchPositionMode();

	//
	// 函数：fetchDisplayMode()
	//
	// 目的： 取得元件显示模式
	//
	void fetchDisplayMode();

	//
	// 函数：fetchWidthAndHeight()
	//
	// 目的：取得元件的宽和高，不包含margin区域
	//      border和padding区域在width和height之内
	//
	void fetchWidthAndHeight();

	//
	// 函数：updateHeight(unsigned int height)
	//
	// 目的：设置元件的高
	//
	void updateHeight(int height);

	//
	// 函数：buildBackground()
	//
	// 目的：构建元素背景
	//
	// 注释：
	//      目前有四种背景模式, 绘制背景优先级从上到下
	//         YCAnimationBackground
	//         YCSplitBackground  -> 九块
	//                            -> x方向平铺 3块
	//                            -> y方向平铺 3块
	//         YCImageBackground
	//         YCColorBackground
	//
	void buildBackground();

	//
	// 函数：extraDraw()
	//
	// 目的：UI元件的自定义绘制
	//
	virtual void extraDraw() = 0;

	//
	// 函数：extraCSS()
	//
	// 目的：处理标签特定的CSS属性
	//
	virtual void extraCSS() = 0;

	//
	// 函数：deleteSubTags()
	//
	// 目的：删除所有子控件
	//
	void deleteSubTags();

	//
	// 函数：onCSSUpdated()
	//
	// 目的：当CSS发生变化时
	//
	void onCSSUpdated();

	//
	// 函数：onDOMUpdated()
	//
	// 目的：当DOM发生变化时
	//
	void onDOMUpdated();

private:

	//
	// 函数：validateSubTag(const char* name)
	//
	// 目的：校验子控件
	//
	virtual bool validateSubTag(const char* name) = 0;
		
	//
	// 函数：fetchPosition()
	//
	// 目的：从CSS的top、left来确定位置
	//
	void fetchPosition();

	//
	// 函数：calculateViewport()
	//
	// 目的：计算视口裁剪区
	//
	void calculateViewport();

	//
	// 函数：validateProperty(E_UITAG_CSS property)
	//
	// 目的：校验属性是否针对此Tag有效
	//
	bool validateProperty(E_UITAG_CSS property);
	
	//
	// 函数：handleBorder(E_UITAG_CSS property, int& border, unsigned int& color)
	//
	// 目的：处理CSS边框宽度
	//
	void handleBorder(E_UITAG_CSS property, int& border, unsigned int& color);

	//
	// 函数：drawBorder()
	//
	// 目的：画边框，继承类可以覆盖，但不能调用基类的实现
	//
	virtual void drawBorder();

protected:

	//
	// 属性：元件状态
	// 
	unsigned int myStatus;

	//
	// 属性：元件可见
	//
	bool myVisiable;

	//
	// 属性：当前元件相对位置，以元件中心为锚点
	//
	int myPositionX;
	int myPositionY;

	//
	// 属性：当前元件屏幕绝对位置
	//
	int myScreenPositionX;
	int myScreenPositionY;

	//
	// 属性：裁剪区
	//
	IntRect myViewPort;

	//
	// 元件浮动模式：
	//
	E_UITAG_FLOAT_MODE myFloatMode;

	//
	// 属性：元件定位模式
	//
	E_UITAG_POSITION_MODE myPositionMode;

	//
	// 属性：元件显示模式
	//
	E_UITAG_DISPLAY_MODE myDisplayMode;

	//
	// 属性：元件背景 
	//
	YCIBackground* myBackground;

	//
	// 属性：元件透明度，控制自身及其子标签
	//
	// 说明：值区间[0.0f, 1.0f]
	//       0.0f 元件不可见，visiable等于false，需要重新排版
	//       >0.0f 元件可见， visiable等于true，需要重新排版
	//
	float myAlphaValue;

	//
	// 属性：外边距
	//
	int myMarginTop;
	int myMarginLeft;
	int myMarginRight;
	int myMarginBottom;

	//
	// 属性：边框宽
	//
	int myBorderTop;
	int myBorderLeft;
	int myBorderRight;
	int myBorderBottom;

	//
	// 属性：边框颜色
	//
	unsigned int myBorderTopColor;
	unsigned int myBorderLeftColor;
	unsigned int myBorderRightColor;
	unsigned int myBorderBottomColor;

	//
	// 属性：内填充
	//
	int myPaddingTop;
	int myPaddingLeft;
	int myPaddingRight;
	int myPaddingBottom;
	
	int myOriginalWidth;    //初始状态宽      myWidth
	int myOriginalHeight;	//初始状态高      myHeight

	int myWidthWithMargin;  //含边距宽        myWidthWithMargin = myWidth + myMarginLeft + myMarginRight
	int myHeightWithMargin; //含边距宽        myHeightWithMargin = myHeight + myMarginTop + myMarginBottom
	int myWidth;			//当前宽(背景)
	int myHeight;			//当前高(背景)
	int myClientWidth;      //当前客户区宽    myClientWidth = myWidth - myPaddingLeft - myPaddingRight
	int myClientHeight;     //当前客户区高    myClientHeight = myHeight - myPaddingTop - myPaddingBottom

	//
	// 属性：myActionQueue
	//
	// 说明: action队列
	//
	YCQueue *myActionQueue;

	//
	// 属性：myContainer
	//
	// 说明: 顶层容器
	//
	YCIContainer *myContainer;

	//
	// 属性：myHoverContext
	//
	// 说明：Hover伪类
	//
	YCUIHoverState* myHoverState;

			
	//
	// 属性：myLayoutContext
	//
	// 说明：CSS Layout上下文，
	//      最外层的container持有生命周期
	//
	const YCCSSContext *myLayoutContext;

	//
	// 属性：myParent
	//
	// 说明: 父容器
	//
	YCIUITag *myParent;
	
	//
	// 属性：mySubTags
	//
	// 说明：子元件
	//
	YCDList * mySubTags;

	//
	// 属性：myDynamicUIDOM
	//
	// 说明：当前UITag的真实YCUIDom,
	//       反映当前UITag的UIDom，可以append，remove，html。。。
	//       在createSubTags中创建
	//       生命周期控制在YCIContainer类中
	//
	YCDynamicUIDOM* myDynamicUIDOM;

	//
	// 属性：myTagMeta
	//
	// 说明：标签元数据信息
	//
	const YCUITagMeta* myTagMeta;

	//
	// 属性：myUITagImpl
	//
	// 说明：标签类内部实现，持有CSS和Attribute信息
	//
	YCIUITagImpl* myUITagImpl;
};

#endif