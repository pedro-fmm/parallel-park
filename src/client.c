/**
 * Esse arquivo tem como objetivo a implementação do cliente em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "defs.h"
#include "client.h"
#include "queue.h"
#include "shared.h"

pthread_t *client_thread_ids; // alocando o vetor de threads de maneira pública, para conseguir rodar o close_gate
int client_n_args;  // Alocando o numero de clientes de forma global

pthread_mutex_t mutex_gate_queue; // mutex para proteger queue
pthread_mutex_t mutex_qnt_turistas; // mutex para proteger n_pessoas_parque


// Thread que implementa o fluxo do cliente no parque.
void *enjoy(void *arg){

    client_t *client = (client_t *) arg; // fazendo cast do client

    pthread_mutex_init(&client->mutex, NULL); // Inicializando o mutex de cada cliente

    queue_enter(client); // Turista entra na fila, e compra as moedas



    debug("[ENTER] - O turista [%d] entrou no parque.\n", client->id);

    // Enquanto cliente tem moeda, escolher um brinquedo e brinca nele
    while (client->coins > 0) {
        // Cliente escolhe um brinquedo
        int toy_id = (rand() % NUM_TOYS) ;
        toy_t *toy = client->toys[toy_id];

        //Protege o acesso a n_clientes_atual
        pthread_mutex_lock(&toy->mutex_numero_clientes);

        // Aguarda o brinquedo terminar se ele já estiver em funcionamento, e aguarda ele esvaziar se tiver cheio
        while (toy->ocupado || toy->n_clientes_atual >= toy->capacity) {
            pthread_cond_wait(&toy->cond_toy, &toy->mutex_numero_clientes);
        }

        // Atualiza o numero de clientes no brinquedo
        toy->n_clientes_atual++;
        debug("[PLAY] - Turista [%d] entrou no brinquedo [%d].\n", client->id, toy_id + 1);
        pthread_mutex_unlock(&toy->mutex_numero_clientes);

        // Espera o tempo do turista terminar de brincar no brinquedo
        sleep(2);

        // Sai do brinquedo e atualiza o numero de clientes
        pthread_mutex_lock(&toy->mutex_numero_clientes);
        debug("[PLAY] - Turista [%d] saiu do brinquedo [%d].\n", client->id, toy_id + 1);
        toy->n_clientes_atual--;
        pthread_mutex_unlock(&toy->mutex_numero_clientes);

        // Diminui a quantidade de moedas do cliente
        client->coins--;



    }
    n_pessoas_parque--; // Decrementa o numero de pessoas no parque;

    debug("[EXIT] - O turista [%d] saiu do parque.\n", client->id);
    pthread_exit(NULL);
}


// Funcao onde o cliente compra as moedas para usar os brinquedos
void buy_coins(client_t *self){
    self->coins = rand() % (MAX_COINS - MIN_COINS + 1) + MIN_COINS;
}

// Função onde o cliente espera a liberacao da bilheteria para adentrar ao parque.
void wait_ticket(client_t *self){
    // Utilizamos a thread condicional, para que o turista só entre no parque depois que for atendido pela bilheteria
    pthread_mutex_lock(&mutex_client_id_global);
    while (self->id != client_id_global) {
        pthread_cond_wait(&cond_client_id, &mutex_client_id_global);
    }
    pthread_mutex_unlock(&mutex_client_id_global);
}

// Funcao onde o cliente entra na fila da bilheteria
void queue_enter(client_t *self){
    debug("[WAITING] - Turista [%d] entrou na fila do portao principal\n", self->id);

    pthread_mutex_lock(&mutex_gate_queue); // protege o acesso a fila
    enqueue(gate_queue, self->id); // enfileira o cliente para a bilheteria
    pthread_mutex_unlock(&mutex_gate_queue); 

    // Espera o cliente ser atendido na bilheteria para comprar as moedas
    wait_ticket(self);
    buy_coins(self);

    debug("[CASH] - Turista [%d] comprou [%d] moedas.\n", self->id, self->coins);
}

// Essa função recebe como argumento informações sobre o cliente e deve iniciar os clientes.
void open_gate(client_args *args){
    pthread_mutex_init(&mutex_gate_queue, NULL);
    pthread_mutex_init(&mutex_qnt_turistas, NULL);
    client_n_args = args->n; // Pega o numero inicial de pessoas no parque;
    n_pessoas_parque = client_n_args; // Passa esse numero para a variavel global
    client_thread_ids = (pthread_t *) malloc(client_n_args * sizeof(pthread_t)); // alocando o vetor de threads com tamanho dinâmico
    for (int i = 0; i < client_n_args; i++) {
        pthread_create(&client_thread_ids[i], NULL, enjoy, (void *) args->clients[i]); // inicializando cada cliente
    }
}

// Essa função deve finalizar os clientes
void close_gate(){
    for (int i = 0; i < client_n_args; i++)
        pthread_join(client_thread_ids[i], NULL);  // Dando join em todas threads
    free(client_thread_ids); // Liberando a memoria do vetor de threads
    pthread_mutex_destroy(&mutex_gate_queue); // Liberando os mutexs usados
    pthread_mutex_destroy(&mutex_qnt_turistas);

}