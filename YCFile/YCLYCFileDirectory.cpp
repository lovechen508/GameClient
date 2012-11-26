#include "YCLYCFileDirectory.h"

#include <vector>

#include "YCBasic\YCIFilePack.h"
#include "YCBasic\YCIPackPersist.h"
#include "YCBasic\YCIPackFileBuilder.h"

#include "YCLYCFile.h"
#include "YCLYCFilePack.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCPathWalker.h"
#include "YCBasic\YCScopeGuard.h"
#include "YCBasic\YCStringHelper.h"
#include "YCBasic\YCPackException.h"
#include "YCBasic\YCIPackFileBuilder.h"

///////////////////////////////////////////////////////////
// 帮助函数 ：递归添加文件
///////////////////////////////////////////////////////////
struct LYC_DIR_ADD_T {YCIFileDirectory* position; bool force;};
static void* addDirectory(const char* path, const char* filename, void* customData)
{
	SMART_ASSERT(customData != NULL);
	if (customData == NULL)
	{
		throw YCPackException(1002, "addDirectory未设置YCLYCFileDirectory句柄");
	}

	LYC_DIR_ADD_T* addInfo = (LYC_DIR_ADD_T*)customData;
	char theFullPath[MAX_PATH] = {0};
	sprintf(theFullPath, "%s\\%s", path, filename);
	if (YCFileUtil::IsDir(theFullPath))
	{
		if (!addInfo->position->mkdir(filename, false))
		{
			throw YCPackException(1002, "addDirectory创建目录失败");
		}

		LYC_DIR_ADD_T subInfo;
		subInfo.position = (YCIFileDirectory*)addInfo->position->get(filename);
		subInfo.force = addInfo->force;
		char theSubPath[MAX_PATH] = {0};
		sprintf(theSubPath, "%s\\%s\\%s", path, filename, "*");
		YCFileUtil::HandleFileInDir(theSubPath, &addDirectory, &subInfo);
	}
	else
	{
		addInfo->position->add(theFullPath, addInfo->force);
	}
	
	return NULL;
}

YCLYCFileDirectory::YCLYCFileDirectory(YCIFilePack* pkg, const char* filename)
	: YCIFileDirectory(pkg, filename)
	, mySelfHeader(NULL)
	, myHeader(NULL)
{	
	memset(myFiles, 0, sizeof(YCIAbstractFile*)*FILES_IN_DIR);
}


YCLYCFileDirectory::~YCLYCFileDirectory(void)
{
	SAFE_DELETE(myHeader);

	for (int i = 0; i < FILES_IN_DIR; ++i)
	{
		SAFE_DELETE(myFiles[i]);
	}
}

//
// 设置自身Header
//
void YCLYCFileDirectory::setSelfHeader(LYC_FILE_T_1* header)
{
	SMART_ASSERT(header != NULL);
	mySelfHeader = header;
}

//
// 设置文件头
//
void YCLYCFileDirectory::setHeader(LYC_DIR_T_1* header)
{
	SMART_ASSERT(header != NULL);
	SAFE_DELETE(myHeader);
	myHeader = header;
}

//
// 注释：取得文件头
//
LYC_DIR_T_1* YCLYCFileDirectory::getHeader()
{
	return myHeader;
}

//
// 注释：返回文件内容指针
//
const char* YCLYCFileDirectory::getContent()
{
	return (const char*)myHeader;
}

//
// 取得文件在包文件的偏移量
//
unsigned int YCLYCFileDirectory::getOffset()
{
	if (mySelfHeader->offset == 0)
	{
		throw YCPackException(1002, "YCLYCFile::getOffset尚未初始化");
	}
	return mySelfHeader->offset;
}

//
// 注释：返回文件长度
//
int YCLYCFileDirectory::getLength()
{
	return sizeof(LYC_DIR_T_1);
}

