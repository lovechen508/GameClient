#ifndef _INCLUDE_YCPATHWALKER_H_
#define _INCLUDE_YCPATHWALKER_H_

/**
 *
 * YCPathWalker: ·������������
 *
 * ������
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
	// Windows ·���ָ��� \
	//
	static const char WINDOW_PATH_DELIMITER;

	//
	// Linux/Unit ·���ָ��� /
	//
	static const char LINUX_PATH_DELIMITER;

	//
	// ��ǰ�ļ����ͣ�
	//
	//  DISK : ����
	//	FILE : �ļ�
	//	DIR  : Ŀ¼
	//	BOTH : �������ļ���Ҳ������Ŀ¼
	//
	enum E_FILE_TYPE{DISK, FILE, DIR, BOTH};

public:
		
	//
	// �Ƿ����·��
	//
	//   True ��c:\path\to\check
	//          \path\to\check
	//
	static bool isAbsolutePath(const char* path);

public:

	//
	// ����������
	//
	YCPathWalker(const char* path);

	//
	// ����������
	//
	~YCPathWalker();

	//
	// ȡ����һ�Σ�����ֵ����㣬����Ҳ��OUT����
	// ���ǵ��ļ���һ��������չ�����������һ�ο������ļ�
	// Ҳ������Ŀ¼����ʱtype����BOTH, ��ʱ��һ�α�������NULL
	//
	const char* next(int& length, E_FILE_TYPE& type);

	//
	// ����ʣ���·��, �Ƿ��Ƴ�ͷ���ķָ���
	//
	const char* remain(bool trimFrontDelimiter = true) const;

private:

	//
	// ·���ָ��� 
	//
	const char myDelimiter; 

	//
	// ��ǰ������ȫ·��
	//
	const char* myPath;

	//
	// ��ǰ��������λ��
	//
	const char* myPosition;
};

#endif
