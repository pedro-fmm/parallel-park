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

pthread_t *ticket_thread_ids; // Alocando o vetor de threads de maneira pública, para conseguir rodar o close_tickets
int ticket_n_args; // Alocando o numero de tickets de maneira publica

pthread_mutex_t mutex_gate_queue_tickets; // mutex para proteger a queue


// Thread que implementa uma bilheteria
void *sell(void *args){

    ticket_t *ticket = (ticket_t *) args; // cast do ticket 

    debug("[INFO] - Bilheteria [%d] abriu!\n", ticket->id); // Cada funcionário é representado por um número de bilheteria

    // mantem o loop rodando até um break
    while (TRUE) {

        pthread_mutex_lock(&mutex_gate_queue_tickets); // Da lock no mutex para evitar condição de corrida na hora de verificar e retirar da fila
        if (is_queue_empty(gate_queue) && clientes_chegaram){ // verifica se tem cliente na fila de atendimento e verifica se já teve cliente que chegou, para evitar o caso dessa condição acontecer antes de algum cliente ser colocado na fila
            pthread_mutex_unlock(&mutex_gate_queue_tickets);
            break; // quebra o while se a fila está vazia
        }

        int client_id = dequeue(gate_queue); // Repassa para a variavel local, qual cliente foi atendido
        pthread_mutex_unlock(&mutex_gate_queue_tickets);

        pthread_mutex_lock(&mutex_client_id_global); // protege o acesso à variável global
        client_id_global = client_id; // atualiza a variável global
        pthread_cond_broadcast(&cond_client_id); // Sinaliza quando um cliente é atendido
        pthread_mutex_unlock(&mutex_client_id_global);

        debug("[INFO] - Turista [%d] foi atendido na bilheteria [%d]!\n", client_id_global, ticket->id);
        sleep(1);
    }

    pthread_mutex_lock(&mutex_client_id_global);
    pthread_cond_broadcast(&cond_client_id); // Assegura que todos os clientes saiam da espera ao final da bilheteria
    pthread_mutex_unlock(&mutex_client_id_global);
    
    debug("[INFO] - Bilheteria [%d] fechou!\n", ticket->id);

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações sobre a bilheteria e deve iniciar os atendentes.
void open_tickets(tickets_args *args){
    pthread_mutex_init(&mutex_gate_queue_tickets, NULL); // inicializa o mutex para proteger a fila
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
        pthread_join(ticket_thread_ids[i], NULL);  // Dando join em todas threads
    free(ticket_thread_ids); // Liberando a memoria do vetor de threads
    pthread_mutex_destroy(&mutex_gate_queue_tickets); // Libera o mutex usado
}