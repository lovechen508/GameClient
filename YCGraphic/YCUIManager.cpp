#include "YCUIManager.h"

#include <map>

#include "YCScene.h"
#include "YCUIDOM.h"
#include "YCUILayer.h"
#include "YCUITagDef.h"
#include "YCIContainer.h"
#include "YCIController.h"
#include "YCTagFactory.h"
#include "YCUIDomBuilder.h"
#include "YCTextureManager.h"

#include "YCLua\YCLua.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCStringHelper.h"
#include "YCConfig\YCConfig.h"
#include "YCBasic\YCException.h"

#pragma region YCUIManagerImpl

/*
 * YCUIManagerImpl : UIManager内部实现类，仅仅充当容器功能
 */
class YCUIManagerImpl
	: public YCIInputListener
{
public:

	YCUIManagerImpl() 
		: myTagFactory(NULL)
	{
	}

	//
	// 函数：initialize()
	//
	// 目的：初始化
	//
	bool initialize(YCTagFactory* tagFactory, YCInput* input, YCConfig& config)
	{
		SMART_ASSERT(input != NULL);
		myInput = input;

		myTagFactory = tagFactory;
		return true;
	}

	~YCUIManagerImpl()
	{
	}

	//
	// 函数：setUIBasePath(const char* ui)
	//
	// 目的：设置UI基础路径
	//
	void setUIBasePath(const char* ui)
	{
		if (ui == NULL)
		{
			throw YCException(2002, "YCUIManagerImpl::setUIBasePath为NULL");
		}
		myUIBasePath = ui;
	}

	//
	// 函数：getUIBasePath()
	//
	// 目的： 获取UI基础路径
	//
	const char * getUIBasePath()
	{
		return myUIBasePath.c_str();
	}

	//
	// 函数：finalize()
	//
	// 目的：注销YCUIManagerImpl，释放资源
	//
	void finalize()
	{
		for (std::map<std::string, YCIController*>::iterator it = myControllers.begin();
			 it != myControllers.end();
			 ++it)
		{
			delete it->second;
		}
		myControllers.clear();

		for (std::map<std::string, YCUIDOM*>::iterator it = myWindowPool.begin();
			 it != myWindowPool.end();
			 ++it)
		{
			delete it->second;
		}
		myWindowPool.clear();

		for (std::map<YCUIDOM*, char*>::iterator it = myWindowAccelerateKey.begin();
			 it != myWindowAccelerateKey.end();
			 ++it)
		{
			delete[] it->second;
		}
		myWindowAccelerateKey.clear();
	}

	//
	// 函数：registController(const char* name, YCIController* controller)
	//
	// 目的：注册控制器
	//
	void registController(const char* name, YCIController* controller)
	{
		if (name == NULL || controller == NULL)
		{
			throw YCException(2002, "YCUIManager::registController注册值NULL");
		}

		std::map<std::string, YCIController*>::iterator at = myControllers.find(name);
		if (at != myControllers.end())
		{			
			throw YCException(2002, "YCUIManager::registController注册控制器重复");
		}
		myControllers[name] = controller;
	}

	//
	// 函数：findController(const char* name)
	//
	// 目的：查询控制器
	//
	YCIController* findController(const char* name)
	{
		if (name == NULL)
		{
			throw YCException(2002, "YCUIManager::findController中name为ULL");
		}

		std::map<std::string, YCIController*>::iterator at = myControllers.find(name);
		if (at != myControllers.end())
		{
			return myControllers[name];
		}
		return NULL;
	}

	//
	// 函数：removeController(const char* name)
	//
	// 目的：移除控制器
	//
	YCIController* removeController(const char* name)
	{
		if (name == NULL)
		{
			throw YCException(2002, "YCUIManager::removeController中name为ULL");
		}

		YCIController* found = NULL;
		std::map<std::string, YCIController*>::iterator at = myControllers.find(name);
		if (at != myControllers.end())
		{
			found = at->second;
		}
		return found;
	}

	//
	// 函数：registContainer(const char* name, YCUIDOM* uiDom)
	//
	// 目的：注册窗口
	//
	void registContainer(const char* name, YCUIDOM* uiDom)
	{
		if (name == NULL || uiDom == NULL)
		{
			throw YCException(2002, "YCUIManager::registContainer注册值NULL");
		}

		std::map<std::string, YCUIDOM*>::iterator at = myWindowPool.find(name);
		if (at != myWindowPool.end())
		{
			LOG_WARNING("YCUIManager::registContainer注册窗口重复: " << name);
			throw YCException(2002, "YCUIManager::registContainer注册窗口重复！");
		}

		myWindowPool[name] = uiDom;
	}

	//
	// 函数：findContainer(const char* name)
	//
	// 目的：根据窗口名获取窗口
	//
	YCIContainer* findContainer(const char* name)
	{
		if (name == NULL)
		{
			throw YCException(2002, "YCUIManager::findContainer中name为ULL");
		}

		YCIContainer* container = NULL;
		std::map<std::string, YCUIDOM*>::iterator at = myWindowPool.find(name);
		if (at != myWindowPool.end())
		{
			YCUIDOM * uiDom = at->second;
			container = (YCIContainer*)myTagFactory->create(uiDom->getName());		
			if (container != NULL)
			{
				container->bind2DOM(uiDom);
			}
		}
		return container;
	}

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数,使UIManager有机会处理快捷键消息
	//
	// 返回值：已处理，返回true，否则返回false
	//
	bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		const char* windowName = NULL;
		for (std::map<std::string, YCUIDOM*>::iterator it = myWindowPool.begin();
			 it != myWindowPool.end() && !windowName;
			 ++it)
		{
			YCUIDOM* uiDom = it->second;
			if (matchAccelerateKey(uiDom))
			{
				windowName = it->first.c_str();
			}
		}
		
		if (windowName != NULL)
		{
			YCIController *controller = findController(windowName);
			if (controller != NULL)
			{
				controller->handleRequest(NULL); //无上下文
			}
			return true;
		}
		return false;
	}

