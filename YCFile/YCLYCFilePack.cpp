#include "YCLYCFilePack.h"

#include <memory>

#include "YCBasic\YCIPackPersist.h"
#include "YCBasic\YCIPackFileBuilder.h"

#include "YCLYCFile.h"
#include "YCLYCFilePack.h"
#include "YCLYCFileDirectory.h"
#include "YCLYCFileBuilder1.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCPathWalker.h"
#include "YCBasic\YCScopeGuard.h"
#include "YCBasic\YCPackException.h"

///////////////////////////////////////////////////////////
// 帮助函数 ：递归添加文件
///////////////////////////////////////////////////////////
struct LYC_PACK_DIR_ADD_T {YCIFileDirectory* position; bool force;};
static void* addDirectory(const char* path, const char* filename, void* customData)
{
	SMART_ASSERT(customData != NULL);
	if (customData == NULL)
	{
		throw YCPackException(1002, "addDirectory未设置YCLYCFileSystem句柄");
	}

	LYC_PACK_DIR_ADD_T* addInfo = (LYC_PACK_DIR_ADD_T*)customData;
	char theFullPath[MAX_PATH] = {0};
	sprintf(theFullPath, "%s\\%s", path, filename);
	if (YCFileUtil::IsDir(theFullPath))
	{
		if (!addInfo->position->mkdir(filename, false))
		{
			throw YCPackException(1002, "addDirectory创建目录失败");
		}

		LYC_PACK_DIR_ADD_T subInfo;
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

YCLYCFilePack::YCLYCFilePack(const char* name, FILE* fd, LYC_PACK_T_1* header, YCIPackFileBuilder* builder)
	: YCIFilePack(name, fd, builder)
	, myHeader(header)
{
	SMART_ASSERT(name != NULL);
	SMART_ASSERT(header != NULL);
	SMART_ASSERT(builder != NULL);

	memset(myFiles, 0, sizeof(YCIAbstractFile*)*FILES_IN_DIR);

	const char* packName = YCFileUtil::GetFileName(name);
	int iPos = 0;
	while (packName[iPos] != '\0' && packName[iPos] != '.')
	{
		++iPos;
	}
	memcpy(myPackName, packName, iPos);
	myPackName[iPos] = '\0';

	// 查找空闲块
	unsigned int fileSize = YCFileUtil::GetFileLength(name);
	if (header->idle != 0)
	{
		SMART_ASSERT(header->idle%FILE_ROUNDSIZE == 0);

		YCIFilePack::IDLE_BLOCK block;

		for (unsigned int next = header->idle;next != 0; next = block.offset)
		{
			if (next > fileSize)
			{
				throw YCPackException(1002, "YCLYCPackFile::YCLYCPackFile文件偏移量错误", next);
			}

			if (!YCLYCFileBuilder1::read(myFd, next, sizeof(YCIFilePack::IDLE_BLOCK), (char*)&block))
			{
				throw YCPackException(1002, "YCLYCPackFile::YCLYCPackFile读入文件偏移失败，可能文件已损坏", filename());
			}

			addIdleBlock(next, block.length);
		}
	}
}


YCLYCFilePack::~YCLYCFilePack(void)
{
	SAFE_DELETE(myHeader);

	for (int i = 0; i < FILES_IN_DIR; ++i)
	{
		SAFE_DELETE(myFiles[i]);
	}
}

//
// 取得包名
//
const char* YCLYCFilePack::getPackName()
{
	return myPackName;
}

//
// 注释：取得文件头
//
LYC_PACK_T_1* YCLYCFilePack::getHeader()
{
	return myHeader;
}

//
// 注释：返回文件内容指针
//
const char* YCLYCFilePack::getContent()
{
	return (const char*)myHeader;
}

//
// 注释：返回文件长度
//
int YCLYCFilePack::getLength()
{
	return sizeof(LYC_PACK_T_1);
}


//
// 注释：设置文件除头部内容区长度
//
void YCLYCFilePack::setContentLength(int contentLen)
{
	myHeader->size = contentLen;
}

//
// 注释：获取文件除头部内容区长度
//
int YCLYCFilePack::getContentLength()
{
	return myHeader->size;
}


//
// 添加磁盘reguleFile到Pack包中
//
bool YCLYCFilePack::add(const char* reguleFile, bool force)
{
	SMART_ASSERT(reguleFile != NULL);
	
	if (YCFileUtil::IsDir(reguleFile))
	{
		// 添加目录下的文件，不添加文件夹本身
		LYC_PACK_DIR_ADD_T data;
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
			throw YCPackException(1002, "YCLYCFilePack::add文件名过长：", reguleFile);
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
		
		// 已加载至内存
		myFiles[slot] = myBuilder->createNormal(this, slot, reguleFile);
	}

	return true;
}

//
// 添加内存文件到Pack包中
//
bool YCLYCFilePack::add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool force)
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

		myFiles[slot] = myBuilder->createNormal(this, slot, filename, content, length);

		dirty();
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

			myFiles[slot] = myBuilder->createNormal(this, slot, filename, content, length);

			dirty();
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
bool YCLYCFilePack::mkdir(const char* dir, bool recursive)
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

		found = myBuilder->createDir(this, slot, std::string(first, length).c_str());
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
int YCLYCFilePack::has( const char* fileInPack )
{
	SMART_ASSERT(fileInPack != NULL);
	if (fileInPack == NULL || strlen(fileInPack) == 0)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::has查找文件为NULL");
	}

	YCPathWalker walker(fileInPack);
	int length;
	YCPathWalker::E_FILE_TYPE type;
	
	const char* first = walker.next(length, type);

	YCIAbstractFile* found = NULL;
	for (unsigned int i = 0; i < FILES_IN_DIR && !found; ++i)
	{
		const LYC_FILE_T_1& f = myHeader->files[i];
		if (f.type != 0 && strnicmp(first, f.name, length) == 0)
		{
			if (type == YCPathWalker::DIR && f.type != YCIAbstractFile::FILE_DIR)
			{
				throw YCPackException(1002, "YCLYCFileDirectory::has查找目录为文件", f.name);
			}

			// 检查是否已经加载到内存
			found = getFile(i, f);
			if (found == NULL)
			{
				throw YCPackException(1002, "YCLYCFileDirectory::has读取文件失误", f.name);
			}
		}
	}

	if (found != NULL)
	{
		const char* remain = walker.remain();
		if (remain == NULL)
		{
			return found->type();
		}
		else
		{
			if (found->type() == YCIAbstractFile::FILE_NORMAL)
			{
				throw YCPackException(1002, "YCLYCFileDirectory::has查找目录为文件", first);
			}

			return ((YCIFileDirectory*)found)->has(remain);
		}
	}

	return 0;
}

