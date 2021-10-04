#include "pcb.h"
#include "system.h"
#include "SCHEDULE.h"
#include "thread.h"

#include <dos.h>
#include <stdlib.h>

extern int syncPrintf(const char *format, ...);

const int PCB::NEW = 0;
const int PCB::READY = 1;
const int PCB::BLOCKED = 2;
const int PCB::DONE = 3;

const int PCB::bySignal = 1;
const int PCB::byTime = 0;
//-1 starting, 0 idle, >0 korisnicke niti
ID idCounter = -1;
const int PCB::stackSizeLimit = 32767;

PCB::PCB(Thread* thread,StackSize ssize, Time slice){
	lock();
	this->myId = idCounter++;
	this->myThread = thread;
	if(ssize <= stackSizeLimit)
		this->myStackSize = ssize;
	else this->myStackSize = stackSizeLimit;
	this->myTimeSlice = slice;
	this->ss = this->sp = 0;
	this->myStack = 0;
	this->myState = NEW;
	this->myWaitList=new List();
	this->timeCntr = 0;
	this->sleepTime = 0;
	this->mySem = 0;
	System::allPCBs->put(this);
	unlock();
}


PCB::~PCB(){
	lock();
		delete myStack;
		delete myWaitList;
		System::allPCBs->remove(this);
	unlock();
}

unsigned int PCB::getCnt(){
	return PCB::timeCntr;
}



void PCB::createStack(){
	lock();

		myStack = new unsigned char[myStackSize];
		static volatile unsigned newStackSP, newStackSS;// oni mi cuvaju segment i offset novog steka
		static volatile unsigned oldSP, oldSS; // pomocni za cuvanje ss i sp steka funkcije(starog steka)
		static volatile unsigned wrapCodeSeg, wrapInsPoint; // ovde cuvam adresu wrapper funcije da je postavim na stek
#ifndef BCC_BLOCK_IGNORE
		newStackSP = this->sp = FP_OFF(myStack+myStackSize);
		newStackSS = this->ss = FP_SEG(myStack+myStackSize);

		wrapCodeSeg = FP_SEG(&(PCB::wrapper));
		wrapInsPoint = FP_OFF(&(PCB::wrapper));

		asm{
			//Prelazim sa steka ove funkcije na novi stek koji samo kreirao za nit
			mov oldSS,ss
			mov oldSP,sp

			mov ss,newStackSS
			mov sp,newStackSP

			//pusujem flagove
			pushf

			//treba da eksplicitno postavim I bit u psw na 1 da bih dozvolio prekid
			pop ax
			or ax, 1000000000b
			push ax

			//sledeca mesta su za povratnu adresu, tu ubacujem adresu wrappera koju sam sacuvao ranije
			//prvo code segment pa ins pointer
			mov ax, wrapCodeSeg
			push ax

			mov ax, wrapInsPoint
			push ax
			//zatim slede programski dostupni registri koji se pri interruptu automatski postavljaju/skidaju
			mov ax, 0
			push ax
			push bx
			push cx
			push dx
			push es
			push ds
			push si
			push di
			push bp
			//apdejt stek pointera niti
			mov newStackSP, sp
			//povratak na stek  funkcije
			mov sp, oldSP
			mov ss, oldSS

		}


#endif
		this->sp = newStackSP;

	unlock();
}
void clearWaitList(){
	int i;
		PCB* temp = System::running->myWaitList->get();
		while(temp != 0){
		temp->myState = PCB::READY;
		Scheduler::put(temp);
		syncPrintf("%d oslobodio %d i stavio u SCH\n", System::running->myId, temp->myId);
		temp = System::running->myWaitList->get();
		}
		System::running->myState = PCB::DONE;

}

void PCB::wrapper(){
	System::running->myThread->run();
	syncPrintf("%d napustio RUN\n", System::running->myId);
	// zavrsio se run
	lock();
	clearWaitList();
	System::dispatch();
}




