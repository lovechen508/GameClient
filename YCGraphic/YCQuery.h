#ifndef _INCLUDE_YCQUERY_H_
#define _INCLUDE_YCQUERY_H_

#include "YCLua\YCIQuery.h"

class YCLuaContext;

class YCQuery : public YCIQuery
{
public:
	
	//
	// ������YCQuery()
	//
	// Ŀ�ģ����캯�����󶨵�����
	//
	YCQuery();

	virtual ~YCQuery();

	//
	// ������bind(YCLuaContext * context)
	//
	// Ŀ�ģ���������
	//
	void bind(YCLuaContext * context);

	//
	// ������$(const char* selector)
	//
	// Ŀ�ģ�����selectorѡ����ȡ��YCIUITag�б�
	//
	virtual YCQuery& $(const char* selector);

	//
	// ������css(const char *attr, const char *value)
	//
	// Ŀ�ģ��޸�CSS����
	//
	virtual YCQuery& css(const char *css, const char *value);

	//
	// ������attr(const char* attr, const char *value)
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& attr(const char* attr, const char *value);

	//
	// ������value(const char* value)
	//
	// Ŀ�ģ���ȡԪ����ֵ
	//
	virtual YCIQuery& value(const char* value);

	//
	// ������value()
	//
	// Ŀ�ģ���ȡԪ����ֵ
	//
	virtual const char* value();

	//
	// ������html(const char *value)
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& html(const char *html);

	//
	// ������append()
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& append(const char* html);

	//
	// ������each()
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& each();

	//
	// ������hover()
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& hover();

	//
	// ������toggle()
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& toggle();

	//
	// ������hide()
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& hide();

	//
	// ������show()
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& show();

	//
	// ������fadeIn()
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& fadeIn();

	//
	// ������fadeOut()
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& fadeOut();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& animation();

	//
	// ������move(const char* speed, int x, int y)
	//
	// Ŀ�ģ��ƶ�Ԫ�����ĵ㵽(x, y)
	//
	virtual YCQuery& move(const char* speed, int x, int y);

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& parent();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& child();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& click();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& submit();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& slideUp();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& slideDown();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& addClass();

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCQuery& removeClass();

	//
	// ������end()
	//
	// Ŀ�ģ����selector
	//
	virtual YCQuery& end();

private:

	//
	// ���ԣ�myContext
	//
	// Ŀ�ģ���ǰYCQueryִ�е�������
	//       ��ǰ������������ѡ����ƥ�䵽��Tag�б�
	//       ʵ������selectorΪKey������myContext��
	//
	YCLuaContext * myContext;
};

#endif

