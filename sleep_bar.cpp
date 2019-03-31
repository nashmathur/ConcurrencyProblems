#include <bits/stdc++.h>
#include <queue>
#include <atomic>
#include <unistd.h>
#include <mutex>
#include <pthread.h>
using namespace std;

class semaphore {

	private:
		atomic<int> s;
		mutex m;

	public:

		semaphore(int init): s(init) {}

		void P() 
		{
			while (true) {
				while (s <= 0);
				m.lock();
				if (s <= 0) {
					m.unlock();
					continue;
				}
				s--;
				m.unlock();
				break;
			} 
		}

		void V() {
			s++;
		}
};

semaphore AwakeBarber(0);
semaphore WaitOnSeat(1);
semaphore Cutting(0);

int EmptySeats = 5;

void *Barber(void *b) {
	while(true) {
		Cutting.P();
		WaitOnSeat.P();

		cout << "Cutting now. Empty seats: " << EmptySeats << endl;
		EmptySeats += 1;
		sleep(1);

		AwakeBarber.V();
		WaitOnSeat.V();
	}
}

void *Customer(void *b) {
	while(true) {
		WaitOnSeat.P();

		if (EmptySeats > 0) {
			cout << "Customer arrives. Empty seats: " << EmptySeats << endl;
			EmptySeats--;

			Cutting.V();
			WaitOnSeat.V();
			AwakeBarber.P();
		}
		else {
			WaitOnSeat.V();
		}
	}
}

int main() 
{ 
	pthread_t threadIndex[5];

	pthread_create(&threadIndex[0], NULL, Barber, NULL);
	pthread_create(&threadIndex[1], NULL, Customer, NULL);
	pthread_create(&threadIndex[2], NULL, Customer, NULL);
	pthread_create(&threadIndex[3], NULL, Customer, NULL);
	pthread_create(&threadIndex[4], NULL, Customer, NULL);

	pthread_join(threadIndex[0], NULL);
	pthread_join(threadIndex[1], NULL);
	pthread_join(threadIndex[2], NULL);
	pthread_join(threadIndex[3], NULL);
	pthread_join(threadIndex[4], NULL);
}
