/*
#include "thread.h"
#include "semaphor.h"
#include "event.h"
#include "ivtentry.h"

#include <iostream.h>
#include <stdlib.h>

void tick() {}

 PREPAREENTRY(9,1);


Semaphore* mutex = 0;
Semaphore* full = 0;
Semaphore* empty = 0;
int buffer[10];
int i, j;



class Znak : public Thread {
public:
	Znak(char c, int n, Time s = 0) : Thread(), c(c), n(n), s(s) {}
	virtual ~Znak() { waitToComplete(); }

	void run() {
		for (int i = 0; i < n; i++) {
		// for (long i = 0; i < 100000; i++) {
			mutex->wait();
			cout << c;
			mutex->signal();


			// for (int j = 0; j < 10000; j++)
				// for (int k = 0; k < 15000; k++);
			sleep(s);

			 //dispatch();
		}

		mutex->wait();
		cout << endl << c << " finished" << endl;
		mutex->signal();
	}

private:
	char c;
	int n;
	Time s;

};


class Key : public Thread {
public:
	Key(int n) : Thread(), n(n) { }
	virtual ~Key() { waitToComplete(); }

	void run() {
		Event e(9);

		for (int i = 0; i < n; i++) {
			mutex->wait();
			cout << endl << "key waiting " << (i + 1) << endl;
			mutex->signal();

			e.wait();

			mutex->wait();
			cout << endl << "key continue " << (i + 1) << endl;
			mutex->signal();
		}

		mutex->wait();
		cout << endl << "key finished" << endl;
		mutex->signal();
	}

private:
	int n;

};

class Producer : public Thread {
public:
	static int stat_id;
	int id, sl;
	int b[10];
	Producer(int s) : Thread(), sl(s) {
		id = stat_id++;
		for (int i = 0; i < 10; i++) {
			b[i] = 10*stat_id + i;
		}
	}
	virtual ~Producer() {
		waitToComplete();
	}

	void put (int item, int* buffer) {
		full->wait();
		mutex->wait();
		buffer[i] = item;
		i = (i+1) % 10;
		cout << "Proizvodjac : " << id << " je ubacio " << item << " u bafer!" << endl;
		mutex->signal();
		empty->signal();

	}

	void run() {
		for (int i = 0; i < 10; i++) {
			put(b[i],buffer);
			sleep(sl);
		}
		mutex->wait();
		cout << endl << "Proizvodjac " << id << " zavrsio!" << endl;
		mutex->signal();
	}
};

int Producer :: stat_id = 0;

class Consumer : public Thread {
public:
	static int stat_id;
	int id, sl;
	Consumer(int s) : Thread(), sl(s) {
		id = stat_id++;
	}
	virtual ~Consumer() {
		waitToComplete();
	}

	void take (int* buffer) {
		empty->wait();
		mutex->wait();
		int item = buffer[j];
		j = (j+1) % 10;
		cout << "Potrosac : " << id << " je uzeo " << item << " iz bafera!" << endl;
		mutex->signal();
		full->signal();
	}

	void run() {
		for (int i = 0; i < 20; i++) {
			take(buffer);
			sleep(sl);
		}
		cout << endl << "Potorosac " << id << " zavrsio!" << endl;
	}
};

int Consumer :: stat_id = 0;



int userMainMine(int argc, char* argv[]) {
	/*mutex = new Semaphore(1);

	Znak* a = new Znak('a', 10, 4);
	Znak* b = new Znak('b', 15, 3);
	Znak* c = new Znak('c', 20, 2);
	Key* k = new Key(150);

	a->start();
	b->start();
	c->start();
	k->start();

	delete a;
	delete b;
	delete c;
	delete k;

	mutex->wait();
	cout << endl << "userMain finished" << endl;
	mutex->signal();

	delete mutex;
*/

/*
	mutex = new Semaphore(1);
	full = new Semaphore(10);
	empty = new Semaphore(0);

	Producer* p1 = new Producer(3);
	Producer* p2 = new Producer(5);
	Producer* p3 = new Producer(10);
	Producer* p4 = new Producer(1);
	Key* k = new Key(50);

	Consumer* c1 = new Consumer(4);
	Consumer* c2 = new Consumer(1);

	p1->start();
	p2->start();
	p3->start();
	p4->start();
	c1->start();
	c2->start();
	k->start();

	delete p1;
	delete p2;
	delete p3;
	delete p4;
	delete c1;
	delete c2;
	delete k;

	mutex->wait();
	cout << endl << "userMain finished" << endl;
	mutex->signal();

	delete mutex;
	delete full;
	delete empty;





	return 0;
}
*/

