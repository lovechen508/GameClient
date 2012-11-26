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
// ������IsFileExist(const char* filename)
//
// Ŀ�ģ��ж�ָ��·���µ��ļ��Ƿ����
//
bool YCFileUtil::IsFileExist(const char* filename)
{
	SMART_ASSERT(filename != NULL);

	HANDLE hFind;
	WIN32_FIND_DATA wfd;

	hFind = FindFirstFile(filename, &wfd);//�������ˡ�
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	FindClose(hFind);
	return true; 
}

//
// ������IsDir(const char* dirname)
//
// Ŀ�ģ��ж�ָ��·���µ��ļ��Ƿ���ڼ��Ƿ���Dir
//      dirName������\\����������
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
// ������CreateDir(const char* fullPath, bool recursive)
//
// Ŀ�ģ���ָ��λ�ô����ļ���
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
			// c: ��������������
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
// ������GetFilePath(const char* filename, char* buf, int len)
//
// Ŀ�ģ�ȡ���ļ�����·����
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
// ������GetFileName(const char* fullPath)
//
// Ŀ�ģ�ȡ���ļ�����ȥ��·��
//
const char* YCFileUtil::GetFileName(const char* fullPath)
{
	SMART_ASSERT(fullPath != NULL);
	const char* pSlash = strrchr(fullPath, '\\');
	return pSlash ? pSlash+1 : fullPath;
}

//
// ������GetFileLength(const char* filename)
//
// Ŀ�ģ�ȡ���ļ����ȣ�����ļ������ڣ�����-1
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
// ������GetFileExt(const char* filename);
//
// Ŀ�ģ�ȡ���ļ���չ��
//
// ע�ͣ�
//       ����ֵʵ�������ƶ���filenameָ��λ��
//       ���û����չ��������NULL
//
const char* YCFileUtil::GetFileExt(const char* filename)
{
	SMART_ASSERT(filename != NULL);

	int rBegin = strlen(filename) - 1;
	for (int i = rBegin; i >= 0; --i)
	{
		if (filename[i] == '.')
		{
			return filename+i+1;  //+1Խ��'.'
		}
	}
	return NULL;
}

//
// LoadFile(const char* filename, char** out, unsigned int& length)
//
// Ŀ�ģ������ļ����ڴ���
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
// ������RemoveFile(const char* filename)
//
// Ŀ�ģ�ɾ���ļ�
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
// ������handleFileInDir(const char* filter, void(*handle)(const char*, const char*, void*), void* customData)
//
// Ŀ�ģ����δ���Ŀ¼�µ��ļ�
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