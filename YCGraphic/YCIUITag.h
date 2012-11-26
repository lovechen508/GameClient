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
 * ������YCIUITag
 *
 * ע�ͣ�UI���ִ��ڱ�ǩ�ӿ���
 *       �̳���YCIComponent
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
	// ������initialTagAttribute()
	//
	// Ŀ�ģ����ñ�ǩ����
	//
	bool initialTagAttribute();

	//
	// ������isVisible()
	//
	// Ŀ�ģ�Ԫ���Ƿ�ɼ�
	//
	bool isVisible();

	//
	// ������hasStatus(E_UITAG_STATUS status)
	//
	// Ŀ�ģ�ȡ��Ԫ����״̬
	//
	bool hasStatus(E_UITAG_STATUS status);

	//
	// ������setStatus(E_UITAG_STATUS status)
	//
	// Ŀ�ģ�ȡ��Ԫ����״̬
	//
	void setStatus(E_UITAG_STATUS status);

	//
	// ������removeStatus(E_UITAG_STATUS status)
	//
	// Ŀ�ģ�ȡ��Ԫ����״̬
	//
	void removeStatus(E_UITAG_STATUS status);

	//
	// ������setPosition(int x, int y);
	//
	// Ŀ�ģ�����Ԫ�������λ�ã���Ը���ǩ
	//
	void setPosition(int x, int y);

	//
	// ������setScreenPosition(int x, int y);
	//
	// Ŀ�ģ�����Ԫ���ľ���λ�ã���Ļ����
	//
	void setScreenPosition(int x, int y);

	//
	// ������getPositionX()
	//       getPositionY()
	//
	// Ŀ�ģ�
	//      ȡ��Ԫ��λ������
	//
	int getPositionX();
	int getPositionY();

	int getScreenPositionX();
	int getScreenPositionY();

	//
	// ������getWidth();
	//		 getHeight();
	//		 getOriginalWidth();
	//		 getOriginalHeight();
	//
	// Ŀ�ģ�
	//      ȡ��Ԫ���Ŀ��
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
	// ������getFloatMode()
	//
	// Ŀ�ģ�
	//      ȡ��Ԫ���ĸ���ģʽ
	//
	E_UITAG_FLOAT_MODE getFloatMode();

	//
	// ������getDisplayMode()
	//
	// Ŀ�ģ�
	//      ȡ��Ԫ������ʾģʽinline, block
	//
	E_UITAG_DISPLAY_MODE getDisplayMode();

	//
	// ������getPositionMode()
	//
	// Ŀ�ģ�
	//      ȡ��Ԫ���Ķ�λģʽrelative, absolute
	//
	E_UITAG_POSITION_MODE getPositionMode();

	//
	// ������render()
	//
	// Ŀ�ģ���ָ�������Ƹ�UIԪ�������ӿؼ�
	//
	virtual void render();
	
	//
	// ������draw()
	//
	// Ŀ�ģ���ָ�������Ƹ�UIԪ������
	//
	virtual void draw();

	//
	// ������name() const
	//
	// Ŀ�ģ���ȡ��ǩ��
	//
	const char* name() const;

	//
	// ������id() const
	//
	// Ŀ�ģ���ȡid��
	//
	const char* id() const;

	//
	// ������parent() const
	//
	// Ŀ�ģ���ȡ���ڵ�
	//
	YCIUITag* parent() const;

	//
	// ������getClass() const
	//
	// Ŀ�ģ���ȡ����
	//      ͨ��CSS��addClass��removeClass����
	//      �������ܷ����仯
	//
	const char* getClass() const;
		
	//
	// ������bind2CSS(const YCCSSContext* css)
	//
	// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
	//
	void bind2CSS(const YCCSSContext* css);
	
	//
	// ������bind2DynamicDOM(YCDynamicUIDOM* dom)
	//
	// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
	//
	void bind2DynamicDOM(YCDynamicUIDOM* dom);

	//
	// ������setContainer(YCIContainer* container)
	//
	// Ŀ�ģ���������
	//
	void setContainer(YCIContainer* container);

	//
	// ������setParent(YCIUITag* parent)
	//
	// Ŀ�ģ����ø��ؼ�
	//
	void setParent(YCIUITag* parent);
	
	//
	// ������hoverOn()
	//
	// Ŀ�ģ�������ϸ���hoverα���޸�CSS����
	//       ��HOVERON_EVENT�¼�������֮ǰִ��
	//
	void hoverOn();

	//
	// ������hoverOff()
	//
	// Ŀ�ģ�����Ƴ����Ƴ�hoverα���޸ĵ�CSS����
	//       ��HOVEROFF_EVENT�¼�������֮��ִ��
	//
	void hoverOff();

	//
	// ������disable()
	//
	// Ŀ�ģ�ʹԪ��ʧЧ
	//
	void disable();

	//
	// ������enable()
	//
	// Ŀ�ģ�ʹԪ����Ч
	//
	void enable();

	//
	// ������show()
	//
	// Ŀ�ģ�ʹԪ���ɼ�
	//
	void show();
	
	//
	// ������hide()
	//
	// Ŀ�ģ�ʹԪ�����ɼ�
	//
	void hide();

	//
	// ������move(E_UITAG_MOVESPEED speed, int targetX, int targetY)
	//
	// Ŀ�ģ�Ԫ����ָ���ٶ��ƶ���ָ��λ��
	//
	void move(E_UITAG_MOVESPEED speed, int targetX, int targetY);

	//
	// ������hasClass(const char* clazz)
	//
	// Ŀ�ģ��ñ�ǩ�Ƿ��и�clazz
	//
	bool hasClass(const char* clazz);

	//
	// ������addClass(const char* clazz)
	//
	// Ŀ�ģ����ñ�ǩ����class
	//
	void addClass(const char* clazz);

	//
	// ������removeClass(const char* clazz)
	//
	// Ŀ�ģ��Ӹñ�ǩ�Ƴ�class
	//
	void removeClass(const char* clazz);

	//
	// ������css(const char* property, const char* value)
	//
	// Ŀ�ģ�����CSS����ֵ
	//
	void css(const char* property, const char* value);

	//
	// ������attr(const char* attribute, const char* value)
	//
	// Ŀ�ģ���������ֵ
	//
	void attr(const char* attribute, const char* value);

	//
	// ������html(const char* html)
	//
	// Ŀ�ģ��������ӱ�ǩ
	//
	void html(const char* html);

	//
	// ������append(const char* html)
	//
	// Ŀ�ģ��������ӱ�ǩ
	//
	void append(const char* html);
	
	// 
	// ������bind(E_UITAG_EVENT ev, const char* scripts)
	//
	// Ŀ�ģ����¼�������
	//
	// ע�ͣ�
	//      handler LUA�ű�
	//
	void bind(E_UITAG_EVENT ev, const char* scripts);

	// 
	// ������unbind(E_UITAG_EVENT ev)
	//
	// Ŀ�ģ��Ƴ��¼�������
	//
	void unbind(E_UITAG_EVENT ev);

	//
	// ������toggle(const char* odd, const char* even)
	//
	// Ŀ�ģ�toggle�¼�������
	//
	void toggle(const char* odd, const char* even);

	//
	// ������move(int deltaX, int deltaY)
	//
	// Ŀ�ģ��ƶ�����
	//
	void move(int deltaX, int deltaY);

