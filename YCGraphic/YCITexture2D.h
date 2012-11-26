#ifndef _INCLUDE_YCITEXTURE2D_H_
#define _INCLUDE_YCITEXTURE2D_H_

#include "YCRect.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCSharedPtr.h"

class YCITextureFile;

/*
 * YCITexture2D : 纹理抽象接口类
 *
 * 注释：代表每一张2D图片，支持引用计数，和LRU淘汰算法
 *       三个状态：
 *               SUCC      代表图片加载成功
 *               FAKE      代表图片正在加载
 *                         此时允许LRU以n倍时间淘汰，防止重复尝试
 *
 *      引用计数说明：
 *               对象构建好后，引用计数为0，
 *               被YCTexture引用，加一
 *               被YCTexture释放，减一
 * 
 */
class YCITexture2D
{
public:
		
	enum E_TEXTURE_STATUS{SUCC = 1, FAKE = 2};

	//
	// 函数：YCITexture2D()
	//
	// 目的：绑定真实的LPTexture纹理
	//       置对象于有效状态
	//
	// 注释：
	//       优先在本地加载，
	//          如果文件加载成功，status置为SUCC
	//          如果文件加载失败，status置为FAKE
	//
	YCITexture2D(YCITextureFile *texture, int status);

	virtual ~YCITexture2D(void);

	//
	// 函数：hasTexture(unsigned int uniqueId)
	//
	// 目的：测试纹理中是否包含图片
	//
	// 注释：
	//     1. YCSingleTexture2D 只包含一张图片
	//     2. YCAtlasTexture2D 可能包含1..n张图片
	//
	virtual bool hasTexture(unsigned int uniqueId) const = 0;

	//
	// 函数：getRect(unsigned int uniqueId, FloatRect& rect)
	//
	// 目的：返回纹理所在区域
	//
	virtual void getRect(unsigned int uniqueId, FloatRect& rect) const = 0;

	//
	// 函数：status()
	//
	// 目的：获取纹理当前状态
	//
	int status();

	//
	// 函数：increaseReference()
	//
	// 目的：增加引用计数，并返回增加后的值
	//
	int increaseReference();

	//
	// 函数：decreaseReference()
	//
	// 目的：减少引用计数，并返回减少后的值
	//
	int decreaseReference();

	//
	// 函数：referenceCount()
	//
	// 目的：返回当前的引用计数值
	//
	int referenceCount();

	//
	// 函数：isTransparent(unsigned int x, unsigned int y)
	//
	// 目的：测试当前点是否透明，即像素值为零, 或者alpha=255
	//
	bool isTransparent(unsigned int x, unsigned int y);

	//
	// 函数：raw()
	//
	// 目的：返回真实纹理LPDIRECT3DTEXTURE9指针，供YCGraphic使用
	//
	void* raw();

	//
	// 函数：draw()
	//
	// 目的：
	//
	void draw();

	//
	// 函数：setLatestAccess(time_t timestamp)
	//
	// 目的：更新最后访问时间
	//
	void setLatestAccess(time_t timestamp);

	//
	// 函数：getLatestAccess()
	//
	// 目的：获取最后访问时间
	//
	time_t getLatestAccess();

	//
	// 函数：width()
	//
	// 目的：返回纹理真实宽度
	//
	unsigned int width();

	//
	// 函数：height()
	//
	// 目的：返回纹理真实高度
	//
	unsigned int height();
	
protected:

	//纹理有效状态
	int myStatus;

	//纹理引用计数
	volatile unsigned int myRefCount;
	
	//最后访问时间
	time_t myLatestAccess;
			
	//位图矩形的纹理对象包装
	YCITextureFile *myTexture; 
};

#endif

