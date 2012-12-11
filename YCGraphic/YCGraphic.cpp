#include "YCGraphic.h"

#include <map>
#include <vector>

#include "d3d9.h"
#include "d3dx9core.h"
#include "d3dx9math.h"
#include "d3dx9shader.h"
#include "DxErr.h"

#include "resource.h"

#include "YCScene.h"
#include "YCTexture.h"
#include "YCITexture.h"
#include "YCIComponent.h"
#include "YCTextureDef.h"
#include "YCUIManager.h"
#include "YCQueryFactory.h"
#include "YCSceneManager.h"
#include "YCTextureManager.h"
#include "YCAnimationManager.h"
#include "YCNetworkFileLoader.h"

#include "YCIFontSprite.h"
#include "YCFontSpriteFactory.h"

#include "YCLua\YCLua.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCSingleton.h"
#include "YCBasic\YCScopeGuard.h"
#include "YCBasic\YCException.h"
#include "YCInput\YCInput.h"
#include "YCConfig\YCConfig.h"
#include "YCNetwork\YCMSGDefine.h"
#include "YCBasic\YCTimePerformance.h"

YCITexture *texture1 = NULL;
YCITexture *texture2 = NULL;
YCITexture *texture3 = NULL;

/////////////////////////////////////////////////////////////////////
typedef struct _YCVERTEX{
	float x, y, z, rhw;     // 
	DWORD color;          //
	float u, v;             // 纹理坐标
} YCVERTEX;

#define YUCHEN_VERTEX_FVF (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

LPDIRECT3D9 pD3D = NULL;					// the Direct3D object
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;		// the Direct3D device
D3DPRESENT_PARAMETERS d3dpp;
LPDIRECT3DPIXELSHADER9 g_pPS = NULL;  
LPD3DXCONSTANTTABLE g_pPSConstantTable = NULL; 

D3DCAPS9 d3dCaps;                           // the Direct3D Caps 
	
SIZE ScreenSize;							// 当前屏幕分辨率
SIZE WindowSize;							// 当前视窗大小

int fps;									//帧频
int perFpsTime;								//每帧时间   1000/fps

#define VertexCount  40000					//顶点数目4000个
#define ItemsCount VertexCount/2            //纹理队列长度

/////////////////////////////////////////////////////////////////////

//
// 函数：reportError(HRSULT hr)
//
// 目的：弹出MessageBox报告DX错误
//
void reportError(HRESULT hr)
{
	char strError[256] = {0};
	sprintf_s(strError, "错误值：%s \n错误描述：%s", ::DXGetErrorString(hr), ::DXGetErrorDescription(hr));
	::MessageBoxA(0, (LPSTR)strError, "DirectX错误", 0);
}

#pragma region PixShader

static bool initPixShader()
{
	LPD3DXBUFFER pShader = NULL;  
	LPD3DXBUFFER pErrorMsgs = NULL; 

	char execPath[MAX_PATH] = {0};
	if (!YCFileUtil::GetExePath(execPath, MAX_PATH))
	{
		return false;
	}
	char shaderPath[MAX_PATH] = {0};
	sprintf_s(shaderPath, "%s\\..\\conf\\shader\\ps_repeattex.txt", execPath);

	HRESULT hr = D3DXCompileShaderFromFile(TEXT(shaderPath),
					NULL, NULL, "Main", "ps_3_0", 
#ifndef NDEBUG
					D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION, 
#else
					NULL,
#endif
					&pShader, &pErrorMsgs, &g_pPSConstantTable);
	if (hr != D3D_OK)
	{
		if (pErrorMsgs != NULL)
		{
			::MessageBoxA(NULL, (LPSTR)pErrorMsgs->GetBufferPointer(), "DirectX错误", MB_OK);  
			SAFE_RELEASE(pErrorMsgs);
		}
		else 
		{
			reportError(hr);
		}
		return false;
	}

	if(FAILED(g_pd3dDevice->CreatePixelShader((DWORD*)pShader->GetBufferPointer(), &g_pPS)))  
	{
		return false;  
	}

	D3DXHANDLE d3dXHWidth = g_pPSConstantTable->GetConstantByName(0, "width");

	SAFE_RELEASE(pShader);  
	SAFE_RELEASE(pErrorMsgs);

	return true;
}

#pragma endregion PixShader

#pragma region YCD3DSpriteImpl

