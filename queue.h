#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include "SCHEDULE.h"

#ifndef NULL
#define NULL 0L
#endif

class PCB;

typedef unsigned int Time;




class Queue {
public:
	struct Elem {
	       PCB* thread;
	       Elem* next;
	       Elem* prev;

	       Elem (PCB* thr, Elem* n = 0, Elem* p = 0) {
	           thread = thr;
	           next = n;
	           prev = p;
	       }
	   };

    Elem *head, *tail;
    int size;

    Queue (Queue&);
    // zabrana kopiranja i premestanja


    friend class Kernel;

    Queue () {
        head = tail = 0;
        size = 0;
    }
    ~Queue () {
        Elem* temp;
        while (head != tail) {
            temp = head;
            head = head->next;
            delete temp;
            size--;
        }

    }

    Queue* putLast (PCB*);
    Queue* putFirst(PCB*);
    PCB* getFirst (int option);
    PCB* getLast(int option);


    void sleep (Time timeToSleep, PCB* running);
    void refresh();

    // FUNKCIJE ZA PROVERU
    void printQueue();
    int getSize () {return size;}

};

#endif
