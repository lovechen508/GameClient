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
// �������� ���ݹ�����ļ�
///////////////////////////////////////////////////////////
struct LYC_PACK_DIR_ADD_T {YCIFileDirectory* position; bool force;};
static void* addDirectory(const char* path, const char* filename, void* customData)
{
	SMART_ASSERT(customData != NULL);
	if (customData == NULL)
	{
		throw YCPackException(1002, "addDirectoryδ����YCLYCFileSystem���");
	}

	LYC_PACK_DIR_ADD_T* addInfo = (LYC_PACK_DIR_ADD_T*)customData;
	char theFullPath[MAX_PATH] = {0};
	sprintf(theFullPath, "%s\\%s", path, filename);
	if (YCFileUtil::IsDir(theFullPath))
	{
		if (!addInfo->position->mkdir(filename, false))
		{
			throw YCPackException(1002, "addDirectory����Ŀ¼ʧ��");
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

	// ���ҿ��п�
	unsigned int fileSize = YCFileUtil::GetFileLength(name);
	if (header->idle != 0)
	{
		SMART_ASSERT(header->idle%FILE_ROUNDSIZE == 0);

		YCIFilePack::IDLE_BLOCK block;

		for (unsigned int next = header->idle;next != 0; next = block.offset)
		{
			if (next > fileSize)
			{
				throw YCPackException(1002, "YCLYCPackFile::YCLYCPackFile�ļ�ƫ��������", next);
			}

			if (!YCLYCFileBuilder1::read(myFd, next, sizeof(YCIFilePack::IDLE_BLOCK), (char*)&block))
			{
				throw YCPackException(1002, "YCLYCPackFile::YCLYCPackFile�����ļ�ƫ��ʧ�ܣ������ļ�����", filename());
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
// ȡ�ð���
//
const char* YCLYCFilePack::getPackName()
{
	return myPackName;
}

//
// ע�ͣ�ȡ���ļ�ͷ
//
LYC_PACK_T_1* YCLYCFilePack::getHeader()
{
	return myHeader;
}

//
// ע�ͣ������ļ�����ָ��
//
const char* YCLYCFilePack::getContent()
{
	return (const char*)myHeader;
}

//
// ע�ͣ������ļ�����
//
int YCLYCFilePack::getLength()
{
	return sizeof(LYC_PACK_T_1);
}


//
// ע�ͣ������ļ���ͷ������������
//
void YCLYCFilePack::setContentLength(int contentLen)
{
	myHeader->size = contentLen;
}

//
// ע�ͣ���ȡ�ļ���ͷ������������
//
int YCLYCFilePack::getContentLength()
{
	return myHeader->size;
}


//
// ��Ӵ���reguleFile��Pack����
//
bool YCLYCFilePack::add(const char* reguleFile, bool force)
{
	SMART_ASSERT(reguleFile != NULL);
	
	if (YCFileUtil::IsDir(reguleFile))
	{
		// ���Ŀ¼�µ��ļ���������ļ��б���
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
			throw YCPackException(1002, "YCLYCFilePack::add�ļ���������", reguleFile);
		}

		for (int i = 0; i < FILES_IN_DIR; ++i)
		{
			LYC_FILE_T_1& f = myHeader->files[i];
			
			if (f.type != 0 && stricmp(filename, f.name) == 0)
			{
				if (f.type == YCIAbstractFile::FILE_DIR)
				{
					throw YCPackException(1002, "YCLYCFilePack::add�ļ���Ŀ¼��ʽ����", reguleFile);
				}

				if (force)
				{
					remove(reguleFile);
				}
				else
				{
					throw YCPackException(1002, "YCLYCFilePack::add�ļ��Ѵ���", reguleFile);
				}
			}
		}

		int slot = findAvailableSlot();
		if (slot < 0)
		{
			throw YCPackException(1002, "YCLYCFilePack::addĿ¼����");
		}
		
		// �Ѽ������ڴ�
		myFiles[slot] = myBuilder->createNormal(this, slot, reguleFile);
	}

	return true;
}

//
// ����ڴ��ļ���Pack����
//
bool YCLYCFilePack::add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool force)
{
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(content != NULL && length > 0);

	if (filename == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::add�ڴ��ļ���Ϊ��");
	}
	
	if (content == NULL || length == 0)
	{
		throw YCPackException(1002, "YCLYCFilePack::add�ڴ��ļ�Ϊ��");
	}

	if (pathInPack == NULL) //�����ļ���
	{
		if (has(filename))
		{
			if (!force)
			{
				throw YCPackException(1002, "YCLYCFilePack::add�ļ��Ѵ���");
			}
			else
			{
				remove(filename);
			}
		}

		int slot = findAvailableSlot();
		if (slot < 0)
		{
			throw YCPackException(1002, "YCLYCFilePack::add�ļ�������", this->filename());
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
		if (first == NULL) //�����ļ���
		{
			if (has(filename))
			{
				if (!force)
				{
					throw YCPackException(1002, "YCLYCFilePack::add�ļ��Ѵ���");
				}
				else
				{
					remove(filename);
				}
			}

			int slot = findAvailableSlot();
			if (slot < 0)
			{
				throw YCPackException(1002, "YCLYCFilePack::add�ļ�������", this->filename());
			}

			myFiles[slot] = myBuilder->createNormal(this, slot, filename, content, length);

			dirty();
		}
		else //���ļ���
		{
			for (int i = 0; i < FILES_IN_DIR; ++i)
			{
				LYC_FILE_T_1 & f = myHeader->files[i];
				if (f.type != 0 && strnicmp(first, f.name, pathLength) == 0)
				{
					if (f.type != YCIAbstractFile::FILE_DIR)
					{
						throw YCPackException(1002, "YCLYCFilePack::add·�����ļ�����", f.name);
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
// �����Ŀ¼���Ƿ�ݹ�
//
bool YCLYCFilePack::mkdir(const char* dir, bool recursive)
{
	SMART_ASSERT(dir != NULL);
	if (dir == NULL || strlen(dir) == 0)
	{
		throw YCPackException(1002, "YCLYCPackFile::mkdir���Ŀ¼ʧ��");
	}

	YCPathWalker walker(dir);
	int length;
	YCPathWalker::E_FILE_TYPE type;

	const char* first = walker.next(length, type);
	if (length > FILENAME_LENGTH)
	{
		throw YCPackException(1002, "YCLYCFilePack::mkdirĿ¼��������", first);
	}

	YCIFileDirectory* found = NULL;
	for (unsigned int i = 0; i < FILES_IN_DIR && !found; ++i)
	{
		LYC_FILE_T_1& f = myHeader->files[i];
		if (f.type != 0 && strnicmp(f.name, first, length) == 0)
		{
			// �Ƿ���Ŀ¼
			if (f.type != YCIAbstractFile::FILE_DIR)
			{
				throw YCPackException(1002, "YCLYCPackFile::mkdir��Ŀ¼", f.name);
			}

			// ���ص��ڴ�					
			found = (YCIFileDirectory*)getFile(i, f);
			if (found == NULL)
			{
				throw YCPackException(1002, "YCLYCPackFile::mkdir����Ŀ¼ʧ��", f.name);
			}
		}
	}

	if (found == NULL)
	{
		// Ŀ¼�봴��
		int slot = findAvailableSlot();
		if (slot < 0)
		{
			throw YCPackException(1002, "YCLYCPackFile::mkdirĿ¼������", filename());
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
			throw YCPackException(1002, "YCLYCPackFile::mkdir���Ŀ¼ʧ��");
		}
	}

	dirty();

	return true;
}

//
// �Ƿ����fileInPack
//
int YCLYCFilePack::has( const char* fileInPack )
{
	SMART_ASSERT(fileInPack != NULL);
	if (fileInPack == NULL || strlen(fileInPack) == 0)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::has�����ļ�ΪNULL");
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
				throw YCPackException(1002, "YCLYCFileDirectory::has����Ŀ¼Ϊ�ļ�", f.name);
			}

			// ����Ƿ��Ѿ����ص��ڴ�
			found = getFile(i, f);
			if (found == NULL)
			{
				throw YCPackException(1002, "YCLYCFileDirectory::has��ȡ�ļ�ʧ��", f.name);
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
				throw YCPackException(1002, "YCLYCFileDirectory::has����Ŀ¼Ϊ�ļ�", first);
			}

			return ((YCIFileDirectory*)found)->has(remain);
		}
	}

	return 0;
}

//
// ��ȡpackFile�ļ�
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
							throw YCPackException(1002, "YCLYCPackFile::get�ļ����Ͳ�һ��", f.name);
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
					throw YCPackException(1002, "YCLYCFilePack::get�ļ�δ�ҵ�", remain);
				}
					
				found = ((YCIFileDirectory*)found)->get(remain);
			}	
		}			
	}

	return found;
}

//
// ɾ��packFile�ļ����ҵ��ñ�־λ����
//
bool YCLYCFilePack::remove(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::remove�ļ�Ϊ��");
	}

	YCPathWalker walker(packFile);
	int length;
	YCPathWalker::E_FILE_TYPE type;
	const char* first = walker.next(length, type);
	if (first == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::remove·������");
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
						throw YCPackException(1002, "YCLYCFilePack::removeĿ�겻���ļ���", f.name);
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

					// �ļ��еݹ��ͷ�
					if (f.type == YCIAbstractFile::FILE_DIR)
					{
						if (!isFileLoaded(i))
						{
							loadFile(i, f);
						}
						YCIFileDirectory* dir = (YCIFileDirectory*)getLoaded(i);
						dir->clear();
					}

					// �ͷ�����
					if (f.length > 0)
					{	
						addIdleBlock(f.offset, ROUNDSIZE(f.length));
					}

					// ��ձ�ʾ
					memset(&f, 0, sizeof(LYC_FILE_T_1));

					// �ͷ��ڴ滺��
					SAFE_DELETE(myFiles[i]);

					// ��
					dirty();
				}
			}
		}
	}
	
	return found;
}