//
// 添加磁盘reguleFile到目录中
//
bool YCLYCFileDirectory::add(const char* reguleFile, bool force)
{
	SMART_ASSERT(reguleFile != NULL);

	if (YCFileUtil::IsDir(reguleFile))
	{
		LYC_DIR_ADD_T data;
		data.position = this;
		data.force = force;

		std::string theFullPath = reguleFile;
		theFullPath += "\\*";
		YCFileUtil::HandleFileInDir(theFullPath.c_str(), &addDirectory, &data);
	}
	else
	{
		const char* filename = YCFileUtil::GetFileName(reguleFile);
		if (strlen(filename) > FILENAME_LENGTH)
		{
			throw YCPackException(1002, "YCLYCFileDirectory::add文件名过长：", reguleFile);
		}

		for (int i = 0; i < FILES_IN_DIR; ++i)
		{
			LYC_FILE_T_1& f = myHeader->files[i];

			if (f.type != 0 && stricmp(filename, f.name) == 0)
			{
				if (f.type == YCIAbstractFile::FILE_DIR)
				{
					throw YCPackException(1002, "YCLYCFilePack::add文件以目录方式存在", reguleFile);
				}

				if (force)
				{
					remove(reguleFile);
				}
				else
				{
					throw YCPackException(1002, "YCLYCFilePack::add文件已存在", reguleFile);
				}
			}
		}

		int slot = findAvailableSlot();
		if (slot < 0)
		{
			throw YCPackException(1002, "YCLYCFilePack::add目录已满");
		}

		YCIPackFileBuilder* theBuilder = myPack->getBuilder();
		if (theBuilder == NULL)
		{
			throw YCPackException(1002, "YCLYCFilePack::add无Builder");
		}

		// 已加载至内存
		myFiles[slot] = theBuilder->createNormal(this, slot, reguleFile);
	}

	return true;
}

//
// 添加内存文件到目录中
//
bool YCLYCFileDirectory::add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool force)
{
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(content != NULL && length > 0);

	if (filename == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::add内存文件名为空");
	}

	if (content == NULL || length == 0)
	{
		throw YCPackException(1002, "YCLYCFilePack::add内存文件为空");
	}

	if (pathInPack == NULL) //自身文件夹
	{
		if (has(filename))
		{
			if (!force)
			{
				throw YCPackException(1002, "YCLYCFilePack::add文件已存在");
			}
			else
			{
				remove(filename);
			}
		}

		int slot = findAvailableSlot();
		if (slot < 0)
		{
			throw YCPackException(1002, "YCLYCFilePack::add文件夹已满", this->filename());
		}

		YCIPackFileBuilder* theBuilder = myPack->getBuilder();
		if (theBuilder == NULL)
		{
			throw YCPackException(1002, "YCLYCFilePack::add无Builder");
		}

		myFiles[slot] = theBuilder->createNormal(this, slot, filename, content, length);
	}
	else
	{
		YCPathWalker walker(pathInPack);
		int pathLength; 
		YCPathWalker::E_FILE_TYPE type;

		const char* first = walker.next(pathLength, type);
		if (first == NULL) //自身文件夹
		{
			if (has(filename))
			{
				if (!force)
				{
					throw YCPackException(1002, "YCLYCFilePack::add文件已存在");
				}
				else
				{
					remove(filename);
				}
			}

			int slot = findAvailableSlot();
			if (slot < 0)
			{
				throw YCPackException(1002, "YCLYCFilePack::add文件夹已满", this->filename());
			}

			YCIPackFileBuilder* theBuilder = myPack->getBuilder();
			if (theBuilder == NULL)
			{
				throw YCPackException(1002, "YCLYCFilePack::add无Builder");
			}

			myFiles[slot] = theBuilder->createNormal(this, slot, filename, content, length);
		}
		else //子文件夹
		{
			for (int i = 0; i < FILES_IN_DIR; ++i)
			{
				LYC_FILE_T_1 & f = myHeader->files[i];
				if (f.type != 0 && strnicmp(first, f.name, pathLength) == 0)
				{
					if (f.type != YCIAbstractFile::FILE_DIR)
					{
						throw YCPackException(1002, "YCLYCFilePack::add路径以文件存在", f.name);
					}

					YCIFileDirectory* dir = (YCIFileDirectory*)getFile(i, f);
					return dir->add(filename, walker.remain(), content, length, force);
				}
			}
		}
	}

	return true;
}

