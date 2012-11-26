#include "YCActionManager.h"

#include "YCBasic\YCAssert.h"

YCActionManager::YCActionManager(void)
{
}

YCActionManager::~YCActionManager(void)
{
}

YCIAction* YCActionManager::acquire()
{
	return 0;
}

void YCActionManager::release(YCIAction* action)
{
	// TODO ...
}
