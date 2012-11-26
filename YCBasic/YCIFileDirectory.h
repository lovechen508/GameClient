#ifndef _INCLUDEYCIFILEDIRECTORY_H_
#define _INCLUDEYCIFILEDIRECTORY_H_

#include "YCIAbstractFile.h"


class YCDList;
class YCIFilePack;
class YCIFileNormal;

class YCAPI YCIFileDirectory : public YCIAbstractFile
{
public:

	YCIFileDirectory(YCIFilePack* pkg, const char* name, int type=YCIAbstractFile::FILE_DIR);

	virtual ~YCIFileDirectory(void);

	//
	// ȡ���ļ����ڰ�
	//
	YCIFilePack* getPack();

	//
	// ���ø�Ŀ¼���п�����YCIPackFile
	//
	void setParent(YCIFileDirectory* parent);

	//
	// ��ȡ��Ŀ¼
	//
	YCIFileDirectory* getParent() const;

	//
	// ��Ӵ���reguleFile��Ŀ¼��
	//
	virtual bool add(const char* reguleFile, bool overwrite) = 0;

	//
	// ����ڴ��ļ���Ŀ¼��
	//
	virtual bool add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool overwrite) = 0;

	//
	// �����Ŀ¼���Ƿ�ݹ�
	//
	virtual bool mkdir(const char* dir, bool recursive) = 0;

	//
	// �Ƿ����fileInPack, ���ڷ����ļ����ͣ����򷵻�0
	//
	virtual int has(const char* fileInPack) = 0;

	//
	// ��ȡpackFile�ļ�
	//
	virtual YCIAbstractFile* get(const char* packFile) = 0;

	//
	// ɾ��packFile�ļ����ҵ��ñ�־λ����
	//
	virtual bool remove(const char* packFile) = 0;

	//
	// ɾ��packFile�ļ����ҵ��ñ�־λ����
	//
	virtual bool remove(YCIAbstractFile* packFile) = 0;

	//
	// ����ļ����µ������ļ�
	//
	virtual bool clear() = 0;

	//
	// ���ҿ��е��ļ���λ
	//
	virtual int findAvailableSlot() = 0;

protected:

	//
	// Ŀ¼�µ��ļ��Ƿ��Ѿ����ص��ڴ�
	//
	virtual bool isFileLoaded(unsigned int index) = 0;

	//
	// ��ȡ���ص��ڴ��е��ļ�
	//
	virtual YCIAbstractFile* getLoaded(unsigned int index) = 0;

protected:

	//
	// �ϼ�Ŀ¼
	//
	YCIFileDirectory* myParent;

	//
	// ����Pack�ļ�
	//
	YCIFilePack* myPack;
};

#endif


