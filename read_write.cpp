#include <iostream>
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

mutex io;
semaphore w(1);
std::queue<int> Buffer;
int i = 0;
atomic<int> rc{0};

void WriteBuffer() {
	int item = i++;
	printf("Write to Buffer: %d\n", item);
	fflush(stdout);
	usleep(35000);
	Buffer.push(item);
}

int ReadBuffer() {
	int item = Buffer.front();
	printf("Read from Buffer: %d\n", Buffer.front());
	fflush(stdout);
	usleep(20000);
	Buffer.pop();
	return item;
}

void *Write(void *B)
{
	while (true) 
	{
		w.P();

		WriteBuffer();
		sleep(1);

		w.V();
	}
}

void *Read(void *B) 
{
	while (true) 
	{
		io.lock();
		rc++;
		if (rc == 1) w.P();
		io.unlock();

		ReadBuffer();
		sleep(1);

		io.lock();
		rc--;
		if (rc == 0) w.V();
		io.unlock();
	}
}

int32_t main()
{
	srand(time(NULL));

	pthread_t Reader;
	pthread_t Writer;

	pthread_create(&Writer, NULL, Write, NULL);
	pthread_create(&Reader, NULL, Read, NULL);

	pthread_exit(NULL);
}
