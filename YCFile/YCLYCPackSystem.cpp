#include "YCLYCPackSystem.h"

#include "YCLYCDefine.h"
#include "YCLYCFactory.h"
#include "YCLYCFilePack.h"
#include "YCLYCFileBuilder1.h"
#include "YCLYCFileDirectory.h"

#include "YCBasic\YCIAbstractFile.h"
#include "YCBasic\YCIFileNormal.h"
#include "YCBasic\YCIFilePack.h"
#include "YCBasic\YCIFileDirectory.h"

#include "YCBasic\YCDlist.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCScopeGuard.h"
#include "YCBasic\YCPathWalker.h"
#include "YCBasic\YCStringHelper.h"
#include "YCBasic\YCPackException.h"

#pragma region handle
//////////////////////////////////////////////////////
// YCFileUtil::HandleFileInDir ��������
//////////////////////////////////////////////////////

static void* mountPackFile(const char* path, const char* filename, void* customData)
{
	SMART_ASSERT(customData != NULL);
	if (customData == NULL)
	{
		throw YCPackException(1002, "mountPackFileδ����YCLYCFileSystem���");
	}

	char fullname[MAX_PATH] = {0};
	sprintf(fullname, "%s\\%s", path, filename);
	YCLYCPackSystem* fileSystem = (YCLYCPackSystem*)customData;
	fileSystem->open(fullname);

	return NULL;
}

static void* umountPackFile(const char* path, const char* filename, void* customData)
{
	SMART_ASSERT(customData != NULL);
	if (customData == NULL)
	{
		throw YCPackException(1002, "mountPackFileδ����YCLYCFileSystem���");
	}

	char fullname[MAX_PATH] = {0};
	sprintf(fullname, "%s\\%s", path, filename);
	YCLYCPackSystem* fileSystem = (YCLYCPackSystem*)customData;
	fileSystem->close(fullname);

	return NULL;
}

#pragma endregion handle

YCLYCPackSystem::YCLYCPackSystem(void)
	: myFactory(NULL)
	, myMounted(NULL)
{
}


YCLYCPackSystem::~YCLYCPackSystem(void)
{
	SAFE_DELETE(myFactory);
	SAFE_DELETE(myMounted);
}

//
// ע�ͣ���ʼ���ļ�ϵͳ
//
bool YCLYCPackSystem::initialize()
{
	// ����������
	SMART_ASSERT(myFactory == NULL);
	if (myFactory != NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::initialize�ظ���ʼ��");
	}

	myFactory = new YCLYCFactory();
	myFactory->regist(new YCLYCFileBuilder1());

	return true;
}

//
// ע�ͣ�����ļ�����·��
//
bool YCLYCPackSystem::mount(const char* regulePath)
{
	SMART_ASSERT(regulePath != NULL);
	if (regulePath == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::mount·��Ϊ��");
	}

	if (!YCFileUtil::IsFileExist(regulePath))
	{
		throw YCPackException(1002, "YCLYCFileSystem::mount·��������");
	}

	std::string pathWithLYCFilter(regulePath);
	pathWithLYCFilter += LYC_FILE_EXTENSION;
	YCFileUtil::HandleFileInDir(pathWithLYCFilter.c_str(), mountPackFile, this);

	return true;
}

//
// ע�ͣ�ȡ���ļ�����·��
//
bool YCLYCPackSystem::umount(const char* regulePath)
{
	SMART_ASSERT(regulePath != NULL);
	if (regulePath == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::umount·��Ϊ��");
	}

	if (!YCFileUtil::IsFileExist(regulePath))
	{
		throw YCPackException(1002, "YCLYCFileSystem::umount·��������");
	}

	std::string pathWithLYCFilter(regulePath);
	pathWithLYCFilter += LYC_FILE_EXTENSION;
	YCFileUtil::HandleFileInDir(pathWithLYCFilter.c_str(), umountPackFile, this);

	return true;
}

