#include "kernel.h"

#include "pcb.h"
#include "kernels.h"
#include "kernelEv.h"

#include "Thread.h"
#include "infThr.h"
#include "IVTEntry.h"



#include <iostream.h>

void tick();

// INICIJALIZACIJA STATICKIH CLANOVA KLASE




volatile PCB* Kernel :: running = 0;
void interrupt ( *Kernel :: oldRoutine)(...) = 0;
volatile int Kernel :: cntchng = 0;
volatile int Kernel :: sysCall = 0;
volatile PCB* Kernel :: start = 0;
volatile infThread* Kernel :: infinite = 0;
Queue* Kernel :: SleepQueue = 0;

QueueT<PCB>* Kernel :: PCBList = 0;
QueueT<KernelSem>* Kernel :: KSList = 0;
QueueT<KernelEv>* Kernel :: KEList = 0;

PCB* Kernel :: sysPCB = 0;

void interrupt (*Kernel::old61h) (...) = 0;
void interrupt (*Kernel::old63h) (...) = 0;

int Kernel::callTimer = 0;
int Kernel::callIVT[256] = {0};
int Kernel::stopDispatch = 0;


// DEFINISANJE FUNKCIJA KLASE

void interrupt Kernel :: timer(...){ // prekidna rutina
	if (sysCall) {
		callTimer = 1;
		return;
	}

#ifndef BCC_BLOCK_IGNORE
	asm int 60h;
#endif
	running->tpassed++;
	SleepQueue->refresh();
	tick();

	if (running->tpassed >= running->tslice && running->tslice != 0) {
		::dispatch();	// obicni, a ne kernel dispatch jer ovde sigurno nije u sistemskom pozivu
		// pa treba da izvrsi taj sistemski poziv
	}
}




// postavlja novu prekidnu rutinu
void Kernel :: init(){

	sysInit();
	SleepQueue = new Queue();
#ifndef BCC_BLOCK_IGNORE
	lock;
	oldRoutine = getvect(0x08);
	setvect(0x08, timer);
	setvect(0x60, oldRoutine);
	unlock;
#endif
	start = new PCB();
	start->state = RUNNING;
	running = start;
	infinite = new infThread();
	((infThread*)infinite)->startS();
}

// vraca staru prekidnu rutinu
void Kernel :: restore(){
	if (running != start) { return; }
	running->state = DONE;
#ifndef BCC_BLOCK_IGNORE
	lock;
	oldRoutine = getvect(0x60);
	setvect(0x08, oldRoutine);
	unlock;
#endif

	delete infinite;
	delete PCBList->getByID(1,start->tid);	// delete start;

	sysRestore();
}

void Kernel :: dispatch() {
	if (stopDispatch) {
		cntchng = 1;
		return;
	}

	if(running->state != DONE && running->state != BLOCKED && running->myThr != (Thread*)infinite) {
		running->state = READY;
		Scheduler::put((PCB *)running);
	}

	running = Scheduler::get();
	if (running == 0 || running->state != READY)
		running = PCBList->getByID(0,infinite->myID);

	running->tpassed = 0;
	running->state = RUNNING;
}

void Kernel :: wrapper() {
	running->state = DONE;
	while (running->waiting->size > 0) {
		PCB* temp = running->waiting->getLast(1);
		if (temp->state != DONE)
			temp->state = READY;
		if (temp->state != DONE)
			Scheduler :: put((PCB*)temp);
	}

	Kernel :: dispatch();
}

void Kernel :: sleep(Time timeToSleep) {
	running->state = BLOCKED;
	SleepQueue->sleep(timeToSleep, (PCB*)running);
}




void Kernel :: sysInit() {
	PCBList = new QueueT<PCB>();
	KSList = new QueueT<KernelSem>();
	KEList = new QueueT<KernelEv>();

	sysPCB = new PCB(0, 4096, 0);

#ifndef BCC_BLOCK_IGNORE
	lock;
	old61h = getvect(0x61);
	setvect(0x61, sysStartRoutine);
	old63h = getvect(0x63);
	setvect(0x63, sysEndRoutine);
	unlock;
#endif
}

void Kernel :: sysRestore() {
	delete sysPCB;

	delete KEList;
	delete KSList;
	delete PCBList;
}

void interrupt Kernel :: sysStartRoutine(...) {
	static int a, b;
	// IZ REGISTARA PROCESORA AX I BX U KOJIMA CUVAM ADRESU STRUKTURE KOJA SADRZI
	// SVE INFORMACIJE OTPAKUJEM ADRESU I PRAVIM POKAZIVAC
#ifndef BCC_BLOCK_IGNORE
	asm {
		mov a, ax;
		mov b, bx;
	}
#endif

	static SysParam* sysParam;
#ifndef BCC_BLOCK_IGNORE
	sysParam = (SysParam*)MK_FP(a, b);
#endif

// INICIJALIZUJE SISTEMSKI STEK SA PARAMETRIMA SISTEMSKOG WRAPPERA NA KOJI CE SE
// PRECI U OSTATKU KODA
	sysStackInit(sysParam);

// PRELAZAK NA SISTEMSKU NIT (PCB) - CUVA DOKLE JE RUNNING STIGAO
	static int tss, tsp, tbp;
#ifndef BCC_BLOCK_IGNORE
	asm {
		mov tss, ss;
		mov tsp, sp;
		mov tbp, bp;
	}
#endif
	running->mySS = tss;
	running->mySP = tsp;
	running->myBP = tbp;
	tss = sysPCB->mySS;
	tsp = sysPCB->mySP;
	tbp = sysPCB->myBP;
#ifndef BCC_BLOCK_IGNORE
	asm {
		mov ss, tss;
		mov sp, tsp;
		mov bp, tbp;
	}
#endif

	sysCall = 1;
}

