#include "YCFileUtil.h"

#include <string>

#include "YCDef.h"
#include "YCAssert.h"
#include "YCScopeGuard.h"

const unsigned int YCFileUtil::MAX_FILELENGTH = (2<<30);

bool YCFileUtil::GetExePath(char* buf, unsigned len)
{
	SMART_ASSERT(buf != NULL && len >= MAX_PATH);

	memset(buf, 0, len);
	GetModuleFileName(NULL, (LPTSTR)buf, len);
	
	std::string fullname(buf);
	unsigned iPos = fullname.find_last_of('\\');
	if (iPos != std::string::npos)
	{
		if (iPos > (len-1))
		{
			return false;
		}

		memset(buf, 0, len);
		memcpy(buf, fullname.data(), iPos);
		return true;
	}
	return false;
}

//
// 函数：IsFileExist(const char* filename)
//
// 目的：判断指定路径下的文件是否存在
//
bool YCFileUtil::IsFileExist(const char* filename)
{
	SMART_ASSERT(filename != NULL);

	HANDLE hFind;
	WIN32_FIND_DATA wfd;

	hFind = FindFirstFile(filename, &wfd);//出问题了。
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	FindClose(hFind);
	return true; 
}

//
// 函数：IsDir(const char* dirname)
//
// 目的：判断指定路径下的文件是否存在及是否是Dir
//      dirName不能以\\结束！！！
//
bool YCFileUtil::IsDir(const char* dirname)
{
	SMART_ASSERT(dirname != NULL);

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(dirname, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		return true;
	}
	FindClose(hFind);
	return false;
}

//
// 函数：CreateDir(const char* fullPath, bool recursive)
//
// 目的：在指定位置创建文件夹
//
bool YCFileUtil::CreateDir(const char* fullPath, bool recursive)
{
	SMART_ASSERT(fullPath != NULL);
	
	bool result = true;

	if (recursive) 
	{
		char dirPath[MAX_PATH] = {0};
		sprintf(dirPath,"%s",fullPath);

		// c:\liao\zan\xu\ => c:\liao\zan\xu
		for(int i=strlen(dirPath); i > 2; i--)
		{
			if(dirPath[i-1] == '\\')
			{
				dirPath[i-1] = '\0'; 
				break;
			}
		}

		// c:\liao\zan\xu => c:\liao\zan
		for(int i=strlen(dirPath); i > 2; i--)
		{
			if(dirPath[i-1] == '\\')
			{
				dirPath[i-1] = '\0'; 
				break;
			}
		}

		if(GetFileAttributes(dirPath) == 0xffffffff)
		{
			// c: 跳过磁盘驱动器
			if(strlen(dirPath) > 2) 
			{
				result = CreateDir(dirPath, recursive);
			}
		}
	}
	
	if (IsFileExist(fullPath))
	{
		if (!IsDir(fullPath))
		{
			result = false;
		}
	}
	else
	{
		result = (CreateDirectory(fullPath, NULL) == TRUE);
	}

	return result;
}

//
// 函数：GetFilePath(const char* filename, char* buf, int len)
//
// 目的：取得文件所在路径名
//
bool YCFileUtil::GetFilePath(const char* filename, char* buf, int len)
{
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(buf != NULL && len > 0);

	std::string strFilename(filename);
	unsigned iPos = strFilename.find_last_of('\\');
	if (iPos != std::string::npos)
	{
		if (iPos > (unsigned)(len-1))
		{
			return false;
		}

		memset(buf, 0, len);
		memcpy(buf, strFilename.data(), iPos);
		return true;
	}
	return false;
}

//
// 函数：GetFileName(const char* fullPath)
//
// 目的：取得文件名，去除路径
//
const char* YCFileUtil::GetFileName(const char* fullPath)
{
	SMART_ASSERT(fullPath != NULL);
	const char* pSlash = strrchr(fullPath, '\\');
	return pSlash ? pSlash+1 : fullPath;
}

//
// 函数：GetFileLength(const char* filename)
//
// 目的：取得文件长度，如果文件不存在，返回-1
//
int YCFileUtil::GetFileLength(const char* filename)
{
	WIN32_FILE_ATTRIBUTE_DATA   fileInfo;

	SMART_ASSERT(NULL != filename);
	if (!GetFileAttributesEx(filename, GetFileExInfoStandard, (void*)&fileInfo))
	{
		return -1;
	}

	SMART_ASSERT(0 == fileInfo.nFileSizeHigh);
	return (int)fileInfo.nFileSizeLow;
}

//
// 函数：GetFileExt(const char* filename);
//
// 目的：取得文件扩展名
//
// 注释：
//       返回值实际上是移动了filename指针位置
//       如果没有扩展名，返回NULL
//
const char* YCFileUtil::GetFileExt(const char* filename)
{
	SMART_ASSERT(filename != NULL);

	int rBegin = strlen(filename) - 1;
	for (int i = rBegin; i >= 0; --i)
	{
		if (filename[i] == '.')
		{
			return filename+i+1;  //+1越过'.'
		}
	}
	return NULL;
}

//
// LoadFile(const char* filename, char** out, unsigned int& length)
//
// 目的：加载文件到内存中
//
bool YCFileUtil::LoadFile(const char* filename, char** out, unsigned int& length)
{
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(out != NULL);

	if (!IsFileExist(filename))
	{
		return false;
	}

	HANDLE pfile = ::CreateFile(filename, GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
	if (pfile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	SCOPE_GUARD(fd_guard, ::CloseHandle(pfile));

	length = ::GetFileSize(pfile, NULL);
	*out = new char[length + 1];
	
	SCOPE_GUARD(out_guard, delete[] *out);

	unsigned int readSize = 0;
	while (readSize < length)
	{
		DWORD position = readSize;
		if (!::ReadFile(pfile, *out+readSize, length-readSize, &position, NULL))
		{
			return false;
		}
		readSize += position;
	}

	(*out)[length] = 0;
	
	SCOPE_GIVEUP(out_guard);

	return true;
}

//
// 函数：RemoveFile(const char* filename)
//
// 目的：删除文件
//
bool YCFileUtil::RemoveFile(const char* filename)
{
	SMART_ASSERT(filename != NULL);
	if (!IsFileExist(filename))
	{
		return false;
	}

	return (::DeleteFile(filename) == TRUE);
}


//
// 函数：handleFileInDir(const char* filter, void(*handle)(const char*, const char*, void*), void* customData)
//
// 目的：依次处理目录下的文件
//
void*  YCFileUtil::HandleFileInDir(const char* filter, void*(*handle)(const char*, const char*, void*), void* customData)
{
	SMART_ASSERT(filter != NULL);
	SMART_ASSERT(handle != NULL);

	WIN32_FIND_DATA wfd;

	HANDLE hFind = FindFirstFile(filter, &wfd);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return false;
	} 
   
	char path[MAX_PATH] = {0};
	if (!GetFilePath(filter, path, MAX_PATH))
	{
		return false;
	}

	void *result = NULL;
	do {
		// .
		if (strlen(wfd.cFileName) == 1 && strcmp(wfd.cFileName, ".") == 0)
		{
			continue;
		}

		// ..
		if (strlen(wfd.cFileName) == 2 && strcmp(wfd.cFileName, "..") == 0)
		{
			continue;
		}

		result = handle(path, wfd.cFileName, customData);
		if (result != NULL)
		{
			return result;
		}

	} while (FindNextFile(hFind, &wfd));
    
	DWORD dwError = GetLastError();
	if (dwError == ERROR_NO_MORE_FILES)
	{

	}
	FindClose(hFind);

    return result;
}