//
// ע�ͣ���һ��ѹ�����ļ�
//
//  �ڲ�����������mountʱ��ȡ���������ṩop
//
bool YCLYCPackSystem::open(const char * packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::open���ļ�Ϊ��");
	}

	// �Ƿ��Ѽ���
	for (Item_List_T* item = myPackFiles->begin();
		 item != myPackFiles->end();
		 item = myPackFiles->next(item))
	{
		YCIFilePack* thePack = (YCIFilePack*)myPackFiles->payload(item);
		if (stricmp(thePack->filename(), packFile) == 0)
		{
			return true;
		}
	}

	LYC_PACK_HEAD head;
	FILE* fd = fopen(packFile, "r");
	SCOPE_GUARD(fd_guard, fclose(fd));
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::open��ȡ�ļ�ʧ��", packFile);
	}

	if (1 != fread(&head, sizeof(LYC_PACK_HEAD), 1, fd))
	{
		throw YCPackException(1002, "YCLYCFileSystem::open��ȡȫ���ļ�ͷʧ��", packFile);
	}

	YCIPackFileBuilder* builder = myFactory->getBuilder(head.version);
	if (builder == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename��ȡ������ʧ��",packFile);
	}

	YCIFilePack* result = builder->loadPack(packFile);
	if (result == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename����ѹ�����ļ�ʧ��", packFile);
	}

	myPackFiles->append(result, NULL);

	return true;
}

//
// ע�ͣ��ر�һ��ѹ�����ļ�
//
//  �ڲ�����������umountʱ��ȡ���������ṩop
//
bool YCLYCPackSystem::close(const char * packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::open���ļ�Ϊ��");
	}

	YCIFilePack* found = NULL;
	for (Item_List_T* item = myPackFiles->begin();
		 item != myPackFiles->end();
		 item = myPackFiles->next(item))
	{
		YCIFilePack* exist = (YCIFilePack*)myPackFiles->payload(item);
		if (stricmp(packFile, exist->getPackName()) == 0)
		{
			found = exist;
			myPackFiles->remove(item);
			break;
		}
	}
	
	SAFE_DELETE(found);

	return true;
}

//
// ע�ͣ�����һ��ѹ�����ļ�
//      
//       op : touch version xxx.ycp
//
YCIFilePack* YCLYCPackSystem::create(const char * filename, unsigned int version)
{
	SMART_ASSERT(filename != NULL);
	if (filename == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename�ļ���Ϊ��");
	}

	if (YCFileUtil::IsFileExist(filename))
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename�ļ��Ѵ���");
	}

	YCIPackFileBuilder* builder = myFactory->getBuilder(version);
	if (builder == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename��ȡ������ʧ��");
	}

	YCIFilePack* result = builder->createPack(filename);
	if (result == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename����ѹ�����ļ�ʧ��");
	}

	myPackFiles->append(result, NULL);

	return result;
}

//
// ע�ͣ�ɾ��һ��ѹ�����ļ�
//       �����ǰ·���ڸ�ѹ�����ļ��У�ɾ��ʧ��
//
//       op : rm xxx.ycp
//
void YCLYCPackSystem::remove(const char* packFile, bool force)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::removeɾ��ѹ�����ļ�Ϊ��");
	}

	if (!YCFileUtil::IsFileExist(packFile))
	{
		throw YCPackException(1002, "YCLYCFileSystem::removeɾ��ѹ�����ļ��ļ�������");
	}

	bool found = false;
	for (Item_List_T* item = myPackFiles->begin();
		 item != myPackFiles->end();
		 item = myPackFiles->next(item))
	{
		YCLYCFilePack* thePack = (YCLYCFilePack*)myPackFiles->payload(item);
		if (stricmp(packFile, thePack->getPackName()) == 0)
		{
			if (myCurrent->getPack() == thePack)
			{
				throw YCPackException(1002, "YCLYCFileSystem::removeɾ��ѹ�����ļ���ʹ����");
			}
			found = true;
			myPackFiles->remove(item);
			break;
		}
	}

	if (found)
	{
		YCFileUtil::RemoveFile(packFile);
	}
}

//
// ע�ͣ�������Ƭ����
//
//  op ��defrag pathInPack
//
void YCLYCPackSystem::defrag(const char* pathInPack)
{

}

