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

pthread_t *toy_threads_id;
int toy_n_threads;

// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){
    // faz o cast para o tipo toy 
    toy_t *toy = (toy_t *) args;
    
    debug("[ON] - O brinquedo  [%d] foi ligado.\n", toy->id); // Altere para o id do brinquedo

    // Não finalizada, mas é a verificação se o brinquedo está cheio
    // Não terminei pq não sei como o cliente tem acesso ao brinquedo, a struct client tem nela o numero de brinquedos e um array de brinquedos
    // Tem que ver como juntar tudo isso para fazer o controle de cada brinquedo
    int cont = 0;
    while (1){
        if (cont >= toy->capacity){
            break;
        }
        cont++;
    }

    sleep(1);

    debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id); // Altere para o id do brinquedo

    pthread_exit(NULL);
}


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    // Inicializando uma thread para cada um dos brinquedos
    toy_threads_id = (pthread_t *) malloc(args->n * sizeof(pthread_t));
    toy_n_threads = args->n;
    for (int i = 0; i < args->n; i++) {
        // atribui o id da thread ao toy correspondente
        args->toys[i]->thread = toy_threads_id[i];
        pthread_create(&toy_threads_id[i], NULL, turn_on, (void *) args->toys[i]);
    }

}

// Desligando os brinquedos
void close_toys(){
    for (int i = 0; i < toy_n_threads; i++) 
        pthread_join(toy_threads_id[i], NULL);
    free(toy_threads_id);
}