//
// ɾ��packFile�ļ����ҵ��ñ�־λ����
//
bool YCLYCFilePack::remove(YCIAbstractFile* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::remove�ļ�Ϊ��");
	}

	return remove(packFile->filename());
}

//
// ����ļ����µ������ļ�
//
bool YCLYCFilePack::clear()
{
	for (int i = 0; i < FILES_IN_DIR; ++i)
	{
		LYC_FILE_T_1& f = myHeader->files[i];
		if (f.type != 0)
		{
			// �ݹ��ͷ����ļ���
			if (f.type == YCIAbstractFile::FILE_DIR)
			{
				if (!isFileLoaded(i))
				{
					loadFile(i, f);
				}
				YCIFileDirectory* dir = (YCIFileDirectory*)getLoaded(i);
				dir->clear();
			}

			// �ͷ�����
			addIdleBlock(f.offset, ROUNDSIZE(f.length));
			memset(&f, 0, sizeof(LYC_FILE_T_1));

			// �ͷ��ڴ滺��
			SAFE_DELETE(myFiles[i]);

			// ��
			dirty();
		}
	}

	return true;
}

//
// ����Pack���������ļ�
//
void YCLYCFilePack::dump(const char* targetPath, bool overwrite)
{
	SMART_ASSERT(targetPath != NULL);
	if (targetPath == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::dump targetPathΪ��");
	}

	char theFullPath[MAX_PATH] = {0};
	sprintf(theFullPath, "%s\\%s", targetPath, getPackName());
	if (YCFileUtil::IsFileExist(theFullPath))
	{
		if (!YCFileUtil::IsDir(theFullPath))
		{
			throw YCPackException(1002, "YCLYCFilePack::dumpĿ���ļ������ļ�����", theFullPath);
		}

		if (!overwrite)
		{
			throw YCPackException(1002, "YCLYCFilePack::dump�ļ����Ѵ��ڣ� -fѡ�����ǿ�Ƹ���");
		}
	}
	else
	{
		if (!YCFileUtil::CreateDir(theFullPath, false))
		{
			throw YCPackException(1002, "YCLYCFilePack::dump����Ŀ���ļ���ʧ��", theFullPath);
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
				throw YCPackException(1002, "YCLYCFilePack::dump�����ļ�ʧ��", f.name);
			}

			file->dump(theFullPath, overwrite);
		}
	}
}

