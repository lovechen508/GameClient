#ifndef _INCLUDE_YCUIMANAGER_H_
#define _INCLUDE_YCUIMANAGER_H_

#include "YCInput\YCIInputListener.h"

class YCInput;
class YCConfig;
class YCIModel;
class YCLuaRequest;
class YCLuaParameter;
class YCIContainer;
class YCIController;
class YCTextureManager;
class YCUIManagerImpl;
class YCTagFactory;

/*
 * 管理所有的UI界面
 */
class YCAPI YCUIManager
	: public YCIInputListener
{
public:

	YCUIManager(void);

	~YCUIManager(void);

	//
	// 函数：initialize(YCTextureManager* textureManager, YCInput& input, YCConfig& config)
	//
	// 目的：初始化UI管理器，读入缺省UI配置
	//
	bool initialize(YCTextureManager* textureManager, YCInput& input, YCConfig& config);

	//
	// 函数：registController(const char* module, YCIController* controller)
	//
	// 目的：注册处理器
	//
	bool registController(const char *module, YCIController* controller);

	//
	// 函数：handleRequest(const char *module, YCLuaRequest * request)
	//
	// 目的：处理请求打开窗口消息
	//
	void handleRequest(const char *module, YCLuaRequest * request);

	//
	// 函数：removeController(const char *module)
	//
	// 目的：移除处理器
	//
	YCIController* removeController(const char *module);

	//
	// 函数：registUI(const char* name, const char* xmlFile)
	//
	// 目的：注册UI，XML配置元件,完整路径
	//
	bool registUI(const char* name, const char* xmlFile);

	//
	// 函数：open(const char* name, YCLuaParameter* context)
	//
	// 目的：打开UI元件
	//
	bool open(const char* name, YCLuaParameter* context);

	//
	// 函数：finalize()
	//
	// 目的：销毁YCUIManager
	//
	void finalize();

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数,使UIManager有机会处理快捷键消息
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:
		
	YCTagFactory * myTagFactory;

	YCTextureManager *myTextureManager;

	YCUIManagerImpl * myUIManagerImpl;
};

#endif

