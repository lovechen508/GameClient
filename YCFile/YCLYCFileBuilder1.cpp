#include "YCLYCFileBuilder1.h"

#include <stdio.h>
#include <memory>

#include "YCLYCFile.h"
#include "YCLYCDefine.h"
#include "YCLYCFilePack.h"
#include "YCLYCFileDirectory.h"

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCScopeGuard.h"
#include "YCBasic\YCBFEncryption.h"
#include "YCBasic\YCZlibCompress.h"
#include "YCBasic\YCIPackPersist.h"
#include "YCBasic\YCStringHelper.h"
#include "YCBasic\YCPackException.h"
#include "YCBasic\YCIFileDirectory.h"

/*
 * fopen��mode�����м�����̬�ַ���:
 *
 *  r  ��ֻ���ļ������ļ�������ڡ�
 *  r+ �򿪿ɶ�д���ļ������ļ�������ڡ�
 *  w  ��ֻд�ļ������ļ��������ļ�������Ϊ0�������ļ����ݻ���ʧ�����ļ��������������ļ���
 *  w+ �򿪿ɶ�д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������������ļ���
 *  a  �Ը��ӵķ�ʽ��ֻд�ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β�����ļ�ԭ�ȵ����ݻᱻ������
 *  a+ �Ը��ӷ�ʽ�򿪿ɶ�д���ļ������ļ������ڣ���Ὠ�����ļ�������ļ����ڣ�д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������
 *
 * ��������̬�ַ����������ټ�һ��b�ַ�����rb��w+b��ab������ϣ�����b �ַ��������ߺ�����򿪵��ļ�Ϊ�������ļ������Ǵ������ļ���
 * ������POSIXϵͳ������Linux������Ը��ַ���
 * ��fopen()�����������ļ������S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH(0666)Ȩ�ޣ����ļ�Ȩ��Ҳ��ο�umaskֵ��
 */ 

#pragma region Persistence

#define PWD "<ulstyle='padding:0;margin:10px;'><listyle='line-height:16px;padding-left:0;margin-left:10px;font-s"

class YCLYCPersist1 : public YCIPackPersist
{
public:

	YCLYCPersist1() 
		: myCompress(new YCZlibCompress())
		, myEncrypt(new YCBFEncryption(PWD))
	{
		SMART_ASSERT(strlen(PWD) == 99);
	}

	virtual ~YCLYCPersist1()
	{
		SAFE_DELETE(myEncrypt);
		SAFE_DELETE(myCompress);
	}

	//
	// д���ļ�ǰ�Ĵ������ܣ�ѹ����
	//
	virtual bool encode(const char* in, unsigned int ilen, char** out, unsigned int& olen)
	{
		if (myCompress->compress(in, ilen, out, olen)) //�����ڴ�
		{
			return myEncrypt->encode(*out, olen, *out, olen); //�͵ؼ���
		}
		return false;
	}

	//
	// �����ļ��Ĵ�����ѹ�����ܵ�
	//
	virtual bool decode(const char* in, unsigned int ilen, char** out, unsigned int& olen)
	{
		char* tmp = new char[ilen];
		SCOPE_GUARD(tmp_guard, delete[] tmp);
		if (myEncrypt->decode(in, ilen, tmp, ilen))
		{
			return myCompress->uncompress(tmp, ilen, out, olen);
		}
		return false;
	}

private:

	YCZlibCompress * myCompress;

	YCBFEncryption * myEncrypt;
};

#pragma endregion Persistence

YCLYCFileBuilder1::YCLYCFileBuilder1(void)
	: YCIPackFileBuilder(1)
	, myPersist(new YCLYCPersist1())
{
}

YCLYCFileBuilder1::~YCLYCFileBuilder1(void)
{
	SAFE_DELETE(myPersist);
}

