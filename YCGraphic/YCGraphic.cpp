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
	float u, v;             // ��������
} YCVERTEX;

#define YUCHEN_VERTEX_FVF (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

LPDIRECT3D9 pD3D = NULL;					// the Direct3D object
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;		// the Direct3D device
D3DPRESENT_PARAMETERS d3dpp;
LPDIRECT3DPIXELSHADER9 g_pPS = NULL;  
LPD3DXCONSTANTTABLE g_pPSConstantTable = NULL; 

D3DCAPS9 d3dCaps;                           // the Direct3D Caps 
	
SIZE ScreenSize;							// ��ǰ��Ļ�ֱ���
SIZE WindowSize;							// ��ǰ�Ӵ���С

int fps;									//֡Ƶ
int perFpsTime;								//ÿ֡ʱ��   1000/fps

#define VertexCount  40000					//������Ŀ4000��
#define ItemsCount VertexCount/2            //������г���

/////////////////////////////////////////////////////////////////////

//
// ������reportError(HRSULT hr)
//
// Ŀ�ģ�����MessageBox����DX����
//
void reportError(HRESULT hr)
{
	char strError[256] = {0};
	sprintf_s(strError, "����ֵ��%s \n����������%s", ::DXGetErrorString(hr), ::DXGetErrorDescription(hr));
	::MessageBoxA(0, (LPSTR)strError, "DirectX����", 0);
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
			::MessageBoxA(NULL, (LPSTR)pErrorMsgs->GetBufferPointer(), "DirectX����", MB_OK);  
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

		//����λͼ���εĶ��㻺����
		if (FAILED(g_pd3dDevice->CreateVertexBuffer(sizeof(YCVERTEX) * VertexCount,
												  D3DUSAGE_WRITEONLY,
												  YUCHEN_VERTEX_FVF,
												  D3DPOOL_MANAGED,
												  &lpVertexBuffer,
												  NULL)))
		{
			throw YCException(2002, "CreateDirectX �������㻺����ʧ��, DirectX����");
		}

		g_pd3dDevice->SetStreamSource (0, lpVertexBuffer, 0, sizeof(YCVERTEX ));
		LOG_DEBUG("YCD3DSpriteImpl::YCD3DSpriteImpl�������㻺�����ɹ�!");
	}

	~YCD3DSpriteImpl()
	{
		g_pd3dDevice->SetStreamSource(0, NULL, 0, 0);	
		SAFE_RELEASE(lpVertexBuffer);		
		LOG_DEBUG("YCD3DSpriteImpl::~YCD3DSpriteImpl�ͷŶ��㻺��ɹ���");
	}

