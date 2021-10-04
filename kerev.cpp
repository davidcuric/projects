#include "kerev.h"
#include "system.h"
#include "SCHEDULE.h"
#include "ivtentry.h"

KernelEv::KernelEv(IVTNo ivtEntryNum){
	this->myCreator = (PCB*)System::running; // prolazi i bes casta, zasto?
	this->myEntryNum = ivtEntryNum;
	this->PCBblocked = 0;
	this->val = 0;
	IVTEntry::IVTable[ivtEntryNum]->myEvent = this; // da li da mu dodelim event samo ako sam prvi koji dodeljuje? tj ako je polje entry->myEv = 0

}
void KernelEv::signal(){
	if(this->PCBblocked != 0){
		this->PCBblocked->myState = PCB::READY;
		Scheduler::put(this->PCBblocked);
		this->PCBblocked = 0;
		//DA LI OVDE TREBA PROMENITI KONTEKST,BILO NESTO U TEKSTU??
		//Kaze uvek kada se dogodi prekid treba promeniti kontekst,
		//rutina je def u prepareentry tu se poziva signal mogu ovde eksplicitno promeniti kontekst
		System::dispatch();
	}else{
		this->val = 1;
	}

}

void KernelEv::wait(){
	if((PCB*)System::running == this->myCreator){
		if(!val){
			System::running->myState = PCB::BLOCKED;
			this->PCBblocked = (PCB*)System::running;
			System::dispatch();
		}
		else {
			this->val = 0;
		}
	}
}
KernelEv::~KernelEv(){
	IVTEntry::IVTable[this->myEntryNum]->myEvent = 0;
}
