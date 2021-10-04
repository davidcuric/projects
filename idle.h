#ifndef _idle_h_
#define _idle_h_

#include "thread.h"

class Thread;

class IdleThread : public Thread {
public:
	IdleThread();
	virtual void run();
	void start();
	int x;
};

#endif
