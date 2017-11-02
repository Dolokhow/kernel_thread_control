#include <stdio.h>
#include "pcb.h"
#include "queue.h"
#include "kernel.h"

#include <iostream.h>


int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	Kernel:: init();

	int val = userMain(argc, argv);

	Kernel:: restore();

	return val;
}

