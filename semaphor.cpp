#include "semaphor.h"
#include "kernel.h"
#include "kernels.h"

//ID 10
Semaphore::Semaphore(int init) {

	SysParam sysParam;
	sysParam.registers[0] = 10;
	sysParam.registers[1] = init;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif

	myID = (ID)sysParam.registers[4];
}

// ID 11
Semaphore::~Semaphore() {

	SysParam sysParam;
	sysParam.registers[0] = 11;
	sysParam.registers[1] = myID;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif
}


// ID 12
void Semaphore::wait() {

	SysParam sysParam;
	sysParam.registers[0] = 12;
	sysParam.registers[1] = myID;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif
}


// ID 13
void Semaphore::signal() {

	SysParam sysParam;
	sysParam.registers[0] = 13;
	sysParam.registers[1] = myID;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif
}


// ID 14
int Semaphore::val() const {

	SysParam sysParam;
	sysParam.registers[0] = 14;
	sysParam.registers[1] = myID;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif
	return sysParam.registers[4];
}
