#ifndef _INCLUDE_YCGRAPHIC_H_
#define _INCLUDE_YCGRAPHIC_H_

#include <Windows.h>

#include "YCRect.h"
#include "YCUITagDef.h"
#include "YCSceneDef.h"

#include "YCInput\YCIInputListener.h"

class YCScene;
class YCInput;
class YCITexture;
class YCConfig;
class YCFontImpl;
class YCUIManager;
class YCIMapFactory;
class YCQueryFactory;
class YCSceneManager;
class YCD3DSpriteImpl;
class YCTextureManager;
class YCAnimationManager;
class YCNetworkFileLoader;

/*
 *  DirectX 包装器
 *
 *  正确调用序列：
 *     ctor => intialize => repeat render => finalize => dsctor
 *
 */
class YCAPI YCGraphic
	: public YCIInputListener
{
public:

	typedef enum E_PIXSHADER_VERSION{
		PIXSHADER_1_1          = 1,
		PIXSHADER_2_0          = 2,
		PIXSHADER_3_0          = 3,
		PIXSHADER_4_0          = 4,
		PIXSHADER_5_0          = 5,
		PIXSHADER_UNKNOWN      = 6,
	};

public:

	YCGraphic(void);

	~YCGraphic(void);

	//
	// 函数：initialize(HWND hwnd, YCInput& input, YCConfig& config)
	//
	// 目的：初始化DirectX环境
	//
	bool initialize(HWND hwnd, YCInput& input, YCConfig& config);  

	//
	// 函数：registerMapFactory(YCIMapFactory* mapFactory)
	//
	// 目的：注册地图工厂
	//
	void registerMapFactory(YCIMapFactory* mapFactory);

	//
	// 函数：getUIManager()
	//
	// 目的：取得界面管理器
	//
	YCUIManager* getUIManager();

	//
	// 函数：getScene()
	//
	// 目的：取得场景管理器
	//
	YCScene* getScene();

	//
	// 函数：getWindowSize()
	//
	// 目的：获取窗口大小
	//
	SIZE getWindowSize();

	//
	// 函数：setWindowSize()
	//
	// 目的：设置窗口大小
	//
	void setWindowSize(SIZE& size);

	//
	// 函数：setFPS(int fps)
	//
	// 目的：设置帧频
	//
	void setFPS(int fps);

	//
	// 函数：getFPS()
	//
	// 目的：获取帧频
	//
	int getFPS();

	//
	// 函数：setVisible(bool visible)
	//
	// 目的：设置窗口可见性
	//
	void setVisible(bool visible);

	//
	// 函数：updateScene(YCScene* newScene)
	//
	// 目的：设置场景:
	//
	// 注释：返回旧场景指针，首次为NULL
	//
	YCScene* updateScene(YCScene* newScene);

	//
	// 函数：render()
	//
	// 目的：渲染
	//
	void render();

	//
	// 函数：snapshot()
	//
	// 目的：保存截屏
	//
	void snapshot();

	//
	// 函数：GetMaxTextureBlendStages()
	//
	// 目的：当前显卡支持的多重纹理层数
	//
	unsigned int getMaxTextureBlendStages();

	//
	// 函数：getPixShaderVersion()
	//
	// 目的：获取显卡支持的PixShader版本
	//
	E_PIXSHADER_VERSION getPixShaderVersion();

	//
	// 函数：finalize()
	//
	// 目的：释放DirectX资源
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

#pragma region YCD3DSprite

	/*
	 * YCD3DSprite : 2D纹理绘制类
	 *          只能从YCGraphic接口创建
	 */ 
	class YCD3DSprite
	{
	public:

		YCD3DSprite() {}

		virtual ~YCD3DSprite() {}

		//
		// 函数：timestamp()
		//
		// 目的：当前帧时间戳
		// 
		virtual time_t timestamp() = 0;
				
		//
		// 函数：begin()
		//
		// 目的：在draw之前调用
		//
		virtual YCD3DSprite* begin(bool render) = 0;

		//
		// 函数：setViewport()
		//
		// 目的：设置裁剪区域
		//
		virtual IntRect setViewport(const IntRect& viewport) = 0;

		//
		// 函数：narrowViewport(const IntRect& viewport)
		//
		// 目的：缩减裁剪区域
		//
		virtual IntRect narrowViewport(const IntRect& viewport) = 0;

		//
		// 函数：recoverViewport(const IntRect& viewport)
		//
		// 目的：恢复裁剪区域
		//
		virtual void recoverViewport(const IntRect& viewport) = 0;

		//
		// 函数：draw(YCITexture* texture, float left, float top, float repeatX, float repeatY, float alpha, unsigned int color)
		//
		// 目的：添加一次Lock顶点缓冲的绘制纹理
		//
		virtual YCD3DSprite* draw(YCITexture* texture, float left, float top, float repeatX, float repeatY, float alpha, unsigned int color) = 0;
		
		//
		// 函数：end()
		//
		// 目的：目的：在draw结束调用
		//
		virtual void end() = 0;
	};

	//
	// 函数：createSpriteDrawer()
	//
	// 目的：获取精灵绘制接口
	//
	YCD3DSprite* createD3DSprite();

#pragma endregion YCD3DSprite

#pragma region YCD3DPrimitive

	/*
	 * YCD3DPrimitive : 画点，线，矩形，扇形等，无纹理
	 */
	class YCD3DPrimitive
	{
	public:

		YCD3DPrimitive() {}

		virtual ~YCD3DPrimitive() {}
	
		//
		// 函数 drawPoint(unsigned int x, unsigned int y, unsigned int color);
		//
		// 目的：画点
		//
		virtual YCD3DSprite* drawPoint(unsigned int x, unsigned int y, unsigned int color) = 0;

		//
		// 函数 drawLine(unsigned int xSrc, unsigned int ySrc, unsigned int xDst, unsigned int yDst, unsigned int colorV0, unsigned int colorV1);
		//
		// 目的：画线条
		//
		virtual YCD3DSprite* drawLine(unsigned int xSrc, unsigned int ySrc,
				          	          unsigned int xDst, unsigned int yDst,
							          unsigned int colorV0, unsigned int colorV1) = 0;

		// 
		// 函数：drawRetangle(unsigned int left,    unsigned int top,    unsigned int right,   unsigned int bottom,
		//	                 unsigned int colorV0, unsigned int colorV1, unsigned int colorV2, unsigned int colorV3)
		//
		// 参数：
		//
		// 目的：画四边形
		//
		virtual YCD3DSprite* drawRetangle(unsigned int left,    unsigned int top,     unsigned int right,   unsigned int bottom,
			                              unsigned int colorV0, unsigned int colorV1, unsigned int colorV2, unsigned int colorV3) = 0;
	};

	//
	// 函数：createPrimitive()
	//
	// 目的：获取基本件绘制接口
	//
	YCD3DPrimitive* createPrimitive();

#pragma endregion YCD3DPrimitive

#pragma region YCFont
	
	/*
	 * YCFont : 字体绘制类
	 *          只能从YCGraphic接口创建
	 */ 

	class YCAPI YCFont
	{
	public:

		//
		// 函数：YCFont()
		//
		// 目的：构造函数
		//
		YCFont(YCD3DSprite * sprite);

		//
		// 函数：~YCFont()
		//
		// 目的：构造函数
		//
		~YCFont();

        //
        // 函数：setAutoScaled(bool autoScaled)
        //
        // 目的：设置自动缩放
        //
        void setAutoScaled(bool autoScaled);

        //
        // 函数：isAutoScaled()
        //
        // 目的：是否自动缩放
        //
        bool isAutoScaled() const;

		//
		// 函数：getFontWidthAndHeight(E_FONT_TYPE type, E_FONT_DIRECTION direction, unsigned int size, const char* strInfo, int& width, int& height)
		//
		// 目的：取得特定字体及大小下面高和宽
		//
		bool getFontWidthAndHeight(E_FONT_TYPE type, E_FONT_DIRECTION direction, unsigned int size, const char* strInfo, int& width, int& height);

		//
		// 函数：draw(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color, unsigned int left, unsigned int top)
		//
		// 目的：相对屏幕坐标系绘制文本
		//
		// 参数：left    横坐标
		//       top     纵坐标
		//       size    字符大小
		//       color   文本颜色 0xff000000
		//       text    字符串
		//
		void draw(E_FONT_TYPE type, 
			      const char* text,
				  unsigned int size, 
				  unsigned int color,
			      unsigned int left,
				  unsigned int top);

		//
		// 函数：drawWithStroke(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color,
		//	      unsigned int left, unsigned int top, unsigned int strokeColor
		//
		// 目的：相对屏幕坐标系绘制文本
		//
		// 参数：left			横坐标
		//       top			纵坐标
		//       size			字符大小
		//       color			文本颜色 0xff000000
		//       text			字符串
		//       strokeColor	描边色
		//
		void drawWithStroke(E_FONT_TYPE type, 
			                const char* text, 
							unsigned int size, 
							unsigned int color,
							unsigned int left, 
							unsigned int top, 
							unsigned int strokeColor);

		//
		// 函数：drawWithUnderline(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color,
		//	      unsigned int left, unsigned int top, unsigned int underlineColor
		//
		// 目的：相对屏幕坐标系绘制文本
		//
		// 参数：left			横坐标
		//       top			纵坐标
		//       size			字符大小
		//       color			文本颜色 0xff000000
		//       text			字符串
		//       underlineColor	下划线颜色
		//
		void drawWithUnderline(E_FONT_TYPE type, 
			                   const char* text, 
							   unsigned int size, 
							   unsigned int color,
							   unsigned int left, 
							   unsigned int top, 
							   unsigned int underlineColor);

		//
		// 函数：drawWithStrokeAndUnderline(E_FONT_TYPE type, const char* text, unsigned int color,
		//	      unsigned int left, unsigned int top, unsigned int strokeColor, unsigned int underlineColor
		//
		// 目的：相对屏幕坐标系绘制文本
		//
		// 参数：left			横坐标
		//       top			纵坐标
		//       size			字符大小
		//       color			文本颜色 0xff000000
		//       text			字符串
		//       strokeColor	描边色
		//       underlineColor	下划线颜色
		//
		void drawWithStrokeAndUnderline(E_FONT_TYPE type, 
			                            const char* text, 
										unsigned int size, 
										unsigned int color,
										unsigned int left, 
										unsigned int top, 
										unsigned int strokeColor, 
										unsigned int underlineColor);

	private:

		YCFontImpl* myFontImpl;

	};
	
	//
	// 函数：createFont()
	//
	// 目的：获取文本绘制接口
	//
	YCFont* createFont();

#pragma endregion YCFont

private:

	//
	// 函数：shouldRender()
	//
	// 目的：定帧数检测
	//
	bool shouldRender();

	//
	// 函数：validate()
	//
	// 目的：验证DirectX环境
	//
	bool validate();

	//
	// 函数：initializeDevice()
	//
	// 目的：初始化D3DDevice设备
	//
	bool initializeDevice();

	//
	// 函数：onLostDevice()
	//
	// 目的：丢失设备处理
	//
	void onLostDevice();

	//
	// 函数：onResetDevice()
	//
	// 目的：恢复设备处理
	//
	void onResetDevice();
	
private:

	HWND myWnd;

	// 当前帧时间戳
	time_t myTimestamp;

	// 帧速控制器
	unsigned int myNextDraw;

	// 窗口是否可见
	bool myVisible;

	// 当前系统场景
	YCScene *myScene; 

	// Query工厂
	YCQueryFactory *myQueryFactory;

	// 场景管理器，持有生命周期
	YCSceneManager *mySceneManager;

	// 唯一SpriteImpl的实例
	YCD3DSpriteImpl *mySpriteImpl;

	// 唯一YCFont的实例
	YCFont * myFont;
	
	// 界面管理器，持有生命周期
	YCUIManager *myUIManager;

	// 纹理管理器
	YCTextureManager *myTextureManager;

	// 动画管理器
	YCAnimationManager *myAnimationManager;

	// 网络资源下载器
	YCNetworkFileLoader *myNetworkLoader;
};

#endif
