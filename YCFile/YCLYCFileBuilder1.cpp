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
 * fopen中mode有下列几种形态字符串:
 *
 *  r  打开只读文件，该文件必须存在。
 *  r+ 打开可读写的文件，该文件必须存在。
 *  w  打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
 *  w+ 打开可读写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。
 *  a  以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。
 *  a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。
 *
 * 上述的形态字符串都可以再加一个b字符，如rb、w+b或ab＋等组合，加入b 字符用来告诉函数库打开的文件为二进制文件，而非纯文字文件。
 * 不过在POSIX系统，包含Linux都会忽略该字符。
 * 由fopen()所建立的新文件会具有S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH(0666)权限，此文件权限也会参考umask值。
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
	// 写入文件前的处理，加密，压缩等
	//
	virtual bool encode(const char* in, unsigned int ilen, char** out, unsigned int& olen)
	{
		if (myCompress->compress(in, ilen, out, olen)) //分配内存
		{
			return myEncrypt->encode(*out, olen, *out, olen); //就地加密
		}
		return false;
	}

	//
	// 读出文件的处理，解压，解密等
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
// 注释：从packFile构建内存映像，
//       如果packFile不存在，则在指定位置创建
//
YCIFilePack* YCLYCFileBuilder1::createPack(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build文件名为空");
	}

	if (YCFileUtil::IsFileExist(packFile))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build创建包文件失败，文件已存在", packFile);
	}

	std::auto_ptr<LYC_PACK_T_1> header(new LYC_PACK_T_1());
	memset(header.get(), 0, sizeof(LYC_PACK_T_1));
	memcpy(header->head.magic, FILE_MAGICCODE, FILE_MAGIC_LENGTH);
	header->head.version = 1;
	header->number = FILES_IN_DIR;
		
	FILE * fd = fopen(packFile, "wb+"); //写
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build创建文件失败", packFile);
	}

	SCOPE_GUARD(file_guard, fclose(fd));

	//尝试写入文件头
	if (1 != fwrite(header.get(), sizeof(LYC_PACK_T_1), 1, fd))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build写入文件头失败");
	}
		
	if (0 != fflush(fd))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build flush文件头失败");
	}

	if (0 != fclose(fd))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build关闭文件失败");
	}

	fd = fopen(packFile, "rb+");
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::build修改文件打开模式失败");
	}

	SCOPE_GIVEUP(file_guard);

	const char* filename = YCFileUtil::GetFileName(packFile);
	std::vector<std::string> strVector;
	YCStringHelper::split(filename, ".", &strVector);
	return new YCLYCFilePack(strVector[0].c_str(), fd, header.release(), this);
	
}

///////////////////////////////////////////
// 包文件操作
///////////////////////////////////////////
YCIFilePack* YCLYCFileBuilder1::loadPack(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadPack文件名为空");
	}

	FILE * fd = fopen(packFile, "rb+");
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadPack读取文件失败", packFile);
	}

	SCOPE_GUARD(file_guard, fclose(fd));

	// 尝试读入文件头
	std::auto_ptr<LYC_PACK_T_1> header(new LYC_PACK_T_1());
	if (1 != fread(header.get(), sizeof(LYC_PACK_T_1), 1, fd))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadPack读入文件头失败，可能文件已损坏", packFile);
	}

	if (header->head.version != version())
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadPack读入版本不一致，可能文件已损坏", packFile);
	}

	unsigned int fileSize = YCFileUtil::GetFileLength(packFile);
	if (header->size != (fileSize-sizeof(LYC_PACK_T_1)))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadPack文件已损坏", packFile);
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
		throw YCPackException(1002, "YCLYCFileBuilder1::writePack文件为空");
	}

	if (packFile->isDirty())
	{
		FILE* fd = packFile->getFd();

		// 写头部
		if (write(fd, 0, sizeof(LYC_PACK_T_1), packFile->getContent()))
		{
			if (0 != fflush(fd))
			{
				throw YCPackException(1002, "YCIPackFile::flush文件存储失败");
			}
		}
	}	

	return true;
}

