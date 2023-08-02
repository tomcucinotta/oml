#include "queue.h"

define_exception(EFullQueue) extends(EException);
define_exception(EEmptyQueue) extends(EException);

void qinit(pQueue pq) {
  pq->first = pq->last = pq->count = 0;
}

void qput(pQueue pq, char *ptr) {
  if (pq->count == QUEUE_SIZE)
    throw(EFullQueue);
  pq->queue[pq->last] = ptr;
  pq->last = (pq->last+1) % QUEUE_SIZE;
  pq->count++;
}

char *qget(pQueue pq) {
  char *res;

  if (pq->count == 0)
    throw(EEmptyQueue);
  res = pq->queue[pq->first];
  pq->first = (pq->first+1) % QUEUE_SIZE;
  pq->count--;
  return res;
}

int qempty(pQueue pq) {
  return (pq->count == 0);
}
