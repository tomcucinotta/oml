#ifndef SEMAPHOREH
#define SEMAPHOREH

#include "exception.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

extern Exception ESemaphore;	/* Eccezione sollevabile dalle primitive	*/

typedef int hSem;

hSem sem_create(key_t key, int init_value);	/* Crea ed inizializza un semaforo	*/
hSem sem_open(key_t key);

void sem_wait(hSem id);
void sem_signal(hSem id);

void sem_remove(hSem id);			/* Distrugge il semaforo	*/


int sem_read(hSem id);

#endif

