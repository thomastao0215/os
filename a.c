#include<stdio.h>
#include<pthread.h>
char data[5];  // buffer
int size=0;  // inventory
int num=1;  // output line number
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full=PTHREAD_COND_INITIALIZER;
pthread_cond_t empty=PTHREAD_COND_INITIALIZER;
void print()
{
	int i;
	printf("\tline %2d:\t", num++);
	for(i=0;i<size;i++)
		printf("%c ", data[i]);
	printf("\n");
}
// producer thread
void* producer(void *p)
{
	char c;
	for(c='A'; c<='K'; c++) {
		pthread_mutex_lock(&mutex);  // mutex lock
		while(size==5)
			pthread_cond_wait(&full,&mutex);  // condition variable
		printf("PUSH %c\n", c);
		data[size]=c;
		usleep(10000);
		size++;
		print();
		pthread_cond_broadcast(&empty);
		pthread_mutex_unlock(&mutex);
		usleep(200000);
	}
}
// consumer thread
void* consumer(void *p)
{
	int i;
	for(i=0;i<20;i++) {
		pthread_mutex_lock(&mutex);
		while(size==0)
			pthread_cond_wait(&empty,&mutex);
		printf("POP %c\n",data[size-1]);
		size--;
		print();
		pthread_cond_broadcast(&full);
		pthread_mutex_unlock(&mutex);
		usleep(400000);
	}
}
int main()
{
	pthread_t id1,id2,id3;
	pthread_create(&id1,0,producer,0);	
	pthread_create(&id2,0,consumer,0);
	pthread_create(&id3,0,producer,0);
	pthread_join(id1,0);
	pthread_join(id2,0);
	pthread_join(id3,0);
	pthread_mutex_destroy(&mutex);
}
