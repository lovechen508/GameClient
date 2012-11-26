#include "YCPNGTextureFile.h"

#include "d3d9.h"
#include "d3dx9tex.h"

#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"

#include <ddraw.h>
#include "YCBasic\YCAssert.h"
#include <ocmm.h>
#include <atlbase.h>

#pragma region pngdecoder

//定义加载png需要的接口
#define DEFINE_DD_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
            EXTERN_C const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_DD_GUID(CLSID_CoPNGFilter,       0xA3CCEDF7,0x2DE2,0x11D0,0x86,0xF4,0x00,0xA0,0xC9,0x13,0xF7,0x50);
DEFINE_DD_GUID(IID_IDirectDrawSurfacex, 0x6C14DB81,0xA733,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60);

// DIB对象,在内存中生成一张位图
struct DIB
{
    int width;
    int height;
    int bpp;        // BitsPixel
    int wb;         // Width Bytes, bytes in each scan line
    void* bits;
    HBITMAP bmp;
};

// 用image filter接口加载图片需要用户提供IDirectSurface接口，我自己来实现这个接口：

class PNGDecoder
    : public IImageDecodeEventSink
    , public IDirectDrawSurface
{

public:

    PNGDecoder () {}

	//
	STDMETHOD ( QueryInterface ) ( REFIID iid, void ** ppvObject ) {  
		if ( IID_IImageDecodeEventSink == iid ) { *ppvObject = (IImageDecodeEventSink*)this; return S_OK; }
		if ( IID_IDirectDrawSurfacex == iid ) { *ppvObject = (IDirectDrawSurface*)this; return S_OK; }
		return E_NOINTERFACE;
    }

    virtual ULONG STDMETHODCALLTYPE AddRef()
    {
        return 1;
    }
   
    virtual ULONG STDMETHODCALLTYPE Release()
    {
        return 1;
    }
   
    ////////////////////////////////////////////////////////////////////////////
    // IImageDecodeEventSink
    virtual HRESULT STDMETHODCALLTYPE GetSurface (
        LONG nWidth,
        LONG nHeight,
        REFGUID bfid,
        ULONG nPasses,
        DWORD dwHints,
        IUnknown __RPC_FAR *__RPC_FAR * ppSurface )
    {
        int nBits = 0;
       
        if (!ppSurface)
            return E_POINTER;
        if (bfid == BFID_RGB_8 || bfid == BFID_GRAY_8)
            nBits = 8;
        else if (bfid == BFID_RGB_555 || bfid == BFID_RGB_565 || bfid == BFID_GRAY_16 )
            nBits = 16;
        else if (bfid == BFID_RGB_24)
            nBits = 24;
        else if (bfid == BFID_RGB_32 || bfid == BFID_RGBA_32)
            nBits = 32;
        else if (bfid == BFID_RGB_4)
            nBits = 4;
        else if (bfid == BFID_MONOCHROME)
            nBits = 1;
        else
            return E_NOINTERFACE;

        dib_create(nWidth, nHeight, nBits, 0, 0);
        *ppSurface = (IDirectDrawSurface*)this;
        return S_OK;
    }
   
    virtual HRESULT __stdcall OnBeginDecode (
        unsigned long * pdwEvents,
        unsigned long * pnFormats,
        GUID** ppFormats )
    {
        if (!pdwEvents || !pnFormats || !ppFormats)
            return E_POINTER;

        *ppFormats = (GUID*)CoTaskMemAlloc (10 * sizeof(GUID));
        (*ppFormats)[0] = BFID_RGBA_32;
        (*ppFormats)[1] = BFID_RGB_24;
        (*ppFormats)[2] = BFID_RGB_32;
        (*ppFormats)[3] = BFID_RGB_8;
        (*ppFormats)[4] = BFID_RGB_555;
        (*ppFormats)[5] = BFID_RGB_565;
        (*ppFormats)[6] = BFID_MONOCHROME;
        (*ppFormats)[7] = BFID_RGB_4;
        (*ppFormats)[8] = BFID_GRAY_8;
        (*ppFormats)[9] = BFID_GRAY_16;

        *pdwEvents = 0;//IMGDECODE_EVENT_PALETTE|IMGDECODE_EVENT_BITSCOMPLETE | IMGDECODE_EVENT_USEDDRAW;
        *pnFormats = 10;

        return S_OK;
    }
   
    virtual HRESULT __stdcall OnBitsComplete () { return S_OK; }
   
    virtual HRESULT __stdcall OnDecodeComplete (HRESULT hrStatus)
    {
        if (m_dib.bpp!=32)
			return S_OK;

        // windows规定要把alpha值运算到RGB通道上，参看MSDN的alphablend函数说明
        // 这里假设p[3]为alpha通道，也有可能是p[0]，我给忘了，需要的自己做下实验。
        for (int i = 0; i < m_dib.width; ++i)
        {
            for (int j = 0; j < m_dib.height; ++j)
            {
                BYTE* p = dib_pixelpos(i, j);
                p[0] = p[0] * p[3] / 256;
                p[1] = p[1] * p[3] / 256;
                p[2] = p[2] * p[3] / 256;
				//LOG_TRACE("("<<i<<","<<j<<")="<<p[0] << ","<<p[1]<<","<<p[2]);
            }
        }
        return S_OK;
    }
   
    virtual HRESULT __stdcall OnPalette () { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE OnProgress (RECT * pBounds, BOOL bComplete)
    {
        return S_OK;
    }

    ////////////////////////////////////////////////////////////////////////////
    // IDirectDrawSurface
    STDMETHOD(AddAttachedSurface)(THIS_ LPDIRECTDRAWSURFACE) { return S_OK; }
    STDMETHOD(AddOverlayDirtyRect)(THIS_ LPRECT) { return S_OK; }
    STDMETHOD(Blt)(THIS_ LPRECT,LPDIRECTDRAWSURFACE, LPRECT,DWORD, LPDDBLTFX) { return S_OK; }
    STDMETHOD(BltBatch)(THIS_ LPDDBLTBATCH, DWORD, DWORD ) { return S_OK; }
    STDMETHOD(BltFast)(THIS_ DWORD,DWORD,LPDIRECTDRAWSURFACE, LPRECT,DWORD) { return S_OK; }
    STDMETHOD(DeleteAttachedSurface)(THIS_ DWORD,LPDIRECTDRAWSURFACE) { return S_OK; }
    STDMETHOD(EnumAttachedSurfaces)(THIS_ LPVOID,LPDDENUMSURFACESCALLBACK) { return S_OK; }
    STDMETHOD(EnumOverlayZOrders)(THIS_ DWORD,LPVOID,LPDDENUMSURFACESCALLBACK) { return S_OK; }
    STDMETHOD(Flip)(THIS_ LPDIRECTDRAWSURFACE, DWORD) { return S_OK; }
    STDMETHOD(GetAttachedSurface)(THIS_ LPDDSCAPS, LPDIRECTDRAWSURFACE FAR *) { return S_OK; }
    STDMETHOD(GetBltStatus)(THIS_ DWORD) { return E_NOTIMPL; }
    STDMETHOD(GetCaps)(THIS_ LPDDSCAPS) { return E_NOTIMPL; }
    STDMETHOD(GetClipper)(THIS_ LPDIRECTDRAWCLIPPER FAR*) { return E_NOTIMPL; }
    STDMETHOD(GetColorKey)(THIS_ DWORD, LPDDCOLORKEY) { return E_NOTIMPL; }
    STDMETHOD(GetDC)(THIS_ HDC FAR *) { return E_NOTIMPL; }
    STDMETHOD(GetFlipStatus)(THIS_ DWORD) { return E_NOTIMPL; }
    STDMETHOD(GetOverlayPosition)(THIS_ LPLONG, LPLONG ) { return E_NOTIMPL; }
    STDMETHOD(GetPalette)(THIS_ LPDIRECTDRAWPALETTE FAR* p) { return E_NOTIMPL; }
    STDMETHOD(GetPixelFormat)(THIS_ LPDDPIXELFORMAT) { return E_NOTIMPL; }
    STDMETHOD(GetSurfaceDesc)(THIS_ LPDDSURFACEDESC) { return E_NOTIMPL; }
    STDMETHOD(Initialize)(THIS_ LPDIRECTDRAW, LPDDSURFACEDESC) { return S_OK; }
    STDMETHOD(IsLost)(THIS) { return S_OK; }
    STDMETHOD(ReleaseDC)(THIS_ HDC) { return S_OK; }
    STDMETHOD(Restore)(THIS) { return S_OK; }
    STDMETHOD(SetClipper)(THIS_ LPDIRECTDRAWCLIPPER) { return E_NOTIMPL; }   
    STDMETHOD(SetColorKey)(DWORD dwFlags, LPDDCOLORKEY pDDColorKey) { return S_OK; }   
    STDMETHOD(SetOverlayPosition)(THIS_ LONG, LONG ) { return E_NOTIMPL; }
    STDMETHOD(SetPalette)(THIS_ LPDIRECTDRAWPALETTE) { return E_NOTIMPL; }
    STDMETHOD(Unlock)(THIS_ LPVOID) { return S_OK; }
    STDMETHOD(UpdateOverlay)(THIS_ LPRECT, LPDIRECTDRAWSURFACE,LPRECT,DWORD, LPDDOVERLAYFX) { return E_NOTIMPL; }
    STDMETHOD(UpdateOverlayDisplay)(THIS_ DWORD) { return E_NOTIMPL; }
    STDMETHOD(UpdateOverlayZOrder)(THIS_ DWORD, LPDIRECTDRAWSURFACE) { return E_NOTIMPL; }
	
	BYTE* dib_pixelpos(int x, int y)
	{
		SMART_ASSERT(x < m_dib.width && y < m_dib.height);
		return (BYTE*)m_dib.bits + (m_dib.height - y - 1) * m_dib.wb + x * m_dib.bpp / 8;
	}

    STDMETHOD(Lock)(THIS_ LPRECT prc,LPDDSURFACEDESC ddsd,DWORD dwFlag,HANDLE)
    {
        SMART_ASSERT(prc->left >= 0 && prc->top >= 0);
        SMART_ASSERT(prc->right <= m_dib.width && prc->bottom <= m_dib.height);
        ddsd->lpSurface = dib_pixelpos(prc->left, prc->bottom - 1);
        ddsd->lPitch = -m_dib.wb;
        return S_OK;
    }

	// 读取文件
	bool LoadFile(const CLSID* guid, IStream* stream)
    {
        CComQIPtr<IImageDecodeFilter> ptr;
        ptr.CoCreateInstance(*guid);
        if (ptr)
        {
            if (S_OK == ptr->Initialize(this))
            {
                ptr->Process(stream);
                ptr->Terminate(S_OK);
                return (m_dib.bmp != NULL);
            }
        }
        return false;
    }

private:
	
	void dib_create(int w, int h, int bpp, RGBQUAD* ppal, int npal)
	{
		struct
		{
			BITMAPINFO bi;
			RGBQUAD rgb[256];
		} head = { sizeof(BITMAPINFOHEADER), w, h, 1, bpp };

		head.bi.bmiHeader.biClrUsed = npal;
		if (ppal && bpp <= 8)
		{
			memcpy (head.bi.bmiColors, ppal, 256 * sizeof(RGBQUAD));
		}

		void* pData = 0;
		HBITMAP hBmp = ::CreateDIBSection(0, &head.bi, DIB_RGB_COLORS, &pData, 0, 0);
		m_dib.bmp = hBmp;
		m_dib.bits = pData;
		m_dib.width = w;
		m_dib.height = h;
		m_dib.bpp = bpp;

	#   define DWORD_WBYTES(x)          ((((x)+31UL)>>5)<<2)
		m_dib.wb = DWORD_WBYTES(w * bpp);
	}
	
    DIB m_dib;
};

#pragma endregion pngdecoder


YCPNGTextureFile::YCPNGTextureFile(const char* filename)
	: YCITextureFile(filename)
	, myDecoder(NULL)
{
}


YCPNGTextureFile::~YCPNGTextureFile(void)
{
	if (myDecoder != NULL)
	{
		delete myDecoder;
	}
}

//
// 函数：isTransparent(unsigned int x, unsigned int y)
//
// 目的：测试某个点是否透明
//
bool YCPNGTextureFile::isTransparent(unsigned int x, unsigned int y)
{
	if (x >= myWidth || y >= myHeight)
	{
		throw YCException(2002, "YCPNGTextureFile::isTransparent测试点不在纹理内部！");
	}
	BYTE* pt = myDecoder->dib_pixelpos(x, y);
	return (pt && pt[0]==0 && pt[1]==0 && pt[2]==0);
}

//
// 函数：load(void* pd3d9Device)
//
// 目的：加载纹理到内存
//
bool YCPNGTextureFile::load(void* pd3d9Device)
{
	CoInitialize(NULL);  

	// 读取PNG纹理信息到内存
	IStream* istream = NULL;
	if (FAILED(::SHCreateStreamOnFile((LPCSTR)myFilename, STGM_READ, &istream)))
	{
		throw YCException(2002, "YCFileTextureLoader::load读取PNG文件失败！");
	}

	myDecoder = new PNGDecoder();
	bool result = myDecoder->LoadFile(&CLSID_CoPNGFilter, istream);
    if (result)
	{
		D3DXIMAGE_INFO srcInfo;
		LPDIRECT3DTEXTURE9 LPTexture = NULL;
		if (FAILED(D3DXCreateTextureFromFileEx((LPDIRECT3DDEVICE9)pd3d9Device,    //调入是IDirect3DDevice9对象指针
										 myFilename,                              //文件名
										 D3DX_DEFAULT_NONPOW2,                    //图像的宽
										 D3DX_DEFAULT_NONPOW2,                    //图像的长
										 D3DX_FROM_FILE,                          //图片的图层，一般用D3DX_DEFAULT，但也用1的
										 0,                                       //设定这个纹理的使用方法，这个参数可以是0，
                                             									  //D3DUSAGE_RENDERTARGET，和
																				  //D3DUSAGE_DYNAMIC。一般我用的是0，你也可以使用了D3DUSAGE_DYNAMIC，
																				  //这样可以动态改变纹理中的数据。如果你纹理不需要动态性改变，就不需要设定这
																				  //个参数为D3DUSAGE_DYNAMIC。
										 D3DFMT_FROM_FILE,                        // 纹理的格式，可以自己定义，也可以直接定义为D3DFMT_UNKNOWN，让程序
																				  //在载入图像文件时定义象素深度
										 D3DPOOL_MANAGED,                         //这是如何管理这些纹理，一般使用D3DPOOL_MANAGED
										 D3DX_DEFAULT,                            //图像像素的过滤方式
										 D3DX_DEFAULT,                            //MIP的像素过滤方式
										 D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f),//透明色，设定这个颜色，在显示时，这图像中的这个颜色将忽略
										 &srcInfo,                                //记录载入图片信息
										 NULL,                                    //记录调色板信息
										 (LPDIRECT3DTEXTURE9*)&LPTexture)))       //用来储存载入图片的纹理对象实例
		{
			throw YCException(2002, "YCFileTextureLoader::load加载PNG纹理失败！");
		}

		myLPTexture = LPTexture;

		myWidth = srcInfo.Width;
		myHeight = srcInfo.Height;
		myDepth = srcInfo.Depth;
		myFormat = srcInfo.Format;
		myResourceType = srcInfo.ResourceType;
		myImageFileFormat = srcInfo.ImageFileFormat;
	}

	istream->Release();

	return result;
}
