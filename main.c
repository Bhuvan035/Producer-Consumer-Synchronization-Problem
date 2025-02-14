// Bhuvan Chandra - 85579911; Ananya Jajoo - 23181947
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PRODUCERS 16
#define MAX_CONSUMERS 16
#define MAX_BUFFER_SIZE 10

int number_of_producers;
int number_of_consumers;
int items_by_eachproducer;
int buffer_size;
int delay;
int *buffer;
int producercount = 0;
int consumercount = 0;
volatile int produceditemnumber = 0;
volatile int consumeditemnumber = 0;

pthread_mutex_t mutexbuffer = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
  int producer_number = *((int *)arg);

  for (int i = 0; i < items_by_eachproducer; i++) {
    pthread_mutex_lock(&mutexbuffer);

    while (producercount == consumercount &&
           produceditemnumber != consumeditemnumber) {
      pthread_cond_wait(&empty, &mutexbuffer);
    }
    int produced_data = produceditemnumber++;
    buffer[producercount] = produced_data;
    producercount = (producercount + 1) % buffer_size;
    printf("producer_%d produced item %d\n", producer_number, produced_data);
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutexbuffer);
    if (delay == 1) {
      usleep(500000);
    }
  }
  pthread_exit(NULL);
  return NULL;
}

void *consumer(void *arg) {
  int consumer_number = *((int *)arg);
  for (int i = 0;
       i < items_by_eachproducer * number_of_producers / number_of_consumers;
       i++) {
    pthread_mutex_lock(&mutexbuffer);
    while (consumercount == producercount &&
           produceditemnumber == consumeditemnumber) {
      pthread_cond_wait(&full, &mutexbuffer);
    }

    int consumed_data = buffer[consumercount];
    consumercount = (consumercount + 1) % buffer_size;
    printf("consumer_%d consumed item %d\n", consumer_number, consumed_data);
    consumeditemnumber++;

    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutexbuffer);

    if (delay == 0) {
      usleep(500000);
    }
  }
  pthread_exit(NULL);
  return NULL;
}

int main(int argc, char *argv[]) {
  // pthread_mutex_init(&mutexbuffer,NULL);
  //  pthread_cond_init(&empty,0,10);
  //  pthread_cond_init(&full,0,0);
  if (argc != 6) {
    printf("Invalid syntax");
    fflush(stdout);
    exit(0);
  }
  number_of_producers = atoi(argv[1]);
  number_of_consumers = atoi(argv[2]);
  items_by_eachproducer = atoi(argv[3]);
  buffer_size = atoi(argv[4]);
  delay = atoi(argv[5]);
  // printf("%d %d %d %d %d",
  // number_of_producers,number_of_consumers,items_by_eachproducer,buffer_size,delay);
  // fflush(stdout);

  if (number_of_consumers >= (number_of_producers * items_by_eachproducer)) {
    printf("Invalid input C>=p*i");
    fflush(stdout);
    exit(0);
  }
  if (buffer_size > 10) {
    printf("Invalid input, the buffer size cannot be greater than 10");
    exit(0);
  }

  if (number_of_consumers > MAX_CONSUMERS) {
    printf("Invalid input of consumers >16");
    exit(0);
  }
  if (number_of_producers > MAX_PRODUCERS) {
    printf("Invalid input of producers >16");
    exit(0);
  }
  buffer = (int *)malloc(buffer_size * sizeof(int));
  // buffer[buffer_size];

  pthread_t producer_threads[number_of_producers];
  int p_numbers[number_of_producers];
  pthread_t consumer_threads[number_of_consumers];
  int c_numbers[number_of_consumers];

  for (int i = 0; i < number_of_producers; ++i) {
    p_numbers[i] = i;
    pthread_create(&producer_threads[i], NULL, producer, &p_numbers[i]);
  }

  for (int i = 0; i < number_of_consumers; ++i) {
    c_numbers[i] = i;
    pthread_create(&consumer_threads[i], NULL, consumer, &c_numbers[i]);
  }

  for (int i = 0; i < number_of_producers; ++i) {
    pthread_join(producer_threads[i], NULL);
  }

  for (int i = 0; i < number_of_consumers; ++i) {
    pthread_join(consumer_threads[i], NULL);
  }
  free(buffer);
  // pthread_mutex_destroy(&mutexbuffer);
  //  sem_destroy(&empty);
  //  sem_destroy(&full);
  return 0;
}