#include "kernel.h"
#ifndef _ZAGL1_H_
#define _ZAGL1_H_

extern volatile int lockFlag;
class A : public Thread {
  public:
	char symbol;
    A(int option) {
    	switch (option) {
    	case 0: symbol = 'a'; break;
    	case 1: symbol = 'b'; break;
    	case 2: symbol = 'c'; break;
    	case 3: symbol = 'd'; break;
    	case 4: symbol = 'e'; break;
    	case 5: symbol = 'f'; break;
    	default: symbol = 'z';
    	}
    }

    virtual void run(){
    	lockFlag = 0;
    	cout << "Usao u " << symbol << " !" << endl;
    	lockFlag = 1;

    	for (int i =0; i < 30; ++i) {
    		lockFlag = 0;
    		cout<<"u " << symbol << "i =  " << i <<endl;
    		lockFlag = 1;
    		if(Kernel :: cntchng){
    			Kernel :: dispatch();
    		}
    		for (int k = 0; k<100; ++k)
    			for (int j = 0; j <30000; ++j);
    	}
    }
};

#endif
