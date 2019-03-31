#include <bits/stdc++.h>
#include <queue>
#include <atomic>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

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

semaphore emptyCount(5);
semaphore fillCount(0);
int i = 0;
mutex buffer_mutex;
std::queue<int> buffer;

void ProduceInBuffer() 
{
	srand(time(NULL));
	int y = rand();
	cout << "Produced Data: " << y << endl;
	usleep(30000);
	buffer.push(y);
}

void ConsumeFromBuffer() 
{
	cout << "Consumed Data: " << buffer.front() << " " << buffer.size() << " " << endl;
	usleep(20000);
	buffer.pop();
}

void *produce(void *id) 
{
	while (true) 
	{
		emptyCount.P();
		buffer_mutex.lock();

		ProduceInBuffer();
		sleep(1);

		buffer_mutex.unlock();
		fillCount.V();
	}
}

void *consume(void *id) 
{
	while (true) 
	{
		fillCount.P();
		buffer_mutex.lock();

		ConsumeFromBuffer();
		sleep(1);

		buffer_mutex.unlock();    
		emptyCount.V();
	}
}

int32_t main()
{
	srand(time(NULL));

	pthread_t producer[3];
	pthread_t consumer[2];

	pthread_create(&producer[0], NULL, produce, NULL);
	pthread_create(&producer[1], NULL, produce, NULL);
	pthread_create(&producer[2], NULL, produce, NULL);
	pthread_create(&consumer[0], NULL, consume, NULL);
	pthread_create(&consumer[1], NULL, consume, NULL);

	pthread_exit(NULL);
}