///////////////////////////////////////////
// 文件夹操作
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

	// 子文件...
	std::auto_ptr<LYC_DIR_T_1> files(new LYC_DIR_T_1());
	memset(files.get(), 0, sizeof(LYC_DIR_T_1));

	// 1024 取整
	unsigned int round_length = ROUNDSIZE(sizeof(LYC_DIR_T_1));
	FILE* fd = thePack->getFd();

	YCIFilePack::IDLE_BLOCK* idle = thePack->findAvailableBlock(round_length);
	if (idle == NULL)
	{
		// 追加到文件尾
		unsigned int offset = thePack->getContentLength();
		header->offset = offset + sizeof(LYC_PACK_T_1);
		thePack->setContentLength(offset+round_length);
	}
	else
	{
		// 文件长度不会增加
		unsigned int offset = idle->offset;
		header->offset = offset;

		//尚有剩余
		if (idle->length > round_length)
		{
			idle->offset += round_length;
			idle->length -= round_length;

			thePack->addIdleBlock(idle->offset, idle->length);
		}

		SAFE_DELETE(idle);
	}

	// 所有的偏移量都是相对于文件头
	if (!write(fd, header->offset, sizeof(LYC_DIR_T_1), (const char*)files.get()))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::createDir写入文件失败");
	}

	//写填充值
	unsigned int fill_length = round_length - sizeof(LYC_DIR_T_1);
	if (!write(fd, header->offset+sizeof(LYC_DIR_T_1), fill_length, FILL_STUFF))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::createDir填充文件失败");
	}

	// 绑定到FD
	dir->bind(fd);
	//dir->dirty();

	dir->setSelfHeader(header);
	dir->setParent(parent);
	dir->setHeader(files.release());

	// 最后设置成功标志位
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
		throw YCPackException(1002, "YCLYCFileBuilder1::loadDir文件为空");
	}

	LYC_FILE_T_1* dir = (LYC_FILE_T_1*)header;
	SMART_ASSERT(dir->type == YCIAbstractFile::FILE_DIR);
	
	std::auto_ptr<YCLYCFileDirectory> created(new YCLYCFileDirectory(parent->getPack(), dir->name));
	LYC_DIR_T_1* dirInfo = new LYC_DIR_T_1();
	if (!read(parent->getFd(), dir->offset, sizeof(LYC_DIR_T_1), (char*)dirInfo))
	{
		throw YCPackException(1002, "YCLYCFileBuilder1::loadDir加载文件头失败");
	}

	// 绑定到FD
	created->bind(parent->getFd());

	// 最后设置成功标志位
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
		throw YCPackException(1002, "YCLYCFileBuilder1::writeDir文件为空");
	}

	if (dir->isDirty())
	{
		FILE* fd = dir->getFd();

		// 写头部
		if (write(fd, dir->getOffset(), sizeof(LYC_DIR_T_1), dir->getContent()))
		{
			if (0 != fflush(fd))
			{
				throw YCPackException(1002, "YCIPackFile::writeDir文件存储失败");
			}
		}
	}	

	return true;
}

