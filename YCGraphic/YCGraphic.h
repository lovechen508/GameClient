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
 *  DirectX ��װ��
 *
 *  ��ȷ�������У�
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
	// ������initialize(HWND hwnd, YCInput& input, YCConfig& config)
	//
	// Ŀ�ģ���ʼ��DirectX����
	//
	bool initialize(HWND hwnd, YCInput& input, YCConfig& config);  

	//
	// ������registerMapFactory(YCIMapFactory* mapFactory)
	//
	// Ŀ�ģ�ע���ͼ����
	//
	void registerMapFactory(YCIMapFactory* mapFactory);

	//
	// ������getUIManager()
	//
	// Ŀ�ģ�ȡ�ý��������
	//
	YCUIManager* getUIManager();

	//
	// ������getScene()
	//
	// Ŀ�ģ�ȡ�ó���������
	//
	YCScene* getScene();

	//
	// ������getWindowSize()
	//
	// Ŀ�ģ���ȡ���ڴ�С
	//
	SIZE getWindowSize();

	//
	// ������setWindowSize()
	//
	// Ŀ�ģ����ô��ڴ�С
	//
	void setWindowSize(SIZE& size);

	//
	// ������setFPS(int fps)
	//
	// Ŀ�ģ�����֡Ƶ
	//
	void setFPS(int fps);

	//
	// ������getFPS()
	//
	// Ŀ�ģ���ȡ֡Ƶ
	//
	int getFPS();

	//
	// ������setVisible(bool visible)
	//
	// Ŀ�ģ����ô��ڿɼ���
	//
	void setVisible(bool visible);

	//
	// ������updateScene(YCScene* newScene)
	//
	// Ŀ�ģ����ó���:
	//
	// ע�ͣ����ؾɳ���ָ�룬�״�ΪNULL
	//
	YCScene* updateScene(YCScene* newScene);

	//
	// ������render()
	//
	// Ŀ�ģ���Ⱦ
	//
	void render();

	//
	// ������snapshot()
	//
	// Ŀ�ģ��������
	//
	void snapshot();

	//
	// ������GetMaxTextureBlendStages()
	//
	// Ŀ�ģ���ǰ�Կ�֧�ֵĶ����������
	//
	unsigned int getMaxTextureBlendStages();

	//
	// ������getPixShaderVersion()
	//
	// Ŀ�ģ���ȡ�Կ�֧�ֵ�PixShader�汾
	//
	E_PIXSHADER_VERSION getPixShaderVersion();

	//
	// ������finalize()
	//
	// Ŀ�ģ��ͷ�DirectX��Դ
	//
	void finalize();

	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

#pragma region YCD3DSprite

	/*
	 * YCD3DSprite : 2D���������
	 *          ֻ�ܴ�YCGraphic�ӿڴ���
	 */ 
	class YCD3DSprite
	{
	public:

		YCD3DSprite() {}

		virtual ~YCD3DSprite() {}

		//
		// ������timestamp()
		//
		// Ŀ�ģ���ǰ֡ʱ���
		// 
		virtual time_t timestamp() = 0;
				
		//
		// ������begin()
		//
		// Ŀ�ģ���draw֮ǰ����
		//
		virtual YCD3DSprite* begin(bool render) = 0;

		//
		// ������setViewport()
		//
		// Ŀ�ģ����òü�����
		//
		virtual IntRect setViewport(const IntRect& viewport) = 0;

		//
		// ������narrowViewport(const IntRect& viewport)
		//
		// Ŀ�ģ������ü�����
		//
		virtual IntRect narrowViewport(const IntRect& viewport) = 0;

		//
		// ������recoverViewport(const IntRect& viewport)
		//
		// Ŀ�ģ��ָ��ü�����
		//
		virtual void recoverViewport(const IntRect& viewport) = 0;

		//
		// ������draw(YCITexture* texture, float left, float top, float repeatX, float repeatY, float alpha, unsigned int color)
		//
		// Ŀ�ģ����һ��Lock���㻺��Ļ�������
		//
		virtual YCD3DSprite* draw(YCITexture* texture, float left, float top, float repeatX, float repeatY, float alpha, unsigned int color) = 0;
		
		//
		// ������end()
		//
		// Ŀ�ģ�Ŀ�ģ���draw��������
		//
		virtual void end() = 0;
	};

	//
	// ������createSpriteDrawer()
	//
	// Ŀ�ģ���ȡ������ƽӿ�
	//
	YCD3DSprite* createD3DSprite();

#pragma endregion YCD3DSprite

#pragma region YCD3DPrimitive

	/*
	 * YCD3DPrimitive : ���㣬�ߣ����Σ����εȣ�������
	 */
	class YCD3DPrimitive
	{
	public:

		YCD3DPrimitive() {}

		virtual ~YCD3DPrimitive() {}
	
		//
		// ���� drawPoint(unsigned int x, unsigned int y, unsigned int color);
		//
		// Ŀ�ģ�����
		//
		virtual YCD3DSprite* drawPoint(unsigned int x, unsigned int y, unsigned int color) = 0;

		//
		// ���� drawLine(unsigned int xSrc, unsigned int ySrc, unsigned int xDst, unsigned int yDst, unsigned int colorV0, unsigned int colorV1);
		//
		// Ŀ�ģ�������
		//
		virtual YCD3DSprite* drawLine(unsigned int xSrc, unsigned int ySrc,
				          	          unsigned int xDst, unsigned int yDst,
							          unsigned int colorV0, unsigned int colorV1) = 0;

		// 
		// ������drawRetangle(unsigned int left,    unsigned int top,    unsigned int right,   unsigned int bottom,
		//	                 unsigned int colorV0, unsigned int colorV1, unsigned int colorV2, unsigned int colorV3)
		//
		// ������
		//
		// Ŀ�ģ����ı���
		//
		virtual YCD3DSprite* drawRetangle(unsigned int left,    unsigned int top,     unsigned int right,   unsigned int bottom,
			                              unsigned int colorV0, unsigned int colorV1, unsigned int colorV2, unsigned int colorV3) = 0;
	};

	//
	// ������createPrimitive()
	//
	// Ŀ�ģ���ȡ���������ƽӿ�
	//
	YCD3DPrimitive* createPrimitive();

