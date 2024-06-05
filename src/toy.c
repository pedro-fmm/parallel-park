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

pthread_t *toy_thread_ids; // Alocando o vetor de threads de maneira pública, para conseguir rodar o close_toys
int toy_n_args; // Aloca o numero de brinquedos de maneira pública

#define TEMPO_BRINQUEDO 2 // Define o tempo de duração do brinquedo

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args) {

    toy_t *toy = (toy_t *) args;

    pthread_mutex_init(&toy->mutex_numero_clientes, NULL); // Inicializando o mutex que protege o numero de clientes
    pthread_cond_init(&toy->cond_toy, NULL); // Inicializando a condição para o controle de o brinquedo está com a capacidade máxima


    debug("[ON] - O brinquedo  [%d] foi ligado.\n", toy->id);
    sleep(1); // Espera os clientes chegarem no parque;

    while (n_pessoas_parque > 0) { // Enquanto tiver pessoas no parque, os brinquedos continuam ligados
        pthread_mutex_lock(&toy->mutex_numero_clientes); // Protege a leitura do numero de clientes no brinquedo
        if (toy->n_clientes_atual > 0) {
            debug("[BRINCAR] - Brinquedo [%d] está funcionando com [%d] clientes.\n", toy->id, toy->n_clientes_atual);
            toy->ocupado = 1; // Coloca o brinquedo como ocupado, para que não entre clientes enquanto ele está em ação
            pthread_mutex_unlock(&toy->mutex_numero_clientes);
            sleep(5);  // Simula a duração da brincadeira

            pthread_mutex_lock(&toy->mutex_numero_clientes);
            toy->n_clientes_atual = 0;  // Libera todos os clientes do brinquedo após a brincadeira
            toy->ocupado = 0;  // Coloca o brinquedo como disponivel
            pthread_cond_broadcast(&toy->cond_toy);  // Dá o sinal que o brinquedo está vazio 
            pthread_mutex_unlock(&toy->mutex_numero_clientes);
        } else {
            pthread_mutex_unlock(&toy->mutex_numero_clientes);
        }
        sleep(1);  // Pausa breve antes de verificar novamente
    }

    debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id);

    // Libera os mutexs usados
    pthread_mutex_destroy(&toy->mutex_numero_clientes);
    pthread_cond_destroy(&toy->cond_toy);

    pthread_exit(NULL);
}

// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    toy_n_args = args->n; // Pega o número de brinquedos
    NUM_TOYS = toy_n_args; // Passa esse número para o global
    toy_thread_ids = (pthread_t *) malloc(toy_n_args * sizeof(pthread_t)); // alocando o vetor de threads com tamanho dinâmico
    for (int i = 0; i < toy_n_args; i++) {
        pthread_create(&toy_thread_ids[i], NULL, turn_on, (void *) args->toys[i]); // inicializando cada bilheteria
        args->toys[i]->thread = toy_thread_ids[i]; // preenchendo o valor de thread
    }
}

// Desligando os brinquedos
void close_toys(){
    for (int i = 0; i < toy_n_args; i++)
        pthread_join(toy_thread_ids[i], NULL);  // Sicronizando a finalização das threads
    free(toy_thread_ids); // liberando a memoria do vetor de threads
}