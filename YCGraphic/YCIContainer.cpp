#include "YCIContainer.h"

#include "YCBasic\YCAssert.h"

#include "YCUITag.h"
#include "YCUIDOM.h"
#include "YCCSSContext.h"
#include "YCCSSParser.h"
#include "YCTagFactory.h"
#include "YCIBackground.h"
#include "YCDynamicUIDOM.h"

#include "YCLua\YCLua.h"
#include "YCLua\YCLuaContext.h"
#include "YCLua\YCLuaParameter.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCStringHelper.h"

#pragma region YCUILineImpl

/*
 * YCUILineImpl : ����ؼ�������һ������
 *                ��Բ��ֵĿؼ���������YCUILineImpl��
 *                ���Զ�λ�Ŀؼ�����YCUILineImpl��
 */ 
class YCUILineImpl
{

};

#pragma endregion YCUILineImpl

//
// ������YCIContainer(const YCUITagMeta* tagMeta)
//
// Ŀ�ģ����캯�������ô��ڰ󶨵�DOM
//
YCIContainer::YCIContainer(const YCUITagMeta* tagMeta)
	: YCIUITag(tagMeta)
	, myUIDOM(NULL)
{
	memset(myWindowName, 0, MAX_CONTAINER_NAME);
}

//
// ������~YCIContainer(void)
//
// Ŀ�ģ���������
//
YCIContainer::~YCIContainer(void)
{

}

//
// ������initialize(const char* name)
//
// Ŀ�ģ���ʼ����ÿ�δ��ڴ򿪺�Ҫ��ʼ���Ը���DOM����Tag
//
bool YCIContainer::initialize(const char* name)
{
	if (myUIDOM == NULL)
	{
		throw YCException(2002, "YCIContainer::initialize����DOMΪNULL");
	}

	if (name == NULL)
	{
		throw YCException(2002, "YCIContainer::initialize������ΪNULL");
	}

	int len = strlen(name);
	if (len >= MAX_CONTAINER_NAME)
	{
		throw YCException(2002, "YCIContainer::initialize���������ȴ���20!");
	}

	memcpy(myWindowName, name, len);
		
	//////////////////////////////////////////////
	//����ݼ�
	//////////////////////////////////////////////
	initAccelerateKey();

	return true;
}


//
// ������bind2DOM(const YCUIDOM* dom)
//
// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
//
void YCIContainer::bind2DOM(const YCUIDOM* dom)
{
	if (dom == NULL)
	{
		throw YCException(2002, "YCIUITag::bind2DOM����domΪNULL");
	}

	myUIDOM = dom;
}

//
// ������getWindowName()
//
// Ŀ�ģ�ȡ�ô�������
//
const char* YCIContainer::getWindowName()
{
	return myWindowName;
}

