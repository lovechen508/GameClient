#ifndef _INCLUDE_YCLYCFILEDIRECTORY_H_
#define _INCLUDE_YCLYCFILEDIRECTORY_H_

#include "YCLYCDefine.h"
#include "YCBasic\YCIFileDirectory.h"

class YCAPI YCLYCFileDirectory :
	public YCIFileDirectory
{
public:
	
	YCLYCFileDirectory(YCIFilePack* pkg, const char* filename);
	
	virtual ~YCLYCFileDirectory(void);

	//
	// ��������Header
	//
	void setSelfHeader(LYC_FILE_T_1* header);

	//
	// �����ļ�ͷ
	//
	void setHeader(LYC_DIR_T_1* header);

	//
	// ע�ͣ�ȡ���ļ�ͷ
	//
	LYC_DIR_T_1* getHeader();

	//
	// ע�ͣ������ļ�����ָ��
	//
	virtual const char* getContent();

	//
	// ȡ���ļ��ڰ��ļ���ƫ����
	//
	virtual unsigned int getOffset();

	//
	// ע�ͣ������ļ�����
	//
	virtual int getLength();

	//
	// ��Ӵ���reguleFile��Ŀ¼��
	//
	virtual bool add(const char* reguleFile, bool overwrite);

	//
	// ����ڴ��ļ���Ŀ¼��
	//
	virtual bool add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool overwrite);

	//
	// �����Ŀ¼���Ƿ�ݹ�
	//
	virtual bool mkdir(const char* dir, bool recursive);

	//
	// �Ƿ����fileInPack, ���ڷ����ļ����ͣ����򷵻�0
	//
	virtual int has(const char* fileInPack);

	//
	// ��ȡpackFile�ļ�
	//
	virtual YCIAbstractFile* get(const char* packFile);

	//
	// ɾ��packFile�ļ����ҵ��ñ�־λ����
	//
	virtual bool remove(const char* packFile);

	//
	// ɾ��packFile�ļ����ҵ��ñ�־λ����
	//
	virtual bool remove(YCIAbstractFile* packFile);

	//
	// ����ļ����µ������ļ�
	//
	virtual bool clear();

	//
	// ����Ŀ¼�������ļ�
	//
	virtual void dump(const char* targetPath, bool overwrite);

	//
	// �����˸Ķ�֮��ǿ��ˢ�µ�����
	//
	virtual void flush();

	//
	// ���ҿ��е��ļ���λ
	//
	virtual int findAvailableSlot();
	
protected:
	
	//
	// Ŀ¼�µ��ļ��Ƿ��Ѿ����ص��ڴ�
	//
	virtual bool isFileLoaded(unsigned int index);

	//
	// ��ȡ���ص��ڴ��е��ļ�
	//
	virtual YCIAbstractFile* getLoaded(unsigned int index);

	//
	// ��ȡ���ļ����ڴ��еľ���
	//
	YCIAbstractFile* getFile(unsigned int index, const LYC_FILE_T_1& file);

	//
	// �����ļ����ڴ棬����YCIAbstractFile��myFiles
	//
	YCIAbstractFile* loadFile(unsigned int index, const LYC_FILE_T_1& file);

private:

	//
	// �ļ�ͷ�������洢����
	//
	const LYC_FILE_T_1* mySelfHeader;

	//
	// Ŀ¼�е��¼��ļ���������������
	//
	LYC_DIR_T_1* myHeader;

	//
	// �Ѿ����ص��ļ�
	//
	YCIAbstractFile* myFiles[FILES_IN_DIR];

};

#endif