//
// 抽取packFile文件
//
YCIAbstractFile* YCLYCFilePack::get(const char* packFile)
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
							throw YCPackException(1002, "YCLYCPackFile::get文件类型不一致", f.name);
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
					throw YCPackException(1002, "YCLYCFilePack::get文件未找到", remain);
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
bool YCLYCFilePack::remove(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::remove文件为空");
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
						throw YCPackException(1002, "YCLYCFilePack::remove目标不是文件夹", f.name);
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
						addIdleBlock(f.offset, ROUNDSIZE(f.length));
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
bool YCLYCFilePack::remove(YCIAbstractFile* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::remove文件为空");
	}

	return remove(packFile->filename());
}

//
// 清空文件夹下的所有文件
//
bool YCLYCFilePack::clear()
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
			addIdleBlock(f.offset, ROUNDSIZE(f.length));
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
// 导出Pack包中所有文件
//
void YCLYCFilePack::dump(const char* targetPath, bool overwrite)
{
	SMART_ASSERT(targetPath != NULL);
	if (targetPath == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::dump targetPath为空");
	}

	char theFullPath[MAX_PATH] = {0};
	sprintf(theFullPath, "%s\\%s", targetPath, getPackName());
	if (YCFileUtil::IsFileExist(theFullPath))
	{
		if (!YCFileUtil::IsDir(theFullPath))
		{
			throw YCPackException(1002, "YCLYCFilePack::dump目标文件夹以文件存在", theFullPath);
		}

		if (!overwrite)
		{
			throw YCPackException(1002, "YCLYCFilePack::dump文件夹已存在， -f选项可以强制覆盖");
		}
	}
	else
	{
		if (!YCFileUtil::CreateDir(theFullPath, false))
		{
			throw YCPackException(1002, "YCLYCFilePack::dump创建目标文件夹失败", theFullPath);
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
				throw YCPackException(1002, "YCLYCFilePack::dump加载文件失败", f.name);
			}

			file->dump(theFullPath, overwrite);
		}
	}
}

