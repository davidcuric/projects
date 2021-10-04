#include "system.h"
#include "idle.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "kersem.h"

#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

List *System::allPCBs = new List();
extern int syncPrintf(const char *format, ...);
volatile PCB* System::running = 0;
Thread* System::starting = 0;
IdleThread* System::idle = 0;

volatile int System::explicit_dispatch = 0;

const StackSize maxSize = 0x10000;

void interrupt (*System::oldRoutine)(...)=0;

void System::initialize(){ // zamenim rutinu,sacuvam staru, kreiram starting, postavim na raning,kreiram idle i startujem
lock();

#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(0x08);
	setvect(0x08,newTimerRoutine);
#endif
	Thread::createSystemThreads();
	running = (volatile PCB*)starting->myPCB;

unlock();



}

void System::finish(){// vracam staru rutinu u ulaz 0x8 i brisem starting i idles

lock();
#ifndef BCC_BLOCK_IGNORE
	setvect(0x08,oldRoutine);
#endif
	delete idle;
	delete starting;
	delete System::allPCBs;
	delete KernelSem::sleepingList;
unlock();
}

void System::dispatch(){
lock();
	explicit_dispatch = 1;
	newTimerRoutine();
	explicit_dispatch = 0;
unlock();
}



void interrupt System::newTimerRoutine(...){
	//syncPrintf("Usao u promenu\n");
	static volatile PCB* toExecute;
	static volatile unsigned int oldStackSS,oldStackSP;
	static volatile unsigned int newStackSS,newStackSP;
	if(explicit_dispatch == 0){
		//syncPrintf("TIMER\n");
		tick();
		KernelSem::sleepingList->wakingUp();
		(*oldRoutine)();
		if(running->myTimeSlice != 0){
			running->timeCntr=running->timeCntr+1;
		}
		if(running->timeCntr < running->myTimeSlice || running->myTimeSlice==0){
			return;
		}
	}
/*#ifndef BCC_BLOCK_IGNORE
	asm {cli};
#endif*/
	/*if(explicit_dispatch == 0){
		//syncPrintf("TIMER\n");
		if(running->myTimeSlice != 0){
			running->timeCntr++;
		}
		if(running->timeCntr < running->myTimeSlice || running->myTimeSlice==0){
			//syncPrintf("ret\n");
			//if(running->timeCntr < running->myTimeSlice) syncPrintf("%d (%u)", running->myId, running->timeCntr);
			//if(running->myTimeSlice==0)syncPrintf("%d besonacno", running->myId);
			return;
		}
	}*//*else{
		syncPrintf("EXPL\n");
	}*/
	explicit_dispatch = 0;
	if(running != idle->myPCB && running->myState==PCB::READY){

		Scheduler::put((PCB*)running);
		//syncPrintf("dodao u sch %d\n", running->myId);
	}
	//syncPrintf("Prosao if za dodavanje u sch\n");
	//ako nema spremnih u sched postavljam idle
	//syncPrintf("A\n");
	toExecute = Scheduler::get();
	if(toExecute == 0){toExecute = idle->myPCB;}//syncPrintf("Otisao na IDLE\n");}
	//syncPrintf("Uzeo iz schedulera %d\n", toExecute->myId);
	//if(running == toExecute) return; // dodato
//cuvam ss i sp niti koja predaje proc
#ifndef BCC_BLOCK_IGNORE
	asm{
		mov oldStackSP,sp
		mov oldStackSS,ss
	}
#endif

	running->sp = oldStackSP;
	running->ss = oldStackSS;
	//syncPrintf("Presao sa %d na %d\n", running->myId, toExecute->myId);
	//dajem proc novoj niti
	running = toExecute;
	running->timeCntr = 0;
	newStackSP = toExecute->sp;
	newStackSS = toExecute->ss;
//postavljam stack nove niti
	//syncPrintf("\nnewSP=%u  newSS=%u\n", newStackSP,newStackSS);

#ifndef BCC_BLOCK_IGNORE
	asm{
		mov sp,newStackSP
		mov ss,newStackSS
	}
#endif
	//syncPrintf("\n***\n");
	//running->timeCntr = 0;


}