//
// 添加子目录，是否递归
//
bool YCLYCFileDirectory::mkdir(const char* dir, bool recursive)
{
	SMART_ASSERT(dir != NULL);
	if (dir == NULL || strlen(dir) == 0)
	{
		throw YCPackException(1002, "YCLYCPackFile::mkdir添加目录失败");
	}

	YCPathWalker walker(dir);
	int length;
	YCPathWalker::E_FILE_TYPE type;

	const char* first = walker.next(length, type);
	if (length > FILENAME_LENGTH)
	{
		throw YCPackException(1002, "YCLYCFilePack::mkdir目录名过长：", first);
	}

	YCIFileDirectory* found = NULL;
	for (unsigned int i = 0; i < FILES_IN_DIR && !found; ++i)
	{
		LYC_FILE_T_1& f = myHeader->files[i];
		if (f.type != 0 && strnicmp(f.name, first, length) == 0)
		{
			// 是否是目录
			if (f.type != YCIAbstractFile::FILE_DIR)
			{
				throw YCPackException(1002, "YCLYCPackFile::mkdir非目录", f.name);
			}

			// 加载到内存					
			found = (YCIFileDirectory*)getFile(i, f);
			if (found == NULL)
			{
				throw YCPackException(1002, "YCLYCPackFile::mkdir加载目录失败", f.name);
			}
		}
	}

	if (found == NULL)
	{
		// 目录须创建
		int slot = findAvailableSlot();
		if (slot < 0)
		{
			throw YCPackException(1002, "YCLYCPackFile::mkdir目录已满：", filename());
		}

		YCIPackFileBuilder* theBuilder = myPack->getBuilder();
		if (theBuilder == NULL)
		{
			throw YCPackException(1002, "YCLYCFilePack::add无Builder");
		}

		found = theBuilder->createDir(this, slot, std::string(first, length).c_str());
		myFiles[slot] = found;
	}

	const char* remain = walker.remain();
	if (remain)
	{
		if (recursive)
		{
			found->mkdir(remain, recursive);
		}
		else
		{
			throw YCPackException(1002, "YCLYCPackFile::mkdir添加目录失败");
		}
	}

	dirty();

	return true;
}

//
// 是否包含fileInPack
//
int YCLYCFileDirectory::has(const char* fileInPack)
{
	SMART_ASSERT(fileInPack != NULL);
	if (fileInPack == NULL || strlen(fileInPack) == 0)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::has查找文件为NULL");
	}

	std::string strFile(fileInPack);
	size_t pos = strFile.find_first_of('\\');
	const std::string& firstLevel = strFile.substr(0, pos);

	for (int i = 0; i < FILES_IN_DIR; ++i)
	{
		const LYC_FILE_T_1& file = myHeader->files[i];
		if (file.type != 0 && stricmp(firstLevel.c_str(), file.name) == 0)
		{
			if (pos == std::string::npos)
			{
				// 尾节点可以使文件，也可以是文件夹
				return file.type;
			}
			else
			{
				if (file.type != YCIAbstractFile::FILE_DIR)
				{
					throw YCPackException(1002, "YCLYCFileDirectory::has发现非目录", file.name);
				}

				const std::string& restLevel = strFile.substr(pos+1); 
				if (restLevel.length() == 0 || restLevel == "\\")
				{
					return true; //到底了 "\"
				}

				// 检查是否已经加载到内存
				YCIAbstractFile* cachedFile = getFile(i, file);
				if (cachedFile == NULL)
				{
					throw YCPackException(1002, "YCLYCFileDirectory::has读取文件失误", file.name);
				}

				SMART_ASSERT(cachedFile->type() == YCIAbstractFile::FILE_DIR);
				YCLYCFileDirectory* directory = (YCLYCFileDirectory*)cachedFile;				
				return directory->has(restLevel.c_str());
			}
			
			break;
		}
	}

	return 0;
}