public:
	
	//
	// ������onLostDevice()
	// 
	// Ŀ�ģ���ͼ��ϵͳ��ʧ�豸ʱ��YCScene����
	//
	void onLostDevice()
	{

	}

	//
	// ������onResetDevice()
	// 
	// Ŀ�ģ���ͼ��ϵͳ�����豸ʱ��YCScene����
	//
	void onResetDevice()
	{
		SAFE_RELEASE(lpVertexBuffer);		
		g_pd3dDevice->SetStreamSource(0, NULL, 0, 0);

		//����λͼ���εĶ��㻺����
		if (FAILED(g_pd3dDevice->CreateVertexBuffer(sizeof(YCVERTEX) * VertexCount,
												  D3DUSAGE_WRITEONLY,
												  YUCHEN_VERTEX_FVF,
												  D3DPOOL_MANAGED,
												  &lpVertexBuffer,
												  NULL)))
		{
			throw YCException(2002, "CreateDirectX �������㻺����ʧ��, DirectX����");
		}

		g_pd3dDevice->SetStreamSource (0, lpVertexBuffer, 0, sizeof(YCVERTEX ));
	}

	//
	// ������timestamp()
	//
	// Ŀ�ģ���ǰ֡ʱ���
	// 
	time_t timestamp()
	{
		return myTimestamp;
	}

	//
	// ������begin()
	//
	// Ŀ�ģ���draw֮ǰ����
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
	// ������setViewport(const IntRect& viewport)
	//
	// Ŀ�ģ����òü�����
	//
	virtual IntRect setViewport(const IntRect& viewport)
	{				   
		IntRect old;
		
		old.left = myViewport.X;
		old.top = myViewport.Y;
		old.right = myViewport.X + myViewport.Width;
		old.bottom = myViewport.Y + myViewport.Height;

		//����ViewPort
		myViewport.X = viewport.left;
		myViewport.Y = viewport.top;
		myViewport.Width = viewport.width();
		myViewport.Height = viewport.height();
		myViewport.MinZ = 0.0f;
		myViewport.MaxZ = 1.0f;

		return old;
	}

	//
	// ������narrowViewport(const IntRect& viewport)
	//
	// Ŀ�ģ������ü�����
	//
	virtual IntRect narrowViewport(const IntRect& viewport)
	{
		IntRect old;
		
		old.left = myViewport.X;
		old.top = myViewport.Y;
		old.right = myViewport.X + myViewport.Width;
		old.bottom = myViewport.Y + myViewport.Height;

		//����ViewPort
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
	// ������recoverViewport(const IntRect& viewport)
	//
	// Ŀ�ģ��ָ��ü�����
	//
	virtual void recoverViewport(const IntRect& viewport)
	{
		//����ViewPort
		myViewport.X = viewport.left;
		myViewport.Y = viewport.top;
		myViewport.Width = viewport.width();
		myViewport.Height = viewport.height();
		myViewport.MinZ = 0.0f;
		myViewport.MaxZ = 1.0f;
	}

#define CHECK_REAL_RENDER if (!myRealRender) return this

	//
	// ������draw(YCITexture* texture, float left, float top, float repeatX, float repeatY, float alpha, unsigned int color)
	//
	// Ŀ�ģ����һ��Lock���㻺��Ļ�������
	//
	virtual YCD3DSprite* draw(YCITexture* texture, float left, float top, float repeatX, float repeatY, float alpha, unsigned int color)
	{
		CHECK_REAL_RENDER;

		SMART_ASSERT(texture != NULL);
		if (texture == NULL)
		{
			throw YCException(2002, "YCD3DSprite* addTexture��������ֵΪNULL!");
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
	// ���� drawPoint(unsigned int x, unsigned int y, unsigned int color);
	//
	// Ŀ�ģ�����
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
	// ���� drawLine(unsigned int xSrc, unsigned int ySrc, unsigned int xDst, unsigned int yDst, unsigned int color);
	//
	// Ŀ�ģ�������
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
	// ������drawRetangle(unsigned int left,    unsigned int top,    unsigned int right,   unsigned int bottom,
	//	                 unsigned int colorV0, unsigned int colorV1, unsigned int colorV2, unsigned int colorV3)
	//
	// ������
	//
	// Ŀ�ģ����ı���
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
	// ������end()
	//
	// Ŀ�ģ�Ŀ�ģ���draw������������������������
	//
	// ע�ͣ�
	//   V0 (0,0)  --------> U V1 (0,1)
	//            |
	//            |
	//            |   
	//   V2 (0,1) v V          V3 (1,1)
	//
	//  D3DPT_TRIANGLESTRIP �� 
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
				break; //����
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
				LOG_DEBUG("YCD3DSprite::end����δ֪����:" << item.type);
			}
		}

		//resetViewport();
	}

	//
	// ����: reset()
	//
	// Ŀ�ģ�������Ⱦλ�ã���ת��ƽ�ƣ����ž����
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
	// ������validate(int size)
	//
	// Ŀ�ģ�
	//
	void validate(int size)
	{
		if ((myItemIndex == ItemsCount) || (myTextureIndex > (VertexCount-size)))
		{
			throw YCException(2002, "YCSpriteImpl::validate���㻺���þ�");
			/*
			end();	// ��

			memset(myTextureItems, 0, ItemsCount*sizeof(YCDrawItem_T));
			myItemIndex = 0;
			myTextureIndex = 0;

			lpVertexBuffer->Lock(0, sizeof(YCVERTEX),(void**)&pVertex, 0);
			*/
		}
	}

	//
	// ������drawPointImpl(int index, const YCDrawItem_T& item)
	//
	// Ŀ�ģ�����
	//
	void drawPointImpl(int index, const YCDrawItem_T& item)
	{
		g_pd3dDevice->SetViewport(&item.viewport);
		g_pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, index, 1 );
	}

	//
	// ������drawLineImpl(int idx)
	//
	// Ŀ�ģ�����
	//
	void drawLineImpl(int index, const YCDrawItem_T& item)
	{
		g_pd3dDevice->SetViewport(&item.viewport);
		g_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, index, 1 );
	}

	//
	// ������drawRetangleImpl(int index, const YCDrawItem_T& item)
	//
	// Ŀ�ģ�������
	//
	void drawRetangleImpl(int index, const YCDrawItem_T& item)
	{
		g_pd3dDevice->SetViewport(&item.viewport);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, index, 2 );
	}

	//
	// ������drawTextureImpl(int index, const YCDrawItem_T& item)
	//
	// Ŀ�ģ������������ͼƬ����
	//
	void drawTextureImpl(int index, const YCDrawItem_T& item)
	{
		if (item.texture.repeat)
		{
			//׼��ȫ������
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

			// ��Pixel Shader
			g_pd3dDevice->SetPixelShader(g_pPS);
		}

		if (item.texture.diffuse == 0)
		{
			// ��ɫ��Դ-����
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			// ����Alphaͨ����Դ��diffuseͨ��
			//g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
			//g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		}
		else
		{
			// ��ɫ��Դ-diffuse
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

			// ����Alphaͨ����Դ������alphaͨ��
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

	BYTE* pVertex; //���㻺�����

	time_t myTimestamp; //ÿ֡��ʱ���

	bool myRealRender;  //��֡�Ƿ���Ļ���

	int myItemIndex;
	int myTextureIndex;

	struct YCDrawItem_T myTextureItems[ItemsCount];

	YCVERTEX retangleVertex[4];		            //λͼ���εĶ��� 4��
	
	LPDIRECT3DVERTEXBUFFER9  lpVertexBuffer;	//λͼ���εĶ��㻺�����ӿ�ָ��
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
// ������initialize(HWND hwnd, YCInput& input, YCConfig& config)
//
// Ŀ�ģ���ʼ��DirectX����
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
	// ʹ��D3DDEVTYPE_REF�����豸��ʧ��
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL, 
									wndHandle,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&g_pd3dDevice)))
	{
		::MessageBoxA(0, "CreateDirectX �����豸ʧ��", "DirectX����", 0);
		return false;
	}

	LOG_DEBUG("YCGraphic::initialize�����豸�ɹ�");

	//ע���豸
	YCRegistry::set("D3DDevice", g_pd3dDevice);

	//��ȡD3DCAPS9
	if (FAILED(g_pd3dDevice->GetDeviceCaps(&d3dCaps)))
	{
		::MessageBoxA(0, "CreateDirectX ��ȡ�豸����ʧ��", "DirectX����", 0);
		return false;
	}

	LOG_DEBUG("YCGraphic::initialize���PixShaderVersion�汾 : " << getPixShaderVersion());

	//ע��Pix Shader
	if (!initPixShader())
	{
		return false;
	}

	YCRegistry::set("width", new int(WindowSize.cx));
	YCRegistry::set("height", new int(WindowSize.cy));
	
	//��ʼ��D3DDevice�豸����
	if (!initializeDevice())
	{
		::MessageBoxA(0, "��ʼ��D3D�豸ʧ�ܣ�", "DirectX����", 0);
		return false;
	}
	
	// �������������
	mySpriteImpl = new YCD3DSpriteImpl();

	// �����ַ����ƶ���
	myFont = new YCFont(mySpriteImpl);

	//��ʼ����������ϵͳ
	myNetworkLoader = new YCNetworkFileLoader();
	if (!myNetworkLoader->initialize(config))
	{
		::MessageBoxA(0, "��ʼ������������ʧ��", "Application����", 0);
		return false;
	}

	LOG_DEBUG("YCGraphic::initialize��ʼ����������ϵͳ�ɹ�!");

	//��ʼ�����������
	myTextureManager = new YCTextureManager();
	if (!myTextureManager->initialize(myNetworkLoader, config))
	{
		::MessageBoxA(0, "��ʼ�����������ʧ��", "Application����", 0);
		return false;
	}
	YCRegistry::set("textureManager", myTextureManager);
	LOG_DEBUG("YCGraphic::initialize��ʼ������������ɹ�!");

	myAnimationManager = new YCAnimationManager();
	if (!myAnimationManager->initialize(myNetworkLoader, config))
	{
		::MessageBoxA(0, "��ʼ������������ʧ��", "Application����", 0);
		return false;
	}
	YCRegistry::set("animationManager", myAnimationManager);
	LOG_DEBUG("YCGraphic::initialize��ʼ�������������ɹ�!");

	myUIManager = new YCUIManager();
	if (!myUIManager->initialize(myTextureManager, input, config))
	{
		::MessageBoxA(0, "��ʼ��UI������ʧ��", "Application����", 0);
		return false;
	}
	YCRegistry::set("UIManager", myUIManager);
	LOG_DEBUG("YCGraphic::initialize��ʼ��UI�������ɹ�!");

	mySceneManager = new YCSceneManager(this);
	LOG_DEBUG("YCGraphic::initialize��ʼ�������������ɹ���");

	myQueryFactory = new YCQueryFactory();
	YCLua *lua = (YCLua*)YCRegistry::get("lua");
	if (lua == NULL)
	{
		::MessageBoxA(0, "YCGraphic::initialize�޷���ȡLua���", "Application����", 0);
		return false;
	}
	lua->bindQueryFactory(myQueryFactory);

	// ע������
	YCRegistry::set("graphic", this);

	// ����YCIComponentȫ�ֱ���
	YCIComponent::gSprite = mySpriteImpl;
	YCIComponent::gPrimitive = mySpriteImpl;
	YCIComponent::gFont = myFont;

	//myUIManager->registUI("avatar", "D:\\dropbox\\Dropbox\\Public\\Client\\data\\ui\\1\\avatar.xml");
	myUIManager->registUI("avatar", "1\\avatar.xml");

	//texture1 = myTextureManager->fetch(TEXTURE_PLAYER, "1\\1\\1\\2\\00.png");
	//texture2 = myTextureManager->fetch(TEXTURE_PLAYER, "1\\1\\1\\3\\00.png");
	//texture3 = myTextureManager->fetch(TEXTURE_PLAYER, "1\\1\\1\\6\\00.png");

	//��֡
	setFPS(config.fps());

	//ע�ᴦ��YCInput���̣������Ϣ
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
// ������registerMapFactory(YCIMapFactory* mapFactory)
//
// Ŀ�ģ�ע���ͼ����
//
void YCGraphic::registerMapFactory(YCIMapFactory* mapFactory)
{
	YCSceneManager::GetSingleton().registerMapFactory(mapFactory);	

	// TEST CODE
	//YCIPackageResponse response;	
	//updateScene(YCSceneManager::GetSingleton().requireScene(&response));
}

