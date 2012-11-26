#include "YCLuaRequest.h"

#include "YCBasic\YCDef.h"

#pragma region YCLuaRequestHolder

class YCLuaRequestHolder
{
public:

	YCLuaRequestHolder() {}

	~YCLuaRequestHolder() {}
};

#pragma endregion YCLuaRequestHolder

YCLuaRequest::YCLuaRequest(void)
{
}


YCLuaRequest::~YCLuaRequest(void)
{
}

bool YCLuaRequest::hasParameter(const char* key)
{
	return false;
}

int YCLuaRequest::intValue(const char* key)
{
	return 0;
}

const char* YCLuaRequest::strValue(const char* key)
{
	return NULL;
}

float YCLuaRequest::floatValue(const char* key)
{
	return 0.0f;
}
