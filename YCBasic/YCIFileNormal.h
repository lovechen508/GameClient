#ifndef _INCLUDE_YCIFILENORMAL_H_
#define _INCLUDE_YCIFILENORMAL_H_

#include "YCDef.h"
#include "YCIAbstractFile.h"

class YCIFilePack;
class YCIFileDirectory;

/*
 * YCIFileNormal : YCIPackFile��һ���ļ��ĳ���
 */
class YCAPI YCIFileNormal : public YCIAbstractFile
{
public:

	YCIFileNormal(YCIFilePack* pkg, const char* name);

	virtual ~YCIFileNormal(void);

	//
	// ���ø�Ŀ¼���п�����YCIPackFile
	//
	void setParent(YCIFileDirectory* parent);

	//
	// ��ȡ��Ŀ¼
	//
	YCIFileDirectory* getParent() const;

	//
	// ע�ͣ������ļ�����
	//
	void setLength(int len);

	//
	// ע�ͣ������ļ�����
	//
	virtual int getLength();

	//
	// ȡ���ļ����ڰ�
	//
	YCIFilePack* getPack();

	//
	// �������ݣ����ⲿ�ļ�����ʱ, 
	// �����ڳ�ʼ��ʱ�̶�
	// �ӹ���������
	//
	void setContent(char* buf);

protected:

	//
	// ����Pack�ļ�
	//
	YCIFilePack* myPack;

	//
	// �����ļ���
	//
	YCIFileDirectory * myParent;

	//
	// �ļ�����, �ѽ���
	//
	char* myContent;

	//
	// ���ԣ��ļ�������, �ѽ���
	//
	int myLength;
};

#endif