//
// ������matchAccelerateKey()
//
// Ŀ�ģ��Ƿ�ƥ���ݼ�
//
bool YCIContainer::matchAccelerateKey()
{
	return false;
}

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
bool YCIContainer::open(YCLuaParameter* context)
{
	if (!preload())
	{
		throw YCException(2002, "YCIContainer::open�򿪴���preloadʧ��!");
	}

	//////////////////////////////////////////////
	// ���Ȱ�������
	//////////////////////////////////////////////
	bindContext(context);

	//////////////////////////////////////////////
	// ��������DynamicUIDOM
	//////////////////////////////////////////////
	myDynamicUIDOM = YCDynamicUIDOM::build(myUIDOM);
	if (myDynamicUIDOM == NULL)
	{
		LOG_WARNING("YCIContainer::open " << myWindowName << " ����DynamicUIDOMʧ��!");
	}
	
	//////////////////////////////////////////////
	//�����ؼ���
	//////////////////////////////////////////////
	myContainer = this;
	if (!createSubTags(myUIDOM))
	{
		LOG_WARNING("YCIContainer::open�����ӿؼ���ʧ�ܣ������޷��򿪣�" << myWindowName);
		throw YCException(2002, "YCIContainer::initialize�����ӿؼ���ʧ�ܣ������޷���");
	}

	//////////////////////////////////////////////
	//�������¼�
	//////////////////////////////////////////////	
	if (!initialTagAttribute())
	{
		LOG_ERROR("YCIContainer::open��ʼ����ǩ���Գ���");
		return false;
	}

	//////////////////////////////////////////////
	// ��ʼ��CSS
	//////////////////////////////////////////////
	char theBasePath[MAX_PATH] ={0};
	if (!YCFileUtil::GetFilePath(myUIDOM->getFilename(), theBasePath, MAX_PATH))
	{
		throw YCException(2002, "YCIContainer::initialize�޷�ȡ��Layout�ļ�·��");
	}

	YCCSSContext * theLayoutContext = new YCCSSContext();
	const YCDList* layouts = myUIDOM->getLayouts();
	for (const Item_List_T* item = layouts->begin();
		 item != layouts->end();
		 item = layouts->next(item))
	{
		const char* filename = (const char*)layouts->payload(item);
		if (filename != NULL)
		{
			try 
			{
				char theFullPath[MAX_PATH] ={0};
				int len = strlen(filename) + strlen(theBasePath);
				if (len >= MAX_PATH)
				{
					throw YCException(2002, "YCIContainer::initialize��CSS�ļ�·�����ȴ���MAX_PATH");
				}
				sprintf_s(theFullPath, "%s\\%s", theBasePath, filename);

				if (!YCCSSParser::parse(theLayoutContext, theFullPath))
				{
					LOG_WARNING("YCIContainer::initialize����CSS�ļ�����" << filename);
				}
			}
			catch (YCException& e)
			{
				LOG_FATAL("YCIContainer::initialize����CSS�ļ�" << filename << "�쳣��" << e.what());
			}
		}
	}

	//////////////////////////////////////////////
	// ��CSS
	//////////////////////////////////////////////
	bind2CSS(theLayoutContext);

	//////////////////////////////////////////////
	// У�鴰��λ��
	//////////////////////////////////////////////
	if (myPositionX == INT_MIN && myPositionY == INT_MIN)
	{
		setPosition(YCUITagHelper::getSpecialValue("HALF_SCREEN_WIDTH") - myWidthWithMargin, 
		            YCUITagHelper::getSpecialValue("HALF_SCREEN_HEIGHT") - myHeightWithMargin);
	}

	//////////////////////////////////////////////
	// ��ɴ��ڲ���
	//////////////////////////////////////////////
	layout(false);

	//////////////////////////////////////////////
	//���ڽ��붯��
	//////////////////////////////////////////////
	const char* onOpen = getListener(UITAG_OPEN_EVENT);
	if (onOpen != NULL)
	{
		YCLua* lua = (YCLua*)YCRegistry::get("lua");
		if (lua == NULL)
		{
			LOG_ERROR("YCIContainer::initialize��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
			throw YCException(2002, "YCIContainer::initialize��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
		}

		// ����Lua������
		YCLuaContext context;
		context.addContext("container", this);
		context.addContext("this", this);
		lua->execute(&context, onOpen, myWindowName);
	}
	else
	{
		finalOpen();
	}

	myComponentStatus = STATUS_OPEN;

	return true;
}

//
// ������close()
//
// Ŀ�ģ� �ر�һ������ʵ��
//
// ע�ͣ�
//          ͨ��YCUILayer��onMessage������ݼ��ر�
//          ��ʱ�ĵ���Ϊclose(true)
//          �ڲ�ִ���Ǽ���Ƿ��а󶨵�lua OnClose�ű�
//          ����ִ�нű����������finalClose(false)
//
void YCIContainer::close()
{
	//////////////////////////////////////////////
	//�����˳�����
	//////////////////////////////////////////////
	const char* onClose = getListener(UITAG_CLOSE_EVENT);
	if (onClose != NULL)
	{
		YCLua* lua = (YCLua*)YCRegistry::get("lua");
		if (lua == NULL)
		{
			LOG_ERROR("YCIContainer::initialize��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
			throw YCException(2002, "YCIContainer::initialize��ѯYCLua����ʧ�ܣ���ȷ��ע��YCLua��");
		}

		// ����Lua������
		YCLuaContext context;
		context.addContext("container", this);
		context.addContext("this", this);
		lua->execute(&context, onClose, myWindowName);
	}
	else
	{		
		finalClose();
	}
}

//
// ������finalize()
//
// Ŀ�ģ���ʼ����ֻ��YCUIManager::finalize��ע��ʱ����һ��
//
void YCIContainer::finalize()
{
	// ɾ���ؼ���
	try 
	{
		SAFE_DELETE(mySubTags);
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCIContainer::finalizeɾ���ؼ���ʧ��" << e.what());
	}

	// ɾ����̬�ڴ�UIDOM
	SAFE_DELETE(myDynamicUIDOM);

	// ɾ��CSSContext
	SAFE_DELETE(myLayoutContext);
}

//
// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ�������Ϣ������
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCIContainer::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{	
	bool result = YCIUITag::onMessage(message, wParam, lParam);
	if (!result && message == WM_MOUSEMOVE)
	{
		if (gInput->isLMouseButtonDown())
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			if (isMouseIn(x, y))
			{
				int deltaX = 0;
				int deltaY = 0;

				gInput->getMouseMoveDelta(deltaX, deltaY);
				move(deltaX, deltaY);

				return true;
			}
		}
	}

	return result;
}

//
// ������finalOpen()
//
// Ŀ�ģ� ����Ԫ��ʵ��
//        ͬʱ�󶨴��ڴ򿪵�����������
//
// ע�ͣ�
//       ���������Ѿ����
//
bool YCIContainer::finalOpen()
{
	return true;
}

//
// ������finalClose()
//
// Ŀ�ģ����ر�Ԫ��ʵ��
//
// ע�ͣ�
//          ����finalClose����ִ��close(false)
//          ��ʱLua�ű������Ѿ��������˳������Ѿ����
//          ֱ�����ô���״̬ΪSTATUS_CLOSE, ��YCUILayer
//          ��render���Ƴ���ɾ���ô���
//
void YCIContainer::finalClose()
{
	if (!postunload())
	{
		throw YCException(2002, "YCIContainer::close�رմ���ʧ�ܣ�");
	}
	myComponentStatus = STATUS_CLOSE;
	LOG_DEBUG("YCIContainer::close���ڹرճɹ�: " << myWindowName);
}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// ������extraDraw()
//
// Ŀ�ģ�UIԪ�����Զ������
//
void YCIContainer::extraDraw()
{
	
}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// ������validateSubTag(const char* name)
//
// Ŀ�ģ�У���ӿؼ�
//
bool YCIContainer::validateSubTag(const char* name)
{
	SMART_ASSERT(name != NULL);
	return (strcmp("window", name) != 0 && strcmp("dialog", name) != 0);
}

//
// ������fetchPosition()
//
// Ŀ�ģ���CSS��top��left��ȷ��λ��
//
void YCIContainer::fetchPosition()
{
	char position[MAX_PATH] = {0};

	int x = INT_MIN;
	int y = INT_MIN;

	if (getProperty(UITAG_LEFT, position, MAX_PATH))
	{
		if (YCStringHelper::isType(position, YCStringHelper::INTEGER))
		{
			x = atoi(position);				
		}
		else
		{
			try
			{
				x = YCUITagHelper::getSpecialValue(position);
			}
			catch (YCException& e)
			{
				LOG_WARNING("YCIUITag::fetchPosition��ЧUITAG_LEFTֵ" << e.what());
				x = YCUITagHelper::getSpecialValue("HALF_SCREEN_WIDTH");
			}
			x -= myWidthWithMargin/2;
		}
	}

	if (getProperty(UITAG_TOP, position, MAX_PATH))
	{
		if (YCStringHelper::isType(position, YCStringHelper::INTEGER))
		{
			y =  atoi(position);				
		}
		else
		{
			try
			{
				y = YCUITagHelper::getSpecialValue(position);
			}
			catch (YCException& e)
			{
				LOG_WARNING("YCIUITag::fetchPosition��ЧUITAG_TOPֵ" << e.what());
				y = YCUITagHelper::getSpecialValue("HALF_SCREEN_HEIGHT");
			}
			y -= myHeightWithMargin/2;
		}
	}
	
	//���¾��Զ�λ
	else if (hasProperty(UITAG_RIGHT) && hasProperty(UITAG_BOTTOM))
	{
		if (getProperty(UITAG_RIGHT, position, MAX_PATH))
		{
			if (YCStringHelper::isType(position, YCStringHelper::INTEGER))
			{
				x = atoi(position);				
			}
			else
			{
				try
				{
					x = YCUITagHelper::getSpecialValue(position);
				}
				catch (YCException& e)
				{
					LOG_WARNING("YCIUITag::fetchPosition��ЧUITAG_RIGHTֵ" << e.what());
					x = YCUITagHelper::getSpecialValue("HALF_SCREEN_WIDTH");
				}
				x -= myWidthWithMargin/2;
			}
		}

		if (getProperty(UITAG_BOTTOM, position, MAX_PATH))
		{
			if (YCStringHelper::isType(position, YCStringHelper::INTEGER))
			{
				y = atoi(position);				
			}
			else
			{
				try
				{
					y = YCUITagHelper::getSpecialValue(position);
				}
				catch (YCException& e)
				{
					LOG_WARNING("YCIUITag::fetchPosition��ЧUITAG_BOTTOMֵ" << e.what());
					y = YCUITagHelper::getSpecialValue("HALF_SCREEN_HEIGHT");
				}
				y -= myHeightWithMargin/2;
			}
		}
	}

	setPosition(x + myMarginLeft, y + myMarginTop);
}

//
// ������extraCSS()
//
// Ŀ�ģ������ض���CSS����
//
void YCIContainer::extraCSS()
{
	char position[MAX_PATH] = {0};
	
}

//
// ������bindContext(YCLuaParameter* context)
//
// Ŀ�ģ��󶨴��ڴ򿪵�����������
//
void YCIContainer::bindContext(YCLuaParameter* context)
{
	if (context != NULL)
	{
		//������debug����һ��У�鹤��

		context->push();
	}
}

//
// ������initAccelerateKey()
//
// Ŀ�ģ������ݼ�
//
void YCIContainer::initAccelerateKey()
{
	// Do Nothing...
	LOG_INFO("YCIContainer::initAccelerateKey��ݼ�������: " << myWindowName);
}