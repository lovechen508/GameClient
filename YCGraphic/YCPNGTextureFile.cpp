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

//�������png��Ҫ�Ľӿ�
#define DEFINE_DD_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
            EXTERN_C const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

DEFINE_DD_GUID(CLSID_CoPNGFilter,       0xA3CCEDF7,0x2DE2,0x11D0,0x86,0xF4,0x00,0xA0,0xC9,0x13,0xF7,0x50);
DEFINE_DD_GUID(IID_IDirectDrawSurfacex, 0x6C14DB81,0xA733,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60);

// DIB����,���ڴ�������һ��λͼ
struct DIB
{
    int width;
    int height;
    int bpp;        // BitsPixel
    int wb;         // Width Bytes, bytes in each scan line
    void* bits;
    HBITMAP bmp;
};

// ��image filter�ӿڼ���ͼƬ��Ҫ�û��ṩIDirectSurface�ӿڣ����Լ���ʵ������ӿڣ�

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

        // windows�涨Ҫ��alphaֵ���㵽RGBͨ���ϣ��ο�MSDN��alphablend����˵��
        // �������p[3]Ϊalphaͨ����Ҳ�п�����p[0]���Ҹ����ˣ���Ҫ���Լ�����ʵ�顣
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

	// ��ȡ�ļ�
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
// ������isTransparent(unsigned int x, unsigned int y)
//
// Ŀ�ģ�����ĳ�����Ƿ�͸��
//
bool YCPNGTextureFile::isTransparent(unsigned int x, unsigned int y)
{
	if (x >= myWidth || y >= myHeight)
	{
		throw YCException(2002, "YCPNGTextureFile::isTransparent���Ե㲻�������ڲ���");
	}
	BYTE* pt = myDecoder->dib_pixelpos(x, y);
	return (pt && pt[0]==0 && pt[1]==0 && pt[2]==0);
}

//
// ������load(void* pd3d9Device)
//
// Ŀ�ģ����������ڴ�
//
bool YCPNGTextureFile::load(void* pd3d9Device)
{
	CoInitialize(NULL);  

	// ��ȡPNG������Ϣ���ڴ�
	IStream* istream = NULL;
	if (FAILED(::SHCreateStreamOnFile((LPCSTR)myFilename, STGM_READ, &istream)))
	{
		throw YCException(2002, "YCFileTextureLoader::load��ȡPNG�ļ�ʧ�ܣ�");
	}

	myDecoder = new PNGDecoder();
	bool result = myDecoder->LoadFile(&CLSID_CoPNGFilter, istream);
    if (result)
	{
		D3DXIMAGE_INFO srcInfo;
		LPDIRECT3DTEXTURE9 LPTexture = NULL;
		if (FAILED(D3DXCreateTextureFromFileEx((LPDIRECT3DDEVICE9)pd3d9Device,    //������IDirect3DDevice9����ָ��
										 myFilename,                              //�ļ���
										 D3DX_DEFAULT_NONPOW2,                    //ͼ��Ŀ�
										 D3DX_DEFAULT_NONPOW2,                    //ͼ��ĳ�
										 D3DX_FROM_FILE,                          //ͼƬ��ͼ�㣬һ����D3DX_DEFAULT����Ҳ��1��
										 0,                                       //�趨��������ʹ�÷������������������0��
                                             									  //D3DUSAGE_RENDERTARGET����
																				  //D3DUSAGE_DYNAMIC��һ�����õ���0����Ҳ����ʹ����D3DUSAGE_DYNAMIC��
																				  //�������Զ�̬�ı������е����ݡ������������Ҫ��̬�Ըı䣬�Ͳ���Ҫ�趨��
																				  //������ΪD3DUSAGE_DYNAMIC��
										 D3DFMT_FROM_FILE,                        // ����ĸ�ʽ�������Լ����壬Ҳ����ֱ�Ӷ���ΪD3DFMT_UNKNOWN���ó���
																				  //������ͼ���ļ�ʱ�����������
										 D3DPOOL_MANAGED,                         //������ι�����Щ����һ��ʹ��D3DPOOL_MANAGED
										 D3DX_DEFAULT,                            //ͼ�����صĹ��˷�ʽ
										 D3DX_DEFAULT,                            //MIP�����ع��˷�ʽ
										 D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f),//͸��ɫ���趨�����ɫ������ʾʱ����ͼ���е������ɫ������
										 &srcInfo,                                //��¼����ͼƬ��Ϣ
										 NULL,                                    //��¼��ɫ����Ϣ
										 (LPDIRECT3DTEXTURE9*)&LPTexture)))       //������������ͼƬ���������ʵ��
		{
			throw YCException(2002, "YCFileTextureLoader::load����PNG����ʧ�ܣ�");
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
