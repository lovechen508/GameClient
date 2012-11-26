#ifndef _INCLUDE_YCIQUERY_H_
#define _INCLUDE_YCIQUERY_H_

/*
 * YCIQuery : ��JQuery�ӿ�ʵ��
 */

class YCAPI YCIQuery
{
public:

	//
	// ������YCIQuery()
	//
	// Ŀ�ģ����캯�����󶨵�����
	//
	YCIQuery() {}

	virtual ~YCIQuery() {}

	//
	// ������$(const char* selector)
	//
	// Ŀ�ģ�����selectorѡ����ȡ��YCIUITag�б�
	//
	virtual YCIQuery& $(const char* selector) = 0;

	//
	// ������css(const char *attr, const char *value)
	//
	// Ŀ�ģ��޸�CSS����
	//
	virtual YCIQuery& css(const char *css, const char *value) = 0;

	//
	// ������attr(const char* attr, const char *value)
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& attr(const char* attr, const char *value) = 0;

	//
	// ������value(const char* value)
	//
	// Ŀ�ģ���ȡԪ����ֵ
	//
	virtual YCIQuery& value(const char* value) = 0;

	//
	// ������value()
	//
	// Ŀ�ģ���ȡԪ����ֵ
	//
	virtual const char* value() = 0;

	//
	// ������html(const char *value)
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& html(const char *html) = 0;

	//
	// ������append()
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& append(const char* html) = 0;

	//
	// ������each()
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& each() = 0;

	//
	// ������hover()
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& hover() = 0;

	//
	// ������toggle()
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& toggle() = 0;

	//
	// ������hide()
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& hide() = 0;

	//
	// ������show()
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& show() = 0;

	//
	// ������fadeIn()
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& fadeIn() = 0;

	//
	// ������fadeOut()
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& fadeOut() = 0;

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& animation() = 0;

	//
	// ������move(const char* speed, int x, int y)
	//
	// Ŀ�ģ��ƶ�Ԫ�����ĵ㵽(x, y)
	//
	virtual YCIQuery& move(const char* speed, int x, int y) = 0;

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& parent() = 0;

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& child() = 0;

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& click() = 0;

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& submit() = 0;

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& slideUp() = 0;

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& slideDown() = 0;

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& addClass() = 0;

	//
	// ������
	//
	// Ŀ�ģ�
	//
	virtual YCIQuery& removeClass() = 0;

	//
	// ������end()
	//
	// Ŀ�ģ����selector
	//
	virtual YCIQuery& end() = 0;
};

#endif