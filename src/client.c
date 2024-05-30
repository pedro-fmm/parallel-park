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

pthread_mutex_t mutex_client; // Declarando o mutex usado para proteger o enqueue

// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){

    //Sua lógica aqui


    debug("[EXIT] - O turista saiu do parque.\n");

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

    pthread_mutex_lock(&mutex_client); // Protege o acesso a enqueue, para não ocorrer condição de corrida;
    enqueue(gate_queue, self->id);
    pthead_mutex_unlock(&mutex_client);
    // Sua lógica aqui.
    buy_coins(self);

    // Sua lógica aqui.
    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    // Inicializando uma thread para cada um dos clientes
    pthread_mutex_init(&mutex_client, NULL);
    pthread_t id_thread[args->n];
    for (int i = 0; i < args->n; i++) {
        pthread_create(&id_thread[i], NULL, enjoy, (void *) args->clients[i]);
        queue_enter(args->clients[i]);
    }
}

// Essa função deve finalizar os clientes
void close_gate(){
    pthread_mutex_destroy(&mutex_client); // Finaliza o mutex;
   //Sua lógica aqui
}
