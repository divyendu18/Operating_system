#include <pthread.h>
#include<semaphore.h>
#include <stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>


sem_t wrt;
pthread_mutex_t mutex;
int count = 5;
int num_reader = 0;

void *reader(void *read_n)
{   
	sleep(5);
    
    pthread_mutex_lock(&mutex);
    
    if(num_reader == -1) {
        sem_wait(&wrt); 
    }
	num_reader++;
    pthread_mutex_unlock(&mutex);
    printf("Reader %d: read count is %d\n",(*((int *)read_n)),count);
    pthread_mutex_lock(&mutex);
    num_reader--;
    if(num_reader == 0) {
        sem_post(&wrt); 
    }
    pthread_mutex_unlock(&mutex);
}

void *writer(void *write_n)
{   
	int m=rand()%30;
	sleep(m);
    sem_wait(&wrt);
    count = count*2;
    printf("Writer changes: %d count to %d\n",(*((int *)write_n)),count);
    sem_post(&wrt);

}

int main()
{   
	int num_reader,num_writer;
	printf("Enter number of reader between 1 and 10\n");
	scanf("%d",&num_reader);
	printf("Enter number of writer between 1 and 10\n");
	scanf("%d",&num_writer);


    pthread_t read[num_reader],write[num_writer];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    int arr[10] = {1,2,3,4,5,6,7,8,9,10};

	for(int i = 0; i < num_writer; i++) {
		pthread_create(&write[i], NULL, (void *)writer,(void *)&arr[i]);
    }
	
	 for(int i = 0; i < num_reader; i++) {
        pthread_create(&read[i], NULL, (void *)reader,(void *)&arr[i]);
    }
	
	 
    for(int i = 0; i < num_reader; i++) {
        pthread_join(read[i], NULL);
    }
    for(int i = 0; i < num_writer; i++) 
	{
        pthread_join(write[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
    
}