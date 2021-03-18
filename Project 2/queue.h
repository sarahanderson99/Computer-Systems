//Sarah Anderson and Denise Linn

/* queue.h */

#ifndef QUEUE_H
#define QUEUE_H

#include "priority.h"
#include "sim.h"
#include "event.h" 

typedef struct queue_s queue_t;

//allows that any type of variable can be passed in
typedef void *qelem_t;

typedef struct qnode
{
        qelem_t elem; //what the node is holding
        struct qnode *next; //points to the next node 
//        struct qnode *prev; //points to the previous node
}qnode;

struct queue_s
{
        qnode *tail; //points to the end of the queue 
        qnode *head; //points to the start of the queue
        int size;
}queue;
 
/* create and initialize a new queue; must be able to hold at least size items; return pointer to the new queue, NULL if error */
queue_t *queue_init(int size);
 
/* insert an item into the queue; return 0 if successful, -1 otherwise */
int queue_insert(queue_t *q, passenger_t *c);
 
/* return the next item in the queue but do not remove it; return NULL if the queue is empty or an error */
passenger_t *queue_peek(queue_t *q);
 
/* remove the next item from the queue and return it, return NULL if there is an error */
passenger_t *queue_remove(queue_t *q);
 
/* return the number of items in the queue; You can see if queue is empty with this */
int queue_size(queue_t *q);
 
/* return nono-zero if the queue is full; This may be trivial using a linked implementation */
int queue_full(queue_t *q);
 
/* free all resourced used by the queue then free the queue itself */
void queue_finalize(queue_t *q);

/* Checks to see if the queue is empty*/
int queue_empty(queue_t *q);

#endif