class YCD3DSpriteImpl
	: public YCGraphic::YCD3DSprite
	, public YCGraphic::YCD3DPrimitive
{
	typedef enum {POINT = 1, LINE, RETANGLE, TEXTURE} E_ITEM_T;
	
	struct YCDrawItem_T 
	{
		E_ITEM_T type;
		struct
		{
			bool repeat;
			float Umin;
			float Umax;
			float Vmin;
			float Vmax;
			void* raw;
			unsigned int diffuse;
		} texture;
	
		D3DVIEWPORT9 viewport;
	};

public:

	YCD3DSpriteImpl()
		: lpVertexBuffer(NULL)
	{
		SMART_ASSERT(g_pd3dDevice != NULL);

		//创建位图矩形的顶点缓冲区
		if (FAILED(g_pd3dDevice->CreateVertexBuffer(sizeof(YCVERTEX) * VertexCount,
												  D3DUSAGE_WRITEONLY,
												  YUCHEN_VERTEX_FVF,
												  D3DPOOL_MANAGED,
												  &lpVertexBuffer,
												  NULL)))
		{
			throw YCException(2002, "CreateDirectX 创建顶点缓冲区失败, DirectX错误");
		}

		g_pd3dDevice->SetStreamSource (0, lpVertexBuffer, 0, sizeof(YCVERTEX ));
		LOG_DEBUG("YCD3DSpriteImpl::YCD3DSpriteImpl创建顶点缓冲区成功!");
	}

	~YCD3DSpriteImpl()
	{
		g_pd3dDevice->SetStreamSource(0, NULL, 0, 0);	
		SAFE_RELEASE(lpVertexBuffer);		
		LOG_DEBUG("YCD3DSpriteImpl::~YCD3DSpriteImpl释放顶点缓冲成功！");
	}

public:
	
	//
	// 函数：onLostDevice()
	// 
	// 目的：当图形系统丢失设备时由YCScene调用
	//
	void onLostDevice()
	{

	}

	//
	// 函数：onResetDevice()
	// 
	// 目的：当图形系统重置设备时由YCScene调用
	//
	void onResetDevice()
	{
		SAFE_RELEASE(lpVertexBuffer);		
		g_pd3dDevice->SetStreamSource(0, NULL, 0, 0);

		//创建位图矩形的顶点缓冲区
		if (FAILED(g_pd3dDevice->CreateVertexBuffer(sizeof(YCVERTEX) * VertexCount,
												  D3DUSAGE_WRITEONLY,
												  YUCHEN_VERTEX_FVF,
												  D3DPOOL_MANAGED,
												  &lpVertexBuffer,
												  NULL)))
		{
			throw YCException(2002, "CreateDirectX 创建顶点缓冲区失败, DirectX错误");
		}

		g_pd3dDevice->SetStreamSource (0, lpVertexBuffer, 0, sizeof(YCVERTEX ));
	}

	//
	// 函数：timestamp()
	//
	// 目的：当前帧时间戳
	// 
	time_t timestamp()
	{
		return myTimestamp;
	}

	//
	// 函数：begin()
	//
	// 目的：在draw之前调用
	//tiaojian
	virtual YCD3DSprite* begin(bool render)
	{
		memset(myTextureItems, 0, ItemsCount*sizeof(YCDrawItem_T));
		myItemIndex = 0;
		myRealRender = render;
		myTextureIndex = 0;

		resetViewport();

		myTimestamp = ::GetTickCount();

		if (myRealRender)
		{
			lpVertexBuffer->Lock(0, sizeof(YCVERTEX),(void**)&pVertex, 0);
		}
		return this;
	}

	//
	// 函数：setViewport(const IntRect& viewport)
	//
	// 目的：设置裁剪区域
	//
	virtual IntRect setViewport(const IntRect& viewport)
	{				   
		IntRect old;
		
		old.left = myViewport.X;
		old.top = myViewport.Y;
		old.right = myViewport.X + myViewport.Width;
		old.bottom = myViewport.Y + myViewport.Height;

		//创建ViewPort
		myViewport.X = viewport.left;
		myViewport.Y = viewport.top;
		myViewport.Width = viewport.width();
		myViewport.Height = viewport.height();
		myViewport.MinZ = 0.0f;
		myViewport.MaxZ = 1.0f;

		return old;
	}

	//
	// 函数：narrowViewport(const IntRect& viewport)
	//
	// 目的：缩减裁剪区域
	//
	virtual IntRect narrowViewport(const IntRect& viewport)
	{
		IntRect old;
		
		old.left = myViewport.X;
		old.top = myViewport.Y;
		old.right = myViewport.X + myViewport.Width;
		old.bottom = myViewport.Y + myViewport.Height;

		//创建ViewPort
		myViewport.X = max(myViewport.X, viewport.left);
		myViewport.Y = max(myViewport.Y, viewport.top);
		unsigned int right = min(viewport.right, old.right);
		unsigned int bottom = min(viewport.bottom, old.bottom);

		if ((right <= myViewport.X) || (bottom <= myViewport.Y))
		{
			myViewport.Width = 0;
			myViewport.Height = 0;
		}
		else 
		{
			myViewport.Width = right-myViewport.X;
			myViewport.Height = bottom-myViewport.Y;
		}

		myViewport.MinZ = 0.0f;
		myViewport.MaxZ = 1.0f;

		return old;
	}

	//
	// 函数：recoverViewport(const IntRect& viewport)
	//
	// 目的：恢复裁剪区域
	//
	virtual void recoverViewport(const IntRect& viewport)
	{
		//创建ViewPort
		myViewport.X = viewport.left;
		myViewport.Y = viewport.top;
		myViewport.Width = viewport.width();
		myViewport.Height = viewport.height();
		myViewport.MinZ = 0.0f;
		myViewport.MaxZ = 1.0f;
	}

#define CHECK_REAL_RENDER if (!myRealRender) return this

	//
	// 函数：draw(YCITexture* texture, float left, float top, float repeatX, float repeatY, float alpha, unsigned int color)
	//
	// 目的：添加一次Lock顶点缓冲的绘制纹理
	//
	virtual YCD3DSprite* draw(YCITexture* texture, float left, float top, float repeatX, float repeatY, float alpha, unsigned int color)
	{
		CHECK_REAL_RENDER;

		SMART_ASSERT(texture != NULL);
		if (texture == NULL)
		{
			throw YCException(2002, "YCD3DSprite* addTexture绘制纹理值为NULL!");
		}

		validate(4);

		memset(retangleVertex, 0, sizeof(retangleVertex));
		myTextureItems[myItemIndex].type = TEXTURE;
		myTextureItems[myItemIndex].texture.raw = texture->raw();
		myTextureItems[myItemIndex].viewport = myViewport;

		const FloatRect* rect = texture->rect();
		float width = rect->right - rect->left;
		float height = rect->bottom - rect->top;

		retangleVertex[0].x = left-0.5f;
		retangleVertex[0].y = top-0.5f;
		retangleVertex[0].z = 1.0f;
		retangleVertex[0].rhw = 1.0f;

		retangleVertex[1].x = left + width*repeatX-0.5f;
		retangleVertex[1].y = top-0.5f;
		retangleVertex[1].z = 1.0f;
		retangleVertex[1].rhw = 1.0f;

		retangleVertex[2].x = left-0.5f;
		retangleVertex[2].y = top + height*repeatY-0.5f;
		retangleVertex[2].z = 1.0f;
		retangleVertex[2].rhw = 1.0f;

		retangleVertex[3].x = left + width*repeatX-0.5f;
		retangleVertex[3].y = top + height*repeatY-0.5f;
		retangleVertex[3].z = 1.0f;
		retangleVertex[3].rhw = 1.0f;

		texture->fetchUV(retangleVertex[0].u, retangleVertex[0].v,
			             retangleVertex[1].u, retangleVertex[1].v,
						 retangleVertex[2].u, retangleVertex[2].v,
						 retangleVertex[3].u, retangleVertex[3].v);

		myTextureItems[myItemIndex].texture.diffuse = color;

		if (color == 0)
		{
			retangleVertex[3].color = retangleVertex[2].color 
									= retangleVertex[1].color
									= retangleVertex[0].color
									= (DWORD)255*alpha;
		}
		else
		{
			retangleVertex[3].color = retangleVertex[2].color 
									= retangleVertex[1].color
									= retangleVertex[0].color
									= color;
		}

		if ( (repeatX-1.0f) <= EPSINON && (repeatX-1.0f) >= -EPSINON
				&& (repeatY-1.0f) <= EPSINON && (repeatY-1.0f) >= -EPSINON )
		{
			myTextureItems[myItemIndex].texture.repeat = false;
		}
		else
		{
			myTextureItems[myItemIndex].texture.repeat = true;
			myTextureItems[myItemIndex].texture.Umin = retangleVertex[0].u;
			myTextureItems[myItemIndex].texture.Umax = retangleVertex[1].u;
			myTextureItems[myItemIndex].texture.Vmin = retangleVertex[0].v;
			myTextureItems[myItemIndex].texture.Vmax = retangleVertex[2].v;

			float Umax = repeatX*(retangleVertex[1].u - retangleVertex[0].u);
			float Vmax = repeatY*(retangleVertex[2].v - retangleVertex[0].v);
			retangleVertex[1].u = Umax;
			retangleVertex[3].u = Umax;
			retangleVertex[0].u = 0;
			retangleVertex[2].u = 0;

			retangleVertex[2].v = Vmax;
			retangleVertex[3].v = Vmax;
			retangleVertex[0].v = 0;
			retangleVertex[1].v = 0;
		}
				
		memcpy(pVertex+myTextureIndex*sizeof(YCVERTEX), &retangleVertex, 4*sizeof(YCVERTEX));
		
		myItemIndex += 1;
		myTextureIndex += 4;
		return this;
	}
	
	//
	// 函数 drawPoint(unsigned int x, unsigned int y, unsigned int color);
	//
	// 目的：画点
	//
	virtual YCD3DSprite* drawPoint(unsigned int x, unsigned int y, unsigned int color)
	{
		CHECK_REAL_RENDER;

		validate(1);

		myTextureItems[myItemIndex].type = POINT;
		myTextureItems[myItemIndex].viewport = myViewport;

		memset(retangleVertex, 0, sizeof(retangleVertex));
		retangleVertex[0].x = (float)x;
		retangleVertex[0].y = (float)y;
		retangleVertex[0].color = color;

		memcpy(pVertex+myTextureIndex*sizeof(YCVERTEX), &retangleVertex, sizeof(YCVERTEX));

		myItemIndex += 1; 
		myTextureIndex += 1;	

		return this;
	}

	//
	// 函数 drawLine(unsigned int xSrc, unsigned int ySrc, unsigned int xDst, unsigned int yDst, unsigned int color);
	//
	// 目的：画线条
	//
	virtual YCD3DSprite* drawLine(unsigned int xSrc, unsigned int ySrc, unsigned int xDst, unsigned int yDst, 
		          unsigned int colorV0, unsigned int colorV1)
	{
		CHECK_REAL_RENDER;

		if (xSrc == xDst && ySrc == yDst)
		{
			drawPoint(xSrc, ySrc, colorV0);
		}
		else
		{
			myTextureItems[myItemIndex].type = LINE;
			myTextureItems[myItemIndex].viewport = myViewport;

			memset(retangleVertex, 0, sizeof(retangleVertex));
			retangleVertex[0].x = (float)xSrc;
			retangleVertex[0].y = (float)ySrc;
			retangleVertex[0].color = colorV0;
			retangleVertex[1].x = (float)xDst;
			retangleVertex[1].y = (float)yDst;
			retangleVertex[1].color = colorV1;

			memcpy(pVertex+myTextureIndex*sizeof(YCVERTEX), &retangleVertex, 2*sizeof(YCVERTEX));
			
			myItemIndex += 1; 
			myTextureIndex += 2;
		}

		return this;
	}

	// 
	// 函数：drawRetangle(unsigned int left,    unsigned int top,    unsigned int right,   unsigned int bottom,
	//	                 unsigned int colorV0, unsigned int colorV1, unsigned int colorV2, unsigned int colorV3)
	//
	// 参数：
	//
	// 目的：画四边形
	//
	virtual YCD3DSprite* drawRetangle(unsigned int left,    unsigned int top,     unsigned int right,   unsigned int bottom,
		              unsigned int colorV0, unsigned int colorV1, unsigned int colorV2, unsigned int colorV3)
	{	
		CHECK_REAL_RENDER;

		if ( (right-left == 0) || (bottom-top == 0) )
		{
			return this;
		}
				
		myTextureItems[myItemIndex].type = RETANGLE;
		myTextureItems[myItemIndex].viewport = myViewport;

		memset(retangleVertex, 0, sizeof(retangleVertex));
		retangleVertex[0].x = (float)left;
		retangleVertex[0].y = (float)top;
		retangleVertex[0].color = colorV0;

		retangleVertex[1].x = (float)right;
		retangleVertex[1].y = (float)top;
		retangleVertex[1].color = colorV1;

		retangleVertex[2].x = (float)left;
		retangleVertex[2].y = (float)bottom;
		retangleVertex[2].color = colorV2;

		retangleVertex[3].x = (float)right;
		retangleVertex[3].y = (float)bottom;
		retangleVertex[3].color = colorV3;

		memcpy(pVertex+myTextureIndex*sizeof(YCVERTEX), &retangleVertex, 4*sizeof(YCVERTEX));

		myItemIndex += 1; 
		myTextureIndex += 4;

		return this;
	}
		
	//
	// 函数：end()
	//
	// 目的：目的：在draw结束调用完成真正的纹理绘制
	//
	// 注释：
	//   V0 (0,0)  --------> U V1 (0,1)
	//            |
	//            |
	//            |   
	//   V2 (0,1) v V          V3 (1,1)
	//
	//  D3DPT_TRIANGLESTRIP ： 
	//         Triangle 1 : (V0, V1, V2)
	//         Triangle 2 : (V2, V1, V3)
	//
	virtual void end()
	{
		if (!myRealRender) return;

		lpVertexBuffer->Unlock(); 
						
		int index = 0;
		for (int i = 0; i < ItemsCount; ++i)
		{
			const YCDrawItem_T& item = myTextureItems[i];
			if (item.type == 0)
			{
				break; //结束
			}

			if      (item.type == POINT)   
			{
				drawPointImpl(index, item);
				index += 1;
			}
			else if (item.type == LINE)   
			{
				drawLineImpl(index, item);
				index += 2;
			}
			else if (item.type == RETANGLE) 
			{
				drawRetangleImpl(index, item);
				index += 4;
			}
			else if (item.type == TEXTURE)
			{
				drawTextureImpl(index, item);
				index += 4;
			}
			else
			{
				LOG_DEBUG("YCD3DSprite::end绘制未知类型:" << item.type);
			}
		}

		//resetViewport();
	}

	//
	// 函数: reset()
	//
	// 目的：重置渲染位置，旋转，平移，缩放矩阵等
	//
	void resetViewport()
	{		
		myViewport.X = 0; 
		myViewport.Y = 0; 
		myViewport.Width = WindowSize.cx;
		myViewport.Height = WindowSize.cy;
		myViewport.MinZ = 0.0f;
		myViewport.MaxZ = 1.0f;

		g_pd3dDevice->SetViewport(&myViewport);
	}
	
private:

	//
	// 函数：validate(int size)
	//
	// 目的：
	//
	void validate(int size)
	{
		if ((myItemIndex == ItemsCount) || (myTextureIndex > (VertexCount-size)))
		{
			throw YCException(2002, "YCSpriteImpl::validate定点缓冲用尽");
			/*
			end();	// 画

			memset(myTextureItems, 0, ItemsCount*sizeof(YCDrawItem_T));
			myItemIndex = 0;
			myTextureIndex = 0;

			lpVertexBuffer->Lock(0, sizeof(YCVERTEX),(void**)&pVertex, 0);
			*/
		}
	}

	//
	// 函数：drawPointImpl(int index, const YCDrawItem_T& item)
	//
	// 目的：画点
	//
	void drawPointImpl(int index, const YCDrawItem_T& item)
	{
		g_pd3dDevice->SetViewport(&item.viewport);
		g_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, index, 1 );
	}

	//
	// 函数：drawLineImpl(int idx)
	//
	// 目的：画线
	//
	void drawLineImpl(int index, const YCDrawItem_T& item)
	{
		g_pd3dDevice->SetViewport(&item.viewport);
		g_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, index, 1 );
	}

	//
	// 函数：drawRetangleImpl(int index, const YCDrawItem_T& item)
	//
	// 目的：画矩形
	//
	void drawRetangleImpl(int index, const YCDrawItem_T& item)
	{
		g_pd3dDevice->SetViewport(&item.viewport);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, index, 2 );
	}

	//
	// 函数：drawTextureImpl(int index, const YCDrawItem_T& item)
	//
	// 目的：负责绘制所有图片纹理
	//
	void drawTextureImpl(int index, const YCDrawItem_T& item)
	{
		if (item.texture.repeat)
		{
			//准备全局数据
			D3DXHANDLE d3dXHUmin = g_pPSConstantTable->GetConstantByName(0, "Umin");
			D3DXHANDLE d3dXHUmax = g_pPSConstantTable->GetConstantByName(0, "Umax");
			D3DXHANDLE d3dXHVmin = g_pPSConstantTable->GetConstantByName(0, "Vmin");
			D3DXHANDLE d3dXHVmax = g_pPSConstantTable->GetConstantByName(0, "Vmax");
			if ( FAILED(g_pPSConstantTable->SetFloat(g_pd3dDevice, d3dXHUmin, item.texture.Umin))
				|| FAILED(g_pPSConstantTable->SetFloat(g_pd3dDevice, d3dXHUmax, item.texture.Umax))
				|| FAILED(g_pPSConstantTable->SetFloat(g_pd3dDevice, d3dXHVmin, item.texture.Vmin))
				|| FAILED(g_pPSConstantTable->SetFloat(g_pd3dDevice, d3dXHVmax, item.texture.Vmax)))
			{
				return;
			}

			D3DXHANDLE textureHandle = g_pPSConstantTable->GetConstantByName(0, "MySampler");
			unsigned int count;
			D3DXCONSTANT_DESC textureDesc;
			HRESULT hr = g_pPSConstantTable->GetConstantDesc(textureHandle, &textureDesc, &count);
			if (hr != D3D_OK)
			{
				reportError(hr);
				return;
			}

			g_pPSConstantTable->SetDefaults(g_pd3dDevice);

			// 绑定Pixel Shader
			g_pd3dDevice->SetPixelShader(g_pPS);
		}

		if (item.texture.diffuse == 0)
		{
			// 颜色来源-材质
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			// 设置Alpha通道来源于diffuse通道
			//g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
			//g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		}
		else
		{
			// 颜色来源-diffuse
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			// 设置Alpha通道来源于纹理alpha通道
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		}
			
		g_pd3dDevice->SetTexture(0, (IDirect3DBaseTexture9*)item.texture.raw); 
		g_pd3dDevice->SetViewport(&item.viewport);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, index, 2);
		g_pd3dDevice->SetTexture(0, NULL);
		g_pd3dDevice->SetPixelShader(NULL);				
	}

