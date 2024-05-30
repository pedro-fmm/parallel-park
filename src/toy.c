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

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){

    toy_t *toy = (toy_t *) args;

    debug("[ON] - O brinquedo  [%d] foi ligado.\n", toy->id); // Altere para o id do brinquedo

    // sleep(1);

    debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id); // Altere para o id do brinquedo

    pthread_exit(NULL);
}


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    toy_n_args = args->n;
    NUM_TOYS = toy_n_args;
    toy_thread_ids = (pthread_t *) malloc(toy_n_args * sizeof(pthread_t)); // alocando o vetor de threads com tamanho dinâmico
    for (int i = 0; i < toy_n_args; i++) {
        pthread_create(&toy_thread_ids[i], NULL, turn_on, (void *) args->toys[i]); // inicializando cada bilheteria
        args->toys[i]->thread = toy_thread_ids[i]; // preenchendo o valor de thread
    }
}

// Desligando os brinquedos
void close_toys(){
    for (int i = 0; i < toy_n_args; i++)
        pthread_join(toy_thread_ids[i], NULL);  // dando join em todas threads
    free(toy_thread_ids); // liberando a memoria do vetor de threads
}