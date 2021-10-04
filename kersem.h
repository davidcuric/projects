#ifndef _kersem_h_
#define _kersem_h_

#include "list.h"
#include "timeList.h"
class Semaphore;
class timeList;
typedef unsigned int Time;
class KernelSem{
public:
	KernelSem(int value=1);
	~KernelSem();
	void signal();
	int wait(Time maxToWait);
	int val;

	int getVal() const;
	static timeList *sleepingList;
protected:
	friend class Semaphore;
	friend class timeList;
private:

	List *myWaitList;
};


#endif
