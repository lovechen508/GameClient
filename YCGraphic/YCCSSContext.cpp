#include "YCCSSContext.h"

#include "YCCSSNode.h"
#include "YCIUITag.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCException.h"

YCCSSContext::YCCSSContext(void)
	: myDList(new YCDList())
{
}


YCCSSContext::~YCCSSContext(void)
{
	delete myDList;
}

//
// ������addNode(YCCSSNode* node)
//
// Ŀ�ģ����YCCSSNode�ڵ�
//
void YCCSSContext::addNode(YCCSSNode* node)
{
	if (node == NULL)
	{
		throw YCException(2002, "YCCSSContext::addNode���YCCSSNode�ڵ�Ϊ��");
	}

	// �������ȼ�
	node->calculatePriority();

	myDList->append(node, NULL);
}

//
// ������clearWithoutDelete()
//
// Ŀ�ģ�YCIContainer�ų�����������
//      YCUITag�����ӽڵ㲻������������
//
void YCCSSContext::clearWithoutDelete()
{
	if (myDList != NULL)
	{
		myDList->clear();
	}
}

//
// ������nodes() const
//
// Ŀ�ģ���¶����YCCSSNode�Թ�YCIUITag����
//
const YCDList* YCCSSContext::nodes() const
{
	return myDList;
}

//
// ������filter(const YCIUITag* parent, const YCIUITag* child)
//
// Ŀ�ģ�narrow down CSS������
//
YCCSSContext* YCCSSContext::filter(const YCIUITag* parent, const YCIUITag* child) const
{
	return NULL;
}