//
// ע�ͣ����һ���������reguleFile�ļ���pathInPack·����Ӧ��ѹ�����ļ���
//       ��Ӧ·������ͬ���ļ���overwrite�Ƿ�Ϊtrue
//       ���򸲸ǣ������׳�YCPackException�쳣
//
//     op : copy -f c:\pic\xxx.png .
//
bool YCLYCPackSystem::copy( const char* reguleFile, const char* pathInPack, bool overwrite )
{
	SMART_ASSERT(reguleFile != NULL);
	if (reguleFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::copyԴ�ļ�ΪNULL");
	}

	if (pathInPack == NULL || strlen(pathInPack) == 0)
	{
		throw YCPackException(1002, "YCLYCFileSystem::copyĿ��λ��Ϊ��");
	}

	if (!YCFileUtil::IsFileExist(reguleFile))
	{
		throw YCPackException(1002, "YCLYCFileSystem::copyԴ�ļ�������");
	}

	if (YCFileUtil::IsDir(reguleFile))
	{
		//�ݹ����ļ����Ƿ����FILES_IN_DIR
	}
	//else
	//{
	//	if (YCFileUtil::GetFileLength(reguleFile) == 0)
	//	{
	//		throw YCPackException(1002, "YCLYCFileSystem::copyԴ�ļ�Ϊ��");
	//	}
	//}
	

	YCLYCFilePack* thePack = NULL;
	YCIFileDirectory* position = NULL;
	const char* remain = NULL;

	if (!YCPathWalker::isAbsolutePath(pathInPack) && myCurrent != NULL)
	{
		position = myCurrent;
		remain = pathInPack;
	}
	else
	{
		YCPathWalker walker(pathInPack);
		int length;
		YCPathWalker::E_FILE_TYPE type;

		const char* first = walker.next(length, type);

		for (Item_List_T* item = myPackFiles->begin();
			item != myPackFiles->end() && !position;
			item = myPackFiles->next(item))
		{
			thePack = (YCLYCFilePack*)myPackFiles->payload(item);
			if (strnicmp(thePack->getPackName(), first, length) == 0)
			{
				position = thePack;
			}
		}

		if (position == NULL)
		{
			throw YCPackException(1002, "YCLYCFileSystem::copyĿ¼δ�ҵ�", first);
		}

		remain = walker.remain();
	}

	// ��remain�ݹ������Ŀ¼������������򴴽�
	if (remain != NULL)
	{
		if (!position->has(remain))
		{
			position->mkdir(remain, true);
		}

		position = (YCIFileDirectory*)position->get(remain);
		if (position == NULL)
		{
			return false;
		}
	}	
	
	if (!position->add(reguleFile, overwrite))
	{
		return false;
	}

	thePack->flush();
	return true;
}

//
// ע�ͣ���ѹ������ɾ��һ���ļ���ɾ��ʧ�ܻ��ļ��������׳�YCPackException�쳣
//     
//   op : rm -f /pathInPack/xxx.png
//
void YCLYCPackSystem::erase(const char* fileInPack)
{
	SMART_ASSERT(fileInPack != NULL);
	if (fileInPack == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::eraseɾ���ļ���Ϊ��");
	}

	YCIFileDirectory* position = NULL;

	char nextPath[MAX_PATH] = {0};
	if (!YCPathWalker::isAbsolutePath(fileInPack) && myCurrent != NULL)
	{
		myCurrent->remove(fileInPack);
		myCurrent->getPack()->flush();
	}
	else
	{
		YCPathWalker walker(fileInPack);
		int length;
		YCPathWalker::E_FILE_TYPE type;
		const char* first = walker.next(length, type);
		if (first == NULL || type == YCPathWalker::DISK)
		{
			throw YCPackException(1002, "YCLYCFileSystem::erase��·������");
		}

		YCIFileDirectory * position = NULL;
		for (Item_List_T* item = myPackFiles->begin();
			item != myPackFiles->end() && !position;
			item = myPackFiles->next(item))
		{
			YCLYCFilePack* exists = (YCLYCFilePack*)myPackFiles->payload(item);
			if (strnicmp(exists->getPackName(), first, length) == 0)
			{
				position = exists;
			}
		}

		if (NULL == position)
		{
			throw YCPackException(1002, "YCLYCFileSystem::erase���ļ�������");
		}

		const char* remain = walker.remain(false);

		//�Ƴ����ļ�
		if (remain == NULL) 
		{
			throw YCPackException(1002, "YCLYCPackSystem::erase�Ƴ����ļ���ʹ��rm -f");
		}

		// �Ƴ����ļ��µ������ļ�
		if (strlen(remain) == 1 && *remain == '\\')
		{
			position->clear();
		}
		else
		{
			position->remove(remain);
		}

		position->getPack()->flush();
	}
}

