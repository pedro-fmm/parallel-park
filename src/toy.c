/**
 * Esse arquivo tem como objetivo a implementação de um brinquedo em um parque.
 * Você pode criar novas funções dentro desse arquivo caso julgue necessário.
 * ATENÇÃO: NÃO APAGUE OU EDITE O NOME OU O(S) ARGUMENTO(S) DAS FUNÇÕES FORNECIDAS
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "toy.h"
#include "shared.h"

pthread_t *toy_thread_ids; // alocando o vetor de threads de maneira pública, para conseguir rodar o close_toys
int toy_n_args;

#define TEMPO_BRINQUEDO 10
// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args) {

    toy_t *toy = (toy_t *) args;

    toy->thread = pthread_self(); // preenchendo a thread

    pthread_mutex_init(&toy->mutex_numero_clientes, NULL); // Inicializando o mutex do brinquedo
    pthread_cond_init(&toy->cond_toy, NULL); // Inicializando a condição do brinquedo

    debug("[ON] - O brinquedo  [%d] foi ligado.\n", toy->id);

    while (n_pessoas_parque > 0) { // Enquanto tiver pessoas no parque, os brinquedos continuam ligados

        debug("[BRINCAR] - Brinquedo [%d] está aguardando clientes.\n", toy->id);
        sleep(2); // aguardar clientes

        
        pthread_mutex_lock(&toy->mutex_numero_clientes); // Protege a leitura do numero de clientes no brinquedo
        if (toy->n_clientes_atual > 0) {
            debug("[BRINCAR] - Brinquedo [%d] está funcionando com [%d] clientes.\n", toy->id, toy->n_clientes_atual);
            pthread_mutex_unlock(&toy->mutex_numero_clientes);
            sleep(2);  // Simula a duração da brincadeira
            pthread_mutex_lock(&toy->mutex_numero_clientes);
            toy->n_clientes_atual = 0;  // Libera todos os clientes do brinquedo após a brincadeira
            pthread_cond_broadcast(&toy->cond_toy);
            pthread_mutex_unlock(&toy->mutex_numero_clientes);
        } else {
            pthread_mutex_unlock(&toy->mutex_numero_clientes);
        }
        sleep(1);  // Pausa breve antes de verificar novamente
    }

    debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id);

    pthread_mutex_destroy(&toy->mutex_numero_clientes);
    pthread_cond_destroy(&toy->cond_toy);
    pthread_exit(NULL);
}


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    toy_n_args = args->n;
    NUM_TOYS = toy_n_args;
    toy_thread_ids = (pthread_t *) malloc(toy_n_args * sizeof(pthread_t)); // alocando o vetor de threads com tamanho dinâmico
    for (int i = 0; i < toy_n_args; i++)
        pthread_create(&toy_thread_ids[i], NULL, turn_on, (void *) args->toys[i]); // inicializando cada brinquedo
}

// Desligando os brinquedos
void close_toys(){
    for (int i = 0; i < toy_n_args; i++)
        pthread_join(toy_thread_ids[i], NULL);  // Sicronizando a finalização das threads
    free(toy_thread_ids); // liberando a memoria do vetor de threads
}