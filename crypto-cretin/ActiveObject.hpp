// Westley Hennigh
// ActiveObject.hpp: Interface for working with threads.
// Feb 25th 2010

#ifndef ACTIVEO_H
#define ACTIVEO_H

#include "Thread.hpp"

namespace SD {

/*
ActiveObject is basically a cookie cutter thread.
Derive from ActiveObject and you can specify init, run and
flush for your own thread.
*/
class ActiveObject
{
public:
    typedef void (*StaticFunction)( unsigned id );

	ActiveObject          ( unsigned id_, StaticFunction callback_fn_ );
	virtual ~ActiveObject ();

	void Wake ();  // I like the idea of telling the thread to go rather than having it do
								 // so automatically after construction.

	void Kill ();  //^? must call before exiting... perhaps I should do an isDying check in 
								 // the destructor and clean up (just in case)

protected:
	virtual void InitThread  () = 0;
	virtual void Run         () = 0;
	virtual void FlushThread () = 0;

	// thread function will call virtual functions from derived class
	static DWORD WINAPI ThreadFunc (void *pArg);

	bool     isDying;
	Thread   mythread;

    unsigned id;
    StaticFunction callback_fn;

};

} // namespace SD


#endif
