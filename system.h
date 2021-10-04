#ifndef _system_h_
#define _system_h_

#include <dos.h>
#include "thread.h"
#include "pcb.h"
#include "list.h"
#include <stdio.h>
#include <stdarg.h>
#include "idle.h"

class List;


//#define lock() {asm{pushf; cli;}}
#define lock() asm pushf;\
				asm cli;
//#define unlock() {asm{popf}}
#define unlock() asm popf;




class System{
public:
	static volatile PCB* running;
	static Thread *starting;
	static IdleThread *idle;
	static List* allPCBs;
	static void dispatch();
	static void initialize();
	static void finish();



	static void interrupt (*oldRoutine)(...);
	static void interrupt newTimerRoutine(...);

	static volatile int explicit_dispatch;
};
extern void tick();
#endif
