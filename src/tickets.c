/**
 * Esse arquivo tem como objetivo a implementação da bilheteria em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS.
*/
#include <stdio.h>
#include <stdlib.h>
#include <tickets.h>
#include <unistd.h>

#include <queue.h>
#include "shared.h"

pthread_mutex_t mutex_gate_queue;

// Thread que implementa uma bilheteria
void *sell(void *args){

    // faz o cast dos argumentos para ticket
    ticket_t *ticket = (ticket_t *) args;
    
    debug("[INFO] - Bilheteria Abriu!\n");

    // da lock no mutex e testa se a fila está vazia
    pthread_mutex_lock(&mutex_gate_queue);
    while (!is_queue_empty(gate_queue)) {
        // tira da fila um cliente
        int cliente = dequeue(gate_queue);
        debug("[INFO] - Turista %d foi atendido na bilheteria %d!\n", cliente, ticket->id);
    }
    // libera o mutex fora do while, para cobrir o caso da fila estar vazia ou não
    pthread_mutex_unlock(&mutex_gate_queue);

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){
    pthread_mutex_init(&mutex_gate_queue, NULL);
    // Inicializando uma thread para cada uma das bilheterias
    pthread_t id_thread[args->n];
    for (int i = 0; i < args->n; i++) {
        // atribui o id da thread ao ticket correspondente
        args->tickets[i]->thread = id_thread[i];
        pthread_create(&id_thread[i], NULL, sell, (void *) args->tickets[i]);
    }
}

// Essa função deve finalizar a bilheteria
void close_tickets(){
    pthread_mutex_destroy(&mutex_gate_queue);
    //Sua lógica aqui
}