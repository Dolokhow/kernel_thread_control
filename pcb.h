#ifndef _PCB_H_
#define _PCB_H_

#include <stdio.h>
#include <iostream.h>
#include "kernel.h"
#include "Thread.h"
#include "queue.h"

typedef unsigned long StackSize;


typedef unsigned int Time; // time, x 55ms

typedef int ID;



enum State {NEW, READY, BLOCKED, DONE, RUNNING};

class PCB {
protected:
	friend class Kernel;
	friend class Queue;
	friend class Thread;
	friend class infThread;
	friend class PCBList;
	friend class Semaphore;
	friend class KernelSem;


	friend class Thread;
    unsigned int mySP, mySS, myBP;

    Thread* myThr;
    State state;
   // int notForScheduler;



    ID tid;
    static ID id_stat;

    Time tslice, tpassed, tsleep;
    Queue* waiting;

    StackSize stackSize;
    unsigned* myStack;

public:
    PCB (Thread *thread = NULL,StackSize stackSize = defaultStackSize, Time tslice = defaultTimeSlice); // fali thread u konstruktoru, dodacu ga posle !!!
    void createStack();
    ID getID();
    ~PCB ();

};



#endif /* _pcb_h */
