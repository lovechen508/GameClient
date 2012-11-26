#include "YCPathWalker.h"

#include "YCAssert.h"

//
// Windows 路径分隔符 \
//
const char YCPathWalker::WINDOW_PATH_DELIMITER = '\\';

//
// Linux/Unit 路径分隔符 /
//
const char YCPathWalker::LINUX_PATH_DELIMITER = '/';

//
// 是否绝对路径
//
//   True ：c:\path\to\check
//          \path\to\check
//
bool YCPathWalker::isAbsolutePath(const char* path)
{
	SMART_ASSERT(path != NULL);

	if (*path == WINDOW_PATH_DELIMITER)  // \path\to\check
	{
		return true;
	}

	if (strlen(path) >= 2)
	{
		if ( (*path >= 'a' && *path <= 'z')
		  || (*path >= 'A' && *path <= 'Z') )
		{
			return (*(path+1) == ':');
		}
	}  

	return false;
}

//
// 构建函数：
//
YCPathWalker::YCPathWalker(const char* path)
	: myPath(path), myDelimiter(WINDOW_PATH_DELIMITER), myPosition(myPath)
{
	SMART_ASSERT(path != NULL);
}

//
// 析构函数：
//
YCPathWalker::~YCPathWalker()
{

}

//
// 取得下一段，返回值是起点，参数也是OUT类型
//
const char* YCPathWalker::next(int& length, E_FILE_TYPE& type)
{
	// 遍历完毕
	if (*myPosition == '\0') 
	{
		return NULL;
	}

	// 是否分隔符开头，跳过
	while (*myPosition == myDelimiter)
	{
		myPosition += 1;
	}
	
	//是否只剩分隔符且被跳过
	if (*myPosition == '\0')
	{
		return NULL;
	}
	
	length = 0;
	const char* i = myPosition; 
	while (*i != '\0' && *i != myDelimiter)
	{
		length += 1;
		++i;
	}

	//是否磁盘驱动器c:
	if (length == 2 && myPosition[1] == ':')
	{
		type = DISK;
	}
	else
	{
		// 下一个是分隔符
		if (*i == myDelimiter)
		{
			type = DIR;
		}
		else // *i == '\0'
		{
			type = BOTH;
		}
	}

	const char* result = myPosition;
	myPosition += length;

	return result;
}

//
// 返回剩余的路径, 是否移除头部的分隔符
//
const char* YCPathWalker::remain(bool trimFrontDelimiter) const
{
	const char* result = myPosition;
	if (trimFrontDelimiter)
	{
		while (*result == myDelimiter)
		{
			result += 1;
		}
	}

	return (*result == '\0' ? NULL : result);
}