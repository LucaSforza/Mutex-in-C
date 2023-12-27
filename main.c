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

typedef struct {
    u_int64_t count;
    u_int64_t* ptr;
} Buffer;

Buffer buffer;

pthread_t producer_id,consumer_id;

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

u_int64_t remove_item(void) {
    return buffer.ptr[buffer.count];
}

void sleep() {
    //TODO
}

void wakeup(pthread_t thread_id) {
    //TODO
}

void *producer_deamon(void* arg) {
    u_int64_t item;

    for(;;) {
        item = produce_item();
        if (buffer.count == N)
            sleep();
        insert_item(item);
        buffer.count++;
        if(buffer.count == 1)
            wakeup(consumer_id);
        pthread_yield_np();
    }
}

void *consumer_deamon(void* arg) {
    u_int64_t item;

    for(;;) {
        if (buffer.count == 0)
            sleep();
        item = remove_item();
        buffer.count--;
        if (buffer.count == N - 1)
            wakeup(producer_id);
        printf("%llu\n",item);
        pthread_yield_np();
    }
}

int main(void) {
    new_buffer();
    int err_consumer,err_producer;
    err_consumer = pthread_create(&consumer_id, NULL,consumer_deamon,NULL);
    err_producer = pthread_create(&producer_id, NULL,producer_deamon,NULL);
    if (err_consumer | err_producer) {
        fprintf(stderr, "Fatal error: could not create threads for consumer or producer");
    } else {
        printf("Thread id consumer: %llu",(u_int64_t)consumer_id);
        printf("Thread id producer: %llu",(u_int64_t)producer_id);
    }
    realese_buffer();
    return 0;
}