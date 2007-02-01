#ifndef __OML_SYNC_OML_QUEUE_H__
#define __OML_SYNC_OML_QUEUE_H__

/** @file
 *
 * @brief Realizza un tipo di dati "FIFO thread-safe".
 *
 * Utilizza una coda priva di meccanismi di sincronizzazione come
 * supporto, ma definisce le funzioni sync_queue_insert() e
 * sync_queue_extract(), eventualmente bloccanti, che permettono
 * l'inserimento e l'estrazione di elementi in maniera atomica e
 * thread-safe, tramite le primitive della libreria pthread.
 */

#include "queue.h"

#include <pthread.h>
#include <stdio.h>

/** Rappresentazione interna di una FIFO thread-safe */
typedef struct {
  oml_vqueue q;		/**< La coda mono-thread di supporto	*/
  pthread_mutex_t mutex;/**< Mutex per l'intera oml_sync_queue	*/
  /** Condition-Var che segnala la presenza di ulteriori elementi */
  pthread_cond_t more;
  /** Condition-Var che segnala la liberazione di ulteriori posti */
  pthread_cond_t less;
} oml_sync_queue;

/** Inizializza una FIFO thread-safe con uno specifico numero massimo
 * di elementi acFIFObili */
void sync_queue_init(oml_sync_queue *sq, int num_elems) {
  queue_init(&sq->q, num_elems);
  pthread_cond_init(&sq->more, NULL);
  pthread_cond_init(&sq->less, NULL);
}

/** Distrugge una FIFO thread-safe liberando le risorse associate */
void sync_queue_cleanup(oml_sync_queue *sq) {
  pthread_cond_destroy(&sq->more);
  pthread_cond_destroy(&sq->less);
  queue_cleanup(&sq->q);
}

/** Inserisce un elemento nella FIFO, bloccandosi se e' gia' piena */
void sync_queue_insert(oml_sync_queue *sq, char item) {
  pthread_mutex_lock(&sq->mutex);

  while (queue_is_full(&sq->q)) {
    printf("sync_queue_insert() found full queue: waiting on condition 'less'...\n");
    pthread_cond_wait(&sq->less, &sq->mutex);
    printf("sync_queue_insert() woken up: rechecking condition...\n");
  }

  printf("sync_queue_insert() enqueueing element.\n");
  queue_insert(&sq->q, item);

  printf("sync_queue_insert() signaling other threads waiting on condition 'more', if any.\n");
  pthread_cond_signal(&sq->more);
  pthread_mutex_unlock(&sq->mutex);
}

/** Estrae un elemento dalla FIFO, bloccandosi se e' gia' vuota */
char sync_queue_extract(oml_sync_queue *sq) {
  char item;
  pthread_mutex_lock(&sq->mutex);

  while (queue_is_empty(&sq->q)) {
    printf("sync_queue_extract() found empty queue. Waiting on condition 'more'...\n");
    pthread_cond_wait(&sq->more, &sq->mutex);
    printf("sync_queue_extract() woken up: rechecking condition...\n");
  }

  printf("sync_queue_extract() dequeueing element.\n");
  item = queue_extract(&sq->q);

  printf("sync_queue_extract() signaling other threads waiting on condition 'less', if any.\n");
  pthread_cond_signal(&sq->less);
  pthread_mutex_unlock(&sq->mutex);

  return item;
}

#endif
