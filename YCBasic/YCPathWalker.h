#ifndef _INCLUDE_YCPATHWALKER_H_
#define _INCLUDE_YCPATHWALKER_H_

/**
 *
 * YCPathWalker: 路径遍历助手类
 *
 * 用例：
 *       c:\path\to\walk
 *       c:\path\to\walk\
 *       c:\path\to\walk\file.ext
 *
 *       path\to\walk
 *       path\to\walk\
 *       path\to\walk\file.ext
 */
class YCAPI YCPathWalker
{
public:
	
	//
	// Windows 路径分隔符 \
	//
	static const char WINDOW_PATH_DELIMITER;

	//
	// Linux/Unit 路径分隔符 /
	//
	static const char LINUX_PATH_DELIMITER;

	//
	// 当前文件类型：
	//
	//  DISK : 磁盘
	//	FILE : 文件
	//	DIR  : 目录
	//	BOTH : 可能是文件，也可能是目录
	//
	enum E_FILE_TYPE{DISK, FILE, DIR, BOTH};

public:
		
	//
	// 是否绝对路径
	//
	//   True ：c:\path\to\check
	//          \path\to\check
	//
	static bool isAbsolutePath(const char* path);

public:

	//
	// 构建函数：
	//
	YCPathWalker(const char* path);

	//
	// 析构函数：
	//
	~YCPathWalker();

	//
	// 取得下一段，返回值是起点，参数也是OUT类型
	// 考虑到文件不一定都带扩展名，所以最后一段可能是文件
	// 也可能是目录，此时type返回BOTH, 此时下一次遍历返回NULL
	//
	const char* next(int& length, E_FILE_TYPE& type);

	//
	// 返回剩余的路径, 是否移除头部的分隔符
	//
	const char* remain(bool trimFrontDelimiter = true) const;

private:

	//
	// 路径分隔符 
	//
	const char myDelimiter; 

	//
	// 当前解析的全路径
	//
	const char* myPath;

	//
	// 当前解析到的位置
	//
	const char* myPosition;
};

#endif
