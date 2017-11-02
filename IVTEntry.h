#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

#include <dos.h>

class KernelEv;
class Kernel;

typedef void interrupt (*InterruptRoutine)(...);

class IVTEntry {
private:

	static IVTEntry* IVTable[];
	InterruptRoutine oldRoutine;
	InterruptRoutine newRoutine;

	unsigned char ivtNo;
	KernelEv* event;

public:
	friend class KernelEv;
	friend class Kernel;
	IVTEntry (unsigned char, InterruptRoutine newRoutine);
	~IVTEntry();
	void signal();
	void oldInterrupt();
	int canWork();
};

#endif


