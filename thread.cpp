#include "thread.h"
#include "system.h"
#include "SCHEDULE.H"
#include "pcb.h"


extern int syncPrintf(const char *format, ...);
//samo pozivam konstruktor pcba i prosledjujem mu sebe
Thread::Thread(StackSize ssize, Time slice){
	lock();
		this->myPCB = new PCB(this, ssize, slice);
		//syncPrintf("Kreiran thread sa id = %d", this->getId());
	unlock();
}

Thread::~Thread(){
	lock();
	//waitToComplete();
		delete this->myPCB;
	unlock();
}
void Thread::createSystemThreads(){
	lock();
		System::starting = new Thread();
		System::idle = new IdleThread();
		System::starting->myPCB->myState = PCB::READY;
		System::idle->start();
	unlock();
}

ID Thread::getId(){

	return this->myPCB->myId;

}

ID Thread::getRunningId(){
//lock();
	return System::running->myId;
unlock();
}

Thread * Thread::getThreadById(ID id){
	PCB* temp;
	lock();
	temp = System::allPCBs->getViaID(id);
	if(!temp){unlock();return 0;}
	unlock();
	return temp->myThread;

}
//pozivajuca nit(running) se blokira dok se ne zavrsi nit za koju je pozvala wtc, posle kad se nit zavrsi cisti svoj waitQ
void Thread::waitToComplete(){
lock();
	//Uslovi za izlaz iz wtc: Ako ceka samu sebe, ako ceka gotovu nit, ako ceka starting, ako ceka idle;
	if(this->myPCB == (PCB*)System::running || this->myPCB->myState == PCB::DONE || this == System::starting
			|| this == System::idle){unlock(); return;}
	System::running->myState = PCB::BLOCKED;
	syncPrintf("%d stavio %d u WQ\n", this->getId(), System::running->myId);
	this->myPCB->myWaitList->put((PCB*)System::running);
	System::dispatch();
unlock();
}
//Proglasavam nit spremnom i stavljam u sched, create stack ovde ili u konstruktoru pcb?Da li ce svaka kreirana biti i startovana?
void Thread::start(){
lock();
	this->myPCB->myState = PCB::READY;
	this->myPCB->createStack();
	Scheduler::put(this->myPCB);
unlock();
}

void dispatch(){ // samo poziva System::dispatch, dodaj kad uradis
	lock();
		System::dispatch();
	unlock();
}
//void tick(){}

