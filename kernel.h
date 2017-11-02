#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <stdio.h>
#include <dos.h>
#include <iostream.h>
#include <stdlib.h>

#include "queue.h"
#include "sysParam.h"
#include "queuet.h"

typedef void interrupt (*intRoutine) (...);
class Thread;
class infThread;

class PCB;
class KernelSem;
class KernelEv;


#define lock { asm pushf; asm cli; }
#define unlock asm popf;

class Kernel {
public:
	friend class PCB;
	friend class Queue;
	friend class KernelSem;
	static volatile PCB* running;
	static volatile PCB* start;
	static volatile infThread* infinite;
	static void interrupt (*oldRoutine) (...);
	static volatile int cntchng;
	static volatile int sysCall;
	static Queue* SleepQueue;

	static QueueT<PCB>* PCBList;
	static QueueT<KernelSem>* KSList;
	static QueueT<KernelEv>* KEList;

	static PCB* sysPCB;

	static int callTimer;
	static int callIVT[256];
	static int stopDispatch;

	static void sysInit();
	static void sysRestore();
	static void interrupt sysStartRoutine(...);
	static void interrupt (*old61h) (...);
	static void interrupt sysEndRoutine(...);
	static void interrupt (*old63h) (...);
	static void sysWrapper(SysParam* sysParam);
	static void sysStackInit(SysParam* sysParam);
	static void sysCallInterrupts();

	static void interrupt timer(...);
	static void init();
	static void restore();
	static void dispatch();
	static void wrapper();
	static void sleep (Time timeToSleep);

};


#endif
