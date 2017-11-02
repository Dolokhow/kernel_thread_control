#include "IVTEntry.h"
#include "kernel.h"
#include "kernels.h"
#include "kernelev.h"

#include <iostream.h>

IVTEntry* IVTEntry :: IVTable[256];


IVTEntry :: IVTEntry (unsigned char ivtNo,InterruptRoutine newRoutine) {
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	this->ivtNo = ivtNo;
	this->event = 0;
	this->IVTable[ivtNo] = this;
#ifndef BCC_BLOCK_IGNORE
	this->oldRoutine = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
	this->newRoutine = newRoutine;
	unlock;
#endif
}

IVTEntry :: ~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	lock;
	setvect(ivtNo,oldRoutine);

	if (ivtNo == 9) {
		asm {
			mov al, 20h;
			out 20h, al;
		}
	}
	unlock;
#endif
}

void IVTEntry :: signal() {
	if (event != 0) {
		event->signal();
	}
}

void IVTEntry :: oldInterrupt() {
	if (oldRoutine != 0) {
		oldRoutine();
	}
}

int IVTEntry::canWork() {
	if (Kernel::sysCall == 0)
		return 1;
	else {
		Kernel::callIVT[ivtNo] = 1;
		return 0;
	}
}

