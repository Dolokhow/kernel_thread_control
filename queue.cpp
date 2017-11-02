#include "queue.h"
#include "pcb.h"

Queue* Queue :: putLast (PCB* newThr) {
    if (head == 0) head = tail = new Elem(newThr);
    else {
        tail->next = new Elem (newThr,0,tail);
        tail = tail->next;
    }
    size++;
    return this;
}



Queue* Queue :: putFirst (PCB* newThr) {
    if (head == 0) head = tail = new Elem(newThr);
    else {
        head->prev = new Elem(newThr,head,0);
        head = head->prev;
    }
    size++;
    return this;
}

PCB* Queue :: getFirst (int option) {
    if (head == NULL) return 0;
    if (head == tail) {
        if (option) {
            PCB* tempThr = head->thread;
            Elem* tempElem = head;
            head = tail = 0;
            size = 0;
            delete tempElem;
            return tempThr;
        }
        else return head->thread;
    }
    else {
        if (option) {
            Elem* tempElem = head;
            PCB* tempThr = head->thread;
            head = head->next;
            head->prev->next = 0;
            head->prev = 0;
            size--;
            delete tempElem;
            return tempThr;
        }
        else return head->thread;
    }
}

PCB* Queue :: getLast (int option) {
    if (head == 0) return 0;
    if (head == tail) {
        if (option) {
            Elem* tempElem = tail;
            PCB* tempThr = tail->thread;
            head = tail = 0;
            size = 0;
            delete tempElem;
            return tempThr;
        }
        else return tail->thread;
    }
    else {
        if (option) {
            Elem *tempElem = tail;
            PCB* tempThr = tail->thread;
            tail = tail->prev;
            tail->next->prev = 0;
            tail->next = 0;
            size--;
            delete tempElem;
            return tempThr;
        }
        else return tail->thread;
    }
}


void Queue :: printQueue() {
    Elem* temp = head;
    while (temp!=0) {
        temp = temp->next;
    }
}

void Queue :: sleep (Time timeToSleep, PCB* running) {
	Elem* cur = head;
	if (cur == 0) {
		running->tsleep = timeToSleep;
		putFirst(running);
	}
	else {
		while (cur != 0 && timeToSleep > cur->thread->tsleep) {
		timeToSleep -= cur->thread->tsleep;
		cur = cur->next;
		}
		running->tsleep = timeToSleep;
		Elem* prev = 0;
		if (cur != 0) {
			if (cur->prev != 0) {
				prev = cur->prev;
				prev->next = new Elem(running, cur, cur->prev);
				cur->prev = prev->next;
				cur->thread->tsleep -= timeToSleep;
			} else {
				putFirst(running);
				head->next->thread->tsleep -= timeToSleep;
			}

		} else {
			putLast(running);
		}
	}
	size++;
}



void Queue :: refresh() {
	while (head != 0 && head->thread->tsleep <= 0) {
		head->thread->state = READY;
		Scheduler :: put(getFirst(1));
	}
	if (head != 0) head->thread->tsleep--;
}







