#include "Thread.h"
#include "sysParam.h"

#include "kernel.h"
#include "pcb.h"




// ID 0
Thread :: Thread(StackSize stackSize, Time timeSlice) {

	SysParam sysParam;
	sysParam.registers[0] = 0;
	sysParam.registers[1] = (long)this;
	sysParam.registers[2] = stackSize;
	sysParam.registers[3] = timeSlice;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif

	myID = (ID)sysParam.registers[4];
}

//ID 1
Thread :: ~Thread() {

	waitToComplete();
	SysParam sysParam;
	sysParam.registers[0] = 1;
	sysParam.registers[1] = myID;

	#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
	#endif

}

//ID 2
void Thread :: start() {

	SysParam sysParam;
	sysParam.registers[0] = 2;
	sysParam.registers[1] = (long)this;
	sysParam.registers[2] = myID;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif

}

// ID 3
void Thread :: waitToComplete() {

	SysParam sysParam;
	sysParam.registers[0] = 3;
	sysParam.registers[1] = myID;
	sysParam.registers[2] = (long)this;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif
}

// ID 4
void dispatch() {
	SysParam sysParam;
	sysParam.registers[0] = 4;

	#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
	#endif
}

// ID 5
void Thread :: sleep (Time timeToSleep) {
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	SysParam sysParam;
	sysParam.registers[0] = 5;
	sysParam.registers[1] = timeToSleep;

#ifndef BCC_BLOCK_IGNORE
		sysCallPrepare(&sysParam);
		asm int 61h;
#endif

#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

// ID 6
void Thread::wrapper(Thread* running) {
	running->run();

	SysParam sysParam;
	sysParam.registers[0] = 6;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif
}
