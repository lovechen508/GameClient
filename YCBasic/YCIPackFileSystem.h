#ifndef _INCLUDE_YCIPACKFILESYSTEM_H_
#define _INCLUDE_YCIPACKFILESYSTEM_H_

class YCDList;
class YCIFilePack;
class YCIAbstractFile;
class YCIFileDirectory;

/*
 * YCIPackFileSystem : ����ļ�ϵͳ�ӿ���
 *
 * ˵����������ƴ���ļ�ϵͳ
 *      ֧�ֱ�׼�ļ����ʽӿ�
 */
class YCAPI YCIPackFileSystem
{
public:

	YCIPackFileSystem(void);

	virtual ~YCIPackFileSystem(void);

	//
	// ע�ͣ���ʼ���ļ�ϵͳ
	//
	virtual bool initialize() = 0;

	//
	// ע�ͣ�����ļ�����·��
	//
	virtual bool mount(const char* regulePath) = 0;

	//
	// ע�ͣ�ȡ���ļ�����·��
	//
	virtual bool umount(const char* regulePath) = 0;

	//
	// ע�ͣ���һ��ѹ�����ļ�
	//
	//  �ڲ�����������mountʱ��ȡ���������ṩop
	//
	virtual bool open(const char *) = 0;

	//
	// ע�ͣ��ر�һ��ѹ�����ļ�
	//
	//  �ڲ�����������umountʱ��ȡ���������ṩop
	//
	virtual bool close(const char *) = 0;

	//
	// ע�ͣ�����һ��ѹ�����ļ�
	//      
	//       op : touch version xxx.ycp
	//
	virtual YCIFilePack* create(const char *, unsigned int) = 0;

	//
	// ע�ͣ�ɾ��һ��ѹ�����ļ�
	//       �����ǰ·���ڸ�ѹ�����ļ��У�ɾ��ʧ��
	//
	//       op : rm xxx.ycp
	//
	virtual void remove(const char*, bool) = 0;

	//
	// ע�ͣ�������Ƭ����
	//
	//  op ��defrag pathInPack
	//
	virtual void defrag(const char* pathInPack) = 0;

	//
	// ע�ͣ�����ڴ��ļ������ļ���
	//       pathInPack����ʹ���·����Ҳ����ʹ����·��
	//       length > 0 ���������
	//
	virtual bool add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool force) = 0;

	//
	// ע�ͣ����һ���������reguleFile�ļ���pathInPack·����Ӧ��ѹ�����ļ���
	//       ��Ӧ·������ͬ���ļ���overwrite�Ƿ�Ϊtrue
	//       ���򸲸ǣ������׳�YCPackException�쳣
	//
	//     op : copy -f c:\pic\xxx.png .
	//
	virtual bool copy(const char* reguleFile, const char* pathInPack, bool overwrite) = 0;

	//
	// ע�ͣ���ѹ�������Ƴ�һ���ļ����Ƴ�ʧ�ܻ��ļ��������׳�YCPackException�쳣
	//     
	//   op : rm -f /pathInPack/xxx.png
	//
	virtual void erase(const char* fileInPack) = 0;

	//
	// ע�ͣ���ѹ�����в����ļ���pathInPackΪ����·��
	//       ���pathInPackΪ�գ����ڵ�ǰ·���²���
	//       ���pathInPack Ϊ����·������Ӹ�Ŀ¼�²���
	//       ���ز��ҵ��ĵ�һ���ļ������û���ҵ�������NULL
	// 
	//  op : find 0001.png /ui/0/    <= ����·��
	//       find 0001.png           <= ��ǰ·����
	//
	virtual YCIAbstractFile* find(const char* packFile, const char* pathInPack) = 0;

	//
	// ע�ͣ��鿴һ���ļ�����ϸ��Ϣ
	// 
	//   op : file /pathInPack/xxx.png
	//
	virtual YCIAbstractFile* file(const char* packFile) = 0;

	//
	// ע�ͣ�dumpһ��ѹ��packFile�ļ���pathInRegule·����Ӧ�Ĵ����ļ�ϵͳ��
	//       ��Ӧ·������ͬ���ļ���overwrite�Ƿ�Ϊtrue
	//       ���򸲸ǣ������׳�YCPackException�쳣
	//
	//   op : dump -f /pathInPack/xxx.png c:/pic/
	//
	virtual void dump(const char* packFile, const char* pathInRegule, bool overwrite) = 0;

	//
	// ע�ͣ�cd �����ļ�Ŀ¼
	//       /��ͷΪ����·��
	//
	//   op : cd /packFileWithoutExtension/pathInPack/  ��Ŀ¼����
	//        cd pathInPack     ���·������
	//
	virtual bool cd(const char*) = 0;

	//
	// ע�ͣ�������Ŀ¼
	//       recursive : �Ƿ�ݹ鴴�� -p
	//
	// op ��mkdir -p /packFileWithoutExtension/pathInPack/newDir  ��Ŀ¼����
	//      mkdir -p pathUnderCurrentPath/newDir ���·������
	//
	virtual bool mkdir(const char* dirname, bool recursive) = 0;

	//
	// ע�ͣ��ͷ��ļ�ϵͳ
	//
	virtual void finalize() = 0;

protected:

	//
	// ��ǰ��Ŀ¼�µ�����ѹ�����ļ������ṹ
	//
	YCDList *myPackFiles;

	//
	// ��ǰ�������ļ�Ŀ¼λ�ã��ض�����ĳYCIPackFile
	//
	YCIFileDirectory* myCurrent;
};

#endif