private:
	
	D3DVIEWPORT9 myViewport;

	BYTE* pVertex; //顶点缓冲起点

	time_t myTimestamp; //每帧的时间戳

	bool myRealRender;  //本帧是否真的绘制

	int myItemIndex;
	int myTextureIndex;

	struct YCDrawItem_T myTextureItems[ItemsCount];

	YCVERTEX retangleVertex[4];		            //位图矩形的顶点 4个
	
	LPDIRECT3DVERTEXBUFFER9  lpVertexBuffer;	//位图矩形的顶点缓存区接口指针
};

#pragma endregion YCD3DSpriteImpl

void drawFPSWithMousePosition(YCGraphic * graphic);
void drawText(YCGraphic * graphic);
void drawPic(YCGraphic * graphic);

YCGraphic::YCGraphic(void)
	: myScene(NULL)
	, myUIManager(NULL)
	, mySpriteImpl(NULL)
	, myTextureManager(NULL)
	, myAnimationManager(NULL)
	, myNetworkLoader(NULL)
	, myFont(NULL)
	, myVisible(true)
{
	//default value
	WindowSize.cx = 1024;
	WindowSize.cy = 768;

	fps = 60; 
	perFpsTime = 1000 / fps;
	myNextDraw = ::GetTickCount() + perFpsTime;
}

