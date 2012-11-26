#ifndef _INCLUDE_YCICOMPONENT_H_
#define _INCLUDE_YCICOMPONENT_H_

#include "YCEvent.h"
#include "YCUITagDef.h"
#include "YCGraphic.h"

#include "YCBasic\YCDef.h"
#include "YCInput\YCInput.h"

/*
 *    YCIComponent ����UI��������
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
	 * Ԫ�����ĸ�������
	 */
	enum E_COMPONENT_DIRECTION {
		DIRECTION_TOP    = 1,
		DIRECTION_LEFT   = 2,
		DIRECTION_RIGHT  = 3,
		DIRECTION_BOTTOM = 4,
	};

	enum E_COMPONENT_STATUS {
		STATUS_CLOSE  = 0,        // Ԫ����
		STATUS_OPEN   = 1,        // Ԫ���ر�
		STATUS_HIDE   = 2,        // Ԫ������
	};

	// ���ٷ���������
	static YCInput * gInput;

	// ���ٷ��ʻ��ƾ������
	static YCGraphic::YCD3DSprite * gSprite;

	// ���ٷ��ʻ���ͼ�λ��ƶ���
	static YCGraphic::YCD3DPrimitive * gPrimitive;

	// ���ٷ��ʻ����ַ�����
	static YCGraphic::YCFont * gFont;

	// ���ٷ��ʱ�ǩ����
	static YCTagFactory* gTagFactory;

public:

	YCIComponent(void);

	virtual ~YCIComponent(void);
	
	//
	// ������modelUpdate(YCEvent& ev)
	//
	// Ŀ�ģ�֪ͨ����ģ�͸���
	//
	virtual void modelUpdate(YCEvent& ev);

	//
	// ������render()
	//
	// Ŀ�ģ���ָ�������Ƹ�UIԪ�������ӿؼ�
	//
	virtual void render() = 0;
		
	//
	// ������draw()
	//
	// Ŀ�ģ���ָ�������Ƹ�UIԪ������
	//
	virtual void draw() = 0;

	//
	// ������setValue(const char* strValue)
	//
	// Ŀ�ģ�����Ԫ����ֵ
	//
	void setValue(const char* strValue);

	//
	// ������getValue()
	//
	// Ŀ�ģ���ȡԪ����ֵ
	//
	const char* getValue();

	//
	// ������setOrder(unsigned int order)
	//
	// Ŀ�ģ����õ�ǰԪ���ڵ�UILayer�е�order
	//
	void setOrder(unsigned int order);

	//
	// ������getOrder()
	//
	// Ŀ�ģ��õ���ǰԪ���ڵ�UILayer�е�order
	//       orderֵԽ�󣬾��û�Խ��
	//
	// ע�ͣ���YCIComponent��������order�ӿ�
	//       ����Ԫ����orderĬ��Ϊ0��������ӵ�
	//       UILayer���Ⱥ�˳������
	//
	unsigned int getOrder();
	
	//
	// ������E_COMPONENT_STATUS()
	//
	// Ŀ�ģ�ȡ�ô�������
	//
	E_COMPONENT_STATUS getStatus();

	//
	// ������matchAccelerateKey()
	//
	// Ŀ�ģ��Ƿ�ƥ���ݼ�
	//
	virtual bool matchAccelerateKey() = 0;

	//
	// ������open(YCLuaParameter* context)
	//
	// Ŀ�ģ� ����һ��������ʵ��
	//        ͬʱ�󶨴��ڴ򿪵�����������
	//
	// ע�ͣ�
	//       ÿ�δ�YCUIManager����һ�����ڣ�������һ��������ʵ��
	//       ��֤YCUIDOM�ĳ�ʼ������
	//
	virtual bool open(YCLuaParameter* context) = 0;

	//
	// ������close()
	//
	// Ŀ�ģ� �ر�һ��Ԫ��ʵ��
	//
	virtual void close() = 0;


	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	//
	// ������addListener(E_UITAG_EVENT event, const char* scripts)
	//
	// Ŀ�ģ������Ϣ������
	//
	void addListener(E_UITAG_EVENT event, const char* scripts);

	//
	// ������getListener(E_UITAG_EVENT event)
	//
	// Ŀ�ģ���ȡ��Ϣ������
	//
	const char* getListener(E_UITAG_EVENT event);

	//
	// ������removeListener(E_UITAG_EVENT event)
	//
	// Ŀ�ģ�ɾ����Ϣ������
	//
	bool removeListener(E_UITAG_EVENT event);

private:

	//
	// ������fetchPosition()
	//
	// Ŀ�ģ���CSS��top��left��ȷ��λ��
	//
	virtual void fetchPosition() = 0;

protected:
	
	//
	// ���ԣ�myStrValue
	//
	// ˵����Ԫ����ֵ
	//
	char * myStrValue;

	//
	//��UILayer�ϵ�˳��ֵԽ��Խ��
	//
	unsigned int myOrder;            

	//
	// ���ԣ�myWindowStatus
	//
	// Ŀ�ģ����ڴ�״̬
	//
	E_COMPONENT_STATUS myComponentStatus;

	//
	//��������������
	//
	YCDList* myListeners;
};

#endif
