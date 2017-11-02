#ifndef _EVENT_H_
#define _EVENT_H_
typedef unsigned char IVTNo;
typedef int ID;
class KernelEv;

#include "IVTEntry.h"

class Event {
public:
   Event (IVTNo ivtNo);
  ~Event ();
void wait ();
protected:
   // friend class KernelEv;
   void signal(); // can call KernelEv
private:
   ID myID;
};


#define PREPAREENTRY(ivtNo, callOldRoutine)			\
void interrupt newRoutine##ivtNo(...);				\
IVTEntry ivtEntry##ivtNo(ivtNo, newRoutine##ivtNo);	\
void interrupt newRoutine##ivtNo(...) {				\
	if (ivtEntry##ivtNo.canWork() == 0) {			\
		return;										\
	}												\
	if (callOldRoutine)								\
		ivtEntry##ivtNo.oldInterrupt();				\
	ivtEntry##ivtNo.signal();						\
	dispatch();										\
}

#endif
