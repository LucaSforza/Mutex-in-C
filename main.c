#include <stdio.h>

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

int main(int argc, char** argv) {
    printf("Hello World!\n");
    return 0;
}