private:

	bool matchAccelerateKey(YCUIDOM* uiDom)
	{
		const char* accelerateKey = uiDom->getAttribute("accelerateKey");
		if (accelerateKey == NULL)
		{
			return false;
		}

		char* accelerateKeyArray = NULL;
		std::map<YCUIDOM*, char*>::iterator at = myWindowAccelerateKey.find(uiDom);
		if (at == myWindowAccelerateKey.end())
		{
			std::vector<std::string> splited;
			YCStringHelper::split(accelerateKey, "+", &splited);
			SMART_ASSERT(splited.size() >= MAX_ACCELERATE_KEYS).warn(accelerateKey);
			if (splited.size() > MAX_ACCELERATE_KEYS)
			{
				throw YCException(2002, "YCWindow::initAccelerateKey 快捷键组合数目过多!");
			}
			accelerateKeyArray = new char[MAX_ACCELERATE_KEYS];
			memset(accelerateKeyArray, 0, MAX_ACCELERATE_KEYS);
			for (unsigned i = 0; i < splited.size(); ++i)
			{
				accelerateKeyArray[i] = YCUITagHelper::Instance().Key2Char(splited[i].c_str());
			}
			myWindowAccelerateKey[uiDom] = accelerateKeyArray;
		}
		else
		{
			accelerateKeyArray = at->second;
		}

		bool hasAccelerate = false;
		for (int i = 0; i < MAX_ACCELERATE_KEYS; ++i)
		{
			if (accelerateKeyArray[i] != 0)
			{
				hasAccelerate = true;
				if (!myInput->isKeyDown(accelerateKeyArray[i]))
				{
					return false;
				}
			}
		}
		return (hasAccelerate ? true : false);
	}

private:

	YCInput *myInput;
	
	YCTagFactory * myTagFactory;

	std::string myUIBasePath;

	std::map<std::string, YCIController*> myControllers;

	std::map<std::string, YCUIDOM*> myWindowPool;
	
	std::map<YCUIDOM*, char*> myWindowAccelerateKey;
};

#pragma endregion YCUIManagerImpl

YCUIManager::YCUIManager(void)
	: myTextureManager(NULL)
	, myUIManagerImpl(NULL)
	, myTagFactory(NULL)
{
}


YCUIManager::~YCUIManager(void)
{
}

//
// 函数：initialize(YCTextureManager* textureManager, YCInput& input, YCConfig& config)
//
// 目的：初始化UI管理器，读入缺省UI配置
//
bool YCUIManager::initialize(YCTextureManager* textureManager, YCInput& input, YCConfig& config)
{
	if (textureManager == NULL)
	{
		throw YCException(2002, "YCUIManager::initialize中textureManager为空");
	}

	myTagFactory = new YCTagFactory();
	if (!myTagFactory->initialize(config))
	{
		throw YCException(2002, "YCUIManager::initialize初始化标签库失败");
	}

	//绑定鼠标键盘
	YCIComponent::gInput = &input;
	YCIComponent::gTagFactory = myTagFactory;

	//获取当前工作目录
	char theExePath[MAX_PATH] = {0};
	if (!YCFileUtil::GetExePath(theExePath, MAX_PATH))
	{
		return false;
	}

	myTextureManager = textureManager;
	myUIManagerImpl = new YCUIManagerImpl();
	if (!myUIManagerImpl->initialize(myTagFactory, &input, config))
	{
		return false;
	}
	
	char theUIPath[MAX_PATH] = {0};
	sprintf_s(theUIPath, "%s\\..\\%s", theExePath, config.uiPath());
	myUIManagerImpl->setUIBasePath(theUIPath);
	
	return true;
}

