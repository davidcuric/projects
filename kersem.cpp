#include "kersem.h"
#include "system.h"
#include "schedule.h"

int syncPrintf(const char *format, ...);

timeList *KernelSem::sleepingList = new timeList();

KernelSem::KernelSem(int value){
	lock();
		if(value < 0)this->val = 0;
		else this->val = value;
		this->myWaitList = new List();
	unlock();
}


void KernelSem::signal(){
lock();
	//this->val++;
	//syncPrintf("Nit %d pozvala signal\n", System::running->myId);
	if(this->val++ < 0){
		PCB* toUnblock = this->myWaitList->get();
		if(toUnblock){
			//syncPrintf("\n Iz waitlist semafora oslobodjen %d\n", toUnblock->myId);
			//sleepList->remove TREBA DODATI
			sleepingList->remove(toUnblock);//toUnblovk?
			toUnblock->myState = PCB::READY;
			toUnblock->wakeUpType = PCB::bySignal;
			toUnblock->mySem = 0;
			Scheduler::put(toUnblock);
			//System::dispatch(); ??
		}


	}
unlock();
}

int KernelSem::wait(Time maxToWait){// proveri valjaju li lokovi
	lock();
		if(maxToWait < 0)maxToWait = 0;
		//this->val--;
		if(--this->val < 0){ // treba da se blokira
			System::running->mySem = this;
			if(maxToWait!=0){

				System::running->sleepTime = maxToWait;
				sleepingList->put((PCB*)System::running);
			}
			System::running->myState = PCB::BLOCKED;
			this->myWaitList->put((PCB*)System::running);
			//syncPrintf("\nU waitList semafora stavljen %d\n", System::running->myId);

			System::dispatch();
			unlock();
			return System::running->wakeUpType;
		}
	unlock();
		return 0;


}

int KernelSem::getVal() const {
	return val;
}

KernelSem::~KernelSem(){
	lock();
		delete this->myWaitList;

	unlock();
}