//
// 在PackFile有了改动之后，强制刷新到磁盘
//
void YCLYCFilePack::flush()
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
		myBuilder->writePack(this);
	}

	// 刷空闲链表
	if (myHeader->idle != 0)
	{
		Item_List_T* prev = myIdleList->begin();
		while (prev != NULL)
		{
			Item_List_T* next = myIdleList->next(prev);

			YCIFilePack::IDLE_BLOCK* currentBlock = 
				(YCIFilePack::IDLE_BLOCK*)myIdleList->payload(prev);

			int nextOffset = 0;
			if (next != NULL)
			{
				YCIFilePack::IDLE_BLOCK* nextBlock =
					(YCIFilePack::IDLE_BLOCK*)myIdleList->payload(next);
				nextOffset = nextBlock->offset;
			}

			YCIFilePack::IDLE_BLOCK writeBlock;
			writeBlock.length = currentBlock->length;
			writeBlock.offset = nextOffset;
			if (!YCLYCFileBuilder1::write(myFd, currentBlock->offset, sizeof(YCIFilePack::IDLE_BLOCK),(const char*)&writeBlock))
			{
				throw YCPackException(1002, "YCLYCFilePack::flush写入文件空闲链表失败");
			}

			prev = next;
		}

		if (0 != fflush(myFd))
		{
			throw YCPackException(1002, "YCLYCFilePack::flush写入文件空闲链表刷新到磁盘失败");
		}
	}

	myDirty = false;
}

