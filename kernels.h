#ifndef _KERNELS_H_
#define _KERNELS_H_

#include "queue.h"
#include <iostream.h>
typedef int ID;

class KernelSem {
public:
	friend class Kernel;
	static ID id_stat;
	ID tid;
	KernelSem(int init);
	~KernelSem();
	void signal();
	void wait();
	int val();
	ID getID() {return tid;}

	int value;
	Queue* blocked;

};

#endif
