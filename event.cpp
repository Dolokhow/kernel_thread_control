#include "event.h"
#include "kernel.h"
#include "kernelev.h"
#include <iostream.h>

//ID 20
Event :: Event (IVTNo ivtNo) {

	SysParam sysParam;
	sysParam.registers[0] = 20;
	sysParam.registers[1] = ivtNo;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif

	myID = (ID)sysParam.registers[4];
}

//ID 21
Event :: ~Event () {

	SysParam sysParam;
	sysParam.registers[0] = 21;
	sysParam.registers[1] = myID;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif
}

//ID 22
void Event :: wait () {

	SysParam sysParam;
	sysParam.registers[0] = 22;
	sysParam.registers[1] = myID;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif
}

//ID 23
void Event :: signal() {

	SysParam sysParam;
	sysParam.registers[0] = 23;
	sysParam.registers[1] = myID;

#ifndef BCC_BLOCK_IGNORE
	sysCallPrepare(&sysParam);
	asm int 61h;
#endif
}

