#ifndef _INCLUDE_YCSCOPEGUARD_H_
#define _INCLUDE_YCSCOPEGUARD_H_

#include "YCDef.h"

#pragma warning (disable : 4251)

//
// YCScopeGuard : Lambda ���������װ��
//
class YCScopeGuardBase
{
public:

	YCScopeGuardBase()
		: execute(true)
	{
	}

	virtual ~YCScopeGuardBase()
	{
	}

	void giveup()
	{
		execute = false;
	}

protected:
	
	bool execute; //����
};

template<typename Fn>
class YCScopeGuard : public YCScopeGuardBase
{
public:

	YCScopeGuard(Fn& f) : myFunctor(f) {}

	virtual ~YCScopeGuard()
	{
		if (execute) myFunctor();
	}

private:

	Fn myFunctor;
};

class YCScopeWrapper 
{
public:

	template<typename Fn>
	YCScopeWrapper(Fn fn)
		: reference(new YCScopeGuard<Fn>(fn))
	{
	}

	void giveup() {reference->giveup();}

	virtual ~YCScopeWrapper() {SAFE_DELETE(reference);}

private:

	YCScopeGuardBase* reference;
};

//
// LAMBDA Guard
//
#define SCOPE_GUARD(name, fn) YCScopeWrapper name([=](){fn;});

//
// LAMBDA Guard ����fn��ִ�в���
//
#define SCOPE_GIVEUP(name) name.giveup();

#endif