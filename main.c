#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
due processi condividono un buffer comune (di dimensioni fisse) uno dei due che chiameremo produttore mette informazioni nel buffer dell'altro
il consumatore invece le preleva.
Quando un produttore vuole mettere un nuovo, ma esso è già pieno.
La soluzione è porre in sleep il produttore e risvegliarlo quando il consumatore ha rimosso uno o più elementi.
Se il consumatore vuole rimuovere un elemento del buffer e vede che il buffer è vuoto si mette in sleep finché il produttore inserisce qualcosa nel buffer e lo risveglia
Per tenere conto di quanti elementi ci stanno nel buffer uso una variabile 'count'.
Se N è il numero massimo di elementi nel buffer il produttore controlla prima se count è uguale ad N. Se lo è entra in sleep altrimenti aggiunge elementi.
Il consumatore prima controlla se count è uguale a 0, se si allora va a dormire altrimenti aggiunge elementi.
Ognuno dei processi controlla se l'altro va risvegliato
*/

#define N 100
#define ITER 100000

typedef struct {
    u_int64_t count;
    u_int64_t* ptr;
} Buffer;

Buffer buffer;

pthread_t producer_id,consumer_id;

pthread_mutex_t mutex;

pthread_cond_t condc,condp;

void new_buffer(void) {
    buffer.count = 0;
    buffer.ptr = malloc(sizeof(long)*N);
}

void realese_buffer(void) {
    free(buffer.ptr);
}

void insert_item(u_int64_t item) {
    buffer.ptr[buffer.count] = item;
}

u_int64_t produce_item(void) {
    //TODO: creare qualcosa di leggermente più sofisticato tipo creazione di numeri casuali
    return buffer.count;
}

u_int64_t read_item(void) {
    return buffer.ptr[buffer.count];
}

void *producer_deamon(void* arg) {
    u_int64_t item;

    for(int i = 0;i < ITER; i++) {
        item = produce_item();
        pthread_mutex_lock(&mutex);
        while (buffer.count == N)
            pthread_cond_wait(&condp,&mutex);
        insert_item(item);
        buffer.count++;
        printf("Write: %llu\n",item);
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&mutex);
        pthread_yield_np();
    }
}

void *consumer_deamon(void* arg) {
    u_int64_t item;

    for(int i = 0;i < ITER; i++) {
        pthread_mutex_lock(&mutex);
        while (buffer.count == 0)
            pthread_cond_wait(&condc,&mutex);
        item = read_item();
        buffer.count--;
        printf("Read: %llu\n",item);
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&mutex);
        pthread_yield_np();
    }
}

int main(void) {
    new_buffer();
    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&condc,0);
    pthread_cond_init(&condp,0);
    int err_consumer,err_producer;
    err_consumer = pthread_create(&consumer_id, NULL,consumer_deamon,NULL);
    err_producer = pthread_create(&producer_id, NULL,producer_deamon,NULL);
    if (err_consumer | err_producer) {
        fprintf(stderr, "Fatal error: could not create threads for consumer or producer");
    } else {
        printf("Thread id consumer: %llu\n",(u_int64_t)consumer_id);
        printf("Thread id producer: %llu\n",(u_int64_t)producer_id);
    }
    pthread_join(consumer_id,NULL);
    pthread_join(producer_id,NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    realese_buffer();
    return 0;
}