#ifndef _timelist_h_
#define _timelist_h_

#include "thread.h"
#include "pcb.h"
class PCB;

class timeList{
public:
	timeList();
	int length;
	struct Elem{
		PCB* pcb;
		Elem* next;
	};
	void put(PCB* pcb);
	//PCB* get();
	void wakingUp();
	void remove(PCB* pcb);


    ~timeList();
private:
    Elem* first;
    Elem* last;
};
#endif
