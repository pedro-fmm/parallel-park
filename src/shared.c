#include "shared.h"

#include <queue.h>


// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

int NUM_TOYS;
int client_id_global = 0;
int n_pessoas_parque = 0;

// Definição dos mutexes usados
pthread_mutex_t mutex_client_id_global = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_client_id = PTHREAD_COND_INITIALIZER;

/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
Queue *gate_queue = NULL;