//
// ע�ͣ���packFile�����ڴ�ӳ��
//       ���packFile�����ڣ�����ָ��λ�ô���
//
YCIFilePack* YCLYCFileBuilder1::createPack(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build�ļ���Ϊ��");
	}

	if (YCFileUtil::IsFileExist(packFile))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build�������ļ�ʧ�ܣ��ļ��Ѵ���", packFile);
	}

	std::auto_ptr<LYC_PACK_T_1> header(new LYC_PACK_T_1());
	memset(header.get(), 0, sizeof(LYC_PACK_T_1));
	memcpy(header->head.magic, FILE_MAGICCODE, FILE_MAGIC_LENGTH);
	header->head.version = 1;
	header->number = FILES_IN_DIR;
		
	FILE * fd = fopen(packFile, "wb+"); //д
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build�����ļ�ʧ��", packFile);
	}

	SCOPE_GUARD(file_guard, fclose(fd));

	//����д���ļ�ͷ
	if (1 != fwrite(header.get(), sizeof(LYC_PACK_T_1), 1, fd))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::buildд���ļ�ͷʧ��");
	}
		
	if (0 != fflush(fd))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build flush�ļ�ͷʧ��");
	}

	if (0 != fclose(fd))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build�ر��ļ�ʧ��");
	}

	fd = fopen(packFile, "rb+");
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build�޸��ļ���ģʽʧ��");
	}

	SCOPE_GIVEUP(file_guard);

	const char* filename = YCFileUtil::GetFileName(packFile);
	std::vector<std::string> strVector;
	YCStringHelper::split(filename, ".", &strVector);
	return new YCLYCFilePack(strVector[0].c_str(), fd, header.release(), this);
	
}

///////////////////////////////////////////
// ���ļ�����
///////////////////////////////////////////
YCIFilePack* YCLYCFileBuilder1::loadPack(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadPack�ļ���Ϊ��");
	}

	FILE * fd = fopen(packFile, "rb+");
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadPack��ȡ�ļ�ʧ��", packFile);
	}

	SCOPE_GUARD(file_guard, fclose(fd));

	// ���Զ����ļ�ͷ
	std::auto_ptr<LYC_PACK_T_1> header(new LYC_PACK_T_1());
	if (1 != fread(header.get(), sizeof(LYC_PACK_T_1), 1, fd))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadPack�����ļ�ͷʧ�ܣ������ļ�����", packFile);
	}

	if (header->head.version != version())
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadPack����汾��һ�£������ļ�����", packFile);
	}

	unsigned int fileSize = YCFileUtil::GetFileLength(packFile);
	if (header->size != (fileSize-sizeof(LYC_PACK_T_1)))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadPack�ļ�����", packFile);
	}

	SCOPE_GIVEUP(file_guard);
	
	//const char* filename = YCFileUtil::GetFileName(packFile);
	//return new YCLYCFilePack(filename, fd, header.release(), this);
	return new YCLYCFilePack(packFile, fd, header.release(), this);
}

bool YCLYCFileBuilder1::writePack(YCIFilePack* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::writePack�ļ�Ϊ��");
	}

	if (packFile->isDirty())
	{
		FILE* fd = packFile->getFd();

		// дͷ��
		if (write(fd, 0, sizeof(LYC_PACK_T_1), packFile->getContent()))
		{
			if (0 != fflush(fd))
			{
				throw YCPackException(1002, "YCIPackFile::flush�ļ��洢ʧ��");
			}
		}
	}	

	return true;
}

///////////////////////////////////////////
// �ļ��в���
///////////////////////////////////////////
YCIFileDirectory* YCLYCFileBuilder1::createDir(YCIFileDirectory* parent, int slot, const char* dirName)
{
	SMART_ASSERT(parent != NULL);
	SMART_ASSERT(dirName != NULL);

	YCLYCFilePack* thePack = (YCLYCFilePack*)parent->getPack();
	LYC_FILE_T_1* header = NULL;
	if (parent->type() == YCIAbstractFile::FILE_PACK)
	{
		header = &((YCLYCFilePack*)parent)->getHeader()->files[slot];
	}
	else
	{
		header = &((YCLYCFileDirectory*)parent)->getHeader()->files[slot];
	}

	std::auto_ptr<YCLYCFileDirectory> dir(new YCLYCFileDirectory(thePack, dirName));

	// ���ļ�...
	std::auto_ptr<LYC_DIR_T_1> files(new LYC_DIR_T_1());
	memset(files.get(), 0, sizeof(LYC_DIR_T_1));

	// 1024 ȡ��
	unsigned int round_length = ROUNDSIZE(sizeof(LYC_DIR_T_1));
	FILE* fd = thePack->getFd();

	YCIFilePack::IDLE_BLOCK* idle = thePack->findAvailableBlock(round_length);
	if (idle == NULL)
	{
		// ׷�ӵ��ļ�β
		unsigned int offset = thePack->getContentLength();
		header->offset = offset + sizeof(LYC_PACK_T_1);
		thePack->setContentLength(offset+round_length);
	}
	else
	{
		// �ļ����Ȳ�������
		unsigned int offset = idle->offset;
		header->offset = offset;

		//����ʣ��
		if (idle->length > round_length)
		{
			idle->offset += round_length;
			idle->length -= round_length;

			thePack->addIdleBlock(idle->offset, idle->length);
		}

		SAFE_DELETE(idle);
	}

	// ���е�ƫ��������������ļ�ͷ
	if (!write(fd, header->offset, sizeof(LYC_DIR_T_1), (const char*)files.get()))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::createDirд���ļ�ʧ��");
	}

	//д���ֵ
	unsigned int fill_length = round_length - sizeof(LYC_DIR_T_1);
	if (!write(fd, header->offset+sizeof(LYC_DIR_T_1), fill_length, FILL_STUFF))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::createDir����ļ�ʧ��");
	}

	// �󶨵�FD
	dir->bind(fd);
	//dir->dirty();

	dir->setSelfHeader(header);
	dir->setParent(parent);
	dir->setHeader(files.release());

	// ������óɹ���־λ
	header->length = sizeof(LYC_DIR_T_1);
	memcpy(header->name, dirName, strlen(dirName));
	header->type = YCIAbstractFile::FILE_DIR;

	return dir.release();
}

