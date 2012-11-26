#ifndef _INCLUDE_YCICONTAINER_H_
#define _INCLUDE_YCICONTAINER_H_

#include "YCIUITag.h"
#include "YCUITagDef.h"

class YCDList;
class YCTagFactory;
class YCLuaParameter;

/*
 * YCIContainer ������ �̳в��ͼ
 */

class YCAPI YCIContainer :
	public YCIUITag
{

public:

	//
	// ������YCIContainer(const YCUITagMeta* tagMeta)
	//
	// Ŀ�ģ����캯�������ô��ڰ󶨵�DOM
	//
	YCIContainer(const YCUITagMeta* tagMeta);

	//
	// ������~YCIContainer(void)
	//
	// Ŀ�ģ���������
	//
	virtual ~YCIContainer(void);
	
	//
	// ������clone()
	//
	// Ŀ�ģ���¡��ǰ��
	//
	virtual YCIContainer* clone() = 0;

	//
	// ������initialize(const char* name)
	//
	// Ŀ�ģ���ʼ����ÿ�δ��ڴ򿪺�Ҫ��ʼ���Ը���DOM����Tag
	//
	bool initialize(const char* name);

	//
	// ������bind2DOM(const YCUIDOM* dom)
	//
	// Ŀ�ģ��󶨵�DOM�ĵ�����ģ��
	//
	void bind2DOM(const YCUIDOM* dom);

	//
	// ������getWindowName()
	//
	// Ŀ�ģ�ȡ�ô�������
	//
	const char* getWindowName();

	//
	// ������matchAccelerateKey()
	//
	// Ŀ�ģ��Ƿ�ƥ���ݼ�
	//
	virtual bool matchAccelerateKey();
	
	//
	// ������open(YCLuaParameter* context)
	//
	// Ŀ�ģ� ����һ��������ʵ��
	//        ͬʱ�󶨴��ڴ򿪵�����������
	//
	// ע�ͣ�
	//       ÿ�δ�YCUIManager����һ�����ڣ�������һ��������ʵ��
	//       ��֤YCUIDOM�ĳ�ʼ������
	//
	virtual bool open(YCLuaParameter* context);

	//
	// ������close()
	//
	// Ŀ�ģ� �ر�һ������ʵ��
	//
	// ע�ͣ�
	//       1. ͨ��YCUILayer��onMessage������ݼ��ر�
	//          ��ʱ�ĵ���Ϊclose(true)
	//          �ڲ�ִ���Ǽ���Ƿ��а󶨵�lua OnClose�ű�
	//          ����ִ�нű�������ת��������finalClose(false)
	//
	virtual void close();

	//
	// ������finalize()
	//
	// Ŀ�ģ���ʼ����ֻ��YCUIManager::finalize��ע��ʱ����һ��
	//
	void finalize();

	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

public:

	//
	// ������finalOpen()
	//
	// Ŀ�ģ� ����Ԫ��ʵ��
	//        ͬʱ�󶨴��ڴ򿪵�����������
	//
	// ע�ͣ�
	//       ���������Ѿ����
	//
	virtual bool finalOpen();

	//
	// ������finalClose()
	//
	// Ŀ�ģ����ر�Ԫ��ʵ��
	//
	// ע�ͣ�
	//          ����finalClose����ִ��close(false)
	//          ��ʱLua�ű������Ѿ��������˳������Ѿ����
	//          ֱ�����ô���״̬ΪSTATUS_CLOSE, ��YCUILayer
	//          ��render���Ƴ���ɾ���ô���
	//
	virtual void finalClose();

protected:
			
	//
	// ������extraDraw()
	//
	// Ŀ�ģ�UIԪ�����Զ������
	//
	virtual void extraDraw();

	//
	// ������extraCSS()
	//
	// Ŀ�ģ������ض���CSS����
	//
	virtual void extraCSS();

private:

	//
	// ������fetchPosition()
	//
	// Ŀ�ģ���CSS��top��left��ȷ��λ��
	//
	virtual void fetchPosition();

	//
	// ������validateSubTag(const char* name)
	//
	// Ŀ�ģ�У���ӿؼ�
	//
	virtual bool validateSubTag(const char* name);

	//
	// ������bindContext(YCLuaParameter* context)
	//
	// Ŀ�ģ�
	//
	void bindContext(YCLuaParameter* context);

	//
	// ������preload()
	//
	// Ŀ�ģ�׼�����ݵȣ�ÿ�δ򿪴���ǰ����
	//
	virtual bool preload() = 0;
	
	//
	// ������postunload()
	//
	// Ŀ�ģ�ж�����ݵȣ�ÿ�ιرմ���ǰ����
	//
	virtual bool postunload() = 0;

	//
	// ������initAccelerateKey()
	//
	// Ŀ�ģ������ݼ�
	//
	virtual void initAccelerateKey();


protected:

	//
	// ���ԣ�myName
	//
	// Ŀ�ģ���������
	//
	char myWindowName[MAX_CONTAINER_NAME];

protected:
	
	//
	// ���ԣ�myUIDom
	//
	// ˵����ÿһ��UITag�������1��YCUIDom������
	//       ui�����ļ�����ʵ��ӳ
	//
	const YCUIDOM* myUIDOM;
		
};

#endif