//
// 抽取packFile文件
//
YCIAbstractFile* YCLYCFileDirectory::get(const char* packFile)
{
	if (packFile == NULL || strlen(packFile) == 0)
	{
		return this;
	}

	YCIAbstractFile* found = NULL;

	YCPathWalker walker(packFile);
	int length;
	YCPathWalker::E_FILE_TYPE type;
	const char* first = walker.next(length, type);

	if (first != NULL)
	{
		for (unsigned int i = 0; i < FILES_IN_DIR && !found; ++i)
		{
			LYC_FILE_T_1& f = myHeader->files[i];
			if (f.type != 0)
			{
				if (strnicmp(first, f.name, length) == 0)
				{
					if (f.type == YCIAbstractFile::FILE_NORMAL)
					{
						if (type != YCPathWalker::BOTH)
						{
							throw YCPackException(1002, "YCLYCFileDirectory::get文件类型不一致", f.name);
						}
					}

					found = getFile(i, f);
				}
			}		
		}

		if (NULL != found)
		{
			const char* remain = walker.remain();
			if (remain != NULL)
			{
				if (found->type() != YCIAbstractFile::FILE_DIR)
				{
					throw YCPackException(1002, "YCLYCFileDirectory::get文件未找到", remain);
				}

				found = ((YCIFileDirectory*)found)->get(remain);
			}	
		}			
	}

	return found;
}

//
// 删除packFile文件，找到置标志位即可
//
bool YCLYCFileDirectory::remove(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::remove文件为空");
	}

	YCPathWalker walker(packFile);
	int length;
	YCPathWalker::E_FILE_TYPE type;
	const char* first = walker.next(length, type);
	if (first == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::remove路径有误");
	}

	bool found = false;
	if (type == YCPathWalker::DIR)
	{
		for (int i = 0; i < FILES_IN_DIR && !found; ++i)
		{
			LYC_FILE_T_1& f = myHeader->files[i];
			if (f.type != 0)
			{
				if (strnicmp(f.name, first, length) == 0)
				{
					if (f.type != YCIAbstractFile::FILE_DIR)
					{
						throw YCPackException(1002, "YCLYCFileDirectory::remove目标不是文件夹", f.name);
					}

					found = true;

					if (!isFileLoaded(i))
					{
						loadFile(i, f);
					}
					YCIFileDirectory* dir = (YCIFileDirectory*)getLoaded(i);

					const char* remain = walker.remain();
					if (remain == NULL)
					{
						dir->clear();
					}
					else
					{
						dir->remove(remain);
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < FILES_IN_DIR && !found; ++i)
		{
			LYC_FILE_T_1& f = myHeader->files[i];
			if (f.type != 0)
			{
				if (stricmp(packFile, f.name) == 0)
				{
					found = true;

					// 文件夹递归释放
					if (f.type == YCIAbstractFile::FILE_DIR)
					{
						if (!isFileLoaded(i))
						{
							loadFile(i, f);
						}
						YCIFileDirectory* dir = (YCIFileDirectory*)getLoaded(i);
						dir->clear();
					}
					
					// 释放自身
					if (f.length > 0)
					{	
						myPack->addIdleBlock(f.offset, ROUNDSIZE(f.length));
					}

					// 清空标示
					memset(&f, 0, sizeof(LYC_FILE_T_1));

					// 释放内存缓冲
					SAFE_DELETE(myFiles[i]);

					// 脏
					dirty();
				}
			}
		}
	}

	return found;
}

//
// 删除packFile文件，找到置标志位即可
//
bool YCLYCFileDirectory::remove(YCIAbstractFile* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::remove文件为空");
	}

	return remove(packFile->filename());
}

//
// 清空文件夹下的所有文件
//
bool YCLYCFileDirectory::clear()
{
	for (int i = 0; i < FILES_IN_DIR; ++i)
	{
		LYC_FILE_T_1& f = myHeader->files[i];
		if (f.type != 0)
		{
			// 递归释放子文件夹
			if (f.type == YCIAbstractFile::FILE_DIR)
			{
				if (!isFileLoaded(i))
				{
					loadFile(i, f);
				}
				YCIFileDirectory* dir = (YCIFileDirectory*)getLoaded(i);
				dir->clear();
			}

			// 释放自身
			myPack->addIdleBlock(f.offset, ROUNDSIZE(f.length));
			memset(&f, 0, sizeof(LYC_FILE_T_1));

			// 释放内存缓冲
			SAFE_DELETE(myFiles[i]);

			// 脏
			dirty();
		}
	}

	return true;
}

//
// 导出目录中所有文件
//
void YCLYCFileDirectory::dump(const char* targetPath, bool overwrite)
{
	SMART_ASSERT(targetPath != NULL);
	if (targetPath == NULL)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::dump targetPath为空");
	}

	char theFullPath[MAX_PATH] = {0};
	sprintf(theFullPath, "%s\\%s", targetPath, filename());
	if (YCFileUtil::IsFileExist(theFullPath))
	{
		if (!YCFileUtil::IsDir(theFullPath))
		{
			throw YCPackException(1002, "YCLYCFileDirectory::dump目标文件夹以文件存在", theFullPath);
		}

		if (!overwrite)
		{
			throw YCPackException(1002, "YCLYCFileDirectory::dump文件夹已存在， -f选项可以强制覆盖");
		}
	}
	else
	{
		if (!YCFileUtil::CreateDir(theFullPath, false))
		{
			throw YCPackException(1002, "YCLYCFileDirectory::dump创建目标文件夹失败", theFullPath);
		}
	}

	for (int i = 0; i < FILES_IN_DIR; ++i)
	{
		const LYC_FILE_T_1& f = myHeader->files[i];
		if (f.type != 0)
		{
			YCIAbstractFile * file = getFile(i, f);
			if (file == NULL)
			{
				throw YCPackException(1002, "YCLYCFileDirectory::dump加载文件失败", f.name);
			}

			file->dump(theFullPath, overwrite);
		}
	}
}

