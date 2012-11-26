#include "YCTagFactory.h"

#include "YCBasic\YCAssert.h"

#include "YCUITag.h"
#include "YCUITagMeta.h"
#include "YCTagBuilder.h"

#include "YCWindow.h"
#include "YCDialog.h"

#include "YCUIP.h"
#include "YCUITd.h"
#include "YCUITr.h"
#include "YCUITh.h"
#include "YCUITable.h"
#include "YCUIUL.h"
#include "YCUILI.h"
#include "YCUIDiv.h"
#include "YCUIImg.h"
#include "YCUISpan.h"
#include "YCUIRadio.h"
#include "YCUIButton.h"
#include "YCUISelect.h"
#include "YCUISprite.h"
#include "YCUICheckBox.h"
#include "YCUITextArea.h"
#include "YCUITextField.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"

YCTagFactory::YCTagFactory(void)
	: myTagBuilder(NULL)
	, myTagRepository(NULL)
{
}


YCTagFactory::~YCTagFactory(void)
{
}

//
// ������initialize(YCConfig& config)
//
// Ŀ�ģ���ʼ����ǩ����
//
bool YCTagFactory::initialize(YCConfig& config)
{
	myTagRepository = new YCDList();

	myTagBuilder = new YCTagBuilder();
	if (!myTagBuilder->initialize(this, config))
	{
		delete myTagBuilder;
		myTagBuilder = NULL;
		return false;
	}

	// �������б�ǩ
	for (Item_List_T* item = myTagRepository->begin();
		 item != myTagRepository->end();
		 item = myTagRepository->next(item))
	{
		YCUITagMeta* tagMeta = (YCUITagMeta*)myTagRepository->payload(item);
		if (!tagMeta->isCompleted())
		{
			const char* parent = tagMeta->getBaseTagName();

			bool found = false;
			for (Item_List_T * item2 = myTagRepository->begin();
				 item2 != myTagRepository->end();
				 item2 = myTagRepository->next(item2))
			{
				YCUITagMeta* current = (YCUITagMeta*)myTagRepository->payload(item2);
				if (strcmp(current->name(), parent) == 0)
				{
					tagMeta->setBaseTag(current);
					found = true;
					break;
				}
			}

			if (!found)
			{
				LOG_WARNING("YCTagFactory::initialize ��ǩ��������" << tagMeta->name());
				throw YCException(2002, "YCTagFactory::initialize��ǩ������");
			}
		}
	}

	return true;
}

//
// ������registerTag(YCUITagMeta* tag)
//
// Ŀ�ģ�ע���ǩ
//
void YCTagFactory::registerTag(YCUITagMeta* tagMeta)
{
	if (tagMeta == NULL)
	{
		throw YCException(2002, "YCTagFactory::registerTagע���ǩΪNULL");
	}

	const char* tagName = tagMeta->name();
	for (Item_List_T* item = myTagRepository->begin();
		 item != myTagRepository->end();
		 item = myTagRepository->next(item))
	{
		YCUITagMeta* current = (YCUITagMeta*)myTagRepository->payload(item);
		if (strcmp(tagName, current->name()) == 0)
		{
			LOG_FATAL("YCTagFactory::registerTag��ǩ�ض���: " << tagName);
			throw YCException(2002, "YCTagFactory::registerTag��ǩ�ض��壡");
		}
	}

	LOG_DEBUG("YCTagFactory::registerTag��ǩ����: " << tagName);
	myTagRepository->append(tagMeta, NULL); 
}

//
// ������create(const char* tagName)
//
// Ŀ�ģ����ݱ�ǩ��������ǩ
//
YCIUITag* YCTagFactory::create(const char* tagName)
{
	if (tagName == NULL)
	{
		throw YCException(2002, "YCTagFactory::create�����ǩΪNULL");
	}

	YCUITagMeta *found = NULL;
	for (Item_List_T* item = myTagRepository->begin();
		 item != myTagRepository->end();
		 item = myTagRepository->next(item))
	{
		YCUITagMeta* current = (YCUITagMeta*)myTagRepository->payload(item);
		if (strcmp(tagName, current->name()) == 0)
		{
			found = current;
			break;
		}
	}

	if      (strcmp(tagName, "window")		== 0) return new YCWindow(found);
	else if (strcmp(tagName, "dialog")		== 0) return new YCDialog(found);

	else if (strcmp(tagName, "p")			== 0) return new YCUIP(found); 
	else if (strcmp(tagName, "ul")			== 0) return new YCUIUl(found); 
	else if (strcmp(tagName, "li")			== 0) return new YCUILi(found); 
	else if (strcmp(tagName, "img")			== 0) return new YCUIImg(found); 
	else if (strcmp(tagName, "div")			== 0) return new YCUIDiv(found); 
	else if (strcmp(tagName, "span")		== 0) return new YCUISpan(found); 
	else if (strcmp(tagName, "radio")		== 0) return new YCUIRadio(found); 
	else if (strcmp(tagName, "select")		== 0) return new YCUISelect(found); 
	else if (strcmp(tagName, "checkbox")	== 0) return new YCUICheckBox(found);
	else if (strcmp(tagName, "sprite")		== 0) return new YCUISprite(found);
	else if (strcmp(tagName, "textfield")	== 0) return new YCUITextField(found); 
	else if (strcmp(tagName, "textarea")	== 0) return new YCUITextArea(found); 
	else if (strcmp(tagName, "table")       == 0) return new YCUITable(found);
	else if (strcmp(tagName, "th")          == 0) return new YCUITh(found);
	else if (strcmp(tagName, "tr")          == 0) return new YCUITr(found);
	else if (strcmp(tagName, "td")          == 0) return new YCUITd(found);
	
	return NULL;
}

static void tagmeta_release(void* item)
{
	YCUITagMeta* tagmeta = (YCUITagMeta*)item;
	SAFE_DELETE(tagmeta);
}
//
// ������finalize()
//
// Ŀ�ģ�����YCTagFactory
//
void YCTagFactory::finalize()
{
	delete myTagBuilder;
	myTagBuilder = NULL;

	if (myTagRepository != NULL)
	{
		myTagRepository->clear(tagmeta_release);
		delete myTagRepository;
		myTagRepository = NULL;
	}
}