//
// 挂接空闲块, 尝试左右合并
//
void YCLYCFilePack::addIdleBlock(unsigned int offset, unsigned int length)
{
	SMART_ASSERT(offset > 0);
	SMART_ASSERT(length > 0);

	//IDLE_BLOCK* block = new IDLE_BLOCK(offset, length);

	// Insert Position    
	//          1                           2                                                       3
	//          |                           |                                                       |
	//          V                           V                                                       V
	//	           ---------     ---------     ---------     ---------     ---------     ---------
	//	       -->|         |-->|         |-->|         |-->|         |-->|         |-->|         |-->
	//	 head     |         |   |         |   |         |   |         |   |         |   |         |    tail
	//	       <--|         |<--|         |<--|         |<--|         |<--|         |<--|         |<-- 
	//	           ---------     ---------     ---------     ---------     ---------     ---------

	bool inserted = false;

	Item_List_T* left = NULL;
	for (Item_List_T* right = myIdleList->begin();
		right != myIdleList->end() && !inserted;
		left = right, right = myIdleList->next(right))
	{
		IDLE_BLOCK* r_Idle = (IDLE_BLOCK*)myIdleList->payload(right);
		if (r_Idle->offset > offset)
		{
			// case 1 : 右合并,offset向左推进
			if (left == NULL)
			{
				if (offset+length == r_Idle->offset)
				{
					r_Idle->offset = offset;
					r_Idle->length += length;
				}
				else
				{
					myIdleList->insert(new IDLE_BLOCK(offset, length), NULL); // 新头部
				}
			}

			// case 2 : 左右合并
			else
			{
				IDLE_BLOCK* l_merge = NULL;

				// 左
				IDLE_BLOCK* l_Idle = (IDLE_BLOCK*)myIdleList->payload(left);
				if (l_Idle->offset+l_Idle->length == offset)
				{
					l_Idle->length += length;
					l_merge = l_Idle;
				}
				else
				{
					l_merge = new IDLE_BLOCK(offset, length);
					myIdleList->append(l_merge, left);
				}

				// 右
				if (l_merge->offset+l_merge->length == r_Idle->offset)
				{
					l_merge->length += r_Idle->length;
					myIdleList->remove(right);
					SAFE_DELETE(r_Idle);
				}
			}

			inserted = true;
		}
	}

	// case 3 : 尝试左合并
	if (!inserted)
	{
		Item_List_T* tail = myIdleList->r_begin();
		if (tail == NULL)
		{
			myIdleList->append(new IDLE_BLOCK(offset, length), NULL); //空链表
		}
		else
		{
			IDLE_BLOCK* idle = (IDLE_BLOCK*)myIdleList->payload(tail);
			if (idle->offset+idle->length == offset)
			{
				idle->length += length;            // 合并
			}
			else
			{
				myIdleList->append(new IDLE_BLOCK(offset, length), tail); //链表尾
			}
		}
	}

	Item_List_T* header = myIdleList->begin();
	if (header != NULL)
	{
		IDLE_BLOCK* firstBlock = (IDLE_BLOCK*)myIdleList->payload(header);
		myHeader->idle = firstBlock->offset;
	}

	dirty();
}

//
// 查找空闲的文件块
//
YCIFilePack::IDLE_BLOCK* YCLYCFilePack::findAvailableBlock(unsigned int required)
{
	SMART_ASSERT(required > 0);

	Item_List_T* found = NULL;
	unsigned int deviation = INT_MAX;

	for (Item_List_T* item = myIdleList->begin(); 
		item != myIdleList->end();
		item = myIdleList->next(item))
	{
		IDLE_BLOCK* idle = (IDLE_BLOCK*)myIdleList->payload(item);
		if ((idle->length >= required) && (idle->length-required < deviation))
		{
			found = item;
			deviation = idle->length-required;
		}
	}

	IDLE_BLOCK* block = NULL;
	if (found != NULL)
	{
		block = (IDLE_BLOCK*)myIdleList->remove(found);
		
		Item_List_T* header = myIdleList->begin();
		if (header != NULL)
		{
			IDLE_BLOCK* firstBlock = (IDLE_BLOCK*)myIdleList->payload(header);
			myHeader->idle = firstBlock->offset;
		}
		else
		{
			myHeader->idle = 0; //无空闲块
		}

		dirty();
	}

	return block;
}

//////////////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////////////

//
// 获取子文件在内存中的镜像
//
YCIAbstractFile* YCLYCFilePack::getFile(unsigned int index, const LYC_FILE_T_1& file)
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
YCIAbstractFile* YCLYCFilePack::loadFile(unsigned int index, const LYC_FILE_T_1& file)
{
	SMART_ASSERT(file.type != 0);
	if (file.type == 0)
	{
		throw YCPackException(1002, "YCLYCPackFile::loadFile文件槽位空");
	}

	YCIAbstractFile* result = NULL;
	if (file.type == YCIAbstractFile::FILE_DIR)
	{
		result = myBuilder->loadDir(this, (void*)&file);
	}
	else if (file.type == YCIAbstractFile::FILE_NORMAL)
	{
		result = myBuilder->loadNormal(this, (void*)&file);
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
bool YCLYCFilePack::isFileLoaded(unsigned int index)
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
YCIAbstractFile* YCLYCFilePack::getLoaded(unsigned int index)
{
	return (isFileLoaded(index) ? myFiles[index] : NULL);
}

//
// 查找空闲的文件槽位
//
int YCLYCFilePack::findAvailableSlot()
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
