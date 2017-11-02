#include "kernelEv.h"

ID KernelEv :: id_stat = 0;

KernelEv :: KernelEv(IVTNo ivtNo) {
	if(ivtNo >= 0 && ivtNo < 256) {
		tid = id_stat++;
		this->thread = (PCB*)Kernel :: running;
		this->ivtNo = ivtNo;
		this->sem = new KernelSem(0);
		IVTEntry :: IVTable[ivtNo]->event = this; //proveri jos ovo!
		Kernel :: KEList->putLast(this,tid);

		if (ivtNo == 9) {
#ifndef BCC_BLOCK_IGNORE
			asm {
				mov al, 20h;
				out 20h, al;
			}
#endif
		}
	}
}

KernelEv :: ~KernelEv() {
	IVTEntry :: IVTable[ivtNo]->event = 0;
	delete sem;
}

void KernelEv :: wait() {
	if (Kernel :: running == (volatile PCB*)this->thread) {
		sem->wait();
	}
}

void KernelEv :: signal() {
	if (sem->val() >= 1) return;
	sem->signal();
}
