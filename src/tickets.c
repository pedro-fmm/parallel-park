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

pthread_t *ticket_threads_id;
int ticket_n_threads;

pthread_mutex_t mutex_gate_queue_tickets;

// Thread que implementa uma bilheteria
void *sell(void *args){

    // faz o cast dos argumentos para ticket
    ticket_t *ticket = (ticket_t *) args;
    
    debug("[INFO] - Bilheteria %d Abriu!\n", ticket->id);

    while (1) {
        pthread_mutex_lock(&mutex_gate_queue_tickets);// da lock no mutex para evitar condição de corrida na hora de verificar e retirar da fila
        if (is_queue_empty(gate_queue)){
            pthread_mutex_unlock(&mutex_gate_queue_tickets);
            break;
        }
        // tira da fila um cliente
        int cliente = dequeue(gate_queue);
        debug("[INFO] - Turista %d foi atendido na bilheteria %d!\n", cliente, ticket->id);
        pthread_mutex_unlock(&mutex_gate_queue_tickets);
    }

    debug("[INFO] - Bilheteria %d fechou!\n", ticket->id);

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){
    pthread_mutex_init(&mutex_gate_queue_tickets, NULL);
    // Inicializando uma thread para cada uma das bilheterias
    ticket_threads_id = (pthread_t *) malloc(args->n * sizeof(pthread_t));
    ticket_n_threads = args->n;
    for (int i = 0; i < args->n; i++) {
        // atribui o id da thread ao ticket correspondente
        args->tickets[i]->thread = ticket_threads_id[i];
        pthread_create(&ticket_threads_id[i], NULL, sell, (void *) args->tickets[i]);
    }
}

// Essa função deve finalizar a bilheteria
void close_tickets(){
    pthread_mutex_destroy(&mutex_gate_queue_tickets);
    for (int i = 0; i < ticket_n_threads; i++) 
        pthread_join(ticket_threads_id[i], NULL);
    free(ticket_threads_id);
}