//
// ע�ͣ�����ڴ��ļ������ļ���
//       pathInPack����ʹ���·����Ҳ����ʹ����·��
//       length > 0 ���������
//
bool YCLYCPackSystem::add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool force)
{
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(pathInPack != NULL);
	SMART_ASSERT(content != NULL && length > 0);

	if (filename == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::add�ڴ��ļ���Ϊ��");
	}

	if (pathInPack == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::add��·��Ϊ��");
	}

	if (content == NULL || length == 0)
	{
		throw YCPackException(1002, "YCLYCFilePack::add�ڴ��ļ�Ϊ��");
	}

	YCIFileDirectory* position = NULL;

	char nextPath[MAX_PATH] = {0};
	if (!YCPathWalker::isAbsolutePath(pathInPack) && myCurrent != NULL)
	{
		position = myCurrent; 
	}
	else
	{
		YCPathWalker walker(pathInPack);
		int pathLength;
		YCPathWalker::E_FILE_TYPE type;
		const char* first = walker.next(pathLength, type);
		if (first == NULL || type == YCPathWalker::DISK)
		{
			throw YCPackException(1002, "YCLYCFileSystem::add��·������");
		}

		YCIFileDirectory * position = NULL;
		for (Item_List_T* item = myPackFiles->begin();
			item != myPackFiles->end() && !position;
			item = myPackFiles->next(item))
		{
			YCLYCFilePack* exists = (YCLYCFilePack*)myPackFiles->payload(item);
			if (strnicmp(exists->getPackName(), first, pathLength) == 0)
			{
				position = exists;
			}
		}

		if (NULL == position)
		{
			throw YCPackException(1002, "YCLYCFileSystem::add���ļ�������");
		}
	}

	position->add(filename, pathInPack, content, length, force);
	position->getPack()->flush();

	return false;
}

//
// ע�ͣ���ѹ�����в����ļ���pathInPackΪ����·��
//       ���pathInPackΪ�գ����ڵ�ǰ·���²���
//       ���pathInPack Ϊ����·������Ӹ�Ŀ¼�²���
//       ���ز��ҵ��ĵ�һ���ļ������û���ҵ�������NULL
// 
//  op : find 0001.png /ui/0/    <= ����·��
//       find 0001.png           <= ��ǰ·����
//
YCIAbstractFile* YCLYCPackSystem::find(const char* packFile, const char* pathInPack)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::find��������");
	}

	char nextPath[MAX_PATH] = {0};
	if (!YCPathWalker::isAbsolutePath(pathInPack) && myCurrent != NULL)
	{
		sprintf(nextPath, "%s\\%s", pathInPack, packFile);
		return myCurrent->get(nextPath);
	}

	YCPathWalker walker(pathInPack);
	int length;
	YCPathWalker::E_FILE_TYPE type;
	const char* first = walker.next(length, type);
	if (first == NULL || type == YCPathWalker::DISK)
	{
		throw YCPackException(1002, "YCLYCFileSystem::find��·������");
	}

	YCIFileDirectory * position = NULL;
	for (Item_List_T* item = myPackFiles->begin();
			item != myPackFiles->end() && !position;
			item = myPackFiles->next(item))
	{
		YCLYCFilePack* exists = (YCLYCFilePack*)myPackFiles->payload(item);
		if (strnicmp(exists->getPackName(), first, length) == 0)
		{
			position = exists;
		}
	}

	if (NULL == position)
	{
		throw YCPackException(1002, "YCLYCFileSystem::find���ļ�������");
	}

	sprintf(nextPath, "%s\\%s", walker.remain(), packFile);
	return position->get(nextPath);
}

