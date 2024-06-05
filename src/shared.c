#include "shared.h"

#include <queue.h>


// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

int NUM_TOYS; // Variavel global para saber o numero de brinquedos
int client_id_global = 0; // Variavel global para ter o acesso ao id do cliente
int n_pessoas_parque = 0; // Varivel global para saber quantas pessoas estão no parque
int clientes_chegaram = 0; // Variavel global para saber se algum cliente já chegou na fila da bilheteria

// Definição dos mutexes usados globalmente
pthread_mutex_t mutex_client_id_global = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_client_id = PTHREAD_COND_INITIALIZER;


/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;