YCGraphic::~YCGraphic(void)
{

}

//
// 函数：initialize(HWND hwnd, YCInput& input, YCConfig& config)
//
// 目的：初始化DirectX环境
//
bool YCGraphic::initialize(HWND wndHandle, YCInput& input, YCConfig& config)
{
	myWnd = wndHandle;

	WindowSize.cx = config.width();
	WindowSize.cy = config.height();

	// Create the DirectX object
	if( NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)) )
	{
		return false;
	}

	// Get the current display mode
	D3DDISPLAYMODE d3ddm;
	if(FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return false;
	}

	// Fill the presentation parameters structure
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; //d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = WindowSize.cx;
	d3dpp.BackBufferHeight = WindowSize.cy;
	d3dpp.hDeviceWindow = wndHandle;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //D3DPRESENT_INTERVAL_IMMEDIATE;

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// Create a default DirectX device
	// 使用D3DDEVTYPE_REF创建设备会失败
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL, 
									wndHandle,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&g_pd3dDevice)))
	{
		::MessageBoxA(0, "CreateDirectX 创建设备失败", "DirectX错误", 0);
		return false;
	}

	LOG_DEBUG("YCGraphic::initialize创建设备成功");

	//注册设备
	YCRegistry::set("D3DDevice", g_pd3dDevice);

	//获取D3DCAPS9
	if (FAILED(g_pd3dDevice->GetDeviceCaps(&d3dCaps)))
	{
		::MessageBoxA(0, "CreateDirectX 获取设备能力失败", "DirectX错误", 0);
		return false;
	}

	LOG_DEBUG("YCGraphic::initialize获得PixShaderVersion版本 : " << getPixShaderVersion());

	//注册Pix Shader
	if (!initPixShader())
	{
		return false;
	}

	YCRegistry::set("width", new int(WindowSize.cx));
	YCRegistry::set("height", new int(WindowSize.cy));
	
	//初始化D3DDevice设备对象
	if (!initializeDevice())
	{
		::MessageBoxA(0, "初始化D3D设备失败！", "DirectX错误", 0);
		return false;
	}
	
	// 创建纹理精灵对象
	mySpriteImpl = new YCD3DSpriteImpl();

	// 创建字符绘制对象
	myFont = new YCFont(mySpriteImpl);

	//初始化网络下载系统
	myNetworkLoader = new YCNetworkFileLoader();
	if (!myNetworkLoader->initialize(config))
	{
		::MessageBoxA(0, "初始化网络下载器失败", "Application错误", 0);
		return false;
	}

	LOG_DEBUG("YCGraphic::initialize初始化网络下载系统成功!");

	//初始化纹理管理器
	myTextureManager = new YCTextureManager();
	if (!myTextureManager->initialize(myNetworkLoader, config))
	{
		::MessageBoxA(0, "初始化纹理管理器失败", "Application错误", 0);
		return false;
	}
	YCRegistry::set("textureManager", myTextureManager);
	LOG_DEBUG("YCGraphic::initialize初始化纹理管理器成功!");

	myAnimationManager = new YCAnimationManager();
	if (!myAnimationManager->initialize(myNetworkLoader, config))
	{
		::MessageBoxA(0, "初始化动画管理器失败", "Application错误", 0);
		return false;
	}
	YCRegistry::set("animationManager", myAnimationManager);
	LOG_DEBUG("YCGraphic::initialize初始化动画管理器成功!");

	myUIManager = new YCUIManager();
	if (!myUIManager->initialize(myTextureManager, input, config))
	{
		::MessageBoxA(0, "初始化UI管理器失败", "Application错误", 0);
		return false;
	}
	YCRegistry::set("UIManager", myUIManager);
	LOG_DEBUG("YCGraphic::initialize初始化UI管理器成功!");

	mySceneManager = new YCSceneManager(this);
	LOG_DEBUG("YCGraphic::initialize初始化场景管理器成功！");

	myQueryFactory = new YCQueryFactory();
	YCLua *lua = (YCLua*)YCRegistry::get("lua");
	if (lua == NULL)
	{
		::MessageBoxA(0, "YCGraphic::initialize无法获取Lua句柄", "Application错误", 0);
		return false;
	}
	lua->bindQueryFactory(myQueryFactory);

	// 注册自身
	YCRegistry::set("graphic", this);

	// 设置YCIComponent全局变量
	YCIComponent::gSprite = mySpriteImpl;
	YCIComponent::gPrimitive = mySpriteImpl;
	YCIComponent::gFont = myFont;

	//myUIManager->registUI("avatar", "D:\\dropbox\\Dropbox\\Public\\Client\\data\\ui\\1\\avatar.xml");
	myUIManager->registUI("avatar", "1\\avatar.xml");

	//texture1 = myTextureManager->fetch(TEXTURE_PLAYER, "1\\1\\1\\2\\00.png");
	//texture2 = myTextureManager->fetch(TEXTURE_PLAYER, "1\\1\\1\\3\\00.png");
	//texture3 = myTextureManager->fetch(TEXTURE_PLAYER, "1\\1\\1\\6\\00.png");

	//定帧
	setFPS(config.fps());

	//注册处理YCInput键盘，鼠标消息
	input.subscribeListener(this, WM_MOUSEMOVE);
	input.subscribeListener(this, WM_LBUTTONDOWN);
	input.subscribeListener(this, WM_LBUTTONUP);
	input.subscribeListener(this, WM_LBUTTONDBLCLK);
	input.subscribeListener(this, WM_RBUTTONDOWN);
	input.subscribeListener(this, WM_RBUTTONUP);
	input.subscribeListener(this, WM_MOUSEWHEEL);

	input.subscribeListener(this, WM_KEYDOWN);
	input.subscribeListener(this, WM_KEYUP);
	input.subscribeListener(this, WM_CHAR);
	
	return true;
}

