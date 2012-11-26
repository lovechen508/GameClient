#ifndef _INCLUDE_YCIABSTRACTFILE_H_
#define _INCLUDE_YCIABSTRACTFILE_H_

#include <stdlib.h>

#include "YCAssert.h"
#include "YCException.h"

/*
 * YCIAbstractFile : ����ļ�����ӿ���
 *
 *                                   -------------------
 *                                  |  YCIAbstractFile  |
 *                                   -------------------
 *                                            |
 *                         --------------------------------------         
 *                        |                                      |
 *               -----------------                       ------------------ 
 *              |  YCIFileNormal  |                     | YCIFileDirectory |
 *               -----------------                       ------------------
 *                                                               |
 *                                                       ------------------                                                   
 *                                                      |   YCIFilePack    |
 *                                                       ------------------
 *
 */
class YCAPI YCIAbstractFile
{
public:

	enum {
		FILE_NORMAL  = 1, //�����ļ�
		FILE_DIR     = 2, //Ŀ¼�ļ�
		FILE_PACK	 = 3, //���ļ�
	};

public:

	YCIAbstractFile(const char* name, int type)
		: myType(type), myFd(NULL)
	{
		SMART_ASSERT(name != NULL);
		if (strlen(name) >= MAX_PATH)
		{
			throw YCException(1002, "YCIFileHandle�ļ�������");
		}
		memcpy(myName, name, strlen(name));
		myName[strlen(name)] = '\0';
	}

	virtual ~YCIAbstractFile() {}

	//
	// ע�ͣ������ļ���Ӧ����
	//
	//    Normal : �ļ����ڴ��еĳ��ȣ���plain��
	//    Dir    : ͷ������
	//    Pack   : ��ͷ����
	//
	virtual const char* getContent() = 0;
	
	//
	// �����˸Ķ�֮��ǿ��ˢ�µ�����
	//
	virtual void flush() = 0;

	//
	// ȡ���ļ��ڰ��ļ���ƫ����
	//
	virtual unsigned int getOffset() = 0;
	
	//
	// �����ļ�
	//
	virtual void dump(const char* targetPath, bool overwrite) = 0;

	//
	// ���޸�
	//
	void dirty()
	{
		myDirty = true;
	}

	//
	// �Ƿ����޸�
	//
	bool isDirty() const
	{
		return myDirty;
	}

	//
	// �󶨵��ļ�ָ��
	//
	void bind(FILE* fd)
	{
		myFd = fd;
	}

	//
	// �ļ�����
	//
	int type() const
	{
		return myType; 
	}

	//
	// ȡ���ļ�������
	//
	FILE* getFd()
	{
		return myFd;
	}

	//
	// �ļ�����
	//
	const char* filename() const
	{
		return myName;
	}

	//
	// ע�ͣ������ļ�����
	//
	virtual int getLength() = 0;

protected:
	
	//
	// �ļ�ָ��
	//
	FILE * myFd;
	
	//
	// ���ԣ��ڴ���ļ��е��ļ�����ȫ·��
	//
	char myName[MAX_PATH];

	//
	// �ļ�����
	//
	const int myType;
	
	//
	// ���ԣ��ļ����ڴ����Ƿ����޸�
	//
	bool myDirty;
};

#endif