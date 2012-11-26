#include "YCAtlasTexture2D.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"
#include "YCConfig\YCXmlReader.h"

static void atlas_idList_release(void* item)
{
	ATLAS_TEXTURE_INFO_T* atlas = (ATLAS_TEXTURE_INFO_T*)item;
	SAFE_DELETE(atlas);
}

YCAtlasTexture2D::YCAtlasTexture2D(YCDList *idList, YCITextureFile *texture, int status)
	: YCITexture2D(texture, status)
	, myIdList(idList)
{
	SMART_ASSERT(idList != NULL);
}

YCAtlasTexture2D::~YCAtlasTexture2D(void)
{
	myIdList->clear(atlas_idList_release);
	delete myIdList;
}

//
// ������hasTexture(unsigned int uniqueId)
//
// Ŀ�ģ������������Ƿ����ͼƬ
//
// ע�ͣ�
//     1. YCSingleTexture2D ֻ����һ��ͼƬ
//     2. YCAtlasTexture2D ���ܰ���1..n��ͼƬ
//
bool YCAtlasTexture2D::hasTexture(unsigned int uniqueId) const
{
	for (Item_List_T* it = myIdList->begin();
		 it != myIdList->end();
		 it = myIdList->next(it))
	{
		ATLAS_TEXTURE_INFO_T* texture = (ATLAS_TEXTURE_INFO_T*)myIdList->payload(it);
		if (texture->id == uniqueId)
		{
			return true;
		}
	}
	return false;
}

//
// ������rect(unsigned int uniqueId)
//
// Ŀ�ģ�����������������
//       ����NULL��ʾ����Ϊ��������
//
void YCAtlasTexture2D::getRect(unsigned int uniqueId, FloatRect& rect) const
{
	ATLAS_TEXTURE_INFO_T* texture = NULL;
	for (Item_List_T* it = myIdList->begin();
		 it != myIdList->end();
		 it = myIdList->next(it))
	{
		ATLAS_TEXTURE_INFO_T* temp = (ATLAS_TEXTURE_INFO_T*)myIdList->payload(it);
		if (temp->id == uniqueId)
		{
			texture = temp;
			break;
		}
	}

	if (texture == NULL)
	{
		throw YCException(2002, "YCAtlasTexture2D::rect��Ӧ������δ�ҵ�!");
	}

	rect.top = (float)texture->y;
	rect.left = (float)texture->x;
	rect.right = (float)texture->x + texture->w;
	rect.bottom = (float)texture->y + texture->h;
}

