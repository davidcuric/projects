

#include <stdlib.h>
#include "list.h"
#include "system.h"
#include "pcb.h"
#include "thread.h"

List::List(){
	lock();
		this->first =this->last = 0;
		this->length = 0;
	unlock();
}

List::~List(){
	lock();
		Elem* temp = this->first;
		if(temp){
			while(temp){
				temp = this->first;
				this->first = temp->next;
				delete temp;
				if(temp == this->last)temp = 0;
			}
		}else{
			unlock();
			return;
		}

		this->first = this->last = 0;
	unlock();
}


void List::put(PCB* pcb){
	lock();
		Elem* temp = this->first;
		if(!temp){
			this->first = this->last = new Elem();
			this->first->pcb = pcb;
			this->first->next = 0;
		}
		else{
			for(temp = this->first;temp != 0;temp = temp->next){//provera da li vec postoji
				if(temp->pcb == pcb){unlock();return;}
			}
			temp = new Elem();
			temp->pcb = pcb;
			this->last->next = temp;
			this->last = this->last->next;
			this->last->next = 0;
		}
		this->length++;
	unlock();
}

PCB* List::get(){
	lock();
		if(!first){
			unlock();
			return 0;
		}
		Elem* temp = this->first;
		this->first = this->first->next;
		if(this->first == 0)
			this->last = 0;
		PCB* pcb = temp->pcb;
		this->length--;

	unlock();
	return pcb;
}

PCB* List::getViaID(ID id){
	lock();
		if(!first){
			unlock();
			return 0;
		}
		PCB* pcb = 0;
		Elem* temp = this->first;
		while(temp){
			if(temp->pcb->myId == id){
				pcb = temp->pcb;
			}
			temp = temp->next;
		}
	unlock();
	return pcb;

}

void List::remove(PCB* pcb){
Elem* tempHead;
Elem* tempFollow;
Elem* help;

lock();

if(this->first == 0){
	unlock();
	return;
}

if(first->pcb == pcb){
	help = first;
	first=first->next;
	help->next=0;
	length--;
}else{
	tempFollow = first;
	tempHead = first->next;
	while(tempHead){
		if(tempHead->pcb == pcb){
			tempFollow->next = tempHead->next;
			tempHead->next = 0;
			if(tempHead == last)this->last=tempFollow;
			length--;

		}
		tempHead=tempHead->next;
		tempFollow=tempFollow->next;
	}
}
unlock();
}














