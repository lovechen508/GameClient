#ifndef _INCLUDE_YCIPACKFILE_H_
#define _INCLUDE_YCIPACKFILE_H_

#include <stdio.h>
#include "YCDef.h"

#include "YCIFileDirectory.h"

class YCDList;
class YCIFileNormal;
class YCIPackFileBuilder;

/*
 * YCIPackFile : ����ļ������ࣺ��*.mpq
 *               �̳���YCIFileHandle�Ǳ���ʵ������ݹ����
 *               ֧�������飬ɾ
 */
class YCAPI YCIFilePack : public YCIFileDirectory
{
public:

	//
	// ˵�������п�ṹ
	//
	struct IDLE_BLOCK {

		IDLE_BLOCK() : offset(0), length(0) {}

		IDLE_BLOCK(unsigned int o, unsigned int l) : offset(o), length(l) {}

		unsigned int offset;               //��ǰ���п��ƫ����
		unsigned int length;			   //��ǰ���п�Ĵ�С������ΪFILE_ROUNDSIZE��������
	};

public:

	YCIFilePack(const char* name, FILE* fd, YCIPackFileBuilder* builder);

	virtual ~YCIFilePack(void);

	//
	// ȡ�ð���
	//
	virtual const char* getPackName() = 0;

	//
	// ȡ���ļ��ڰ��ļ���ƫ����
	//
	virtual unsigned int getOffset();

	//
	// ע�ͣ������ļ���ͷ������������
	//
	virtual void setContentLength(int contentLen) = 0;

	//
	// ע�ͣ���ȡ�ļ���ͷ������������
	//
	virtual int getContentLength() = 0;

	//
	// ��ȡ��ͨ�ļ���д������
	//
	YCIPackFileBuilder* getBuilder();
	
	//
	// �ҽӿ��п�
	//
	virtual void addIdleBlock(unsigned int offset, unsigned int length) = 0;

	//
	// ���ҿ��е��ļ���
	//
	virtual IDLE_BLOCK* findAvailableBlock(unsigned int required) = 0;

protected:

	//
	// �ļ����أ�д�������
	//
	YCIPackFileBuilder * myBuilder;
	
	//
	// ���п�����
	//
	YCDList * myIdleList;
};

#endif

