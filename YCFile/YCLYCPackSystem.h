#ifndef _INCLUDE_YCLYCPACKSYSTEM_H_
#define _INCLUDE_YCLYCPACKSYSTEM_H_

#include "YCBasic\YCIPackFileSystem.h"

class YCLYCFactory;

/*
 * YCLYCFileSystem ����׺��Ϊ.lyc�Ĵ���ļ�
 */
class YCAPI YCLYCPackSystem :
	public YCIPackFileSystem
{
public:

	YCLYCPackSystem(void);
	
	virtual ~YCLYCPackSystem(void);

	//
	// ע�ͣ���ʼ���ļ�ϵͳ
	//
	virtual bool initialize();

	//
	// ע�ͣ�����ļ�����·��
	//
	virtual bool mount(const char* regulePath);

	//
	// ע�ͣ�ȡ���ļ�����·��
	//
	virtual bool umount(const char* regulePath);

	//
	// ע�ͣ���һ��ѹ�����ļ�
	//
	//  �ڲ�����������mountʱ��ȡ���������ṩop
	//
	virtual bool open(const char *);

	//
	// ע�ͣ��ر�һ��ѹ�����ļ�
	//
	//  �ڲ�����������umountʱ��ȡ���������ṩop
	//
	virtual bool close(const char *);

	//
	// ע�ͣ�����һ��ѹ�����ļ�
	//      
	//       op : touch version xxx.ycp
	//
	virtual YCIFilePack* create(const char *, unsigned int);

	//
	// ע�ͣ�ɾ��һ��ѹ�����ļ�
	//       �����ǰ·���ڸ�ѹ�����ļ��У�ɾ��ʧ��
	//
	//       op : rm -f xxx.ycp
	//
	virtual void remove(const char* packFile, bool force);

	//
	// ע�ͣ�������Ƭ����
	//
	//  op ��defrag pathInPack
	//
	virtual void defrag(const char* pathInPack);

	//
	// ע�ͣ�����ڴ��ļ������ļ���
	//       pathInPack����ʹ���·����Ҳ����ʹ����·��
	//       length > 0 ���������
	//
	virtual bool add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool force);

	//
	// ע�ͣ����һ���������reguleFile�ļ���pathInPack·����Ӧ��ѹ�����ļ���
	//       ��Ӧ·������ͬ���ļ���overwrite�Ƿ�Ϊtrue
	//       ���򸲸ǣ������׳�YCPackException�쳣
	//
	//     op : copy -f c:\pic\xxx.png .
	//
	virtual bool copy(const char* reguleFile, const char* pathInPack, bool overwrite);

	//
	// ע�ͣ���ѹ������ɾ��һ���ļ���ɾ��ʧ�ܻ��ļ��������׳�YCPackException�쳣
	//     
	//   op : rm /pathInPack/xxx.png
	//
	virtual void erase(const char* fileInPack);

	//
	// ע�ͣ���ѹ�����в����ļ���pathInPackΪ����·��
	//       ���pathInPackΪ�գ����ڵ�ǰ·���²���
	//       ���pathInPack Ϊ����·������Ӹ�Ŀ¼�²���
	//       ���ز��ҵ��ĵ�һ���ļ������û���ҵ�������NULL
	// 
	//  op : find 0001.png /ui/0/    <= ����·��
	//       find 0001.png           <= ��ǰ·����
	//
	virtual YCIAbstractFile* find(const char* packFile, const char* pathInPack);

	//
	// ע�ͣ��鿴һ���ļ�����ϸ��Ϣ
	// 
	//   op : file /pathInPack/xxx.png
	//
	virtual YCIAbstractFile* file(const char* packFile);

	//
	// ע�ͣ�dumpһ��ѹ��packFile�ļ���pathInRegule·����Ӧ�Ĵ����ļ�ϵͳ��
	//       ��Ӧ·������ͬ���ļ���overwrite�Ƿ�Ϊtrue
	//       ���򸲸ǣ������׳�YCPackException�쳣
	//
	//   op : dump -f /pathInPack/xxx.png c:/pic/
	//
	virtual void dump(const char* packFile, const char* pathInRegule, bool overwrite);

	//
	// ע�ͣ�cd �����ļ�Ŀ¼
	//       /��ͷΪ����·��
	//
	//   op : cd /packFileWithoutExtension/pathInPack/  ��Ŀ¼����
	//        cd pathInPack     ���·������
	//
	virtual bool cd(const char*);

	//
	// ע�ͣ�������Ŀ¼
	//       recursive : �Ƿ�ݹ鴴�� -p
	//
	// op ��mkdir -p /packFileWithoutExtension/pathInPack/newDir  ��Ŀ¼����
	//      mkdir -p pathUnderCurrentPath/newDir ���·������
	//
	virtual bool mkdir(const char* dirname, bool recursive);

	//
	// ע�ͣ��ͷ��ļ�ϵͳ
	//
	virtual void finalize();

private:

	//
	// Pack�ļ�������
	//
	YCLYCFactory * myFactory;

	//
	// ˵�����ѹҽ�·��
	//
	YCDList * myMounted;

};

#endif