//
// 函数：registController(const char* module, YCIController* controller)
//
// 目的：注册处理器
//
bool YCUIManager::registController(const char *module, YCIController* controller)
{
	SMART_ASSERT(myUIManagerImpl != NULL);

	try 
	{
		myUIManagerImpl->registController(module, controller);	
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCUIManager::registController注册控制器错误: " << e.what());
		return false;
	}
	return true;
}

//
// 函数：handleRequest(const char *module, YCLuaRequest * request)
//
// 目的：处理窗口消息
//
void YCUIManager::handleRequest(const char *module, YCLuaRequest * request)
{
	SMART_ASSERT(myUIManagerImpl != NULL);
	YCIController *controller = myUIManagerImpl->findController(module);
	if (controller == NULL)
	{
		LOG_WARNING("YCUIManager::handleRequest未找到处理器：" << module);
	}
	else
	{
		controller->handleRequest(request);
	}
}

//
// 函数：removeController(const char *module)
//
// 目的：移除处理器
//
YCIController* YCUIManager::removeController(const char *module)
{
	SMART_ASSERT(myUIManagerImpl != NULL);
	return myUIManagerImpl->removeController(module);
}

//
// 函数：registUI(const char* name, const char* xmlFile)
//
// 目的：注册UI，XML配置元件
//
bool YCUIManager::registUI(const char* name, const char* xmlFile)
{
	SMART_ASSERT(myUIManagerImpl != NULL);
	try
	{
		const char* uiPath = myUIManagerImpl->getUIBasePath();
		if (uiPath == NULL)
		{
			throw YCException(2002, "YCUIManager::registUI未设置UI基础路径");
		}
		if (xmlFile == NULL)
		{
			throw YCException(2002, "YCUIManager::registUI未设置xmlFile为空");
		}

		int len = strlen(uiPath) + strlen(xmlFile);
		if (len >= MAX_PATH)
		{
			throw YCException(2002, "YCUIManager::registUI文件路径名过长!");
		}

		char theFullPath[MAX_PATH] = {0};
		sprintf_s(theFullPath, "%s\\%s", uiPath, xmlFile);
		YCUIDOM * uiDom = YCUIDomBuilder::build(theFullPath);
		if (uiDom != NULL)
		{
			myUIManagerImpl->registContainer(name, uiDom);
		}
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCUIManager::registUI解析XML配置文件失败：" << e.what());
		return false;
	}

	return true;
}

//
// 函数：open(const char* name, YCLuaParameter* context)
//
// 目的：获取UI元件
//
bool YCUIManager::open(const char* name, YCLuaParameter* context)
{
	SMART_ASSERT(myUIManagerImpl != NULL);

	YCIContainer* container = myUIManagerImpl->findContainer(name);
	if (container == NULL)
	{
		return false;
	}

	if (!container->initialize(name) || !container->open(context))
	{
		throw YCException(2002, "YCUIManager::open打开窗口失败！");
	}

	YCScene *scene = (YCScene*)YCRegistry::get("scene");
	if (scene == NULL)
	{
		throw YCException(2002, "YCAvatarController::handleRequest无法获取当前场景，打开窗口失败!");
	}
	
	YCUILayer* uiLayer = (YCUILayer*)scene->getLayer(UI);
	if (uiLayer == NULL)
	{
		throw YCException(2002, "YCAvatarController::handleRequest无法获取当前UI层，打开窗口失败!");
	}
	
	uiLayer->addComponent(container);
	
	return true;
}

//
// 函数：finalize()
//
// 目的：销毁YCUIManager
//
void YCUIManager::finalize()
{
	if (myUIManagerImpl != NULL)
	{
		myUIManagerImpl->finalize();
		delete myUIManagerImpl;
		myUIManagerImpl = NULL;
	}

	if (myTagFactory != NULL)
	{
		myTagFactory->finalize();
		delete myTagFactory;
		myTagFactory = NULL;
	}
}

//
// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：窗口消息处理函数,使UIManager有机会处理快捷键消息
//
// 返回值：已处理，返回true，否则返回false
//
bool YCUIManager::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
	{
		return myUIManagerImpl->onMessage(message, wParam, lParam);
	}
	return false;
}
