#ifndef _INCLUDE_YCLUACONTEXT_H_
#define _INCLUDE_YCLUACONTEXT_H_

class YCDList;
class YCLuaContextImpl;

/*
 * YCLuaContext : ����Lua���õ�������
 *
 * ˵����
 *      ÿ�δ�C++��������Luaʱ׼����������
 *      ����ѹ�뵱ǰ���ڵ�ָ�룬container�Ա�selectorѡ�������Թ���
 *      ��ǰTag�ڵ㣬�Ա�this����ѡ�������Թ���
 *      ѹ��YCLuaParameter �� "YCParameter", �Ա�YCParameter����ȡ���������
 *
 *      YCLuaContext��ѹ�뵽YCLuaStackջ�У��Ա�C++��Lua������
 *      ͨ�õĵ��������ǣ�
 *      
 *      C++                                               =��                              Lua
 *
 *      // YCIContainer.cpp                                                          
 *      YCLuaContext context;
 *      context.set("container", this); //���context
 *      context.set("this",      this);
 *      context.set("parameter", YCLuaParameter);
 *      YCLua->execute(&context, luaCode);
 *           |--> YCLuaStack->push(context);                              
 *           |--> lua_pcall --------------------------------------------------------->YCQuery(this).close()              
 *                                                                                          |
 *                                   // YCQuery.cpp                                         |
 *                                   YCLuaContext* context = YCLuaStack->front(); <---------|
 *                                   YCIUITag* uiTag = (YCIUITag*)context->get("this")
 *                                   uiTag->close();
 *                                         |
 *                                         |----------------------------------------------->|
 *                                                                                          |
 *           |-- YCLuaStack->pop() <--------------------------------------------------------|
 *           |
 *           o    
 *
 *  ע�⣺�����ڵ�context����������������
 *       �����߱����Լ��ͷ�
 */
class YCAPI YCLuaContext
{
public:

	YCLuaContext(void);

	~YCLuaContext(void);

	//
	// ������addContext(const char* key, void* context)
	//
	// Ŀ�ģ��趨һ��������ֵ
	//
	void addContext(const char* key, void* context);

	//
	// ������removeContext(const char* key);
	//
	// Ŀ�ģ��ı�һ��������ֵ
	//
	void removeContext(const char* key);
	
	//
	// ������updateSelector(YCDList* selector);
	//
	// Ŀ�ģ��ı�һ����YCQuery.selector��ѡ����YCUITag�ڵ��б�
	//
	void updateSelector(YCDList* selector);
	
	//
	// ������lookup(const char* key)
	//
	// Ŀ�ģ���ѯһ��������ֵ
	//
	void* lookup(const char* key);

private:

	YCLuaContextImpl *myContextImpl;
};

#endif

