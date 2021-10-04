#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"
#include "system.h"
#include "list.h"
#include "kersem.h"
#include "SCHEDULE.H"

//class Thread;
class List;
class Thread;
class KernelSem;


class PCB{
public:
	PCB(Thread* myThread,StackSize myStackSize, Time myTimeSlice);
	~PCB();
	ID myId;
	Thread *myThread;
	StackSize myStackSize;
	Time myTimeSlice;

	List *myWaitList;

	unsigned int ss,sp;

	unsigned char* myStack;
	unsigned int timeCntr;
	unsigned int getCnt();
	Time sleepTime;
	KernelSem* mySem;

	volatile int myState;

	volatile int wakeUpType;

	static const int NEW,READY,BLOCKED,DONE;
	static const int bySignal, byTime;
	static const int stackSizeLimit;

	void createStack();

	static void wrapper();
};

void clearWaitList();

#endif
