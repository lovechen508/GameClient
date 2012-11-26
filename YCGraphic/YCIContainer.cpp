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
 * YCUILineImpl : 虚拟控件，代表一行数据
 *                相对布局的控件都必须在YCUILineImpl中
 *                绝对定位的控件不在YCUILineImpl中
 */ 
class YCUILineImpl
{

};

#pragma endregion YCUILineImpl

//
// 函数：YCIContainer(const YCUITagMeta* tagMeta)
//
// 目的：构造函数，设置窗口绑定的DOM
//
YCIContainer::YCIContainer(const YCUITagMeta* tagMeta)
	: YCIUITag(tagMeta)
	, myUIDOM(NULL)
{
	memset(myWindowName, 0, MAX_CONTAINER_NAME);
}

//
// 函数：~YCIContainer(void)
//
// 目的：析构函数
//
YCIContainer::~YCIContainer(void)
{

}

//
// 函数：initialize(const char* name)
//
// 目的：初始化，每次窗口打开后都要初始化以根据DOM创建Tag
//
bool YCIContainer::initialize(const char* name)
{
	if (myUIDOM == NULL)
	{
		throw YCException(2002, "YCIContainer::initialize窗口DOM为NULL");
	}

	if (name == NULL)
	{
		throw YCException(2002, "YCIContainer::initialize窗口名为NULL");
	}

	int len = strlen(name);
	if (len >= MAX_CONTAINER_NAME)
	{
		throw YCException(2002, "YCIContainer::initialize窗口名长度大于20!");
	}

	memcpy(myWindowName, name, len);
		
	//////////////////////////////////////////////
	//检查快捷键
	//////////////////////////////////////////////
	initAccelerateKey();

	return true;
}


//
// 函数：bind2DOM(const YCUIDOM* dom)
//
// 目的：绑定到DOM文档对象模型
//
void YCIContainer::bind2DOM(const YCUIDOM* dom)
{
	if (dom == NULL)
	{
		throw YCException(2002, "YCIUITag::bind2DOM参数dom为NULL");
	}

	myUIDOM = dom;
}

//
// 函数：getWindowName()
//
// 目的：取得窗口名称
//
const char* YCIContainer::getWindowName()
{
	return myWindowName;
}