#pragma region property

	//
	// ������setProperty(YCUITagMeta::E_UITAG_CSS property, int priority, bool bValue)
	//
	// Ŀ�ģ����� bool ��������
	//
	void setProperty(E_UITAG_CSS property, int priority, bool bValue);

	//
	// ������setProperty(YCUITagMeta::E_UITAG_CSS property, int priority, int iValue)
	//
	// Ŀ�ģ����� int ��������
	//
	void setProperty(E_UITAG_CSS property, int priority, int iValue);

	//
	// ������setProperty(YCUITagMeta::E_UITAG_CSS property, int priority, float fValue)
	//
	// Ŀ�ģ����� float ��������
	//
	void setProperty(E_UITAG_CSS property, int priority, double fValue);

	//
	// ������setProperty(E_UITAG_CSS property, int priority, const char* strValue)
	//
	// Ŀ�ģ����� �ַ��� ��������
	//
	void setProperty(E_UITAG_CSS property, int priority, const char* strValue);

	//
	// ������getProperty(E_UITAG_CSS property, bool& bValue)
	//
	// Ŀ�ģ���ȡ int ��������
	//
	bool getProperty(E_UITAG_CSS property, bool& bValue);

	//
	// ������hasProperty(E_UITAG_CSS property)
	//
	// Ŀ�ģ��Ƿ������ø�����
	//
	bool hasProperty(E_UITAG_CSS property);

	//
	// ������getProperty(E_UITAG_CSS property, int& iValue)
	//
	// Ŀ�ģ���ȡ int ��������
	//
	bool getProperty(E_UITAG_CSS property, int& iValue);

	//
	// ������getProperty(E_UITAG_CSS property, double& fValue)
	//
	// Ŀ�ģ���ȡ float ��������
	//
	bool getProperty(E_UITAG_CSS property, double& fValue);

	//
	// ������getProperty(E_UITAG_CSS property, char* strValue, unsigned int length)
	//
	// Ŀ�ģ���ȡ �ַ��� ��������
	//
	bool getProperty(E_UITAG_CSS property, char* strValue, unsigned int length);