#pragma endregion YCD3DPrimitive

#pragma region YCFont
	
	/*
	 * YCFont : ���������
	 *          ֻ�ܴ�YCGraphic�ӿڴ���
	 */ 

	class YCAPI YCFont
	{
	public:

		//
		// ������YCFont()
		//
		// Ŀ�ģ����캯��
		//
		YCFont(YCD3DSprite * sprite);

		//
		// ������~YCFont()
		//
		// Ŀ�ģ����캯��
		//
		~YCFont();

        //
        // ������setAutoScaled(bool autoScaled)
        //
        // Ŀ�ģ������Զ�����
        //
        void setAutoScaled(bool autoScaled);

        //
        // ������isAutoScaled()
        //
        // Ŀ�ģ��Ƿ��Զ�����
        //
        bool isAutoScaled() const;

		//
		// ������getFontWidthAndHeight(E_FONT_TYPE type, E_FONT_DIRECTION direction, unsigned int size, const char* strInfo, int& width, int& height)
		//
		// Ŀ�ģ�ȡ���ض����弰��С����ߺͿ�
		//
		bool getFontWidthAndHeight(E_FONT_TYPE type, E_FONT_DIRECTION direction, unsigned int size, const char* strInfo, int& width, int& height);

		//
		// ������draw(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color, unsigned int left, unsigned int top)
		//
		// Ŀ�ģ������Ļ����ϵ�����ı�
		//
		// ������left    ������
		//       top     ������
		//       size    �ַ���С
		//       color   �ı���ɫ 0xff000000
		//       text    �ַ���
		//
		void draw(E_FONT_TYPE type, 
			      const char* text,
				  unsigned int size, 
				  unsigned int color,
			      unsigned int left,
				  unsigned int top);

		//
		// ������drawWithStroke(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color,
		//	      unsigned int left, unsigned int top, unsigned int strokeColor
		//
		// Ŀ�ģ������Ļ����ϵ�����ı�
		//
		// ������left			������
		//       top			������
		//       size			�ַ���С
		//       color			�ı���ɫ 0xff000000
		//       text			�ַ���
		//       strokeColor	���ɫ
		//
		void drawWithStroke(E_FONT_TYPE type, 
			                const char* text, 
							unsigned int size, 
							unsigned int color,
							unsigned int left, 
							unsigned int top, 
							unsigned int strokeColor);

		//
		// ������drawWithUnderline(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color,
		//	      unsigned int left, unsigned int top, unsigned int underlineColor
		//
		// Ŀ�ģ������Ļ����ϵ�����ı�
		//
		// ������left			������
		//       top			������
		//       size			�ַ���С
		//       color			�ı���ɫ 0xff000000
		//       text			�ַ���
		//       underlineColor	�»�����ɫ
		//
		void drawWithUnderline(E_FONT_TYPE type, 
			                   const char* text, 
							   unsigned int size, 
							   unsigned int color,
							   unsigned int left, 
							   unsigned int top, 
							   unsigned int underlineColor);

		//
		// ������drawWithStrokeAndUnderline(E_FONT_TYPE type, const char* text, unsigned int color,
		//	      unsigned int left, unsigned int top, unsigned int strokeColor, unsigned int underlineColor
		//
		// Ŀ�ģ������Ļ����ϵ�����ı�
		//
		// ������left			������
		//       top			������
		//       size			�ַ���С
		//       color			�ı���ɫ 0xff000000
		//       text			�ַ���
		//       strokeColor	���ɫ
		//       underlineColor	�»�����ɫ
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
	// ������createFont()
	//
	// Ŀ�ģ���ȡ�ı����ƽӿ�
	//
	YCFont* createFont();

#pragma endregion YCFont

private:

	//
	// ������shouldRender()
	//
	// Ŀ�ģ���֡�����
	//
	bool shouldRender();

	//
	// ������validate()
	//
	// Ŀ�ģ���֤DirectX����
	//
	bool validate();

	//
	// ������initializeDevice()
	//
	// Ŀ�ģ���ʼ��D3DDevice�豸
	//
	bool initializeDevice();

	//
	// ������onLostDevice()
	//
	// Ŀ�ģ���ʧ�豸����
	//
	void onLostDevice();

	//
	// ������onResetDevice()
	//
	// Ŀ�ģ��ָ��豸����
	//
	void onResetDevice();
	
private:

	HWND myWnd;

	// ��ǰ֡ʱ���
	time_t myTimestamp;

	// ֡�ٿ�����
	unsigned int myNextDraw;

	// �����Ƿ�ɼ�
	bool myVisible;

	// ��ǰϵͳ����
	YCScene *myScene; 

	// Query����
	YCQueryFactory *myQueryFactory;

	// ������������������������
	YCSceneManager *mySceneManager;

	// ΨһSpriteImpl��ʵ��
	YCD3DSpriteImpl *mySpriteImpl;

	// ΨһYCFont��ʵ��
	YCFont * myFont;
	
	// �����������������������
	YCUIManager *myUIManager;

	// ���������
	YCTextureManager *myTextureManager;

	// ����������
	YCAnimationManager *myAnimationManager;

	// ������Դ������
	YCNetworkFileLoader *myNetworkLoader;
};

#endif
