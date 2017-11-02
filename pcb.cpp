#include "pcb.h"


int PCB :: id_stat = 0;

PCB :: PCB (Thread *thread, StackSize stackSize, Time tslice) {

    mySP = mySS = myBP = 0;
    this->myThr = thread;
    tid = id_stat++;
    this->tslice = tslice;
    tpassed = 0.;
    tsleep = 0;
    this->stackSize = stackSize;
    myStack = NULL;
    waiting = new Queue();
	state = NEW;

    Kernel :: PCBList->putLast(this, tid);
}

void PCB :: createStack() {
    myStack = new unsigned int[stackSize];
#ifndef BCC_BLOCK_IGNORE
	myStack[stackSize-1] = FP_SEG(myThr);
	myStack[stackSize-2] = FP_OFF(myThr);
    myStack[stackSize-5] = 0x200;

    myStack[stackSize-6] = FP_SEG(Thread :: wrapper);
    myStack[stackSize-7] = FP_OFF(Thread :: wrapper);
    mySP = FP_OFF(myStack+stackSize-16);
    mySS = FP_SEG(myStack+stackSize-16);
    myBP = FP_OFF(myStack+stackSize-16);
#endif
}

ID PCB :: getID() {return tid;}

PCB :: ~PCB() {
    delete waiting;
    delete[] myStack;
}

