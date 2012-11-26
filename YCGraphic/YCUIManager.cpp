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
 * YCUIManagerImpl : UIManager�ڲ�ʵ���࣬�����䵱��������
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
	// ������initialize()
	//
	// Ŀ�ģ���ʼ��
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
	// ������setUIBasePath(const char* ui)
	//
	// Ŀ�ģ�����UI����·��
	//
	void setUIBasePath(const char* ui)
	{
		if (ui == NULL)
		{
			throw YCException(2002, "YCUIManagerImpl::setUIBasePathΪNULL");
		}
		myUIBasePath = ui;
	}

	//
	// ������getUIBasePath()
	//
	// Ŀ�ģ� ��ȡUI����·��
	//
	const char * getUIBasePath()
	{
		return myUIBasePath.c_str();
	}

	//
	// ������finalize()
	//
	// Ŀ�ģ�ע��YCUIManagerImpl���ͷ���Դ
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
	// ������registController(const char* name, YCIController* controller)
	//
	// Ŀ�ģ�ע�������
	//
	void registController(const char* name, YCIController* controller)
	{
		if (name == NULL || controller == NULL)
		{
			throw YCException(2002, "YCUIManager::registControllerע��ֵNULL");
		}

		std::map<std::string, YCIController*>::iterator at = myControllers.find(name);
		if (at != myControllers.end())
		{			
			throw YCException(2002, "YCUIManager::registControllerע��������ظ�");
		}
		myControllers[name] = controller;
	}

	//
	// ������findController(const char* name)
	//
	// Ŀ�ģ���ѯ������
	//
	YCIController* findController(const char* name)
	{
		if (name == NULL)
		{
			throw YCException(2002, "YCUIManager::findController��nameΪULL");
		}

		std::map<std::string, YCIController*>::iterator at = myControllers.find(name);
		if (at != myControllers.end())
		{
			return myControllers[name];
		}
		return NULL;
	}

	//
	// ������removeController(const char* name)
	//
	// Ŀ�ģ��Ƴ�������
	//
	YCIController* removeController(const char* name)
	{
		if (name == NULL)
		{
			throw YCException(2002, "YCUIManager::removeController��nameΪULL");
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
	// ������registContainer(const char* name, YCUIDOM* uiDom)
	//
	// Ŀ�ģ�ע�ᴰ��
	//
	void registContainer(const char* name, YCUIDOM* uiDom)
	{
		if (name == NULL || uiDom == NULL)
		{
			throw YCException(2002, "YCUIManager::registContainerע��ֵNULL");
		}

		std::map<std::string, YCUIDOM*>::iterator at = myWindowPool.find(name);
		if (at != myWindowPool.end())
		{
			LOG_WARNING("YCUIManager::registContainerע�ᴰ���ظ�: " << name);
			throw YCException(2002, "YCUIManager::registContainerע�ᴰ���ظ���");
		}

		myWindowPool[name] = uiDom;
	}

	//
	// ������findContainer(const char* name)
	//
	// Ŀ�ģ����ݴ�������ȡ����
	//
	YCIContainer* findContainer(const char* name)
	{
		if (name == NULL)
		{
			throw YCException(2002, "YCUIManager::findContainer��nameΪULL");
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
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������,ʹUIManager�л��ᴦ���ݼ���Ϣ
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
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
				controller->handleRequest(NULL); //��������
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
				throw YCException(2002, "YCWindow::initAccelerateKey ��ݼ������Ŀ����!");
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
// ������initialize(YCTextureManager* textureManager, YCInput& input, YCConfig& config)
//
// Ŀ�ģ���ʼ��UI������������ȱʡUI����
//
bool YCUIManager::initialize(YCTextureManager* textureManager, YCInput& input, YCConfig& config)
{
	if (textureManager == NULL)
	{
		throw YCException(2002, "YCUIManager::initialize��textureManagerΪ��");
	}

	myTagFactory = new YCTagFactory();
	if (!myTagFactory->initialize(config))
	{
		throw YCException(2002, "YCUIManager::initialize��ʼ����ǩ��ʧ��");
	}

	//��������
	YCIComponent::gInput = &input;
	YCIComponent::gTagFactory = myTagFactory;

	//��ȡ��ǰ����Ŀ¼
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
// ������registController(const char* module, YCIController* controller)
//
// Ŀ�ģ�ע�ᴦ����
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
		LOG_WARNING("YCUIManager::registControllerע�����������: " << e.what());
		return false;
	}
	return true;
}

//
// ������handleRequest(const char *module, YCLuaRequest * request)
//
// Ŀ�ģ���������Ϣ
//
void YCUIManager::handleRequest(const char *module, YCLuaRequest * request)
{
	SMART_ASSERT(myUIManagerImpl != NULL);
	YCIController *controller = myUIManagerImpl->findController(module);
	if (controller == NULL)
	{
		LOG_WARNING("YCUIManager::handleRequestδ�ҵ���������" << module);
	}
	else
	{
		controller->handleRequest(request);
	}
}

//
// ������removeController(const char *module)
//
// Ŀ�ģ��Ƴ�������
//
YCIController* YCUIManager::removeController(const char *module)
{
	SMART_ASSERT(myUIManagerImpl != NULL);
	return myUIManagerImpl->removeController(module);
}

//
// ������registUI(const char* name, const char* xmlFile)
//
// Ŀ�ģ�ע��UI��XML����Ԫ��
//
bool YCUIManager::registUI(const char* name, const char* xmlFile)
{
	SMART_ASSERT(myUIManagerImpl != NULL);
	try
	{
		const char* uiPath = myUIManagerImpl->getUIBasePath();
		if (uiPath == NULL)
		{
			throw YCException(2002, "YCUIManager::registUIδ����UI����·��");
		}
		if (xmlFile == NULL)
		{
			throw YCException(2002, "YCUIManager::registUIδ����xmlFileΪ��");
		}

		int len = strlen(uiPath) + strlen(xmlFile);
		if (len >= MAX_PATH)
		{
			throw YCException(2002, "YCUIManager::registUI�ļ�·��������!");
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
		LOG_WARNING("YCUIManager::registUI����XML�����ļ�ʧ�ܣ�" << e.what());
		return false;
	}

	return true;
}

//
// ������open(const char* name, YCLuaParameter* context)
//
// Ŀ�ģ���ȡUIԪ��
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
		throw YCException(2002, "YCUIManager::open�򿪴���ʧ�ܣ�");
	}

	YCScene *scene = (YCScene*)YCRegistry::get("scene");
	if (scene == NULL)
	{
		throw YCException(2002, "YCAvatarController::handleRequest�޷���ȡ��ǰ�������򿪴���ʧ��!");
	}
	
	YCUILayer* uiLayer = (YCUILayer*)scene->getLayer(UI);
	if (uiLayer == NULL)
	{
		throw YCException(2002, "YCAvatarController::handleRequest�޷���ȡ��ǰUI�㣬�򿪴���ʧ��!");
	}
	
	uiLayer->addComponent(container);
	
	return true;
}

//
// ������finalize()
//
// Ŀ�ģ�����YCUIManager
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
// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ�������Ϣ������,ʹUIManager�л��ᴦ���ݼ���Ϣ
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCUIManager::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
	{
		return myUIManagerImpl->onMessage(message, wParam, lParam);
	}
	return false;
}
