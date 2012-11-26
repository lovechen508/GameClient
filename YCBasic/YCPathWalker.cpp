#include "YCPathWalker.h"

#include "YCAssert.h"

//
// Windows ·���ָ��� \
//
const char YCPathWalker::WINDOW_PATH_DELIMITER = '\\';

//
// Linux/Unit ·���ָ��� /
//
const char YCPathWalker::LINUX_PATH_DELIMITER = '/';

//
// �Ƿ����·��
//
//   True ��c:\path\to\check
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
// ����������
//
YCPathWalker::YCPathWalker(const char* path)
	: myPath(path), myDelimiter(WINDOW_PATH_DELIMITER), myPosition(myPath)
{
	SMART_ASSERT(path != NULL);
}

//
// ����������
//
YCPathWalker::~YCPathWalker()
{

}

//
// ȡ����һ�Σ�����ֵ����㣬����Ҳ��OUT����
//
const char* YCPathWalker::next(int& length, E_FILE_TYPE& type)
{
	// �������
	if (*myPosition == '\0') 
	{
		return NULL;
	}

	// �Ƿ�ָ�����ͷ������
	while (*myPosition == myDelimiter)
	{
		myPosition += 1;
	}
	
	//�Ƿ�ֻʣ�ָ����ұ�����
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

	//�Ƿ����������c:
	if (length == 2 && myPosition[1] == ':')
	{
		type = DISK;
	}
	else
	{
		// ��һ���Ƿָ���
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
// ����ʣ���·��, �Ƿ��Ƴ�ͷ���ķָ���
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