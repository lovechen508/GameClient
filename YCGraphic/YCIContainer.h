#ifndef _INCLUDE_YCICONTAINER_H_
#define _INCLUDE_YCICONTAINER_H_

#include "YCIUITag.h"
#include "YCUITagDef.h"

class YCDList;
class YCTagFactory;
class YCLuaParameter;

/*
 * YCIContainer 容器类 继承层次图
 */

class YCAPI YCIContainer :
	public YCIUITag
{

public:

	//
	// 函数：YCIContainer(const YCUITagMeta* tagMeta)
	//
	// 目的：构造函数，设置窗口绑定的DOM
	//
	YCIContainer(const YCUITagMeta* tagMeta);

	//
	// 函数：~YCIContainer(void)
	//
	// 目的：析构函数
	//
	virtual ~YCIContainer(void);
	
	//
	// 函数：clone()
	//
	// 目的：克隆当前类
	//
	virtual YCIContainer* clone() = 0;

	//
	// 函数：initialize(const char* name)
	//
	// 目的：初始化，每次窗口打开后都要初始化以根据DOM创建Tag
	//
	bool initialize(const char* name);

	//
	// 函数：bind2DOM(const YCUIDOM* dom)
	//
	// 目的：绑定到DOM文档对象模型
	//
	void bind2DOM(const YCUIDOM* dom);

	//
	// 函数：getWindowName()
	//
	// 目的：取得窗口名称
	//
	const char* getWindowName();

	//
	// 函数：matchAccelerateKey()
	//
	// 目的：是否匹配快捷键
	//
	virtual bool matchAccelerateKey();
	
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
	virtual bool open(YCLuaParameter* context);

	//
	// 函数：close()
	//
	// 目的： 关闭一个窗口实例
	//
	// 注释：
	//       1. 通过YCUILayer中onMessage触发快捷键关闭
	//          此时的调用为close(true)
	//          内部执行是检查是否有绑定的lua OnClose脚本
	//          有则执行脚本，否则转发至调用finalClose(false)
	//
	virtual void close();

	//
	// 函数：finalize()
	//
	// 目的：初始化，只在YCUIManager::finalize中注销时调用一次
	//
	void finalize();

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

public:

	//
	// 函数：finalOpen()
	//
	// 目的： 最后打开元件实例
	//        同时绑定窗口打开的数据上下文
	//
	// 注释：
	//       进场动画已经完成
	//
	virtual bool finalOpen();

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
	virtual void finalClose();

protected:
			
	//
	// 函数：extraDraw()
	//
	// 目的：UI元件的自定义绘制
	//
	virtual void extraDraw();

	//
	// 函数：extraCSS()
	//
	// 目的：处理特定的CSS属性
	//
	virtual void extraCSS();

private:

	//
	// 函数：fetchPosition()
	//
	// 目的：从CSS的top、left来确定位置
	//
	virtual void fetchPosition();

	//
	// 函数：validateSubTag(const char* name)
	//
	// 目的：校验子控件
	//
	virtual bool validateSubTag(const char* name);

	//
	// 函数：bindContext(YCLuaParameter* context)
	//
	// 目的：
	//
	void bindContext(YCLuaParameter* context);

	//
	// 函数：preload()
	//
	// 目的：准备数据等，每次打开窗口前调用
	//
	virtual bool preload() = 0;
	
	//
	// 函数：postunload()
	//
	// 目的：卸载数据等，每次关闭窗口前调用
	//
	virtual bool postunload() = 0;

	//
	// 函数：initAccelerateKey()
	//
	// 目的：处理快捷键
	//
	virtual void initAccelerateKey();


protected:

	//
	// 属性：myName
	//
	// 目的：窗口名称
	//
	char myWindowName[MAX_CONTAINER_NAME];

protected:
	
	//
	// 属性：myUIDom
	//
	// 说明：每一个UITag必须包含1个YCUIDom，不变
	//       ui配置文件的真实反映
	//
	const YCUIDOM* myUIDOM;
		
};

#endif