//
// 在有了改动之后，强制刷新到磁盘
//
void YCLYCFileDirectory::flush()
{
	// 通知子文件刷新到磁盘
	for (int i = 0; i < FILES_IN_DIR; ++i)
	{
		YCIAbstractFile* file = myFiles[i];
		if (file && file->isDirty())
		{
			file->flush();
		}
	}

	// 刷文件头
	if (isDirty())
	{
		YCIPackFileBuilder* theBuilder = myPack->getBuilder();
		if (theBuilder == NULL)
		{
			throw YCPackException(1002, "YCLYCFileDirectory::flush Builder为空");
		}
		theBuilder->writeDir(this);

		myDirty = false;
	}
}

//////////////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////////////

//
// 获取子文件在内存中的镜像
//
YCIAbstractFile* YCLYCFileDirectory::getFile(unsigned int index, const LYC_FILE_T_1& file)
{
	// 检查是否已经加载到内存
	YCIAbstractFile* found = getLoaded(index);
	
	// 尚未加载
	if (found == NULL)
	{
		found = loadFile(index, file);
		if (found != NULL)
		{
			myFiles[index] = found;
		}
	}

	return found;
}

//
// 加载文件到内存，构建YCIAbstractFile到myFiles
//
YCIAbstractFile* YCLYCFileDirectory::loadFile(unsigned int index, const LYC_FILE_T_1& file)
{
	SMART_ASSERT(file.type != 0);
	if (file.type == 0)
	{
		throw YCPackException(1002, "YCLYCPackFile::loadFile文件槽位空");
	}

	YCIPackFileBuilder* theBuilder = myPack->getBuilder();
	if (theBuilder == NULL)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::mkdir查找builder失败");
	}

	YCIAbstractFile* result = NULL;
	if (file.type == YCIAbstractFile::FILE_DIR)
	{
		result = theBuilder->loadDir(this, (void*)&file);
	}
	else if (file.type == YCIAbstractFile::FILE_NORMAL)
	{
		result = theBuilder->loadNormal(this, (void*)&file);
	}
	else
	{
		throw YCPackException(1002, "YCLYCPackFile::loadFile未知文件类型");
	}

	if (result != NULL)
	{
		myFiles[index] = result;
	}

	return result;
}

//
// 目录下的文件是否已经加载到内存
//
bool YCLYCFileDirectory::isFileLoaded(unsigned int index)
{
	SMART_ASSERT(index < FILES_IN_DIR);

	if (index >= FILES_IN_DIR)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::isFileLoaded下标异常");
	}

	return (myFiles[index] != NULL);
}

//
// 获取加载到内存中的文件
//
YCIAbstractFile* YCLYCFileDirectory::getLoaded(unsigned int index)
{
	return (isFileLoaded(index) ? myFiles[index] : NULL);
}

//
// 查找空闲的文件槽位
//
int YCLYCFileDirectory::findAvailableSlot()
{
	int available = -1;

	for (int i = 0; (i < FILES_IN_DIR) && (available == -1); ++i)
	{
		const LYC_FILE_T_1& slot = myHeader->files[i];
		if (slot.type == 0)
		{
			available = i;
		}
	}
	return available;
}