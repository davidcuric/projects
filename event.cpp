#include "event.h"
#include "system.h"
#include "kerev.h"


Event::Event(IVTNo n){
	lock();
		this->myImpl = new KernelEv(n);
	unlock();
}
Event::~Event(){
	lock();
		delete this->myImpl;
	unlock();
}

void Event::signal(){
	lock();
		this->myImpl->signal();
	unlock();
}

void Event::wait(){
	lock();
		this->myImpl->wait();
	unlock();
}