///////////////////////////////////////////
// 普通文件操作
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
				throw YCPackException(1002, "YCLYCFileBuilder1::createNormal文件加密失败", filename);
			}
			buffer = encrypted;
		}

		// 1024 取整
		unsigned int round_length = ROUNDSIZE(length);
		unsigned int fill_length = round_length - length;

		YCIFilePack::IDLE_BLOCK* idle = thePack->findAvailableBlock(round_length);
		if (idle == NULL)
		{
			// 追加到文件尾
			int packLength = thePack->getContentLength();
			header->offset = packLength+sizeof(LYC_PACK_T_1);
			thePack->setContentLength(packLength+round_length);
		}
		else
		{
			// 文件长度不会增加
			unsigned int offset = idle->offset;
			header->offset = offset;

			//尚有剩余
			if (idle->length > round_length)
			{
				idle->offset += round_length;
				idle->length -= round_length;

				thePack->addIdleBlock(idle->offset, idle->length);
			}

			SAFE_DELETE(idle);
		}

		// 所有的偏移量都是相对于文件头
		if (!write(fd, header->offset, length, buffer))
		{
			throw YCPackException(1002, "YCLYCFileBuilder1::createNormal写入文件失败");
		}

		//写填充值
		if (!write(fd, header->offset+length, fill_length, FILL_STUFF))
		{
			throw YCPackException(1002, "YCLYCFileBuilder1::createDir填充文件失败");
		}
	}

	// 绑定到FD
	file->bind(fd);
	//file->dirty();

	file->setSelfHeader(header);
	file->setParent(parent);

	// 最后设置成功标志位
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
		throw YCPackException(1002, "YCLYCFileBuilder1::createNormal加载文件失败");
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
				throw YCPackException(1002, "YCLYCFileBuilder1::createNormal文件加密失败", filename);
			}
			buffer = encrypted;
		}
		
		// 1024 取整
		unsigned int round_length = ROUNDSIZE(length);
		unsigned int fill_length = round_length - length;

		YCIFilePack::IDLE_BLOCK* idle = thePack->findAvailableBlock(round_length);
		if (idle == NULL)
		{
			// 追加到文件尾
			int packLength = thePack->getContentLength();
			header->offset = packLength+sizeof(LYC_PACK_T_1);
			thePack->setContentLength(packLength+round_length);
		}
		else
		{
			// 文件长度不会增加
			unsigned int offset = idle->offset;
			header->offset = offset;

			//尚有剩余
			if (idle->length > round_length)
			{
				idle->offset += round_length;
				idle->length -= round_length;

				thePack->addIdleBlock(idle->offset, idle->length);
			}

			SAFE_DELETE(idle);
		}

		// 所有的偏移量都是相对于文件头
		if (!write(fd, header->offset, length, buffer))
		{
			throw YCPackException(1002, "YCLYCFileBuilder1::createNormal写入文件失败");
		}

		//写填充值
		if (!write(fd, header->offset+length, fill_length, FILL_STUFF))
		{
			throw YCPackException(1002, "YCLYCFileBuilder1::createDir填充文件失败");
		}
	}
		
	// 绑定到FD
	file->bind(fd);
	//file->dirty();

	file->setSelfHeader(header);
	file->setParent(parent);

	// 最后设置成功标志位
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
		throw YCPackException(1002, "YCLYCFileBuilder1::loadDir文件为空");
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
			throw YCPackException(1002, "YCLYCFileBuilder1::loadNormal加载文件失败");
		}

		if (myPersist != NULL)
		{
			if (!myPersist->decode(encrypted, length, &buffer, length))
			{
				throw YCPackException(1002, "YCLYCFileBuilder::loadNormal解压文件失败");
			}
		}
		else
		{
			SCOPE_GIVEUP(encrypted_guard);
			buffer = encrypted;
		}
	}

	YCLYCFile* created = new YCLYCFile(parent->getPack(), file->name);

	// 绑定到FD
	created->bind(parent->getFd());

	// 最后设置成功标志位
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
		throw YCPackException(1002, "YCLYCFileBuilder1::writeDir文件为空");
	}

	if (normal->isDirty())
	{
		unsigned int length = normal->getLength();
		if (length > 0)
		{
			FILE* fd = normal->getFd();

			char* encrypted = NULL;
			SCOPE_GUARD(encrypted_guard, delete[] encrypted);

			// 先加密
			if (myPersist != NULL)
			{
				if (!myPersist->encode(normal->getContent(), length, &encrypted, length))
				{
					throw YCPackException(1002, "YCLYCFilePack::add文件加密失败", normal->filename());
				}
			}

			// 写文件
			if (write(fd, normal->getOffset(), length, encrypted))
			{
				if (0 != fflush(fd))
				{
					throw YCPackException(1002, "YCIPackFile::writeDir文件存储失败");
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
// 加载文件段，客户自己分配内存
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
// 写入文件段
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