#pragma endregion property

	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

	//
	// ������isMouseIn(int mouseX, int mouseY)
	// 
	// Ŀ�ģ�����Ƿ��ڱ�ǩ�ڲ�
	//
	bool isMouseIn(int mouseX, int mouseY);

	//
	// ������layout(bool resizable)
	//
	// Ŀ�ģ�����Ԫ��������Ԫ���Ĳ���
	//
	virtual void layout(bool resizable);

protected:

	//
	// ������createDynamicUIDOM(const YCUIDOM* uiDom)
	//
	// Ŀ�ģ������ڴ�Dynamic UIDOM, ���ر�ǩΪcode�Ĵ���ڵ�
	//
	const YCUIDOM* createDynamicUIDOM(const YCUIDOM* uiDom);

	//
	// ������createSubTags(const YCUIDOM *uiDom)
	//
	// Ŀ�ģ������ӿؼ�
	//
	bool createSubTags(const YCUIDOM *uiDom);

	//
	// ������fetchAlpha()
	//
	// Ŀ�ģ���CSS��alpha��ȷ��͸����
	//
	void fetchAlpha();

	//
	// ������fetchFloatMode()
	//
	// Ŀ�ģ�ȡ��Ԫ������ģʽ
	//
	void fetchFloatMode();

	//
	// ������fetchPositionMode()
	//
	// Ŀ�ģ� ȡ��Ԫ����λģʽ
	//
	void fetchPositionMode();

	//
	// ������fetchDisplayMode()
	//
	// Ŀ�ģ� ȡ��Ԫ����ʾģʽ
	//
	void fetchDisplayMode();

	//
	// ������fetchWidthAndHeight()
	//
	// Ŀ�ģ�ȡ��Ԫ���Ŀ�͸ߣ�������margin����
	//      border��padding������width��height֮��
	//
	void fetchWidthAndHeight();

	//
	// ������updateHeight(unsigned int height)
	//
	// Ŀ�ģ�����Ԫ���ĸ�
	//
	void updateHeight(int height);

	//
	// ������buildBackground()
	//
	// Ŀ�ģ�����Ԫ�ر���
	//
	// ע�ͣ�
	//      Ŀǰ�����ֱ���ģʽ, ���Ʊ������ȼ����ϵ���
	//         YCAnimationBackground
	//         YCSplitBackground  -> �ſ�
	//                            -> x����ƽ�� 3��
	//                            -> y����ƽ�� 3��
	//         YCImageBackground
	//         YCColorBackground
	//
	void buildBackground();

	//
	// ������extraDraw()
	//
	// Ŀ�ģ�UIԪ�����Զ������
	//
	virtual void extraDraw() = 0;

	//
	// ������extraCSS()
	//
	// Ŀ�ģ������ǩ�ض���CSS����
	//
	virtual void extraCSS() = 0;

	//
	// ������deleteSubTags()
	//
	// Ŀ�ģ�ɾ�������ӿؼ�
	//
	void deleteSubTags();

	//
	// ������onCSSUpdated()
	//
	// Ŀ�ģ���CSS�����仯ʱ
	//
	void onCSSUpdated();

	//
	// ������onDOMUpdated()
	//
	// Ŀ�ģ���DOM�����仯ʱ
	//
	void onDOMUpdated();

private:

	//
	// ������validateSubTag(const char* name)
	//
	// Ŀ�ģ�У���ӿؼ�
	//
	virtual bool validateSubTag(const char* name) = 0;
		
	//
	// ������fetchPosition()
	//
	// Ŀ�ģ���CSS��top��left��ȷ��λ��
	//
	void fetchPosition();

	//
	// ������calculateViewport()
	//
	// Ŀ�ģ������ӿڲü���
	//
	void calculateViewport();

	//
	// ������validateProperty(E_UITAG_CSS property)
	//
	// Ŀ�ģ�У�������Ƿ���Դ�Tag��Ч
	//
	bool validateProperty(E_UITAG_CSS property);
	
	//
	// ������handleBorder(E_UITAG_CSS property, int& border, unsigned int& color)
	//
	// Ŀ�ģ�����CSS�߿���
	//
	void handleBorder(E_UITAG_CSS property, int& border, unsigned int& color);

	//
	// ������drawBorder()
	//
	// Ŀ�ģ����߿򣬼̳�����Ը��ǣ������ܵ��û����ʵ��
	//
	virtual void drawBorder();

