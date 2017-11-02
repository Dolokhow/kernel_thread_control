#ifndef _QUEUET_H_
#define _QUEUET_H_

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include "SCHEDULE.h"

#ifndef NULL
#define NULL 0L
#endif


typedef int ID;



template<class T>
class QueueT {
public:
	struct Elem {
	       T* content;
	       ID id;
	       Elem* next;
	       Elem* prev;

	       Elem (T* cont, ID idd, Elem* n = 0, Elem* p = 0) {
	           content = cont;
	           id = idd;
	           next = n;
	           prev = p;
	       }
	   };

    Elem *head, *tail;
    int size;
    friend class Kernel;

    QueueT () {
        head = tail = 0;
        size = 0;
    }
    ~QueueT () {
        Elem* temp;
        while (head != tail) {
            temp = head;
            head = head->next;
            delete temp;
            size--;
        }
    }

    void putLast (T* newCont, ID id) {
        if (head == 0) head = tail = new Elem(newCont,id,0,0);
        else {
            tail->next = new Elem (newCont,id,0,tail);
            tail = tail->next;
        }
        size++;
    }

    void QueueT<T> :: putFirst (T* newCont, ID id) {
        if (head == 0) head = tail = new Elem(newCont,id,0,0);
        else {
            head->prev = new Elem(newCont,id,head,0);
            head = head->prev;
        }
        size++;
    }

    T* getFirst (int option) {
        if (head == 0) return 0;
        if (head == tail) {
            if (option) {
                T* tempCont = head->content;
                Elem* tempElem = head;
                head = tail = 0;
                size = 0;
                delete tempElem;
                return tempCont;
            }
            else return head->content;
        }
        else {
            if (option) {
                Elem* tempElem = head;
                T* tempCont = head->content;
                head = head->next;
                head->prev->next = 0;
                head->prev = 0;
                size--;
                delete tempElem;
                return tempCont;
            }
            else return head->content;
        }
    }

    T* getLast (int option) {
        if (head == 0) return 0;
        if (head == tail) {
            if (option) {
                Elem* tempElem = tail;
                T* tempCont = tail->content;
                head = tail = 0;
                size = 0;
                delete tempElem;
                return tempCont;
            }
            else return tail->content;
        }
        else {
            if (option) {
                Elem *tempElem = tail;
                T* tempCont = tail->content;
                tail = tail->prev;
                tail->next->prev = 0;
                tail->next = 0;
                size--;
                delete tempElem;
                return tempCont;
            }
            else return tail->content;
        }
    }

    T* getByID(int option, ID id) {
        Elem* cur = head;
        while (cur != 0 && cur->id != id) cur = cur->next;
        if (cur == 0) return 0;
        if (option) {
          T* tempCont = cur->content;

          if (size == 1) head = tail = 0;
          else {
            if (cur == tail) {
              tail = cur->prev;
              tail->next = 0;
              }
            else if (cur == head) {
              head = cur->next;
              head->prev = 0;
            }
            else {
              cur->prev->next = cur->next;
              cur->next->prev = cur->prev;
            }
          }
          delete cur;
          size--;
          return tempCont;

          }
        else return cur->content;
    }

    void printQueue() {
        Elem* temp = head;
        while (temp!=0) {
            temp = temp->next;
        }
    }

};



#endif
