#include "YCIPackFileSystem.h"

#include "YCDList.h"

YCIPackFileSystem::YCIPackFileSystem(void) 
	: myPackFiles(new YCDList())
	, myCurrent(NULL)
{

}

YCIPackFileSystem::~YCIPackFileSystem(void) 
{
	SAFE_DELETE(myPackFiles);
}

