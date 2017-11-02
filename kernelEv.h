#ifndef _KERNELEV_H_
#define _KERNELEV_H_
#include "event.h"
#include "pcb.h"
#include "kernels.h"
#include "Thread.h"
#include "IVTEntry.h"
#include "kernel.h"

class KernelEv {
private:
	IVTNo ivtNo;
	PCB* thread;
	KernelSem *sem;
	ID tid;
	static ID id_stat;
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();
	ID getID() {return tid;}
};

#endif
