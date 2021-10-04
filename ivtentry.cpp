#include "ivtentry.h"
#include <dos.h>
#include "system.h"
#include "kerev.h"
IVTEntry * IVTEntry::IVTable[256] = {0};

IVTEntry::IVTEntry(IVTNo n, pInterrupt newRoutine){
	lock();
		this->entryNum = n;
		this->oldRout = 0;
		this->myEvent = 0;
		IVTable[n] = this;
#ifndef BCC_BLOCK_IGNORE
		this->oldRout = getvect(n);
		setvect(n,newRoutine);
#endif
	unlock();
}
IVTEntry::~IVTEntry(){
	lock();
		IVTable[this->entryNum] = 0;
#ifndef BCC_BLOCK_IGNORE
		setvect(this->entryNum,oldRout);
#endif
	unlock();
}
void IVTEntry::oldrout(){
	lock();
		oldRout();
	unlock();
}
void IVTEntry::signal(){
	lock();
		if(this->myEvent == 0){unlock();return;}
		else this->myEvent->signal();
	unlock();
}
