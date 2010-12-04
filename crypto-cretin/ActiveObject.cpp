// Westley Hennigh
// ActiveObject.cpp: Interface for working with threads.
// Feb 25th 2010

#include "ActiveObject.hpp"
namespace SD {

ActiveObject::ActiveObject( unsigned id_, StaticFunction callback_fn_ )
	:   isDying (0),
		#pragma warning(disable: 4355)  // 4355 == 'this' used before initialized warning
		mythread (ThreadFunc, this),
		#pragma warning(default: 4355)
        id (id_),
        callback_fn (callback_fn_)

{}


ActiveObject::~ActiveObject()
{}

// Wake is not virtual because any code that you would put in here really should go
// in your constructor instead.  All this does it resume the thread.
void ActiveObject::Wake()
{
	mythread.Resume();
}


// Kill must be called to clean up the thread
void ActiveObject::Kill()
{
	isDying = true;
	FlushThread();

	// Wait for the thread to die
	mythread.WaitForDeath();
}


// The thread executes the run function of its handler
DWORD WINAPI ActiveObject::ThreadFunc( void* pAO )
{
	ActiveObject* ActiveObj = static_cast<ActiveObject*>(pAO);
	ActiveObj->InitThread();
	ActiveObj->Run();

	return 0;  // no one cares what we return in this case
}

} // namespace SD
