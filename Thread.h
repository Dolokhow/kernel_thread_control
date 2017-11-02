#ifndef THREAD_H_
#define THREAD_H_

#include <iostream.h>

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

// class PCB; // Kernel's implementation of a user's thread


class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	static void sleep(Time timeToSleep);

protected:
	friend class PCB;
	friend class Kernel;
	friend class infThread;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}

private:
	static void wrapper(Thread* running);
	ID myID;
};

void dispatch ();

#endif /* THREAD_H_ */