void interrupt Kernel :: sysEndRoutine(...) {
	static int tss, tsp, tbp;
	tss = running->mySS;
	tsp = running->mySP;
	tbp = running->myBP;
#ifndef BCC_BLOCK_IGNORE
	asm {
		mov ss, tss;
		mov sp, tsp;
		mov bp, tbp;
	}
#endif

	sysCall = 0;

	sysCallInterrupts();
}

void Kernel :: sysWrapper(SysParam* sysParam) {

	switch (sysParam->registers[0]) {
		// MESTA 0-9 REZERVISANA ZA KLASU THREAD
		case 0: {
			PCB* pcb = new PCB(
				(Thread*)sysParam->registers[1],
				(StackSize)sysParam->registers[2],
				(Time)sysParam->registers[3]);

			sysParam->registers[4] = pcb->getID();
		} break;

		case 1: {
			PCB* myPCB = PCBList->getByID(1,sysParam->registers[1]);
			delete myPCB;
		} break;

		case 2: {
			PCB* myPCB = PCBList->getByID(0,sysParam->registers[2]);
			myPCB->createStack();
			myPCB->state = READY;
			Scheduler :: put((PCB*)myPCB);
		}
		break;

		case 3: {
			PCB* myPCB = Kernel :: PCBList->getByID(0,sysParam->registers[1]);
			if (myPCB != 0) {
				if ((myPCB->state != DONE) &&
					(sysParam->registers[2] != (long)Kernel :: infinite))
				{
					Kernel :: running->state = BLOCKED;
					myPCB->waiting->putLast((PCB*)Kernel :: running);
					Kernel :: dispatch();
				}
			}
		} break;

		case 4: {
			Kernel :: dispatch();
		}
		break;

		case 5:
			Kernel :: sleep((Time) sysParam->registers[1]);
			Kernel :: dispatch();
		break;

		case 6:
			Kernel :: wrapper();
		break;

		case 7: {
			PCB* idlePCB = PCBList->getByID(0,sysParam->registers[1]);
			idlePCB->state = READY;
			idlePCB->createStack();
		} break;

		// MESTA 10-19 REZERVISANA ZA KLASU SEMAFOR
		case 10: {
			KernelSem* sem = new KernelSem(
				(int)sysParam->registers[1]
			);
			sysParam->registers[3] = (long)sem;
			sysParam->registers[4] = sem->getID();
		}
		break;

		case 11:
			delete (KSList->getByID(1,sysParam->registers[1]));
		break;

		case 12:
			(KSList->getByID(0,sysParam->registers[1]))->wait();
		break;

		case 13:
			(KSList->getByID(0,sysParam->registers[1]))->signal();
		break;

		case 14: {
			int value = (KSList->getByID(0,sysParam->registers[1]))->val();
			sysParam->registers[4] = value;
		}
		break;

		//MESTA 20 - 29 REZERVISANA ZA EVENT
		case 20: {
			KernelEv* kev = new KernelEv((IVTNo)sysParam->registers[1]);
			sysParam->registers[4] = kev->getID();
			sysParam->registers[3] = (long) kev;
		} break;

		case 21: {
			KernelEv* kev = KEList->getByID(1,sysParam->registers[1]);
			delete kev;
		} break;

		case 22: {
			KernelEv* kev = KEList->getByID(0,sysParam->registers[1]);
			kev->wait();
		} break;

		case 23: {
			KernelEv* kev = KEList->getByID(0,sysParam->registers[1]);
			kev->signal();
		} break;

		default:
			break;
	}

#ifndef BCC_BLOCK_IGNORE
	asm int 63h;
#endif

}

void Kernel :: sysStackInit(SysParam* sysParam) {

	if (sysPCB->myStack == 0)
		sysPCB->myStack = new unsigned int[sysPCB->stackSize];
#ifndef BCC_BLOCK_IGNORE
	sysPCB->myStack[sysPCB->stackSize-1] = FP_SEG(sysParam);
	sysPCB->myStack[sysPCB->stackSize-2] = FP_OFF(sysParam);
	sysPCB->myStack[sysPCB->stackSize-5] = 0x200;
	sysPCB->myStack[sysPCB->stackSize-6] = FP_SEG(Kernel :: sysWrapper);
	sysPCB->myStack[sysPCB->stackSize-7] = FP_OFF(Kernel :: sysWrapper);

	sysPCB->mySP = FP_OFF(sysPCB->myStack+sysPCB->stackSize-16);
	sysPCB->mySS = FP_SEG(sysPCB->myStack+sysPCB->stackSize-16);
	sysPCB->myBP = FP_OFF(sysPCB->myStack+sysPCB->stackSize-16);
#endif
}

void Kernel :: sysCallInterrupts() {
	stopDispatch = 1;

	if (callTimer) {
		callTimer = 0;
		timer();
	}

	for (int i = 0; i < 256; i++) {
		if (callIVT[i] && IVTEntry::IVTable[i]) {
			callIVT[i] = 0;
#ifndef BCC_BLOCK_IGNORE
			lock;
#endif
			IVTEntry::IVTable[i]->newRoutine();
#ifndef BCC_BLOCK_IGNORE
			unlock;
#endif
		}
	}

	stopDispatch = 0;
	if (cntchng) {
		cntchng = 0;
		::dispatch();
	}
}