YCIFileDirectory* YCLYCFileBuilder1::loadDir(YCIFileDirectory* parent, void * header)
{
	SMART_ASSERT(parent != NULL);
	SMART_ASSERT(header != NULL);
	if (parent == NULL || header == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadDir�ļ�Ϊ��");
	}

	LYC_FILE_T_1* dir = (LYC_FILE_T_1*)header;
	SMART_ASSERT(dir->type == YCIAbstractFile::FILE_DIR);
	
	std::auto_ptr<YCLYCFileDirectory> created(new YCLYCFileDirectory(parent->getPack(), dir->name));
	LYC_DIR_T_1* dirInfo = new LYC_DIR_T_1();
	if (!read(parent->getFd(), dir->offset, sizeof(LYC_DIR_T_1), (char*)dirInfo))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadDir�����ļ�ͷʧ��");
	}

	// �󶨵�FD
	created->bind(parent->getFd());

	// ������óɹ���־λ
	created->setParent(parent);
	created->setSelfHeader(dir);
	created->setHeader(dirInfo);
		
	return created.release();
}

bool YCLYCFileBuilder1::writeDir(YCIFileDirectory* dir)
{
	SMART_ASSERT(dir != NULL);
	if (dir == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::writeDir�ļ�Ϊ��");
	}

	if (dir->isDirty())
	{
		FILE* fd = dir->getFd();

		// дͷ��
		if (write(fd, dir->getOffset(), sizeof(LYC_DIR_T_1), dir->getContent()))
		{
			if (0 != fflush(fd))
			{
				throw YCPackException(1002, "YCIPackFile::writeDir�ļ��洢ʧ��");
			}
		}
	}	

	return true;
}

