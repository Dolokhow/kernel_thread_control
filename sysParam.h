#ifndef _SYSPARAM_H_
#define _SYSPARAM_H_

#include <dos.h>

#define sysCallPrepare(sysParam) 	\
{ 									\
	int a = FP_SEG(sysParam); 		\
	int b = FP_OFF(sysParam); 		\
	asm { 							\
		mov ax, a; 					\
		mov bx, b; 					\
	} 								\
}

struct SysParam {
	long registers[5];
};

#endif
