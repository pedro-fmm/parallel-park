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


// Thread que o brinquedo vai usar durante toda a simulacao do sistema
void *turn_on(void *args){
    // faz o cast para o tipo toy 
    toy_t *toy = (toy_t *) args;

    debug("[ON] - O brinquedo  [%d] foi ligado.\n", toy->id); // Altere para o id do brinquedo

    sleep(5);

    debug("[OFF] - O brinquedo [%d] foi desligado.\n", toy->id); // Altere para o id do brinquedo

    pthread_exit(NULL);
}


// Essa função recebe como argumento informações e deve iniciar os brinquedos.
void open_toys(toy_args *args){
    // Inicializando uma thread para cada um dos brinquedos
    pthread_t id_thread[args->n];
    for (int i = 0; i < args->n; i++) {
        // atribui o id da thread ao toy correspondente
        args->toys[i]->thread = id_thread[i];
        pthread_create(&id_thread[i], NULL, turn_on, (void *) args->toys[i]);
    }

}

// Desligando os brinquedos
void close_toys(){
    
}