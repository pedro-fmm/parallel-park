/**
 * Esse arquivo tem como objetivo a implementação do cliente em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "client.h"
#include "queue.h"
#include "shared.h"

pthread_t *client_threads_id;
int client_n_threads;

pthread_mutex_t mutex_gate_queue_client; // Declarando o mutex usado para proteger o enqueue

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){

    client_t *client = (client_t *) arg;
    pthread_mutex_init(&client->mutex, NULL);

    pthread_mutex_lock(&client->mutex);
    queue_enter(client);
    // pthread_mutex_unlock(&client->mutex);

    debug("[ENTROU] - O turista %d entrou no parque.\n", client->id);

    debug("[EXIT] - O turista %d saiu do parque.\n", client->id);

    pthread_exit(NULL);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    self->coins = rand() % (MAX_COINS - MIN_COINS + 1) + MIN_COINS;
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){
    // Sua lógica aqui
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){
    // Sua lógica aqui.
    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);

    pthread_mutex_lock(&mutex_gate_queue_client); // Protege o acesso a enqueue, para não ocorrer condição de corrida;
    enqueue(gate_queue, self->id);
    pthread_mutex_unlock(&mutex_gate_queue_client);

    // Sua lógica aqui.
    buy_coins(self);

    // Sua lógica aqui.
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    // Inicializando uma thread para cada um dos clientes
    pthread_mutex_init(&mutex_gate_queue_client, NULL);
    client_threads_id = (pthread_t *) malloc(args->n * sizeof(pthread_t));
    client_n_threads = args->n;
    for (int i = 0; i < args->n; i++) {
        pthread_create(&client_threads_id[i], NULL, enjoy, (void *) args->clients[i]);
    }
}

// Essa função deve finalizar os clientes
void close_gate(){
    pthread_mutex_destroy(&mutex_gate_queue_client); // Finaliza o mutex;
    for (int i = 0; i < client_n_threads; i++) 
        pthread_join(client_threads_id[i], NULL);
    free(client_threads_id);
}
