#ifndef __SHARED_H__
#define __SHARED_H__

#include <defs.h>

// Você pode declarar novas funções (ou variaveis compartilhadas) aqui

extern int NUM_TOYS;
extern int client_id_global;

// Mutexes para proteger as variáveis globais do controle de entrada de cliente no parque
extern pthread_mutex_t mutex_client_id_global;
extern pthread_cond_t cond_client_id;


/**********************************
 *          ATENÇÃO               *
 *   NÃO EDITAR ESSAS VARIAVEIS   *
 *          ATENÇÃO               *
 *********************************/
extern Queue *gate_queue;

#endif