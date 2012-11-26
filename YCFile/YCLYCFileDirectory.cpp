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
// �������� ���ݹ�����ļ�
///////////////////////////////////////////////////////////
struct LYC_DIR_ADD_T {YCIFileDirectory* position; bool force;};
static void* addDirectory(const char* path, const char* filename, void* customData)
{
	SMART_ASSERT(customData != NULL);
	if (customData == NULL)
	{
		throw YCPackException(1002, "addDirectoryδ����YCLYCFileDirectory���");
	}

	LYC_DIR_ADD_T* addInfo = (LYC_DIR_ADD_T*)customData;
	char theFullPath[MAX_PATH] = {0};
	sprintf(theFullPath, "%s\\%s", path, filename);
	if (YCFileUtil::IsDir(theFullPath))
	{
		if (!addInfo->position->mkdir(filename, false))
		{
			throw YCPackException(1002, "addDirectory����Ŀ¼ʧ��");
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
// ��������Header
//
void YCLYCFileDirectory::setSelfHeader(LYC_FILE_T_1* header)
{
	SMART_ASSERT(header != NULL);
	mySelfHeader = header;
}

//
// �����ļ�ͷ
//
void YCLYCFileDirectory::setHeader(LYC_DIR_T_1* header)
{
	SMART_ASSERT(header != NULL);
	SAFE_DELETE(myHeader);
	myHeader = header;
}

//
// ע�ͣ�ȡ���ļ�ͷ
//
LYC_DIR_T_1* YCLYCFileDirectory::getHeader()
{
	return myHeader;
}

//
// ע�ͣ������ļ�����ָ��
//
const char* YCLYCFileDirectory::getContent()
{
	return (const char*)myHeader;
}

//
// ȡ���ļ��ڰ��ļ���ƫ����
//
unsigned int YCLYCFileDirectory::getOffset()
{
	if (mySelfHeader->offset == 0)
	{
		throw YCPackException(1002, "YCLYCFile::getOffset��δ��ʼ��");
	}
	return mySelfHeader->offset;
}

//
// ע�ͣ������ļ�����
//
int YCLYCFileDirectory::getLength()
{
	return sizeof(LYC_DIR_T_1);
}

//
// ��Ӵ���reguleFile��Ŀ¼��
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
			throw YCPackException(1002, "YCLYCFileDirectory::add�ļ���������", reguleFile);
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

		YCIPackFileBuilder* theBuilder = myPack->getBuilder();
		if (theBuilder == NULL)
		{
			throw YCPackException(1002, "YCLYCFilePack::add��Builder");
		}

		// �Ѽ������ڴ�
		myFiles[slot] = theBuilder->createNormal(this, slot, reguleFile);
	}

	return true;
}

//
// ����ڴ��ļ���Ŀ¼��
//
bool YCLYCFileDirectory::add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool force)
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

		YCIPackFileBuilder* theBuilder = myPack->getBuilder();
		if (theBuilder == NULL)
		{
			throw YCPackException(1002, "YCLYCFilePack::add��Builder");
		}

		myFiles[slot] = theBuilder->createNormal(this, slot, filename, content, length);
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

			YCIPackFileBuilder* theBuilder = myPack->getBuilder();
			if (theBuilder == NULL)
			{
				throw YCPackException(1002, "YCLYCFilePack::add��Builder");
			}

			myFiles[slot] = theBuilder->createNormal(this, slot, filename, content, length);
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
bool YCLYCFileDirectory::mkdir(const char* dir, bool recursive)
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

		YCIPackFileBuilder* theBuilder = myPack->getBuilder();
		if (theBuilder == NULL)
		{
			throw YCPackException(1002, "YCLYCFilePack::add��Builder");
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
			throw YCPackException(1002, "YCLYCPackFile::mkdir���Ŀ¼ʧ��");
		}
	}

	dirty();

	return true;
}

