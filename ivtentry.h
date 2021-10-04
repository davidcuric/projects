#ifndef _ivtentry_h_
#define _ivtentry_h_

class KernelEv;
typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

#define PREPAREENTRY(n,oldFlag)\
void interrupt inter##n(...);\
IVTEntry entry##n(n,inter##n);\
void interrupt inter##n(...){\
	entry##n.signal();\
	if(oldFlag)\
		entry##n.oldrout();\
}

class IVTEntry{
public:
	IVTEntry(IVTNo ivtNo,pInterrupt newRoutine);

	static IVTEntry* IVTable[256];
	KernelEv * myEvent;
	friend class KernelEv;
	void signal();
	void oldrout();
	pInterrupt oldRout;
	IVTNo entryNum;
	~IVTEntry();

};


#endif
