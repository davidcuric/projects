#ifndef _kerev_h_
#define _kerev_h_

class PCB;

typedef unsigned char IVTNo;

class KernelEv{
public:
	KernelEv(IVTNo ivtEntryNum);
	void wait();
	void signal();
	int val;
	unsigned char myEntryNum;
	PCB* myCreator;
	PCB* PCBblocked;
	~KernelEv();

};

#endif
