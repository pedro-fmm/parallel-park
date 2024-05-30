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

pthread_t *client_thread_ids; // alocando o vetor de threads de maneira pública, para conseguir rodar o close_gate
int client_n_args;

pthread_mutex_t mutex_gate_queue; // mutex para proteger queue
pthread_cond_t cond_gate_open; // condição para liberar entrada ao parque/portao


// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){

    client_t *client = (client_t *) arg; // fazendo cast do client

    pthread_mutex_init(&client->mutex, NULL); // travando acesso ao cliente

    queue_enter(client);
    wait_client(client);

    debug("[EXIT] - O turista [%d] entrou no parque.\n", client->id);

    // enquanto cliente tem moeda, escolher um brinquedo e andar
    while (client->coins > 0) {
        // int toy_id = rand() % NUM_TOYS;
        // debug("[BRINCAR] - Turista [%d] está na fila do brinquedo [%d].\n", client->id, toy_id);
        // toy_use(toy_id, client);
        client->coins--;
    }

    debug("[EXIT] - O turista [%d] saiu do parque.\n", client->id);
    pthread_exit(NULL);
}

// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    self->coins = rand() % (MAX_COINS - MIN_COINS + 1) + MIN_COINS;
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_client(client_t *self){
    pthread_mutex_lock(&self->mutex);
    // while (!self->ticket) {
    //     pthread_cond_wait(&cond_gate_open, &self->mutex);
    // }
    pthread_mutex_unlock(&self->mutex);
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){
    // Sua lógica aqui.
    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);

    pthread_mutex_lock(&mutex_gate_queue); // protege o acesso a fila
    enqueue(gate_queue, self->id); // enfileira o cliente para a bilheteria
    pthread_mutex_unlock(&mutex_gate_queue); 

    // Sua lógica aqui.
    buy_coins(self);

    // Sua lógica aqui.
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    pthread_mutex_init(&mutex_gate_queue, NULL);
    pthread_cond_init(&cond_gate_open, NULL);
    client_n_args = args->n;
    client_thread_ids = (pthread_t *) malloc(client_n_args * sizeof(pthread_t)); // alocando o vetor de threads com tamanho dinâmico
    for (int i = 0; i < client_n_args; i++) {
        pthread_create(&client_thread_ids[i], NULL, enjoy, (void *) args->clients[i]); // inicializando cada bilheteria
    }
}

// Essa função deve finalizar os clientes
void close_gate(){
    for (int i = 0; i < client_n_args; i++)
        pthread_join(client_thread_ids[i], NULL);  // dando join em todas threads
    free(client_thread_ids); // liberando a memoria do vetor de threads
    pthread_mutex_destroy(&mutex_gate_queue);
    pthread_cond_destroy(&cond_gate_open);
}