//
// ������getUIManager()
//
// Ŀ�ģ�ȡ�ý��������
//
YCUIManager* YCGraphic::getUIManager()
{
	SMART_ASSERT(myUIManager != NULL);
	return myUIManager;
}

//
// ������getScene()
//
// Ŀ�ģ�ȡ�ó���������
//
YCScene* YCGraphic::getScene()
{
	SMART_ASSERT(myScene != NULL);
	return myScene;
}

//
// ������getWindowSize()
//
// Ŀ�ģ���ȡ���ڴ�С
//
SIZE YCGraphic::getWindowSize()
{
	return WindowSize;
}

//
// ������setWindowSize()
//
// Ŀ�ģ����ô��ڴ�С
//
void YCGraphic::setWindowSize(SIZE& size)
{
	WindowSize.cx = size.cx;
	WindowSize.cy = size.cy;
}

//
// ������setFPS(int fps)
//
// Ŀ�ģ�����֡Ƶ
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
// ������getFPS()
//
// Ŀ�ģ���ȡ֡Ƶ
//
int YCGraphic::getFPS()
{
	return fps;
}

//
// ������setVisible(bool visible)
//
// Ŀ�ģ����ô��ڿɼ���
//
void YCGraphic::setVisible(bool visible)
{
	myVisible = visible;
}

