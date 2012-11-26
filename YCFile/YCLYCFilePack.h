#ifndef _INCLUDE_YCLYCFILEPACK_H_
#define _INCLUDE_YCLYCFILEPACK_H_

#include "YCBasic\YCIFilePack.h"
#include "YCLYCDefine.h"

class YCDList;

//
// YCLYCFilePack : LYC ���ļ�
//
// ˵����
//       myLength = �ļ��ܳ��� - sizeof(LYC_PACK_T_1)
//                  ���ļ�β׷����Ҫ�������ֵ
//
class YCAPI YCLYCFilePack :
	public YCIFilePack
{
public:
	
	YCLYCFilePack(const char* name, FILE* fd, LYC_PACK_T_1* header, YCIPackFileBuilder* builder);
	
	virtual ~YCLYCFilePack(void);

	//
	// ȡ�ð���
	//
	virtual const char* getPackName();

	//
	// ע�ͣ�ȡ���ļ�ͷ
	//
	LYC_PACK_T_1* getHeader();

	//
	// ע�ͣ������ļ�ͷHeader����
	//
	virtual int getLength();

	//
	// ע�ͣ������ļ�����ָ��
	//
	virtual const char* getContent();

	//
	// ע�ͣ������ļ���ͷ������������
	//
	virtual void setContentLength(int contentLen);

	//
	// ע�ͣ���ȡ�ļ���ͷ������������
	//
	virtual int getContentLength();

	//
	// ע�ͣ���Ӵ���reguleFile��Pack����
	//
	virtual bool add(const char* reguleFile, bool overwrite);

	//
	// ע�ͣ�����ڴ��ļ���Pack����
	//
	virtual bool add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool overwrite);

	//
	// ע�ͣ������Ŀ¼���Ƿ�ݹ�
	//
	virtual bool mkdir(const char* dir, bool recursive);

	//
	// ע�ͣ��Ƿ����fileInPack
	//
	virtual int has(const char* fileInPack);

	//
	// ע�ͣ���ȡpackFile�ļ�
	//
	virtual YCIAbstractFile* get(const char* packFile);

	//
	// ע�ͣ�ɾ��packFile�ļ����ҵ��ñ�־λ����
	//
	virtual bool remove(const char* packFile);

	//
	// ע�ͣ�ɾ��packFile�ļ����ҵ��ñ�־λ����
	//
	virtual bool remove(YCIAbstractFile* packFile);

	//
	// ע�ͣ�����ļ����µ������ļ�
	//
	virtual bool clear();

	//
	// ע�ͣ�����Pack���������ļ�
	//
	virtual void dump(const char* targetPath, bool overwrite);
	
	//
	// ע�ͣ���PackFile���˸Ķ�֮��ǿ��ˢ�µ�����
	//
	virtual void flush();

	//
	// ע�ͣ����ҿ��е��ļ���λ
	//
	virtual int findAvailableSlot();
	
	//
	// �ҽӿ��п�
	//
	virtual void addIdleBlock(unsigned int offset, unsigned int length);

	//
	// ���ҿ��е��ļ���
	//
	virtual IDLE_BLOCK* findAvailableBlock(unsigned int required);

protected:

	//
	// ע�ͣ�Ŀ¼�µ��ļ��Ƿ��Ѿ����ص��ڴ�
	//
	virtual bool isFileLoaded(unsigned int index);

	//
	// ע�ͣ���ȡ���ص��ڴ��е��ļ�
	//
	virtual YCIAbstractFile* getLoaded(unsigned int index);

	//
	// ע�ͣ���ȡ���ļ����ڴ��еľ���
	//
	YCIAbstractFile* getFile(unsigned int index, const LYC_FILE_T_1& file);

	//
	// ע�ͣ������ļ����ڴ棬����YCIAbstractFile��myFiles
	//
	YCIAbstractFile* loadFile(unsigned int index, const LYC_FILE_T_1& file);

private:

	//
	// ������ȥ���ļ�·������չ��
	//
	char myPackName[FILENAME_LENGTH];
	
	//
	// �ļ��飬������������
	//
	LYC_PACK_T_1* myHeader;

	//
	// �Ѿ����ص��ļ�
	//
	YCIAbstractFile* myFiles[FILES_IN_DIR];
};

#endif

