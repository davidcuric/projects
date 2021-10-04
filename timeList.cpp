#include "timeList.h"
#include "kersem.h"

int syncPrintf(const char *format, ...);
timeList::timeList(){
	lock();
		this->first =this->last = 0;
		this->length = 0;
	unlock();
}

timeList::~timeList(){
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
		this->length = 0;
	unlock();
}

void timeList::put(PCB* pcb){
	lock();
		Elem* toAdd = new Elem();
		toAdd->pcb = pcb;
		toAdd->next = 0;

		if(first == 0){
			first = last = toAdd;
			this->length++;
			unlock();
			return;
		}

		Elem* curr;
		Elem* prev;
		int timeSum = 0;
		int x=pcb->sleepTime;
		curr = first;
		timeSum = curr->pcb->sleepTime;
		prev = 0;
		while((timeSum <= pcb->sleepTime) && curr != 0 ){
			prev = curr;
			curr = curr->next;
			if(curr != 0){
				timeSum+= curr->pcb->sleepTime;
			}


		}
		if(curr != 0){

			toAdd->pcb->sleepTime += curr->pcb->sleepTime - timeSum;
			//syncPrintf("Ubacujem u sleepQ nit %d i vrednost sleep tajma u listi je %d\n",toAdd->pcb->myId,toAdd->pcb->sleepTime);
			toAdd->next = curr;
			if(curr == first){
				first = toAdd;
			}else{
				prev->next = toAdd;
			}
			if(toAdd->pcb->sleepTime != 0)
				curr->pcb->sleepTime = timeSum - x;
			//syncPrintf("Zbog novog unosa promenjen sleepTime niti %d na vrednost %d",curr->pcb->myId,curr->pcb->sleepTime);

		}else {
			toAdd->pcb->sleepTime -= timeSum;
			//syncPrintf("Ubacujem u sleepQ nit %d i vrednost sleep tajma u listi je %d\n",toAdd->pcb->myId,toAdd->pcb->sleepTime);
			toAdd->next=0;
			prev->next = toAdd;
			last = toAdd;

		}
	unlock();
}

void timeList::wakingUp(){
	lock();
		if(!first){unlock();return;}
		else {
			first->pcb->sleepTime -= 1;
			while(first->pcb->sleepTime == 0){ 
				first->pcb->myState = PCB::READY;
				Scheduler::put(first->pcb);
				first->pcb->wakeUpType = PCB::byTime;
				first->pcb->mySem->val += 1;//proveri
				//first->pcb->myWaitList->remove(first->pcb);
				first->pcb->mySem->myWaitList->remove(first->pcb);
				Elem* toDelete = first;
				first = first->next;
				this->length--;
				delete toDelete;


			}
		}

	unlock();
}

void timeList::remove(PCB* pcb){
Elem* tempHead;
Elem* tempFollow;
Elem* help;

lock();
if(!first){unlock();return;}
if(first->pcb == pcb){
	help = first;
	first->next->pcb->sleepTime += first->pcb->sleepTime;
	first=first->next;
	help->next=0;
	delete help;
	length--;
}else{
	tempFollow = first;
	tempHead = first->next;
	while(tempHead){
		if(tempHead->pcb == pcb){
			tempFollow->next = tempHead->next;
			if(tempHead->next){
				tempHead->next->pcb->sleepTime += tempHead->pcb->sleepTime;
			}
			tempHead->next = 0;
			if(tempHead == last)this->last=tempFollow;
			length--;
			//delete tempHead;
		}
		tempHead=tempHead->next;
		tempFollow=tempFollow->next;
	}
}
//delete tempHead;
//delete tempFollow;
//delete help;
unlock();
}





