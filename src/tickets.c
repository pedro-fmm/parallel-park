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

pthread_t *ticket_thread_ids; // alocando o vetor de threads de maneira pública, para conseguir rodar o close_tickets
int ticket_n_args;

pthread_mutex_t mutex_gate_queue_tickets; // mutex para proteger a queue
pthread_cond_t cond_gate_open_tickets; // condicional de liberar acesso ao portao

// Thread que implementa uma bilheteria
void *sell(void *args){

    ticket_t *ticket = (ticket_t *) args; // cast do ticket 

    debug("[INFO] - Bilheteria [%d] abriu!\n", ticket->id);

    // mantem o loop rodando até um break
    while (1) {

        pthread_mutex_lock(&mutex_gate_queue_tickets); // da lock no mutex para evitar condição de corrida na hora de verificar e retirar da fila
        if (is_queue_empty(gate_queue)){ // verifica se tem cliente na fila de atendimento
            pthread_mutex_unlock(&mutex_gate_queue_tickets);
            break; // quebra o while se a fila está vazia
        }

        int cliente_id = dequeue(gate_queue);
        debug("[INFO] - Turista %d foi atendido na bilheteria %d!\n", cliente_id, ticket->id);
        pthread_mutex_unlock(&mutex_gate_queue_tickets);

        // client_t *client = find_client_by_id(cliente_id);
        // pthread_mutex_lock(&client->mutex);
        // client->ticket = 1;
        // pthread_cond_signal(&cond_gate_open_tickets);
        // pthread_mutex_unlock(&client->mutex);
    }

    debug("[INFO] - Bilheteria [%d] fechou!\n", ticket->id);

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){
    pthread_mutex_init(&mutex_gate_queue_tickets, NULL);
    pthread_cond_init(&cond_gate_open_tickets, NULL);
    ticket_n_args = args->n;
    ticket_thread_ids = (pthread_t *) malloc(ticket_n_args * sizeof(pthread_t)); // alocando o vetor de threads com tamanho dinâmico
    for (int i = 0; i < ticket_n_args; i++) {
        pthread_create(&ticket_thread_ids[i], NULL, sell, (void *) args->tickets[i]); // inicializando cada bilheteria
        args->tickets[i]->thread = ticket_thread_ids[i]; // preenchendo o valor de thread
    }
}

// Essa função deve finalizar a bilheteria
void close_tickets(){
    for (int i = 0; i < ticket_n_args; i++)
        pthread_join(ticket_thread_ids[i], NULL);  // dando join em todas threads
    free(ticket_thread_ids); // liberando a memoria do vetor de threads
    pthread_mutex_destroy(&mutex_gate_queue_tickets);
    pthread_cond_destroy(&cond_gate_open_tickets);    
}