//
// ������updateScene(YCScene* newScene)
//
// Ŀ�ģ����ó���:
//
// ע�ͣ����ؾɳ���ָ�룬�״�ΪNULL
//
YCScene* YCGraphic::updateScene(YCScene* newScene)
{
	YCScene* oldScene = myScene;
	myScene = newScene;
	YCRegistry::set("scene", myScene);
	return oldScene;
}

/*
 * ��Ⱦÿһ֡
 */
void YCGraphic::render()
{
	myTimestamp = ::GetTickCount();

	// ����豸�Ƿ�ʧ
	if (validate())
	{	
		bool realRender = shouldRender();

		if (realRender)
		{
			//���󱸻�������ɺ�ɫ
			g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET,
				D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

			// ��ʼ����
			if(SUCCEEDED(g_pd3dDevice->BeginScene()))
			{
				mySpriteImpl->begin(realRender);
				// ��ȾͼԪ
				drawFPSWithMousePosition(this);
				if (myScene) { myScene->render();}
				//drawText(this);
				mySpriteImpl->end();
			}

			// ��������
			g_pd3dDevice->EndScene();
			
			// ��ʾ����
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
		else //���߼�
		{
			mySpriteImpl->begin(realRender);
			if (myScene) { myScene->render();}
			mySpriteImpl->end();
			::Sleep(10); //��Ӧ��˯�ߣ�����
		}
	}
}

//
// ������GetMaxTextureBlendStages()
//
// Ŀ�ģ���ǰ�Կ�֧�ֵĶ����������
//
unsigned int YCGraphic::getMaxTextureBlendStages()
{
	return d3dCaps.MaxTextureBlendStages;
}

//
// ������snapshot()
//
// Ŀ�ģ��������
//
void YCGraphic::snapshot()
{

}

/*
 * �ͷ�DirectX��Դ
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
	LOG_DEBUG("YCGraphic::finalize�˳������ɹ���");

	SAFE_DELETE(mySceneManager);
	LOG_DEBUG("YCGraphic::finalize�ͷų����������ɹ���");

	if (myUIManager != NULL)
	{
		myUIManager->finalize();
		delete myUIManager;
	}
	LOG_DEBUG("YCGraphic::finalize�ͷ�UI�������ɹ���");

	if (myAnimationManager != NULL)
	{
		myAnimationManager->finalize();
		delete myAnimationManager;
	}
	LOG_DEBUG("YCGraphic::finalize�ͷŶ����������ɹ���");

	if (myTextureManager != NULL)
	{
		myTextureManager->finalize();
		delete myTextureManager;
	}
	LOG_DEBUG("YCGraphic::finalize�ͷ�����������ɹ���");

	if (myNetworkLoader != NULL)
	{
		myNetworkLoader->finalize();
		delete myNetworkLoader;
	}
	LOG_DEBUG("YCGraphic::finalize�ͷ���������ϵͳ�ɹ���");

	SAFE_RELEASE(g_pd3dDevice);
	LOG_DEBUG("YCGraphic::finalize�ͷ��豸�ɹ���");

	SAFE_RELEASE(pD3D);
	LOG_DEBUG("YCGraphic::finalize�ͷ�DirectX�ɹ���");

	// ȡ������ע��
	YCRegistry::remove("graphic");
}

//////////////////////////////////////////////////////
// ��Ϣ���� ���ܺ���
//////////////////////////////////////////////////////

//
// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ�������Ϣ������
//
// ����ֵ���Ѵ�������true�����򷵻�false
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
// YCFont �ַ�������
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
	// ������getFontWidthAndHeight(E_FONT_TYPE type, E_FONT_DIRECTION direction, unsigned int size, const char* strInfo, int& width, int& height)
	//
	// Ŀ�ģ�ȡ���ض����弰��С����ߺͿ�
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
// ������setAutoScaled(bool autoScaled)
//
// Ŀ�ģ������Զ�����
//
void YCGraphic::YCFont::setAutoScaled(bool autoScaled)
{

}

//
// ������isAutoScaled()
//
// Ŀ�ģ��Ƿ��Զ�����
//
bool YCGraphic::YCFont::isAutoScaled() const
{
    return false;
}

//
// ������getFontWidthAndHeight(E_FONT_TYPE type, E_FONT_DIRECTION direction, unsigned int size, const char* strInfo, int& width, int& height)
//
// Ŀ�ģ�ȡ���ض����弰��С����ߺͿ�
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
// ������draw(E_FONT_TYPE type, const char* text, unsigned int size, unsigned int color, unsigned int left, unsigned int top)
//
// Ŀ�ģ������Ļ����ϵ�����ı�
//
// ������left			������
//       top			������
//       size			�ַ���С
//       color			�ı���ɫ 0xff000000
//       text			�ַ���
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
// ������drawWithStroke(E_FONT_TYPE type, const char* text, unsigned int color,
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
// ������createSpriteDrawer()
//
// Ŀ�ģ���ȡ������ƽӿ�
//
YCGraphic::YCD3DSprite* YCGraphic::createD3DSprite()
{
	return mySpriteImpl;
}

//
// ������createPrimitive()
//
// Ŀ�ģ���ȡ���������ƽӿ�
//
YCGraphic::YCD3DPrimitive* YCGraphic::createPrimitive()
{
	return mySpriteImpl;
}

//////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////

//
// ������initializeDevice()
//
// Ŀ�ģ���ʼ��D3DDevice�豸
//
bool YCGraphic::initializeDevice()
{
	g_pd3dDevice->SetFVF(YUCHEN_VERTEX_FVF);
	
	//��Ϊ��άͼ��ʹ�ö�����ɫ��������Ⱦ�����Թرչ���
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	//�ر� Z ����
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

	//����Alpha���
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE); 

	//ʹ�ü���ͼƬ srcalpha 
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);

	//ʹ�� 1-srcalpha
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA); 

	return true;
}

//
// ������shouldRender()
//
// Ŀ�ģ���֡�����
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
// ������validate()
//
// Ŀ�ģ���֤DirectX����
//
bool YCGraphic::validate()
{
	return (g_pd3dDevice != NULL);
}

//
// ������onLostDevice()
//
// Ŀ�ģ���ʧ�豸����
//
void YCGraphic::onLostDevice()
{
	mySpriteImpl->onLostDevice();
}

//
// ������onResetDevice()
//
// Ŀ�ģ��ָ��豸����
//
void YCGraphic::onResetDevice()
{
	if(FAILED(g_pd3dDevice->Reset(&d3dpp)))
    {
        throw YCException(2002, "YCGraphic::onResetDevice�����豸ʧ��");
    }

	if (!initializeDevice())
	{
		throw YCException(2002, "YCGraphic::onResetDevice��ʼ���豸ʧ��");
	}

	mySpriteImpl->onResetDevice();
}

//
// ������getPixShaderVersion()
//
// Ŀ�ģ���ȡ�Կ�֧�ֵ�PixShader�汾
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

//���Դ���
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