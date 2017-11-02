#ifndef _INFTHR_H_
#define _INFTHR_H_

#include "Thread.h"

class PCB;

class infThread: public Thread {
public:
	friend class PCB;
	infThread();
	void startS();
	virtual void run();
};





#endif