//
// 函数：registerMapFactory(YCIMapFactory* mapFactory)
//
// 目的：注册地图工厂
//
void YCGraphic::registerMapFactory(YCIMapFactory* mapFactory)
{
	YCSceneManager::GetSingleton().registerMapFactory(mapFactory);	

	// TEST CODE
	//YCIPackageResponse response;	
	//updateScene(YCSceneManager::GetSingleton().requireScene(&response));
}

//
// 函数：getUIManager()
//
// 目的：取得界面管理器
//
YCUIManager* YCGraphic::getUIManager()
{
	SMART_ASSERT(myUIManager != NULL);
	return myUIManager;
}

//
// 函数：getScene()
//
// 目的：取得场景管理器
//
YCScene* YCGraphic::getScene()
{
	SMART_ASSERT(myScene != NULL);
	return myScene;
}

//
// 函数：getWindowSize()
//
// 目的：获取窗口大小
//
SIZE YCGraphic::getWindowSize()
{
	return WindowSize;
}

//
// 函数：setWindowSize()
//
// 目的：设置窗口大小
//
void YCGraphic::setWindowSize(SIZE& size)
{
	WindowSize.cx = size.cx;
	WindowSize.cy = size.cy;
}

//
// 函数：setFPS(int fps)
//
// 目的：设置帧频
//
void YCGraphic::setFPS(int fps)
{
	if (fps > 0 && fps < 60)
	{
		fps = fps;
		perFpsTime = 1000 / fps;
	}
	myNextDraw = ::GetTickCount() + perFpsTime;
}

