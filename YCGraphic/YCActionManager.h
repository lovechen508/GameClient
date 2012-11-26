#ifndef _INCLUDE_YCACTIONMANAGER_H_
#define _INCLUDE_YCACTIONMANAGER_H_

class YCIAction;

/*
 * YCActionManager : 动作管理器
 *
 *
 */ 
class YCAPI YCActionManager
{
public:

	enum {
		SINGLE_ACTION		= 1,
		SEQUENCE_ACTION		= 2,
		REPEAT_ACTION		= 2,
		PARALLAL_ACTION		= 3,
	};

public:

	YCActionManager(void);

	~YCActionManager(void);

	YCIAction* acquire();

	void release(YCIAction* action);

};

#endif
