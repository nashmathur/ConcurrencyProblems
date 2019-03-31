#include <bits/stdc++.h>
#include <queue>
#include <atomic>
#include <unistd.h>
#include <pthread.h>
using namespace std;

class semaphore {
	private:
		mutex m;
		atomic<int> s;
	public:
		semaphore(int init = 1): s(init) {}
		void P() {
			while (1) {
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

std::vector<semaphore> S(5);
mutex mut;

int indexes[5] = {0, 1, 2, 3, 4}; 

void* dine(void* num) 
{ 
	int* i = (int *)num; 

	while (true) {

		srand(time(NULL));
		mut.lock();

		S[*i].P();
		S[(*i + 1) % 5].P();

		mut.unlock();

		cout << "Philosopher " << *i << " is now eating." << endl ;
		sleep(2);

		S[*i].V();
		S[(*i + 1) % 5].V();

		usleep((rand() % 1000) * 1000);
	} 
}

int main() 
{ 
	int i;
	pthread_t thread_index[3]; 

	for (i = 0; i < 3; i++) { 
		pthread_create(&thread_index[i], NULL, dine, &indexes[i]); 
	}

	for (i = 0; i < 3; i++) {
		pthread_join(thread_index[i], NULL);
	}	 
}
