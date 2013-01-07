#ifndef _INCLUDE_YCFILEUTIL_H_
#define _INCLUDE_YCFILEUTIL_H_

/*
 * YCFileUtil : �ļ���Ŀ¼����������
 *
 * ȫ��̬Public����
 *
 */
class YCAPI YCFileUtil
{
public:

	static const unsigned int MAX_FILELENGTH;

	//
	// ������const char* GetExePath(char* buf, int len)
	//
	// Ŀ�ģ���ȡ��ǰ��Ϸ��Ŀ¼
	//
	// ������
	//       buf    IN,OUT ����ջ�Ϸ���
	//              �������MAX_PATH����
	//              �����ڲ�����0���
	//
	// ����ֵ����������\
	//
	static bool GetExePath(char* buf, unsigned len);

	//
	// ������IsFileExist(const char* filename)
	//
	// Ŀ�ģ��ж�ָ��·���µ��ļ��Ƿ����
	//
	static bool IsFileExist(const char* filename);

	//
	// ȡ���ļ�����
	//
	static int GetFileSize(const char* filename);

	//
	// ������IsDir(const char* dirname)
	//
	// Ŀ�ģ��ж�ָ��·���µ��ļ��Ƿ���ڼ��Ƿ���Dir
	//      dirName������\\����������
	//
	static bool IsDir(const char* dirname);

	//
	// ������CreateDir(const char* fullPath, bool recursive)
	//
	// Ŀ�ģ���ָ��λ�ô����ļ���
	//
	static bool CreateDir(const char* fullPath, bool recursive);

	//
	// ������GetFilePath(const char* filename, char* buf, int len)
	//
	// Ŀ�ģ�ȡ���ļ�����·����
	//
	static bool GetFilePath(const char* filename, char* buf, int len);

	//
	// ������GetFileName(const char* fullPath)
	//
	// Ŀ�ģ�ȡ���ļ�����ȥ��·��
	//
	// ע�ͣ�
	//       ����ֵʵ�������ƶ���fullPathָ��λ��
	//       ���û���ҵ�\, ����fullPath
	//
	static const char* GetFileName(const char* fullPath);

	//
	// ������GetFileLength(const char* filename)
	//
	// Ŀ�ģ�ȡ���ļ����ȣ�����ļ������ڣ�����-1
	//
	static int GetFileLength(const char* filename);

	//
	// ������GetFileExt(const char* filename);
	//
	// Ŀ�ģ�ȡ���ļ���չ��
	//
	// ע�ͣ�
	//       ����ֵʵ�������ƶ���filenameָ��λ��
	//       ���û����չ��������NULL
	//
	static const char* GetFileExt(const char* filename);

	//
	// LoadFile(const char* filename, char** out, unsigned int& length)
	//
	// Ŀ�ģ������ļ����ڴ���
	//
	static bool LoadFile(const char* filename, char** out, unsigned int& length);

	//
	// ������RemoveFile(const char* filename)
	//
	// Ŀ�ģ�ɾ���ļ�
	//
	static bool RemoveFile(const char* filename);

	//
	// ������handleFileInDir(const char* filter, void(*handle)(const char*, const char*, void*), void* customData)
	//
	// Ŀ�ģ����δ���Ŀ¼�µ��ļ�
	//
	// ������
	//       filter : �ļ���������
	//       path : ������·��
	//       void*(*handle)(const char* path, const char* filename, void* customData) ��������, �������ֵ��Ϊ�գ�������������
	//       customData : �Զ������    
	//
	// ����ֵ��
	//       �������һ��handle���õķ���ֵ
	//  
	static void* HandleFileInDir(const char* filter, void*(*handle)(const char*, const char*, void*), void* customData);

};

#endif