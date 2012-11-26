#include "YCUIDOM.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCException.h"

static void filename_release(void* item)
{
	delete[] (const char*)item;
}

YCUIDOM::YCUIDOM()
	: mySubUIDOM(new YCDList())
	, myAttributes(new YCDList())
	, myScriptFiles(NULL)
	, myLayoutFiles(NULL)
	, myFilename(NULL)
	, myParent(NULL)
	, myTagValue(NULL)
{
	memset(myTagName, 0, TAG_NAME_LENGTH);
}

YCUIDOM::~YCUIDOM()
{
	mySubUIDOM->clear(YCDListClearHelper<YCUIDOM>);
	delete mySubUIDOM;
	
	myAttributes->clear(YCDListClearHelper<YCUIAttribute>);
	delete myAttributes;
	
	delete myTagValue;

	if (myScriptFiles != NULL)
	{
		myScriptFiles->clear(filename_release);
		delete myScriptFiles;
	}

	if (myLayoutFiles != NULL)
	{
		myLayoutFiles->clear(filename_release);
		delete myLayoutFiles;
	}
	delete myFilename;
}

//
// ������setFilename(const char* filename)
//
// Ŀ�ģ������ļ���
//
void YCUIDOM::setFilename(const char* filename)
{	
	if (filename == NULL)
	{
		throw YCException(2002, "YCUIDOM::setFilename�ļ���ΪNULL");
	}

	int len = strlen(filename);
	if (len >= MAX_PATH)
	{
		throw YCException(2002, "YCUIDOM::setFilename�ļ������ȴ���MAX_PATH");
	}
	myFilename = new char[len+1];
	memset(myFilename, 0, len+1);
	memcpy(myFilename, filename, len);
}

//
// ������getFilename()
//
// Ŀ�ģ���ȡ�ļ���
//
const char* YCUIDOM::getFilename() const
{
	return myFilename;
}

//
// ������setName(const char* name)
//
// Ŀ�ģ�����DOM�ڵ���
//
void YCUIDOM::setName(const char* name)
{
	SMART_ASSERT(name != NULL);
	memcpy(myTagName, name, strlen(name));
}

//
// ������getName()
//
// Ŀ�ģ���ȡDOM�ڵ���
//
const char* YCUIDOM::getName() const
{
	return myTagName;
}

//
// ������setValue(const char* value)
//
// Ŀ�ģ�����DOM�ڵ�ֵ
//
void YCUIDOM::setValue(const char* value)
{
	if (value == NULL)
	{
		return;
	}

	int len = strlen(value);
	if (len > 0)
	{
		myTagValue = new char[len+1];
		memset(myTagValue, 0, len+1);
		memcpy(myTagValue, value, len);
	}
}

//
// ������getValue()
//
// Ŀ�ģ���ȡDOM�ڵ�ֵ
//
const char* YCUIDOM::getValue() const
{
	return myTagValue;
}

//
// ������setParent(YCUIDOM * parent)
//
// Ŀ�ģ����ø�DOM�ڵ���
//
void YCUIDOM::setParent(YCUIDOM * parent)
{
	myParent = parent;
}

//
// ������setScript(const char* filename)
//
// Ŀ�ģ����ô��ڸ��ӵ�Lua�ű�
//
void YCUIDOM::setScript(const char* filename)
{
	if (filename == NULL)
	{
		throw YCException(2002, "YCUIDOM::setScripts�ļ���ΪNULL");
	}

	if (myScriptFiles == NULL)
	{
		myScriptFiles = new YCDList();
	}

	bool found = false;
	for (Item_List_T *item = myScriptFiles->begin();
		 item != myScriptFiles->end();
		 item = myScriptFiles->next(item))
	{
		const char* script = (const char*)myScriptFiles->payload(item);
		if (strcmp(script, filename) == 0)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		int len = strlen(filename);
		char * target = new char[len+1];
		memset(target, 0, len+1);
		memcpy(target, filename, len);
		myScriptFiles->append(target, NULL);
	}
}

//
// ������getScripts()
//
// Ŀ�ģ���ȡ���ڸ��ӵ�Lua�ű�
//
const YCDList* YCUIDOM::getScripts() const
{
	return myScriptFiles;
}