//
// ע�ͣ��鿴һ���ļ�����ϸ��Ϣ
// 
//   op : file /pathInPack/xxx.png
//
YCIAbstractFile* YCLYCPackSystem::file(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL || strlen(packFile) == 0)
	{
		throw YCPackException(1002, "YCLYCFileSystem::file���ļ�������");
	}

	YCIAbstractFile* found = NULL;

	try
	{
		// ����·�� or ���·��
		YCIFileDirectory* position = (packFile[0] == '\\') ? NULL : myCurrent;

		std::vector<std::string> dirs;
		YCStringHelper::split(packFile, "\\", &dirs);
		for (unsigned int i = 0; i < dirs.size(); ++i)
		{
			const std::string& dir = dirs[i];
			if (position == NULL) 
			{
				for (Item_List_T* item = myPackFiles->begin();
					item != myPackFiles->end();
					item = myPackFiles->next(item))
				{
					YCLYCFilePack* pack = (YCLYCFilePack*)myPackFiles->payload(item);
					if (stricmp(pack->getPackName(), dir.c_str()) == 0)
					{
						position = pack;
					}
				}

				if (position == NULL)
				{
					throw YCPackException(1002, "YCLYCFileSystem::fileĿ¼δ�ҵ�", dir.c_str());
				}
			}
			else
			{

			}
		}
	}
	catch (YCPackException& e)
	{
		LOG_ERROR("YCLYCFileSystem::file�����ļ�" << packFile << "����" << e.what());
	}

	return found;
}

//
// ע�ͣ�dumpһ��ѹ��packFile�ļ���pathInRegult·����Ӧ�Ĵ����ļ�ϵͳ��
//       ��Ӧ·������ͬ���ļ���overwrite�Ƿ�Ϊtrue
//       ���򸲸ǣ������׳�YCPackException�쳣
//
//   op : dump -f /pathInPack/xxx.png c:/pic/
//
void YCLYCPackSystem::dump(const char* packFile, const char* pathInRegule, bool overwrite)
{
	SMART_ASSERT(packFile != NULL);
	SMART_ASSERT(pathInRegule != NULL);

	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCPackSystem::dumpԴ�ļ�Ϊ��");
	}

	if (pathInRegule == NULL)
	{
		throw YCPackException(1002, "YCLYCPackSystem::dumpĿ���ļ���Ϊ��");
	}

	if (!YCFileUtil::IsDir(pathInRegule))
	{
		throw YCPackException(1002, "YCLYCPackSystem::dumpĿ���ļ��в�����");
	}

	YCPathWalker walker(packFile);
	int length;
	YCPathWalker::E_FILE_TYPE type;

	YCIFileDirectory* position = NULL;
	if (!YCPathWalker::isAbsolutePath(packFile) && myCurrent != NULL)
	{
		position = myCurrent; //��ǰ·���²���
	}
		
	const char* first = NULL;
	while ((first = walker.next(length, type)) != NULL) //�ݹ������Ŀ¼
	{
		std::string current(first, length);

		//�Ӱ��ļ�������
		if (position == NULL)
		{
			for (Item_List_T* item = myPackFiles->begin();
				item != myPackFiles->end() && !position;
				item = myPackFiles->next(item))
			{
				YCLYCFilePack* pack = (YCLYCFilePack*)myPackFiles->payload(item);
				if (stricmp(pack->getPackName(), current.c_str()) == 0)
				{
					position = pack;
				}
			}

			if (position == NULL)
			{
				throw YCPackException(1002, "YCLYCFileSystem::dumpĿ¼δ�ҵ�", current.c_str());
			}
		}
		else
		{
			YCIAbstractFile* abstractFile = position->get(current.c_str());
			if (type == YCPathWalker::DIR) //�������ļ���
			{
				if (abstractFile->type() != YCIAbstractFile::FILE_DIR)
				{
					throw YCPackException(1002, "YCLYCPackSystem::dump�ļ����Ͳ�һ��", current.c_str());
				}

				position = (YCIFileDirectory*)abstractFile;				
			}
		}
	}

	//�����������������ļ�Ҳ�������ļ���
	if (position != NULL)
	{
		position->dump(pathInRegule, overwrite);
	}
}

