#ifndef _INCLUDE_YCLYCFILE_H_
#define _INCLUDE_YCLYCFILE_H_

#include "YCBasic\YCIFileNormal.h"

struct LYC_FILE_T_1;

class YCAPI YCLYCFile :
	public YCIFileNormal
{
public:
	
	YCLYCFile(YCIFilePack* pkg, const char* filename);
	
	virtual ~YCLYCFile(void);

	//
	// ��������Header
	//
	void setSelfHeader(LYC_FILE_T_1* header);

	//
	// �����˸Ķ�֮��ǿ��ˢ�µ�����
	//
	virtual void flush();

	//
	// ע�ͣ������ļ�����ָ��
	//
	virtual const char* getContent();

	//
	// ȡ���ļ��ڰ��ļ���ƫ����
	//
	virtual unsigned int getOffset();
	
	//
	// ���������ļ�
	//
	virtual void dump(const char* targetPath, bool overwrite);

private:

	//
	// �ļ�ͷ�������洢����
	//
	LYC_FILE_T_1* mySelfHeader;
};

#endif