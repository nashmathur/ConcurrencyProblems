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


semaphore TnP(0);
semaphore PnM(0);
semaphore MnT(0);
semaphore Smoked(1);

void *agent(void *lul)
{
	while (true) {

		srand(time(NULL));
		Smoked.P();
		int r = rand() % 3;
		switch( r ) {
			case 0: TnP.V();
				break;
			case 1: PnM.V();
				break;
			case 2: MnT.V();
				break;
		}
	}
}

void *Amar(void *x)
{
	while(true) {
		TnP.P();
		cout << "Amar is smoking!" << endl;
		sleep(1);
		Smoked.V();
	}
}

void *Akbar(void *x)
{
	while(true) {
		PnM.P();
		cout << "Akbar is smoking!" << endl;
		sleep(1);
		Smoked.V();
	}
}

void *Anthony(void *x)
{
	while(true) {
		MnT.P();
		cout << "Anthony is smoking!" << endl;
		sleep(1);
		Smoked.V();
	}
}


int main()
{
	pthread_t thread_id[4];
	pthread_create(&thread_id[0], NULL, Amar, NULL);
	pthread_create(&thread_id[1], NULL, Akbar, NULL);
	pthread_create(&thread_id[2], NULL, Anthony, NULL);
	pthread_create(&thread_id[3], NULL, agent, NULL);

	pthread_join(thread_id[0], NULL);
	pthread_join(thread_id[1], NULL);
	pthread_join(thread_id[3], NULL);
	pthread_join(thread_id[2], NULL);
}
