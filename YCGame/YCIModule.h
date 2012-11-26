#ifndef _INCLUDE_YCIMODULE_H_
#define _INCLUDE_YCIMODULE_H_

#include "YCBasic\YCIDumper.h"

class YCDList;

/*
 * YCIModule : ģ�����ӿ���
 *             ��װ��ģ��Ĳ���
 *             ���εײ�module�������
 *
 *     --------      load       --------      active      --------
 *    | INITED | <===========> | LOADED | <============> | ACTIVE |
 *     --------     unload      --------     deactive     --------
 *        |                        |                          |
 *        |                        V                          |
 *        |                     --------                      |
 *         ------------------> | FAILED | <-------------------
 *                              --------
 *  Module״̬���壺
 *
 *       INITED : ϵͳ���������������ļ����첽����ģ�� 
 *
 *       LOADED : PRELOAD���ģ���Ƿ������أ�����ģ�飬with�ӿ�ע��������Դ
 * 
 *       ACTIVE : ����ģ�飬��ʼ�����ṩ��������Ӧ��꣬���̣�������Ϣ��
 * 
 *       FAILED : ?
 */

class YCXmlReader;

class YCAPI YCIModule : public YCIDumper
{
public:

	// ģ��״̬����
	enum {INITED = 1, LOADED = 2, ACTIVE = 3, FAILED = 4};
	
public:

	YCIModule();

	virtual ~YCIModule();

	//
	// ������with(const char* depend, void* pointer)
	//
	// Ŀ�ģ�����ģ���������ʵ������ע��
	//
	virtual void with(const char* depend, void* pointer) = 0;

	//
	// ������initialize()
	//
	// Ŀ�ģ�������ɺ�ĳ�ʼ����ģ��״̬ת��ΪINITED
	//
	virtual bool initialize(YCXmlReader& reader) = 0;
	
	//
	// ������load()
	//
	// Ŀ�ģ�ִ��������ģ�����
	//       ע�᳡���㣬ע�ᴰ���࣬������Դ
	//
	virtual void load() = 0;


	//
	// ������active()
	//
	// Ŀ�ģ�����ģ��
	//       ʹģ�鿪ʼ���������ṩ����
	//
	virtual void active() = 0;


	//
	// ������deactive()
	//
	// Ŀ�ģ�����ģ��
	//       ģ�鲻�ٽ������󣬲��ٶ����ṩ����
	//
	virtual void deactive() = 0;

	//
	// ������load()
	//
	// Ŀ�ģ�ִ��������ģ��ж��
	//       ж�س����㣬ж�ش����࣬ж����Դ
	//
	virtual void unload() = 0;

	//
	// ������finialize()
	//
	// Ŀ�ģ� �˳�ʱ����Դ������ģ��״̬ת��ΪUNINITED
	//
	virtual bool finialize() = 0;

	//
	// ������name()
	//
	// Ŀ�ģ�ȡ��ģ������
	//
	const char* name();

	//
	// ������version()
	//
	// Ŀ�ģ�ȡ��ģ��汾
	//
	int version();

	//
	// ������status()
	//
	// Ŀ�ģ�ȡ��ģ��״̬
	//
	int status();

	//
	// ������depend()
	//
	// Ŀ�ģ�����ģ�������б�
	//
	YCDList* dependencies();

private:

	//
	// ������preload()
	//
	// Ŀ�ģ�����ǰ��׼������
	//
	virtual void preload() = 0;

	//
	// ������postload����
	//
	// Ŀ�ģ�������ɺ�ĸ�������
	//
	virtual void postload() = 0;

	//
	// ������preunload()
	//
	// Ŀ�ģ�ж��ǰ��׼������
	//
	virtual void preunload() = 0;

	//
	// ������postunload����
	//
	// Ŀ�ģ�ж����ɺ�ĸ�������
	//
	virtual void postunload() = 0;

protected:

	int myStatus;

	char* myName;
	int myVersion;

	YCDList* myDependencies;
};

#endif