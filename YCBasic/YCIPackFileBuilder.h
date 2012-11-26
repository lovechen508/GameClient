#ifndef _INCLUDE_YCIPACKFILEBUILDER_H_
#define _INCLUDE_YCIPACKFILEBUILDER_H_

class YCIFilePack;
class YCIFileNormal;
class YCIFileDirectory;

//
// YCIPackFileBuilder : ��װ���̲���
//
// ˵����
//       ÿ���ļ����Ͷ��������������
//
//       1. �����ڣ��Ӵ��̴���
//       2. �Ѵ��ڣ��Ӵ��̶���
//       3. ���޸ģ�������д��
//
class YCAPI YCIPackFileBuilder
{
public:

	YCIPackFileBuilder(unsigned int version);

	virtual ~YCIPackFileBuilder(void);

	//
	// ע�ͣ�����ָ��λ�ô������ļ�
	//

	///////////////////////////////////////////
	// ���ļ�����
	///////////////////////////////////////////
	virtual YCIFilePack* createPack(const char* packFile) = 0;
	virtual YCIFilePack* loadPack(const char* packFile) = 0;
	virtual bool writePack(YCIFilePack* packFile) = 0;

	///////////////////////////////////////////
	// �ļ��в���
	///////////////////////////////////////////
	virtual YCIFileDirectory* createDir(YCIFileDirectory* parent, int slot, const char* dirName) = 0;
	virtual YCIFileDirectory* loadDir(YCIFileDirectory* parent, void * header) = 0;
	virtual bool writeDir(YCIFileDirectory* dir) = 0;

	///////////////////////////////////////////
	// ��ͨ�ļ�����
	///////////////////////////////////////////
	virtual YCIFileNormal* createNormal(YCIFileDirectory* parent, int slot, const char* filename, const char* content, unsigned int length) = 0;
	virtual YCIFileNormal* createNormal(YCIFileDirectory* parent, int slot, const char* fullPath) = 0;
	virtual YCIFileNormal* loadNormal(YCIFileDirectory* parent, void * header) = 0;
	virtual bool writeNormal(YCIFileNormal* normal) = 0;

	//
	// ע�ͣ�ȡ��Builder��������ļ��汾
	//
	unsigned int version() const;

private:

	//
	// Builder��Ӧ���ļ��汾
	//
	const unsigned int myVersion;

};

#endif