///////////////////////////////////////////
// ��ͨ�ļ�����
///////////////////////////////////////////
YCIFileNormal* YCLYCFileBuilder1::createNormal(YCIFileDirectory* parent, int slot, const char* filename, const char* content, unsigned int length)
{
	SMART_ASSERT(parent != NULL);
	SMART_ASSERT(slot > 0);
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(content != NULL && length > 0);

	YCLYCFilePack* thePack = (YCLYCFilePack*)parent->getPack();
	LYC_FILE_T_1* header = NULL;
	if (parent->type() == YCIAbstractFile::FILE_PACK)
	{
		header = &((YCLYCFilePack*)parent)->getHeader()->files[slot];
	}
	else
	{
		header = &((YCLYCFileDirectory*)parent)->getHeader()->files[slot];
	}

	char* encrypted = NULL;
	SCOPE_GUARD(encrypted_guard, delete[] encrypted);

	std::auto_ptr<YCLYCFile> file(new YCLYCFile(thePack, filename));
	char* buffer = new char[length];
	memcpy(buffer, content, length);
	file->setContent(buffer);
	file->setLength(length);

	FILE* fd = thePack->getFd();
	if (length > 0)
	{
		if (myPersist != NULL)
		{
			if (!myPersist->encode(content, length, &encrypted, length))
			{
				throw YCPackException(1002, "YCLYCFileBuilder1::createNormal�ļ�����ʧ��", filename);
			}
			buffer = encrypted;
		}

		// 1024 ȡ��
		unsigned int round_length = ROUNDSIZE(length);
		unsigned int fill_length = round_length - length;

		YCIFilePack::IDLE_BLOCK* idle = thePack->findAvailableBlock(round_length);
		if (idle == NULL)
		{
			// ׷�ӵ��ļ�β
			int packLength = thePack->getContentLength();
			header->offset = packLength+sizeof(LYC_PACK_T_1);
			thePack->setContentLength(packLength+round_length);
		}
		else
		{
			// �ļ����Ȳ�������
			unsigned int offset = idle->offset;
			header->offset = offset;

			//����ʣ��
			if (idle->length > round_length)
			{
				idle->offset += round_length;
				idle->length -= round_length;

				thePack->addIdleBlock(idle->offset, idle->length);
			}

			SAFE_DELETE(idle);
		}

		// ���е�ƫ��������������ļ�ͷ
		if (!write(fd, header->offset, length, buffer))
		{
			throw YCPackException(1002, "YCLYCFileBuilder1::createNormalд���ļ�ʧ��");
		}

		//д���ֵ
		if (!write(fd, header->offset+length, fill_length, FILL_STUFF))
		{
			throw YCPackException(1002, "YCLYCFileBuilder1::createDir����ļ�ʧ��");
		}
	}

	// �󶨵�FD
	file->bind(fd);
	//file->dirty();

	file->setSelfHeader(header);
	file->setParent(parent);

	// ������óɹ���־λ
	header->length = length;
	memcpy(header->name, filename, strlen(filename));
	header->type = YCIAbstractFile::FILE_NORMAL;

	return file.release();
}

YCIFileNormal* YCLYCFileBuilder1::createNormal(YCIFileDirectory* parent, int slot, const char* fullPath)
{
	SMART_ASSERT(parent != NULL);
	SMART_ASSERT(fullPath != NULL);

	YCLYCFilePack* thePack = (YCLYCFilePack*)parent->getPack();
	LYC_FILE_T_1* header = NULL;
	if (parent->type() == YCIAbstractFile::FILE_PACK)
	{
		header = &((YCLYCFilePack*)parent)->getHeader()->files[slot];
	}
	else
	{
		header = &((YCLYCFileDirectory*)parent)->getHeader()->files[slot];
	}

	unsigned int length = 0;
	char * buffer = NULL;
	if (!YCFileUtil::LoadFile(fullPath, &buffer, length))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::createNormal�����ļ�ʧ��");
	}

	char* encrypted = NULL;
	SCOPE_GUARD(encrypted_guard, delete[] encrypted);
	const char* filename = YCFileUtil::GetFileName(fullPath);

	std::auto_ptr<YCLYCFile> file(new YCLYCFile(thePack, filename));
	file->setContent(buffer);
	file->setLength(length);

	FILE* fd = thePack->getFd();
	if (length > 0)
	{
		if (myPersist != NULL)
		{
			if (!myPersist->encode(buffer, length, &encrypted, length))
			{
				throw YCPackException(1002, "YCLYCFileBuilder1::createNormal�ļ�����ʧ��", filename);
			}
			buffer = encrypted;
		}
		
		// 1024 ȡ��
		unsigned int round_length = ROUNDSIZE(length);
		unsigned int fill_length = round_length - length;

		YCIFilePack::IDLE_BLOCK* idle = thePack->findAvailableBlock(round_length);
		if (idle == NULL)
		{
			// ׷�ӵ��ļ�β
			int packLength = thePack->getContentLength();
			header->offset = packLength+sizeof(LYC_PACK_T_1);
			thePack->setContentLength(packLength+round_length);
		}
		else
		{
			// �ļ����Ȳ�������
			unsigned int offset = idle->offset;
			header->offset = offset;

			//����ʣ��
			if (idle->length > round_length)
			{
				idle->offset += round_length;
				idle->length -= round_length;

				thePack->addIdleBlock(idle->offset, idle->length);
			}

			SAFE_DELETE(idle);
		}

		// ���е�ƫ��������������ļ�ͷ
		if (!write(fd, header->offset, length, buffer))
		{
			throw YCPackException(1002, "YCLYCFileBuilder1::createNormalд���ļ�ʧ��");
		}

		//д���ֵ
		if (!write(fd, header->offset+length, fill_length, FILL_STUFF))
		{
			throw YCPackException(1002, "YCLYCFileBuilder1::createDir����ļ�ʧ��");
		}
	}
		
	// �󶨵�FD
	file->bind(fd);
	//file->dirty();

	file->setSelfHeader(header);
	file->setParent(parent);

	// ������óɹ���־λ
	header->length = length;
	memcpy(header->name, filename, strlen(filename));
	header->type = YCIAbstractFile::FILE_NORMAL;

	return file.release();
}

