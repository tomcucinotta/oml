/* 
 * 
 * Interfaccia semplificata alle semaforiche del System V 
 *
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaphore.h"
#include <errno.h>

exception(ESemaphore) extends(EError);
/*
union semun {
  int val; /* used for SETVAL only *
  struct semid_ds *buf; /* for IPC_STAT and IPC_SET *
  ushort *array;  /* used for GETALL and SETALL *
} 
*/

union semun semctl_arg;

/*
 * Crea un semaforo con il valore iniziale specificato (init_value).
 * Se il semaforo gia' esiste non e' creato e non inizializzato.
 *
 * throws: ESemaphore 
 *
 */

hSem sem_create(key_t key, int init_value)
{
  hSem id;

  if ( (id = semget(key, 1, 0666 | IPC_CREAT)) < 0)
    throw(ESemaphore); /* permission problem or tables full */
  semctl_arg.val = init_value;
  if (semctl(id, 0, SETVAL, semctl_arg) < 0)
    throw(ESemaphore); /* can't set init_value */
  return id;
}  

hSem sem_open(key_t key)
{
  hSem id;

  if ( (id = semget(key, 1, 0)) < 0)
    throw(ESemaphore); /* permission problem or tables full */
  return id;
}  
  
void sem_remove(hSem id)
{
  semctl_arg.val = 0;
  if (semctl(id, 0, IPC_RMID, semctl_arg) < 0)
    throw(ESemaphore);
}

void sem_op(hSem id, int value)
{
  struct sembuf op[1] =
  {
    0, 
    0,
    SEM_UNDO
  };
  int ret;
  
  op[0].sem_op = value;

  do
    ret = semop((int)id, &op[0], 1);
  while ( (ret < 0) && (errno == EINTR) );
  if( (ret < 0) && (errno != EINTR) )
    throw(ESemaphore);
}

void sem_wait(hSem id)
{
  sem_op(id, -1);
}

void sem_signal(hSem id)
{
  sem_op(id, 1);
}

int sem_read(hSem id)
{
  int val;

  semctl_arg.val = 0;
  if ( (val = semctl(id, 0, GETVAL, semctl_arg)) < 0)
    throw(ESemaphore); /* can't set init_value */
  return val;
}


  






