#ifndef _INCLUDE_YCFILEUTIL_H_
#define _INCLUDE_YCFILEUTIL_H_

/*
 * YCFileUtil : 文件，目录处理辅助函数
 *
 * 全静态Public函数
 *
 */
class YCAPI YCFileUtil
{
public:

	static const unsigned int MAX_FILELENGTH;

	//
	// 函数：const char* GetExePath(char* buf, int len)
	//
	// 目的：获取当前游戏主目录
	//
	// 参数：
	//       buf    IN,OUT 利于栈上分配
	//              建议分配MAX_PATH长度
	//              函数内部会用0清空
	//
	// 返回值：不带最后的\
	//
	static bool GetExePath(char* buf, unsigned len);

	//
	// 函数：IsFileExist(const char* filename)
	//
	// 目的：判断指定路径下的文件是否存在
	//
	static bool IsFileExist(const char* filename);

	//
	// 取得文件长度
	//
	static int GetFileSize(const char* filename);

	//
	// 函数：IsDir(const char* dirname)
	//
	// 目的：判断指定路径下的文件是否存在及是否是Dir
	//      dirName不能以\\结束！！！
	//
	static bool IsDir(const char* dirname);

	//
	// 函数：CreateDir(const char* fullPath, bool recursive)
	//
	// 目的：在指定位置创建文件夹
	//
	static bool CreateDir(const char* fullPath, bool recursive);

	//
	// 函数：GetFilePath(const char* filename, char* buf, int len)
	//
	// 目的：取得文件所在路径名
	//
	static bool GetFilePath(const char* filename, char* buf, int len);

	//
	// 函数：GetFileName(const char* fullPath)
	//
	// 目的：取得文件名，去除路径
	//
	// 注释：
	//       返回值实际上是移动了fullPath指针位置
	//       如果没有找到\, 返回fullPath
	//
	static const char* GetFileName(const char* fullPath);

	//
	// 函数：GetFileLength(const char* filename)
	//
	// 目的：取得文件长度，如果文件不存在，返回-1
	//
	static int GetFileLength(const char* filename);

	//
	// 函数：GetFileExt(const char* filename);
	//
	// 目的：取得文件扩展名
	//
	// 注释：
	//       返回值实际上是移动了filename指针位置
	//       如果没有扩展名，返回NULL
	//
	static const char* GetFileExt(const char* filename);

	//
	// LoadFile(const char* filename, char** out, unsigned int& length)
	//
	// 目的：加载文件到内存中
	//
	static bool LoadFile(const char* filename, char** out, unsigned int& length);

	//
	// 函数：RemoveFile(const char* filename)
	//
	// 目的：删除文件
	//
	static bool RemoveFile(const char* filename);

	//
	// 函数：handleFileInDir(const char* filter, void(*handle)(const char*, const char*, void*), void* customData)
	//
	// 目的：依次处理目录下的文件
	//
	// 参数：
	//       filter : 文件名过滤器
	//       path : 待处理路径
	//       void*(*handle)(const char* path, const char* filename, void* customData) ：处理函数, 如果返回值不为空，函数立即返回
	//       customData : 自定义参数    
	//
	// 返回值：
	//       返回最后一次handle调用的返回值
	//  
	static void* HandleFileInDir(const char* filter, void*(*handle)(const char*, const char*, void*), void* customData);

};

#endif