//
// ע�ͣ�cd �����ļ�Ŀ¼
//       \��ͷΪ����·��
//
//   op : cd /packFileWithoutExtension/pathInPack/  ��Ŀ¼����
//        cd pathInPack     ���·������
//
bool YCLYCPackSystem::cd(const char* path)
{
	if (path == NULL || strlen(path) == 0)
	{
		throw YCPackException(2002, "YCLYCFileSystem::cd��������ṩ·��");
	}

	try
	{
		YCPathWalker walker(path);
		int length;
		YCPathWalker::E_FILE_TYPE type;

		YCIFileDirectory* position = NULL;
		if (!YCPathWalker::isAbsolutePath(path) && myCurrent != NULL)
		{
			position = myCurrent;
		}

		for (const char* first = walker.next(length, type); first != NULL; first = walker.next(length, type))
		{
			std::string current(first, length);
			if (position == NULL) //��
			{
				for (Item_List_T* item = myPackFiles->begin();
					item != myPackFiles->end();
					item = myPackFiles->next(item))
				{
					YCLYCFilePack* pack = (YCLYCFilePack*)myPackFiles->payload(item);
					if (stricmp(pack->getPackName(), current.c_str()) == 0)
					{
						position = pack;
					}
				}

				if (position == NULL)
				{
					throw YCPackException(1002, "YCLYCFileSystem::cdĿ¼δ�ҵ�", current.c_str());
				}
			}
			else
			{
				YCIAbstractFile* file = position->get(current.c_str());
				if (file == NULL)
				{
					throw YCPackException(1002, "YCLYCFileSystem::cdĿ¼δ�ҵ�", current.c_str());
				}

				if (file->type() != YCIAbstractFile::FILE_DIR)
				{
					throw YCPackException(1002, "YCLYCFileSystem::cd����Ŀ¼�ļ�", current.c_str());
				}
				position = (YCIFileDirectory*)file;
			}
		}
		
		myCurrent = position;
	}
	catch (YCPackException& e)
	{
		LOG_ERROR("YCLYCFileSystem:cd����" << e.what());
		return false;
	}	

	return true;
}

//
// ע�ͣ�������Ŀ¼
//
// op ��mkdir /packFileWithoutExtension/pathInPack/newDir  ��Ŀ¼����
//      mkdir pathUnderCurrentPath/newDir ���·������
//
bool YCLYCPackSystem::mkdir(const char* dirname, bool recursive)
{
	if (dirname == NULL || strlen(dirname) == 0)
	{
		throw YCPackException(2002, "YCLYCFileSystem::mkdir��������ṩ·��");
	}

	bool result = false;
	try
	{
		YCPathWalker walker(dirname);
		int length;
		YCPathWalker::E_FILE_TYPE type;

		YCIFileDirectory* position = NULL;
		if (YCPathWalker::isAbsolutePath(dirname) && myCurrent != NULL)
		{
			position = myCurrent;
		}

		if (position == NULL) //��
		{
			const char* first = walker.next(length, type);
			if (first == NULL)
			{
				throw YCPackException(2002, "YCLYCFileSystem::mkdir����·������", dirname);
			}

			for (Item_List_T* item = myPackFiles->begin();
				item != myPackFiles->end() && !position;
				item = myPackFiles->next(item))
			{
				YCIFilePack* pack = (YCLYCFilePack*)myPackFiles->payload(item);
				if (strnicmp(pack->getPackName(), first, length) == 0)
				{
					position = pack;
				}
			}

			// ��Ŀ¼�޷�������ʵ�����ǰ��ļ�
			if (position == NULL)
			{
				throw YCPackException(1002, "YCLYCFileSystem::mkdirĿ¼δ�ҵ�", first);
			}

			const char* remain = walker.remain();
			if (remain != NULL)
			{
				result = position->mkdir(remain, recursive);
			}
			else
			{
				result = true; // ʲô������
			}
		}
		else  //��ǰĿ¼
		{
			result = position->mkdir(dirname, recursive);
		}
	}
	catch (YCPackException& e)
	{
		UNREFERENCED_PARAMETER(e);
	}

	return result;
}


//
// ע�ͣ��ͷ��ļ�ϵͳ
//
void YCLYCPackSystem::finalize()
{
	if (myPackFiles->size() > 0)
	{
		// ˢ�µ�����
		for (Item_List_T* item = myPackFiles->begin();
			 item != myPackFiles->end();
			 item = myPackFiles->next(item))
		{
			YCLYCFilePack* thePack = (YCLYCFilePack*)myPackFiles->payload(item);
			thePack->flush();
		}

		myPackFiles->clear(YCDListClearHelper<YCIFilePack>);
	}

	if (myFactory != NULL)
	{
		myFactory->finalize();
	}
}