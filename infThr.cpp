#include "infThr.h"
#include "sysParam.h"

#include "kernel.h"

infThread :: infThread() : Thread() {}

void infThread :: run() {
	while (1) {
		dispatch();
	}
}

void infThread :: startS() {

	SysParam sysParam;
	sysParam.registers[0] = 7;
	sysParam.registers[1] = myID;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif
}