//
// �Ƿ����fileInPack
//
int YCLYCFileDirectory::has(const char* fileInPack)
{
	SMART_ASSERT(fileInPack != NULL);
	if (fileInPack == NULL || strlen(fileInPack) == 0)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::has�����ļ�ΪNULL");
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
				// β�ڵ����ʹ�ļ���Ҳ�������ļ���
				return file.type;
			}
			else
			{
				if (file.type != YCIAbstractFile::FILE_DIR)
				{
					throw YCPackException(1002, "YCLYCFileDirectory::has���ַ�Ŀ¼", file.name);
				}

				const std::string& restLevel = strFile.substr(pos+1); 
				if (restLevel.length() == 0 || restLevel == "\\")
				{
					return true; //������ "\"
				}

				// ����Ƿ��Ѿ����ص��ڴ�
				YCIAbstractFile* cachedFile = getFile(i, file);
				if (cachedFile == NULL)
				{
					throw YCPackException(1002, "YCLYCFileDirectory::has��ȡ�ļ�ʧ��", file.name);
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
// ��ȡpackFile�ļ�
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
							throw YCPackException(1002, "YCLYCFileDirectory::get�ļ����Ͳ�һ��", f.name);
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
					throw YCPackException(1002, "YCLYCFileDirectory::get�ļ�δ�ҵ�", remain);
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
bool YCLYCFileDirectory::remove(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::remove�ļ�Ϊ��");
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
						throw YCPackException(1002, "YCLYCFileDirectory::removeĿ�겻���ļ���", f.name);
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
						myPack->addIdleBlock(f.offset, ROUNDSIZE(f.length));
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
bool YCLYCFileDirectory::remove(YCIAbstractFile* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::remove�ļ�Ϊ��");
	}

	return remove(packFile->filename());
}

//
// ����ļ����µ������ļ�
//
bool YCLYCFileDirectory::clear()
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
			myPack->addIdleBlock(f.offset, ROUNDSIZE(f.length));
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
// ����Ŀ¼�������ļ�
//
void YCLYCFileDirectory::dump(const char* targetPath, bool overwrite)
{
	SMART_ASSERT(targetPath != NULL);
	if (targetPath == NULL)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::dump targetPathΪ��");
	}

	char theFullPath[MAX_PATH] = {0};
	sprintf(theFullPath, "%s\\%s", targetPath, filename());
	if (YCFileUtil::IsFileExist(theFullPath))
	{
		if (!YCFileUtil::IsDir(theFullPath))
		{
			throw YCPackException(1002, "YCLYCFileDirectory::dumpĿ���ļ������ļ�����", theFullPath);
		}

		if (!overwrite)
		{
			throw YCPackException(1002, "YCLYCFileDirectory::dump�ļ����Ѵ��ڣ� -fѡ�����ǿ�Ƹ���");
		}
	}
	else
	{
		if (!YCFileUtil::CreateDir(theFullPath, false))
		{
			throw YCPackException(1002, "YCLYCFileDirectory::dump����Ŀ���ļ���ʧ��", theFullPath);
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
				throw YCPackException(1002, "YCLYCFileDirectory::dump�����ļ�ʧ��", f.name);
			}

			file->dump(theFullPath, overwrite);
		}
	}
}

//
// �����˸Ķ�֮��ǿ��ˢ�µ�����
//
void YCLYCFileDirectory::flush()
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
		YCIPackFileBuilder* theBuilder = myPack->getBuilder();
		if (theBuilder == NULL)
		{
			throw YCPackException(1002, "YCLYCFileDirectory::flush BuilderΪ��");
		}
		theBuilder->writeDir(this);

		myDirty = false;
	}
}

//////////////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////////////

//
// ��ȡ���ļ����ڴ��еľ���
//
YCIAbstractFile* YCLYCFileDirectory::getFile(unsigned int index, const LYC_FILE_T_1& file)
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
YCIAbstractFile* YCLYCFileDirectory::loadFile(unsigned int index, const LYC_FILE_T_1& file)
{
	SMART_ASSERT(file.type != 0);
	if (file.type == 0)
	{
		throw YCPackException(1002, "YCLYCPackFile::loadFile�ļ���λ��");
	}

	YCIPackFileBuilder* theBuilder = myPack->getBuilder();
	if (theBuilder == NULL)
	{
		throw YCPackException(1002, "YCLYCFileDirectory::mkdir����builderʧ��");
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
bool YCLYCFileDirectory::isFileLoaded(unsigned int index)
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
YCIAbstractFile* YCLYCFileDirectory::getLoaded(unsigned int index)
{
	return (isFileLoaded(index) ? myFiles[index] : NULL);
}

//
// ���ҿ��е��ļ���λ
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