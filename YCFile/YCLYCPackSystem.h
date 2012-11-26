#ifndef _INCLUDE_YCLYCPACKSYSTEM_H_
#define _INCLUDE_YCLYCPACKSYSTEM_H_

#include "YCBasic\YCIPackFileSystem.h"

class YCLYCFactory;

/*
 * YCLYCFileSystem ：后缀名为.lyc的打包文件
 */
class YCAPI YCLYCPackSystem :
	public YCIPackFileSystem
{
public:

	YCLYCPackSystem(void);
	
	virtual ~YCLYCPackSystem(void);

	//
	// 注释：初始化文件系统
	//
	virtual bool initialize();

	//
	// 注释：添加文件搜索路径
	//
	virtual bool mount(const char* regulePath);

	//
	// 注释：取消文件搜索路径
	//
	virtual bool umount(const char* regulePath);

	//
	// 注释：打开一个压缩包文件
	//
	//  内部函数，用于mount时读取，不对外提供op
	//
	virtual bool open(const char *);

	//
	// 注释：关闭一个压缩包文件
	//
	//  内部函数，用于umount时读取，不对外提供op
	//
	virtual bool close(const char *);

	//
	// 注释：创建一个压缩包文件
	//      
	//       op : touch version xxx.ycp
	//
	virtual YCIFilePack* create(const char *, unsigned int);

	//
	// 注释：删除一个压缩包文件
	//       如果当前路径在该压缩包文件中，删除失败
	//
	//       op : rm -f xxx.ycp
	//
	virtual void remove(const char* packFile, bool force);

	//
	// 注释：尝试碎片整理
	//
	//  op ：defrag pathInPack
	//
	virtual void defrag(const char* pathInPack);

	//
	// 注释：添加内存文件到包文件中
	//       pathInPack可以使相对路径，也可以使绝对路径
	//       length > 0 必须大于零
	//
	virtual bool add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool force);

	//
	// 注释：添加一个常规磁盘reguleFile文件到pathInPack路径对应的压缩包文件中
	//       对应路径下有同名文件则看overwrite是否为true
	//       是则覆盖，否则抛出YCPackException异常
	//
	//     op : copy -f c:\pic\xxx.png .
	//
	virtual bool copy(const char* reguleFile, const char* pathInPack, bool overwrite);

	//
	// 注释：从压缩包中删除一个文件，删除失败或文件不存在抛出YCPackException异常
	//     
	//   op : rm /pathInPack/xxx.png
	//
	virtual void erase(const char* fileInPack);

	//
	// 注释：在压缩包中查找文件，pathInPack为查找路径
	//       如果pathInPack为空，则在当前路径下查找
	//       如果pathInPack 为绝对路径，则从根目录下查找
	//       返回查找到的第一个文件，如果没有找到，返回NULL
	// 
	//  op : find 0001.png /ui/0/    <= 绝对路径
	//       find 0001.png           <= 当前路径下
	//
	virtual YCIAbstractFile* find(const char* packFile, const char* pathInPack);

	//
	// 注释：查看一个文件的详细信息
	// 
	//   op : file /pathInPack/xxx.png
	//
	virtual YCIAbstractFile* file(const char* packFile);

	//
	// 注释：dump一个压缩packFile文件到pathInRegule路径对应的磁盘文件系统中
	//       对应路径下有同名文件则看overwrite是否为true
	//       是则覆盖，否则抛出YCPackException异常
	//
	//   op : dump -f /pathInPack/xxx.png c:/pic/
	//
	virtual void dump(const char* packFile, const char* pathInRegule, bool overwrite);

	//
	// 注释：cd 进入文件目录
	//       /开头为绝对路径
	//
	//   op : cd /packFileWithoutExtension/pathInPack/  根目录进入
	//        cd pathInPack     相对路径进入
	//
	virtual bool cd(const char*);

	//
	// 注释：创建子目录
	//       recursive : 是否递归创建 -p
	//
	// op ：mkdir -p /packFileWithoutExtension/pathInPack/newDir  根目录进入
	//      mkdir -p pathUnderCurrentPath/newDir 相对路径进入
	//
	virtual bool mkdir(const char* dirname, bool recursive);

	//
	// 注释：释放文件系统
	//
	virtual void finalize();

private:

	//
	// Pack文件构建器
	//
	YCLYCFactory * myFactory;

	//
	// 说明：已挂接路径
	//
	YCDList * myMounted;

};

#endif

