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
// 函数：setFilename(const char* filename)
//
// 目的：设置文件名
//
void YCUIDOM::setFilename(const char* filename)
{	
	if (filename == NULL)
	{
		throw YCException(2002, "YCUIDOM::setFilename文件名为NULL");
	}

	int len = strlen(filename);
	if (len >= MAX_PATH)
	{
		throw YCException(2002, "YCUIDOM::setFilename文件名长度大于MAX_PATH");
	}
	myFilename = new char[len+1];
	memset(myFilename, 0, len+1);
	memcpy(myFilename, filename, len);
}

//
// 函数：getFilename()
//
// 目的：获取文件名
//
const char* YCUIDOM::getFilename() const
{
	return myFilename;
}

//
// 函数：setName(const char* name)
//
// 目的：设置DOM节点名
//
void YCUIDOM::setName(const char* name)
{
	SMART_ASSERT(name != NULL);
	memcpy(myTagName, name, strlen(name));
}

//
// 函数：getName()
//
// 目的：获取DOM节点名
//
const char* YCUIDOM::getName() const
{
	return myTagName;
}

//
// 函数：setValue(const char* value)
//
// 目的：设置DOM节点值
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
// 函数：getValue()
//
// 目的：获取DOM节点值
//
const char* YCUIDOM::getValue() const
{
	return myTagValue;
}

//
// 函数：setParent(YCUIDOM * parent)
//
// 目的：设置父DOM节点名
//
void YCUIDOM::setParent(YCUIDOM * parent)
{
	myParent = parent;
}

//
// 函数：setScript(const char* filename)
//
// 目的：设置窗口附加的Lua脚本
//
void YCUIDOM::setScript(const char* filename)
{
	if (filename == NULL)
	{
		throw YCException(2002, "YCUIDOM::setScripts文件名为NULL");
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
// 函数：getScripts()
//
// 目的：获取窗口附加的Lua脚本
//
const YCDList* YCUIDOM::getScripts() const
{
	return myScriptFiles;
}

//
// 函数：setLayput(const char* filename)
//
// 目的：设置窗口附加的Layout样式表
//
void YCUIDOM::setLayout(const char* filename)
{
	if (filename == NULL)
	{
		throw YCException(2002, "YCUIDOM::setLayout文件名为NULL");
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
// 函数：getLayouts()
//
// 目的：获取窗口附加的Layout样式表
//
const YCDList* YCUIDOM::getLayouts() const
{
	return myLayoutFiles;
}

//
// 函数：setAttribute(const char* name, const char* value)
//
// 目的：设置属性
//
void YCUIDOM::setAttribute(const char* name, const char* value)
{
	if (name == NULL)
	{
		throw YCException(2002, "YCUIDOM::setAttribute属性名为NULL");
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
// 函数：getAttribute(const char* name)
//
// 目的：获取属性值
//
const char* YCUIDOM::getAttribute(const char* name) const
{
	if (name == NULL)
	{
		throw YCException(2002, "YCUIDOM::getAttribute属性名为NULL");
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
// 函数：getAttributes()
//
// 目的：获取属性列表
//
const YCDList * YCUIDOM::getAttributes() const 
{
	return myAttributes;
}

//
// 函数：getSubDom()
//
// 目的：取得子节点集合
//
const YCDList * YCUIDOM::getSubDom() const
{
	return mySubUIDOM;
}

//
// 函数：addSubDom(YCUIDOM* dom)
//
// 目的：添加SubDom子节点
//
void YCUIDOM::addSubDom(YCUIDOM* dom)
{
	if (dom != NULL)
	{
		mySubUIDOM->append(dom, NULL);
	}
}

//
// 函数：removeSubDom(YCUIDOM* dom)
//
// 目的：删除SubDom子节点
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
// 函数：updateSubDom(YCUIDOM* dom)
//
// 目的：替换SubDom子节点
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
// 函数：removeSubDom(const char* name)
//
// 目的：删除SubDom子节点
//
YCUIDOM* YCUIDOM::removeSubDom(const char* name)
{
	throw YCException(2002, "YCUIDOM::removeSubDom尚未实现");
	return NULL;
}