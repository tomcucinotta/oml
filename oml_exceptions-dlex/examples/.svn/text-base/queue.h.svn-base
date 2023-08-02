#ifndef QUEUE_H
#define QUEUE_H

#include "exception.h"

extern Exception EFullQueue;	/* extends (EException); */
extern Exception EEmptyQueue;	/* extends (EException); */

#define QUEUE_SIZE	10

typedef struct {
  char* queue[QUEUE_SIZE];
  int first;
  int last;
  int count;
} Queue;

typedef Queue* pQueue;

void qinit(pQueue pq);
void qput(pQueue pq, char *ptr);
char *qget(pQueue pq);
int  qempty(pQueue pq);


//declare_Queue( q8, 8 )

#endif