//
// 函数：getFPS()
//
// 目的：获取帧频
//
int YCGraphic::getFPS()
{
	return fps;
}

//
// 函数：setVisible(bool visible)
//
// 目的：设置窗口可见性
//
void YCGraphic::setVisible(bool visible)
{
	myVisible = visible;
}

//
// 函数：updateScene(YCScene* newScene)
//
// 目的：设置场景:
//
// 注释：返回旧场景指针，首次为NULL
//
YCScene* YCGraphic::updateScene(YCScene* newScene)
{
	YCScene* oldScene = myScene;
	myScene = newScene;
	YCRegistry::set("scene", myScene);
	return oldScene;
}

/*
 * 渲染每一帧
 */
void YCGraphic::render()
{
	myTimestamp = ::GetTickCount();

	// 检查设备是否丢失
	if (validate())
	{	
		bool realRender = shouldRender();

		if (realRender)
		{
			//将后备缓冲清除成黑色
			g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
				D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

			// 开始场景
			if(SUCCEEDED(g_pd3dDevice->BeginScene()))
			{
				mySpriteImpl->begin(realRender);
				// 渲染图元
				drawFPSWithMousePosition(this);
				if (myScene) { myScene->render();}
				//drawText(this);
				mySpriteImpl->end();
			}

			// 结束场景
			g_pd3dDevice->EndScene();
			
			// 显示场景
			HRESULT hr = g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
			if(hr == D3DERR_DEVICELOST)
			{
				if(g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
				{
					onLostDevice();
					onResetDevice();
				}
			}
		}
		else //跑逻辑
		{
			mySpriteImpl->begin(realRender);
			if (myScene) { myScene->render();}
			mySpriteImpl->end();
			::Sleep(10); //不应该睡眠！！！
		}
	}
}

//
// 函数：GetMaxTextureBlendStages()
//
// 目的：当前显卡支持的多重纹理层数
//
unsigned int YCGraphic::getMaxTextureBlendStages()
{
	return d3dCaps.MaxTextureBlendStages;
}

//
// 函数：snapshot()
//
// 目的：保存截屏
//
void YCGraphic::snapshot()
{

}

/*
 * 释放DirectX资源
 */
void YCGraphic::finalize()
{		
	//SAFE_DELETE(texture1);
	//SAFE_DELETE(texture2);
	//SAFE_DELETE(texture3);

	SAFE_DELETE(myQueryFactory);
	
	void *width = YCRegistry::remove("width");
	void *height = YCRegistry::remove("height");
	SAFE_DELETE(width);
	SAFE_DELETE(height);

	SAFE_RELEASE(g_pPS);
    SAFE_RELEASE(g_pPSConstantTable);

	SAFE_DELETE(mySpriteImpl);
	SAFE_DELETE(myFont);

	mySceneManager->releaseScene(myScene);
	myScene = NULL;
	LOG_DEBUG("YCGraphic::finalize退出场景成功！");

	SAFE_DELETE(mySceneManager);
	LOG_DEBUG("YCGraphic::finalize释放场景管理器成功！");

	if (myUIManager != NULL)
	{
		myUIManager->finalize();
		delete myUIManager;
	}
	LOG_DEBUG("YCGraphic::finalize释放UI管理器成功！");

	if (myAnimationManager != NULL)
	{
		myAnimationManager->finalize();
		delete myAnimationManager;
	}
	LOG_DEBUG("YCGraphic::finalize释放动画管理器成功！");

	if (myTextureManager != NULL)
	{
		myTextureManager->finalize();
		delete myTextureManager;
	}
	LOG_DEBUG("YCGraphic::finalize释放纹理管理器成功！");

	if (myNetworkLoader != NULL)
	{
		myNetworkLoader->finalize();
		delete myNetworkLoader;
	}
	LOG_DEBUG("YCGraphic::finalize释放网络下载系统成功！");

	SAFE_RELEASE(g_pd3dDevice);
	LOG_DEBUG("YCGraphic::finalize释放设备成功！");

	SAFE_RELEASE(pD3D);
	LOG_DEBUG("YCGraphic::finalize释放DirectX成功！");

	// 取消自身注册
	YCRegistry::remove("graphic");
}

//////////////////////////////////////////////////////
// 消息处理 功能函数
//////////////////////////////////////////////////////

//
// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：窗口消息处理函数
//
// 返回值：已处理，返回true，否则返回false
//
bool YCGraphic::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (myScene != NULL)
	{
		if (!myScene->onMessage(message, wParam, lParam))
		{
			if (myUIManager != NULL)
			{
				return myUIManager->onMessage(message, wParam, lParam);
			}
		}
	}
	return false; 
}

