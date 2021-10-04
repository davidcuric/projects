#include "idle.h"
#include "system.h"
#include "pcb.h"
#include "thread.h"
extern int syncPrintf(const char *format, ...);
IdleThread::IdleThread(){
x=0;
}

void IdleThread::run(){
	while(1){

		//syncPrintf("idle \n" );
	}
}

void IdleThread::start(){
	lock();
	//myPCB->myState = PCB::READY;
	this->myPCB->myState = PCB::READY;
	myPCB->createStack();
	unlock();
}
