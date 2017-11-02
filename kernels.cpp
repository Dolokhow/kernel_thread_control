#include "kernels.h"
#include "pcb.h"

ID KernelSem :: id_stat = 0;

KernelSem::KernelSem(int init) {
	tid = id_stat++;
	value = init;
	blocked = new Queue();
	Kernel :: KSList->putLast(this, tid);
}

KernelSem::~KernelSem() {
	while (value < 0) {
		PCB* pcb = blocked->getFirst(1);
		pcb->state = READY;
		Scheduler::put(pcb);
	}
}

void KernelSem :: signal() {
	value++;
	PCB* pcb = blocked->getFirst(1);
	if (pcb) {
		pcb->state = READY;
		Scheduler::put(pcb);
	}
}

void KernelSem :: wait() {
	if (--value < 0) {
		blocked->putLast((PCB*)Kernel::running);
		Kernel::running->state = BLOCKED;
		Kernel::dispatch();
	}
}

int KernelSem :: val() {
	return value;
}
