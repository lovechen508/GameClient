#ifndef _INCLUDE_YCUIHOVERSTATE_H_
#define _INCLUDE_YCUIHOVERSTATE_H_

#include "YCUITag.h"

class YCCSSNode;
class YCIBackground;

/*
 * YCUIHoverState : 伪类，鼠标在上面
 */
class YCUIHoverState
	: public YCUITag
{

public:

	YCUIHoverState(const YCUITagMeta* tagMeta, YCIUITag* host);
	
	virtual ~YCUIHoverState(void);

	//
	// 函数：bindCSSNode(const YCCSSNode* node)
	//
	// 目的：绑定到DOM文档对象模型
	//
	virtual void bindCSSNode(const YCCSSNode* node);

	//
	// 函数：getBackground()
	//
	// 目的：绑定到DOM文档对象模型
	//
	YCIBackground* getBackground();

	//
	// 函数：getFontColor(unsigned int &hoverColor)
	//
	// 目的：取得Hover状态文字颜色
	//
	bool getFontColor(unsigned int &hoverColor);

	//
	// 函数：draw()
	//
	// 目的：完成正式的绘制
	//       自己有新的状态就绘制
	//       否则就交回元件自身绘制
	//
	virtual void draw();

private:

	//
	// 函数：validateSubTag(const char* name)
	//
	// 目的：校验子控件
	//
	virtual bool validateSubTag(const char* name);

private:
	
	//
	// 属性：myContext
	//
	// 说明：Hover伪类
	//
	const YCCSSNode *myContext;

	//
	// 属性：myHost
	//
	// 说明：所属元件
	//
	YCIUITag* myHost;

	//
	// 属性：myHoverColorEnable
	//
	// 目的：是否有文字Hover颜色
	//
	bool myColorEnable;

	//
	// 属性：myHoverColor
	//
	// 目的：鼠标移上文字颜色
	//
	unsigned int myColor;
};

#endif

