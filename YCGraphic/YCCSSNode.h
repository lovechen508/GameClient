#ifndef _INCLUDE_YCCSSNODE_H_
#define _INCLUDE_YCCSSNODE_H_

#include "YCUITagDef.h"

class YCIUITag;
class YCCSSItem;
class YCCSSNodeImpl;

/*
 * YCCSSNode : CSS�ڵ�
 *
 *  ˵��������css�ļ�һ��{}��Χ�Ľڵ�
 *
 *        div {      
 *			width : 100;
 *          height : 200;
 *			background-color : 0xFFCCCCCC;
 *        }
 */ 
class YCCSSNode
{
public:

	//  node �ڵ�α��
	enum CSS_NODE_TYPE
	{
		NODE_NORMAL  = 1,  //����
		NODE_HOVER   = 2,  //������
		NODE_CLICK   = 3,  //��갴��
		NODE_DISABLE = 4,  //ʧЧ
	};

public:
	
	//
	// ������YCCSSNode(const char* sel)
	//
	// Ŀ�ģ�������
	//
	YCCSSNode(const char* sel);
	
	//
	// ������~YCCSSNode(void)
	//
	// Ŀ�ģ�������
	//
	~YCCSSNode(void);

	//
	// ������getNodeType()
	//
	// Ŀ�ģ��ýڵ��Ƿ���α������
	//
	CSS_NODE_TYPE getNodeType() const;
	
	//
	// ������getPriority()
	//
	// Ŀ�ģ�ȡ�øýڵ��CSS���ȼ�
	//
	int getPriority() const;

	//
	// ������getSelector()
	//
	// Ŀ�ģ�ȡ�øýڵ��ѡ���
	//
	const char* getSelector() const;

	//
	// ������apply(YCIUITag* tag) const
	//
	// Ŀ�ģ�Ӧ��CSS���򵽱�ǩ
	//
	void apply(YCIUITag* tag) const;
	
	//
	// ������setProperty(YCUITagMeta::E_UITAG_CSS property, YCCSSItem *cssItem)
	//
	// Ŀ�ģ�����CSS����
	//
	void setProperty(E_UITAG_CSS property, YCCSSItem* cssItem);

	//
	// ������getProperty(E_UITAG_CSS property) const
	//
	// Ŀ�ģ���ȡ int ��������
	//
	const YCCSSItem* getProperty(E_UITAG_CSS property) const;
	
	//
	// ������calculatePriority()
	//
	// Ŀ�ģ�����ýڵ�����ȼ�ֵ
	//
	// ע�ͣ�
	//       ÿ��IDѡ���(�� #id),��0,1,0,0
	//       ÿ��Classѡ���(�� .class)��ÿ������ѡ���(�� [attribute=])��ÿ��α��(�� :hover)��0,0,1,0
	//       ÿ��Ԫ��ѡ�������p����αԪ��ѡ���(�� :firstchild)�ȣ���0,0,0,1
	//
	void calculatePriority();

private:

	//
	// ������isPseudoType(CSS_NODE_TYPE type) const
	//
	// Ŀ�ģ��ýڵ��Ƿ���ĳα��
	//
	bool isPseudoType(CSS_NODE_TYPE type) const;
	
private:

	char* mySelector;    //ѡ���� div

	CSS_NODE_TYPE myType; //�ڵ�����

	int myPriority;
	
	YCCSSNodeImpl *myNodeImpl;
};

#endif

