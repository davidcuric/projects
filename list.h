#ifndef _list_h_
#define _list_h_

#include "thread.h"
#include "pcb.h"
class PCB;

class List{
public:
	List();
	int length;
	struct Elem{
		PCB* pcb;
		Elem* next;
	};
	void put(PCB* pcb);
	PCB* get();
	PCB* getViaID(ID id);
	void remove(PCB* pcb);


    ~List();
private:
    Elem* first;
    Elem* last;
};
#endif