#pragma region YCFont

//////////////////////////////////////////////////////
// YCFont 字符绘制类
//////////////////////////////////////////////////////
class YCFontImpl 
{
public:
	YCFontImpl(YCGraphic::YCD3DSprite* sprite)
	{
		myFactory =new YCFontSpriteFactory(sprite);
	}

	~YCFontImpl()
	{
		for (std::map<E_FONT_TYPE, YCIFontSprite*>::iterator it = myFontSprites.begin();
			 it != myFontSprites.end();
			 ++it)
		{
			YCIFontSprite* fontSprite = (YCIFontSprite*)it->second;
			SAFE_DELETE(fontSprite);
		}

		myFontSprites.clear();

		SAFE_DELETE(myFactory);
	}

	YCIFontSprite* fetch(E_FONT_TYPE type)
	{
		YCIFontSprite* found = NULL;

		std::map<E_FONT_TYPE, YCIFontSprite*>::iterator at = myFontSprites.find(type);
		if (at == myFontSprites.end())
		{
			myFontSprites[type] = found = myFactory->createFontSprite(type);
		}
		else
		{
			found = at->second;
		}

		return found;
	}

	//
	// 函数：getFontWidthAndHeight(E_FONT_TYPE type, E_FONT_DIRECTION direction, unsigned int size, const char* strInfo, int& width, int& height)
	//
	// 目的：取得特定字体及大小下面高和宽
	//
	bool getFontWidthAndHeight(E_FONT_TYPE type, E_FONT_DIRECTION direction, unsigned int size, const char* strInfo, int& width, int& height)
	{
		YCIFontSprite* found = fetch(type);
		if (found != NULL)
		{
			return found->getWidthAndHeight(size, direction, strInfo, width, height);
		}

		return false;
	}

private:

	YCFontSpriteFactory* myFactory;
	
	std::map<E_FONT_TYPE, YCIFontSprite*> myFontSprites;
};

YCGraphic::YCFont::YCFont(YCGraphic::YCD3DSprite* sprite)
{
	myFontImpl = new YCFontImpl(sprite);
}

YCGraphic::YCFont::~YCFont()
{
	SAFE_DELETE(myFontImpl);
}

//
// 函数：setAutoScaled(bool autoScaled)
//
// 目的：设置自动缩放
//
void YCGraphic::YCFont::setAutoScaled(bool autoScaled)
{

}

//
// 函数：isAutoScaled()
//
// 目的：是否自动缩放
//
bool YCGraphic::YCFont::isAutoScaled() const
{
    return false;
}

//
// 函数：getFontWidthAndHeight(E_FONT_TYPE type, E_FONT_DIRECTION direction, unsigned int size, const char* strInfo, int& width, int& height)
//
// 目的：取得特定字体及大小下面高和宽
//
bool YCGraphic::YCFont::getFontWidthAndHeight(E_FONT_TYPE type, E_FONT_DIRECTION direction, unsigned int size, const char* strInfo, int& width, int& height)
{
	if (strInfo == NULL)
	{
		return false;
	}

	return myFontImpl->getFontWidthAndHeight(type, direction, size, strInfo, width, height);
}

//
// 函数：draw(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color, unsigned int left, unsigned int top)
//
// 目的：相对屏幕坐标系绘制文本
//
// 参数：left			横坐标
//       top			纵坐标
//       size			字符大小
//       color			文本颜色 0xff000000
//       text			字符串
//
void YCGraphic::YCFont::draw(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color,
			unsigned int left, unsigned int top)
{
	YCIFontSprite* fontSprite = myFontImpl->fetch(type);
	if (fontSprite != NULL)
	{
		fontSprite->initialize()
			      ->setProperty(YCIFontSprite::FONT_COLOR, color)
			      ->setProperty(YCIFontSprite::FONT_SIZE, size)
				  ->draw(text, left, top);
	}
}