//
// ��PackFile���˸Ķ�֮��ǿ��ˢ�µ�����
//
void YCLYCFilePack::flush()
{
	// ֪ͨ���ļ�ˢ�µ�����
	for (int i = 0; i < FILES_IN_DIR; ++i)
	{
		YCIAbstractFile* file = myFiles[i];
		if (file && file->isDirty())
		{
			file->flush();
		}
	}

	// ˢ�ļ�ͷ
	if (isDirty())
	{
		myBuilder->writePack(this);
	}

	// ˢ��������
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
				throw YCPackException(1002, "YCLYCFilePack::flushд���ļ���������ʧ��");
			}

			prev = next;
		}

		if (0 != fflush(myFd))
		{
			throw YCPackException(1002, "YCLYCFilePack::flushд���ļ���������ˢ�µ�����ʧ��");
		}
	}

	myDirty = false;
}

//
// �ҽӿ��п�, �������Һϲ�
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
			// case 1 : �Һϲ�,offset�����ƽ�
			if (left == NULL)
			{
				if (offset+length == r_Idle->offset)
				{
					r_Idle->offset = offset;
					r_Idle->length += length;
				}
				else
				{
					myIdleList->insert(new IDLE_BLOCK(offset, length), NULL); // ��ͷ��
				}
			}

			// case 2 : ���Һϲ�
			else
			{
				IDLE_BLOCK* l_merge = NULL;

				// ��
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

				// ��
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

	// case 3 : ������ϲ�
	if (!inserted)
	{
		Item_List_T* tail = myIdleList->r_begin();
		if (tail == NULL)
		{
			myIdleList->append(new IDLE_BLOCK(offset, length), NULL); //������
		}
		else
		{
			IDLE_BLOCK* idle = (IDLE_BLOCK*)myIdleList->payload(tail);
			if (idle->offset+idle->length == offset)
			{
				idle->length += length;            // �ϲ�
			}
			else
			{
				myIdleList->append(new IDLE_BLOCK(offset, length), tail); //����β
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
// ���ҿ��е��ļ���
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
			myHeader->idle = 0; //�޿��п�
		}

		dirty();
	}

	return block;
}

//////////////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////////////

//
// ��ȡ���ļ����ڴ��еľ���
//
YCIAbstractFile* YCLYCFilePack::getFile(unsigned int index, const LYC_FILE_T_1& file)
{
	// ����Ƿ��Ѿ����ص��ڴ�
	YCIAbstractFile* found = getLoaded(index);

	// ��δ����
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
// �����ļ����ڴ棬����YCIAbstractFile��myFiles
//
YCIAbstractFile* YCLYCFilePack::loadFile(unsigned int index, const LYC_FILE_T_1& file)
{
	SMART_ASSERT(file.type != 0);
	if (file.type == 0)
	{
		throw YCPackException(1002, "YCLYCPackFile::loadFile�ļ���λ��");
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
		throw YCPackException(1002, "YCLYCPackFile::loadFileδ֪�ļ�����");
	}

	if (result != NULL)
	{
		myFiles[index] = result;
	}
	
	return result;
}

//
// Ŀ¼�µ��ļ��Ƿ��Ѿ����ص��ڴ�
//
bool YCLYCFilePack::isFileLoaded(unsigned int index)
{
	SMART_ASSERT(index < FILES_IN_DIR);

	if (index >= FILES_IN_DIR)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::isFileLoaded�±��쳣");
	}

	return (myFiles[index] != NULL);
}

//
// ��ȡ���ص��ڴ��е��ļ�
//
YCIAbstractFile* YCLYCFilePack::getLoaded(unsigned int index)
{
	return (isFileLoaded(index) ? myFiles[index] : NULL);
}

//
// ���ҿ��е��ļ���λ
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
