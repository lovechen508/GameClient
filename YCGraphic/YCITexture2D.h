#ifndef _INCLUDE_YCITEXTURE2D_H_
#define _INCLUDE_YCITEXTURE2D_H_

#include "YCRect.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCSharedPtr.h"

class YCITextureFile;

/*
 * YCITexture2D : �������ӿ���
 *
 * ע�ͣ�����ÿһ��2DͼƬ��֧�����ü�������LRU��̭�㷨
 *       ����״̬��
 *               SUCC      ����ͼƬ���سɹ�
 *               FAKE      ����ͼƬ���ڼ���
 *                         ��ʱ����LRU��n��ʱ����̭����ֹ�ظ�����
 *
 *      ���ü���˵����
 *               ���󹹽��ú����ü���Ϊ0��
 *               ��YCTexture���ã���һ
 *               ��YCTexture�ͷţ���һ
 * 
 */
class YCITexture2D
{
public:
		
	enum E_TEXTURE_STATUS{SUCC = 1, FAKE = 2};

	//
	// ������YCITexture2D()
	//
	// Ŀ�ģ�����ʵ��LPTexture����
	//       �ö�������Ч״̬
	//
	// ע�ͣ�
	//       �����ڱ��ؼ��أ�
	//          ����ļ����سɹ���status��ΪSUCC
	//          ����ļ�����ʧ�ܣ�status��ΪFAKE
	//
	YCITexture2D(YCITextureFile *texture, int status);

	virtual ~YCITexture2D(void);

	//
	// ������hasTexture(unsigned int uniqueId)
	//
	// Ŀ�ģ������������Ƿ����ͼƬ
	//
	// ע�ͣ�
	//     1. YCSingleTexture2D ֻ����һ��ͼƬ
	//     2. YCAtlasTexture2D ���ܰ���1..n��ͼƬ
	//
	virtual bool hasTexture(unsigned int uniqueId) const = 0;

	//
	// ������getRect(unsigned int uniqueId, FloatRect& rect)
	//
	// Ŀ�ģ�����������������
	//
	virtual void getRect(unsigned int uniqueId, FloatRect& rect) const = 0;

	//
	// ������status()
	//
	// Ŀ�ģ���ȡ����ǰ״̬
	//
	int status();

	//
	// ������increaseReference()
	//
	// Ŀ�ģ��������ü��������������Ӻ��ֵ
	//
	int increaseReference();

	//
	// ������decreaseReference()
	//
	// Ŀ�ģ��������ü����������ؼ��ٺ��ֵ
	//
	int decreaseReference();

	//
	// ������referenceCount()
	//
	// Ŀ�ģ����ص�ǰ�����ü���ֵ
	//
	int referenceCount();

	//
	// ������isTransparent(unsigned int x, unsigned int y)
	//
	// Ŀ�ģ����Ե�ǰ���Ƿ�͸����������ֵΪ��, ����alpha=255
	//
	bool isTransparent(unsigned int x, unsigned int y);

	//
	// ������raw()
	//
	// Ŀ�ģ�������ʵ����LPDIRECT3DTEXTURE9ָ�룬��YCGraphicʹ��
	//
	void* raw();

	//
	// ������draw()
	//
	// Ŀ�ģ�
	//
	void draw();

	//
	// ������setLatestAccess(time_t timestamp)
	//
	// Ŀ�ģ�����������ʱ��
	//
	void setLatestAccess(time_t timestamp);

	//
	// ������getLatestAccess()
	//
	// Ŀ�ģ���ȡ������ʱ��
	//
	time_t getLatestAccess();

	//
	// ������width()
	//
	// Ŀ�ģ�����������ʵ���
	//
	unsigned int width();

	//
	// ������height()
	//
	// Ŀ�ģ�����������ʵ�߶�
	//
	unsigned int height();
	
protected:

	//������Ч״̬
	int myStatus;

	//�������ü���
	volatile unsigned int myRefCount;
	
	//������ʱ��
	time_t myLatestAccess;
			
	//λͼ���ε���������װ
	YCITextureFile *myTexture; 
};

#endif