//
// 函数：drawWithStroke(E_FONT_TYPE type, const char* text, unsigned int color,
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
void YCGraphic::YCFont::drawWithStroke(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color,
			unsigned int left, unsigned int top, unsigned int strokeColor)
{
	YCIFontSprite* fontSprite = myFontImpl->fetch(type);
	if (fontSprite != NULL)
	{
		fontSprite->initialize()
			      ->setProperty(YCIFontSprite::FONT_COLOR, color)
			      ->setProperty(YCIFontSprite::FONT_SIZE, size)
				  ->setProperty(YCIFontSprite::FONT_STOKER, strokeColor)
				  ->draw(text, left, top);
	}
}

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
void YCGraphic::YCFont::drawWithUnderline(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color,
			unsigned int left, unsigned int top, unsigned int underlineColor)
{
	YCIFontSprite* fontSprite = myFontImpl->fetch(type);
	if (fontSprite != NULL)
	{
		fontSprite->initialize()
			      ->setProperty(YCIFontSprite::FONT_COLOR, color)
			      ->setProperty(YCIFontSprite::FONT_SIZE, size)
				  ->setProperty(YCIFontSprite::FONT_UNDERLINE, underlineColor)
				  ->draw(text, left, top);
	}
}

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
void YCGraphic::YCFont::drawWithStrokeAndUnderline(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color,
			unsigned int left, unsigned int top, unsigned int strokeColor, unsigned int underlineColor)
{
	YCIFontSprite* fontSprite = myFontImpl->fetch(type);
	if (fontSprite != NULL)
	{
		fontSprite->initialize()
			      ->setProperty(YCIFontSprite::FONT_COLOR, color)
			      ->setProperty(YCIFontSprite::FONT_SIZE, size)
				  ->setProperty(YCIFontSprite::FONT_STOKER, strokeColor)
				  ->setProperty(YCIFontSprite::FONT_UNDERLINE, underlineColor)
				  ->draw(text, left, top);
	}
}

YCGraphic::YCFont* YCGraphic::createFont()
{
	SMART_ASSERT(myFont != NULL);
	return myFont;
}

#pragma endregion YCFont

//
// 函数：createSpriteDrawer()
//
// 目的：获取精灵绘制接口
//
YCGraphic::YCD3DSprite* YCGraphic::createD3DSprite()
{
	return mySpriteImpl;
}

//
// 函数：createPrimitive()
//
// 目的：获取基本件绘制接口
//
YCGraphic::YCD3DPrimitive* YCGraphic::createPrimitive()
{
	return mySpriteImpl;
}

//////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////

//
// 函数：initializeDevice()
//
// 目的：初始化D3DDevice设备
//
bool YCGraphic::initializeDevice()
{
	g_pd3dDevice->SetFVF(YUCHEN_VERTEX_FVF);
	
	//因为二维图形使用顶点颜色或纹理渲染，所以关闭光照
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	//关闭 Z 缓存
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

	//激活Alpha混合
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE); 

	//使用加载图片 srcalpha 
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);

	//使用 1-srcalpha
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA); 

	return true;
}

//
// 函数：shouldRender()
//
// 目的：定帧数检测
//
bool YCGraphic::shouldRender()
{
	if (myVisible && (myNextDraw - myTimestamp < 10))
	{
		myNextDraw += perFpsTime;
		return true;
	}
	return false;
}

//
// 函数：validate()
//
// 目的：验证DirectX环境
//
bool YCGraphic::validate()
{
	return (g_pd3dDevice != NULL);
}

//
// 函数：onLostDevice()
//
// 目的：丢失设备处理
//
void YCGraphic::onLostDevice()
{
	mySpriteImpl->onLostDevice();
}

//
// 函数：onResetDevice()
//
// 目的：恢复设备处理
//
void YCGraphic::onResetDevice()
{
	if(FAILED(g_pd3dDevice->Reset(&d3dpp)))
    {
        throw YCException(2002, "YCGraphic::onResetDevice重置设备失败");
    }

	if (!initializeDevice())
	{
		throw YCException(2002, "YCGraphic::onResetDevice初始化设备失败");
	}

	mySpriteImpl->onResetDevice();
}

//
// 函数：getPixShaderVersion()
//
// 目的：获取显卡支持的PixShader版本
//
YCGraphic::E_PIXSHADER_VERSION YCGraphic::getPixShaderVersion()
{	
	switch (d3dCaps.PixelShaderVersion)
	{
	case D3DPS_VERSION(1, 1): return PIXSHADER_1_1;
	case D3DPS_VERSION(2, 0): return PIXSHADER_2_0;
	case D3DPS_VERSION(3, 0): return PIXSHADER_3_0;
	case D3DPS_VERSION(4, 0): return PIXSHADER_4_0;
	case D3DPS_VERSION(5, 0): return PIXSHADER_5_0;
	};
	return PIXSHADER_UNKNOWN;
}

#pragma region TestCode

#include "YCTexture.h"

YCTexture *texture = NULL;
YCGraphic::YCD3DSprite *sprite = NULL;
YCGraphic::YCD3DPrimitive *primitive = NULL;

//测试代码
YCGraphic::YCFont * font = NULL;
YCInput * input = NULL;
void drawFPSWithMousePosition(YCGraphic * graphic)
{
	if (sprite == NULL)
	{
		sprite = graphic->createD3DSprite();
	}

	if (font == NULL)
	{
		font = graphic->createFont();
	}

	if (input == NULL)
	{
		input = (YCInput*)YCRegistry::get("input");
		if (input == NULL)
		{
			return;
		}
	}

	char buf[64] = {0};
	unsigned int mouseX;
	unsigned int mouseY;

	input->getMousePosition(mouseX, mouseY);
	sprintf_s(buf, "(%d, %d)", mouseX, mouseY);
	font->draw(FONT_SONG, buf, 12, 0xFFFFFF, 5, 5);
}

void drawPic(YCGraphic * graphic)
{	
	if (sprite == NULL)
	{
		sprite = graphic->createD3DSprite();
	}

	if (primitive == NULL)
	{
		primitive = graphic->createPrimitive();
	}	

	//sprite->draw(texture1, 0, 0, 1, 1, 1.0f, 0)
	//      ->draw(texture2, 90, 80, 13, 5, 1.0f);

	//primitive->drawPoint(760, 480, 0xFFFF0000);
	//primitive->drawLine(0, 256, 1024, 256, 0xFFFF0000, 0xFFFF0000);
	//primitive->drawRetangle(700, 500, 800, 600, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFF0000);
}

#pragma endregion TestCode