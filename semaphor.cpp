#include "semaphor.h"
#include "system.h"
#include "kersem.h"


Semaphore::Semaphore(int val){
	lock();
		this->myImpl = new KernelSem(val);
	unlock();
}

Semaphore::~Semaphore(){
	lock();
		delete myImpl;
	unlock();
}

void Semaphore::signal(){
	lock();
		this->myImpl->signal();
	unlock();
}

int Semaphore::wait(Time maxTimeToWait){
	int ret;
	lock();
		ret = this->myImpl->wait(maxTimeToWait);
	unlock();
	return ret;
}

int Semaphore::val() const{
	int ret;
	lock();
		ret = this->myImpl->getVal();
	unlock();
	return ret;
}
