#ifndef _INCLUDE_YCUIMANAGER_H_
#define _INCLUDE_YCUIMANAGER_H_

#include "YCInput\YCIInputListener.h"

class YCInput;
class YCConfig;
class YCIModel;
class YCLuaRequest;
class YCLuaParameter;
class YCIContainer;
class YCIController;
class YCTextureManager;
class YCUIManagerImpl;
class YCTagFactory;

/*
 * �������е�UI����
 */
class YCAPI YCUIManager
	: public YCIInputListener
{
public:

	YCUIManager(void);

	~YCUIManager(void);

	//
	// ������initialize(YCTextureManager* textureManager, YCInput& input, YCConfig& config)
	//
	// Ŀ�ģ���ʼ��UI������������ȱʡUI����
	//
	bool initialize(YCTextureManager* textureManager, YCInput& input, YCConfig& config);

	//
	// ������registController(const char* module, YCIController* controller)
	//
	// Ŀ�ģ�ע�ᴦ����
	//
	bool registController(const char *module, YCIController* controller);

	//
	// ������handleRequest(const char *module, YCLuaRequest * request)
	//
	// Ŀ�ģ���������򿪴�����Ϣ
	//
	void handleRequest(const char *module, YCLuaRequest * request);

	//
	// ������removeController(const char *module)
	//
	// Ŀ�ģ��Ƴ�������
	//
	YCIController* removeController(const char *module);

	//
	// ������registUI(const char* name, const char* xmlFile)
	//
	// Ŀ�ģ�ע��UI��XML����Ԫ��,����·��
	//
	bool registUI(const char* name, const char* xmlFile);

	//
	// ������open(const char* name, YCLuaParameter* context)
	//
	// Ŀ�ģ���UIԪ��
	//
	bool open(const char* name, YCLuaParameter* context);

	//
	// ������finalize()
	//
	// Ŀ�ģ�����YCUIManager
	//
	void finalize();

	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������,ʹUIManager�л��ᴦ���ݼ���Ϣ
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:
		
	YCTagFactory * myTagFactory;

	YCTextureManager *myTextureManager;

	YCUIManagerImpl * myUIManagerImpl;
};

#endif