//
// 函数：matchAccelerateKey()
//
// 目的：是否匹配快捷键
//
bool YCIContainer::matchAccelerateKey()
{
	return false;
}

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
bool YCIContainer::open(YCLuaParameter* context)
{
	if (!preload())
	{
		throw YCException(2002, "YCIContainer::open打开窗口preload失败!");
	}

	//////////////////////////////////////////////
	// 首先绑定上下文
	//////////////////////////////////////////////
	bindContext(context);

	//////////////////////////////////////////////
	// 创建自身DynamicUIDOM
	//////////////////////////////////////////////
	myDynamicUIDOM = YCDynamicUIDOM::build(myUIDOM);
	if (myDynamicUIDOM == NULL)
	{
		LOG_WARNING("YCIContainer::open " << myWindowName << " 构建DynamicUIDOM失败!");
	}
	
	//////////////////////////////////////////////
	//创建控件集
	//////////////////////////////////////////////
	myContainer = this;
	if (!createSubTags(myUIDOM))
	{
		LOG_WARNING("YCIContainer::open创建子控件集失败，窗口无法打开：" << myWindowName);
		throw YCException(2002, "YCIContainer::initialize创建子控件集失败，窗口无法打开");
	}

	//////////////////////////////////////////////
	//绑定属性事件
	//////////////////////////////////////////////	
	if (!initialTagAttribute())
	{
		LOG_ERROR("YCIContainer::open初始化标签属性出错！");
		return false;
	}

	//////////////////////////////////////////////
	// 初始化CSS
	//////////////////////////////////////////////
	char theBasePath[MAX_PATH] ={0};
	if (!YCFileUtil::GetFilePath(myUIDOM->getFilename(), theBasePath, MAX_PATH))
	{
		throw YCException(2002, "YCIContainer::initialize无法取得Layout文件路径");
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
					throw YCException(2002, "YCIContainer::initialize中CSS文件路径长度大于MAX_PATH");
				}
				sprintf_s(theFullPath, "%s\\%s", theBasePath, filename);

				if (!YCCSSParser::parse(theLayoutContext, theFullPath))
				{
					LOG_WARNING("YCIContainer::initialize处理CSS文件出错：" << filename);
				}
			}
			catch (YCException& e)
			{
				LOG_FATAL("YCIContainer::initialize处理CSS文件" << filename << "异常：" << e.what());
			}
		}
	}

	//////////////////////////////////////////////
	// 绑定CSS
	//////////////////////////////////////////////
	bind2CSS(theLayoutContext);

	//////////////////////////////////////////////
	// 校验窗口位置
	//////////////////////////////////////////////
	if (myPositionX == INT_MIN && myPositionY == INT_MIN)
	{
		setPosition(YCUITagHelper::getSpecialValue("HALF_SCREEN_WIDTH") - myWidthWithMargin, 
		            YCUITagHelper::getSpecialValue("HALF_SCREEN_HEIGHT") - myHeightWithMargin);
	}

	//////////////////////////////////////////////
	// 完成窗口布局
	//////////////////////////////////////////////
	layout(false);

	//////////////////////////////////////////////
	//窗口进入动画
	//////////////////////////////////////////////
	const char* onOpen = getListener(UITAG_OPEN_EVENT);
	if (onOpen != NULL)
	{
		YCLua* lua = (YCLua*)YCRegistry::get("lua");
		if (lua == NULL)
		{
			LOG_ERROR("YCIContainer::initialize查询YCLua环境失败，请确认注册YCLua！");
			throw YCException(2002, "YCIContainer::initialize查询YCLua环境失败，请确认注册YCLua！");
		}

		// 构建Lua上下文
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
// 函数：close()
//
// 目的： 关闭一个窗口实例
//
// 注释：
//          通过YCUILayer中onMessage触发快捷键关闭
//          此时的调用为close(true)
//          内部执行是检查是否有绑定的lua OnClose脚本
//          有则执行脚本，否则调用finalClose(false)
//
void YCIContainer::close()
{
	//////////////////////////////////////////////
	//窗口退出动画
	//////////////////////////////////////////////
	const char* onClose = getListener(UITAG_CLOSE_EVENT);
	if (onClose != NULL)
	{
		YCLua* lua = (YCLua*)YCRegistry::get("lua");
		if (lua == NULL)
		{
			LOG_ERROR("YCIContainer::initialize查询YCLua环境失败，请确认注册YCLua！");
			throw YCException(2002, "YCIContainer::initialize查询YCLua环境失败，请确认注册YCLua！");
		}

		// 构建Lua上下文
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
// 函数：finalize()
//
// 目的：初始化，只在YCUIManager::finalize中注销时调用一次
//
void YCIContainer::finalize()
{
	// 删除控件集
	try 
	{
		SAFE_DELETE(mySubTags);
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCIContainer::finalize删除控件集失败" << e.what());
	}

	// 删除动态内存UIDOM
	SAFE_DELETE(myDynamicUIDOM);

	// 删除CSSContext
	SAFE_DELETE(myLayoutContext);
}

//
// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：窗口消息处理函数
//
// 返回值：已处理，返回true，否则返回false
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
// 函数：finalOpen()
//
// 目的： 最后打开元件实例
//        同时绑定窗口打开的数据上下文
//
// 注释：
//       进场动画已经完成
//
bool YCIContainer::finalOpen()
{
	return true;
}

//
// 函数：finalClose()
//
// 目的：最后关闭元件实例
//
// 注释：
//          调用finalClose则是执行close(false)
//          此时Lua脚本调用已经结束，退场动画已经完成
//          直接设置窗口状态为STATUS_CLOSE, 在YCUILayer
//          中render内移除并删除该窗口
//
void YCIContainer::finalClose()
{
	if (!postunload())
	{
		throw YCException(2002, "YCIContainer::close关闭窗口失败！");
	}
	myComponentStatus = STATUS_CLOSE;
	LOG_DEBUG("YCIContainer::close窗口关闭成功: " << myWindowName);
}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// 函数：extraDraw()
//
// 目的：UI元件的自定义绘制
//
void YCIContainer::extraDraw()
{
	
}

//////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////

//
// 函数：validateSubTag(const char* name)
//
// 目的：校验子控件
//
bool YCIContainer::validateSubTag(const char* name)
{
	SMART_ASSERT(name != NULL);
	return (strcmp("window", name) != 0 && strcmp("dialog", name) != 0);
}

//
// 函数：fetchPosition()
//
// 目的：从CSS的top、left来确定位置
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
				LOG_WARNING("YCIUITag::fetchPosition无效UITAG_LEFT值" << e.what());
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
				LOG_WARNING("YCIUITag::fetchPosition无效UITAG_TOP值" << e.what());
				y = YCUITagHelper::getSpecialValue("HALF_SCREEN_HEIGHT");
			}
			y -= myHeightWithMargin/2;
		}
	}
	
	//右下绝对定位
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
					LOG_WARNING("YCIUITag::fetchPosition无效UITAG_RIGHT值" << e.what());
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
					LOG_WARNING("YCIUITag::fetchPosition无效UITAG_BOTTOM值" << e.what());
					y = YCUITagHelper::getSpecialValue("HALF_SCREEN_HEIGHT");
				}
				y -= myHeightWithMargin/2;
			}
		}
	}

	setPosition(x + myMarginLeft, y + myMarginTop);
}

//
// 函数：extraCSS()
//
// 目的：处理特定的CSS属性
//
void YCIContainer::extraCSS()
{
	char position[MAX_PATH] = {0};
	
}

//
// 函数：bindContext(YCLuaParameter* context)
//
// 目的：绑定窗口打开的数据上下文
//
void YCIContainer::bindContext(YCLuaParameter* context)
{
	if (context != NULL)
	{
		//可以在debug中做一下校验工作

		context->push();
	}
}

//
// 函数：initAccelerateKey()
//
// 目的：处理快捷键
//
void YCIContainer::initAccelerateKey()
{
	// Do Nothing...
	LOG_INFO("YCIContainer::initAccelerateKey快捷键被忽略: " << myWindowName);
}