//
// ������setLayput(const char* filename)
//
// Ŀ�ģ����ô��ڸ��ӵ�Layout��ʽ��
//
void YCUIDOM::setLayout(const char* filename)
{
	if (filename == NULL)
	{
		throw YCException(2002, "YCUIDOM::setLayout�ļ���ΪNULL");
	}

	if (myLayoutFiles == NULL)
	{
		myLayoutFiles = new YCDList();
	}

	bool found = false;
	for (Item_List_T *item = myLayoutFiles->begin();
		 item != myLayoutFiles->end();
		 item = myLayoutFiles->next(item))
	{
		const char* script = (const char*)myLayoutFiles->payload(item);
		if (strcmp(script, filename) == 0)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		int len = strlen(filename);
		char * target = new char[len+1];
		memset(target, 0, len+1);
		memcpy(target, filename, len);
		myLayoutFiles->append(target, NULL);
	}
}

//
// ������getLayouts()
//
// Ŀ�ģ���ȡ���ڸ��ӵ�Layout��ʽ��
//
const YCDList* YCUIDOM::getLayouts() const
{
	return myLayoutFiles;
}

//
// ������setAttribute(const char* name, const char* value)
//
// Ŀ�ģ���������
//
void YCUIDOM::setAttribute(const char* name, const char* value)
{
	if (name == NULL)
	{
		throw YCException(2002, "YCUIDOM::setAttribute������ΪNULL");
	}

	bool found = false;
	for (Item_List_T *item = myAttributes->begin();
		 item != myAttributes->end();
		 item = myAttributes->next(item))
	{
		YCUIAttribute* current = (YCUIAttribute*)myAttributes->payload(item);
		if (current->name == name)
		{
			current->value = value;
			found = true;
			break;
		}
	}

	if (!found)
	{
		YCUIAttribute * attribute = new YCUIAttribute(name, value);
		myAttributes->append(attribute, NULL);
	}
}

//
// ������getAttribute(const char* name)
//
// Ŀ�ģ���ȡ����ֵ
//
const char* YCUIDOM::getAttribute(const char* name) const
{
	if (name == NULL)
	{
		throw YCException(2002, "YCUIDOM::getAttribute������ΪNULL");
	}

	const char* result = NULL;
	for (Item_List_T *item = myAttributes->begin();
		 item != myAttributes->end();
		 item = myAttributes->next(item))
	{
		YCUIAttribute* current = (YCUIAttribute*)myAttributes->payload(item);
		if (current->name == name)
		{
			result = current->value.c_str();
			break;
		}
	}
	return result;
}

//
// ������getAttributes()
//
// Ŀ�ģ���ȡ�����б�
//
const YCDList * YCUIDOM::getAttributes() const 
{
	return myAttributes;
}

//
// ������getSubDom()
//
// Ŀ�ģ�ȡ���ӽڵ㼯��
//
const YCDList * YCUIDOM::getSubDom() const
{
	return mySubUIDOM;
}

//
// ������addSubDom(YCUIDOM* dom)
//
// Ŀ�ģ����SubDom�ӽڵ�
//
void YCUIDOM::addSubDom(YCUIDOM* dom)
{
	if (dom != NULL)
	{
		mySubUIDOM->append(dom, NULL);
	}
}

//
// ������removeSubDom(YCUIDOM* dom)
//
// Ŀ�ģ�ɾ��SubDom�ӽڵ�
//
YCUIDOM * YCUIDOM::removeSubDom(YCUIDOM* dom)
{
	if (dom != NULL)
	{
		Item_List_T* item = mySubUIDOM->find(dom);
		if (item != NULL)
		{
			return (YCUIDOM*)mySubUIDOM->remove(item);
		}
	}
	return NULL;
}

//
// ������updateSubDom(YCUIDOM* dom)
//
// Ŀ�ģ��滻SubDom�ӽڵ�
//
bool YCUIDOM::updateSubDom(YCUIDOM* old, YCUIDOM* add)
{
	SMART_ASSERT(old != NULL);
	SMART_ASSERT(add != NULL);

	Item_List_T* item = mySubUIDOM->find(old);
	if (item != NULL)
	{
		mySubUIDOM->append(add, item);
		mySubUIDOM->remove(item);
		return true;
	}
	return false;
}

//
// ������removeSubDom(const char* name)
//
// Ŀ�ģ�ɾ��SubDom�ӽڵ�
//
YCUIDOM* YCUIDOM::removeSubDom(const char* name)
{
	throw YCException(2002, "YCUIDOM::removeSubDom��δʵ��");
	return NULL;
}