protected:

	//
	// ���ԣ�Ԫ��״̬
	// 
	unsigned int myStatus;

	//
	// ���ԣ�Ԫ���ɼ�
	//
	bool myVisiable;

	//
	// ���ԣ���ǰԪ�����λ�ã���Ԫ������Ϊê��
	//
	int myPositionX;
	int myPositionY;

	//
	// ���ԣ���ǰԪ����Ļ����λ��
	//
	int myScreenPositionX;
	int myScreenPositionY;

	//
	// ���ԣ��ü���
	//
	IntRect myViewPort;

	//
	// Ԫ������ģʽ��
	//
	E_UITAG_FLOAT_MODE myFloatMode;

	//
	// ���ԣ�Ԫ����λģʽ
	//
	E_UITAG_POSITION_MODE myPositionMode;

	//
	// ���ԣ�Ԫ����ʾģʽ
	//
	E_UITAG_DISPLAY_MODE myDisplayMode;

	//
	// ���ԣ�Ԫ������ 
	//
	YCIBackground* myBackground;

	//
	// ���ԣ�Ԫ��͸���ȣ������������ӱ�ǩ
	//
	// ˵����ֵ����[0.0f, 1.0f]
	//       0.0f Ԫ�����ɼ���visiable����false����Ҫ�����Ű�
	//       >0.0f Ԫ���ɼ��� visiable����true����Ҫ�����Ű�
	//
	float myAlphaValue;

	//
	// ���ԣ���߾�
	//
	int myMarginTop;
	int myMarginLeft;
	int myMarginRight;
	int myMarginBottom;

	//
	// ���ԣ��߿��
	//
	int myBorderTop;
	int myBorderLeft;
	int myBorderRight;
	int myBorderBottom;

	//
	// ���ԣ��߿���ɫ
	//
	unsigned int myBorderTopColor;
	unsigned int myBorderLeftColor;
	unsigned int myBorderRightColor;
	unsigned int myBorderBottomColor;

	//
	// ���ԣ������
	//
	int myPaddingTop;
	int myPaddingLeft;
	int myPaddingRight;
	int myPaddingBottom;
	
	int myOriginalWidth;    //��ʼ״̬��      myWidth
	int myOriginalHeight;	//��ʼ״̬��      myHeight

	int myWidthWithMargin;  //���߾��        myWidthWithMargin = myWidth + myMarginLeft + myMarginRight
	int myHeightWithMargin; //���߾��        myHeightWithMargin = myHeight + myMarginTop + myMarginBottom
	int myWidth;			//��ǰ��(����)
	int myHeight;			//��ǰ��(����)
	int myClientWidth;      //��ǰ�ͻ�����    myClientWidth = myWidth - myPaddingLeft - myPaddingRight
	int myClientHeight;     //��ǰ�ͻ�����    myClientHeight = myHeight - myPaddingTop - myPaddingBottom

	//
	// ���ԣ�myActionQueue
	//
	// ˵��: action����
	//
	YCQueue *myActionQueue;

	//
	// ���ԣ�myContainer
	//
	// ˵��: ��������
	//
	YCIContainer *myContainer;

	//
	// ���ԣ�myHoverContext
	//
	// ˵����Hoverα��
	//
	YCUIHoverState* myHoverState;

			
	//
	// ���ԣ�myLayoutContext
	//
	// ˵����CSS Layout�����ģ�
	//      ������container������������
	//
	const YCCSSContext *myLayoutContext;

	//
	// ���ԣ�myParent
	//
	// ˵��: ������
	//
	YCIUITag *myParent;
	
	//
	// ���ԣ�mySubTags
	//
	// ˵������Ԫ��
	//
	YCDList * mySubTags;

	//
	// ���ԣ�myDynamicUIDOM
	//
	// ˵������ǰUITag����ʵYCUIDom,
	//       ��ӳ��ǰUITag��UIDom������append��remove��html������
	//       ��createSubTags�д���
	//       �������ڿ�����YCIContainer����
	//
	YCDynamicUIDOM* myDynamicUIDOM;

	//
	// ���ԣ�myTagMeta
	//
	// ˵������ǩԪ������Ϣ
	//
	const YCUITagMeta* myTagMeta;

	//
	// ���ԣ�myUITagImpl
	//
	// ˵������ǩ���ڲ�ʵ�֣�����CSS��Attribute��Ϣ
	//
	YCIUITagImpl* myUITagImpl;
};

#endif