YCIFileNormal* YCLYCFileBuilder1::loadNormal(YCIFileDirectory* parent, void * header)
{
	SMART_ASSERT(parent != NULL);
	SMART_ASSERT(header != NULL);
	if (parent == NULL || header == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadDir�ļ�Ϊ��");
	}

	LYC_FILE_T_1* file = (LYC_FILE_T_1*)header;
	SMART_ASSERT(file->type == YCIAbstractFile::FILE_NORMAL);

	char * buffer = NULL;
	unsigned int length = file->length;

	if (length > 0)
	{
		char* encrypted = new char[length];
		SCOPE_GUARD(encrypted_guard, delete[] encrypted);

		if (!read(parent->getFd(), file->offset, length, encrypted))
		{
			throw YCPackException(1002, "YCLYCFileBuilder1::loadNormal�����ļ�ʧ��");
		}

		if (myPersist != NULL)
		{
			if (!myPersist->decode(encrypted, length, &buffer, length))
			{
				throw YCPackException(1002, "YCLYCFileBuilder::loadNormal��ѹ�ļ�ʧ��");
			}
		}
		else
		{
			SCOPE_GIVEUP(encrypted_guard);
			buffer = encrypted;
		}
	}

	YCLYCFile* created = new YCLYCFile(parent->getPack(), file->name);

	// �󶨵�FD
	created->bind(parent->getFd());

	// ������óɹ���־λ
	created->setParent(parent);
	created->setContent(buffer);
	created->setLength(length);

	return created;
}

bool YCLYCFileBuilder1::writeNormal(YCIFileNormal* normal)
{
	SMART_ASSERT(normal != NULL);
	if (normal == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::writeDir�ļ�Ϊ��");
	}

	if (normal->isDirty())
	{
		unsigned int length = normal->getLength();
		if (length > 0)
		{
			FILE* fd = normal->getFd();

			char* encrypted = NULL;
			SCOPE_GUARD(encrypted_guard, delete[] encrypted);

			// �ȼ���
			if (myPersist != NULL)
			{
				if (!myPersist->encode(normal->getContent(), length, &encrypted, length))
				{
					throw YCPackException(1002, "YCLYCFilePack::add�ļ�����ʧ��", normal->filename());
				}
			}

			// д�ļ�
			if (write(fd, normal->getOffset(), length, encrypted))
			{
				if (0 != fflush(fd))
				{
					throw YCPackException(1002, "YCIPackFile::writeDir�ļ��洢ʧ��");
				}
			}
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
// Private Part
/////////////////////////////////////////////////////////////////////

//
// �����ļ��Σ��ͻ��Լ������ڴ�
//
bool YCLYCFileBuilder1::read(FILE* fd, unsigned int offset, unsigned int length, char* out)
{
	if (fd == NULL)
	{
		return false; 
	}

	if (length == 0) return false;
	if (out == NULL) return false;

	if (0 != fseek(fd, offset, SEEK_SET))
	{
		return false; 
	}
	
	unsigned int left = length;
	while (!feof(fd) && left > 0)
	{
		size_t readed = fread(out+(length-left), 1, left, fd);
		int error = ferror(fd);
		if (error != 0)
		{
			return false; 
		}

		left -= readed;
	}

	return (left == 0);
}

//
// д���ļ���
//
bool YCLYCFileBuilder1::write(FILE* fd, unsigned int offset, unsigned int length, const char* src)
{
	if (fd == NULL)
	{
		return false; 
	}

	if (length == 0) return false;
	if (src == NULL) return false;

	if (0 != fseek(fd, offset, SEEK_SET))
	{
		return false; 
	}

	unsigned int left = length;
	while (!feof(fd) && left > 0)
	{
		size_t writed = fwrite(src+(length-left), 1, left, fd);
		int error = ferror(fd);
		if (error != 0)
		{
			return false; 
		}

		left -= writed;
	}

	return (left == 0);
}