//Sarah Anderson and Denise Linn

/* priority.c */
#include "priority.h"
#include "queue.h"
#include <stdio.h>
#include "event.h"
#include <stdlib.h>
#include "sim.h"

/* create and initialize a new priority queue must be able to hold at least size items return pointer to the new priority queue, NULL if error */
priority_t *priority_init(int size){

	priority_t *PQ;

	PQ = (priority_t *)malloc(sizeof(priority_t)*size);
	PQ->count = 0;
	return(PQ);	
}

/* insert an item into the priority queue  return 0 if successful, -1 otherwise */
int priority_insert(priority_t *q, event_t *ev){
	
	int current;
	int parent;	

	q->count += 1;
	current = q->count;
	if(current == 1)
	{
		q->PQArray[current] = ev;
		return(0);
	}
	else
	{
		parent = (q->count)/2;

		while(parent != 0)
		{
			if(q->PQArray[parent]->event_time <= ev->event_time)
			{
				q->PQArray[current] = ev;
				return(0);
			}
			else
			{
				q->PQArray[current] = q->PQArray[parent];
				current = parent;
				parent /= 2;
			}
		}
		q->PQArray[current] = ev; // return(-1);
		return(0);
	}
	return(-1);	
}

/*remove the highest priority item from the queue and return it, return NULL if there is an error*/
event_t *priority_remove(priority_t *q){
	int current;
	int child;

	event_t *move;
	event_t *next;

	if(priority_empty(q) == 1)
	{
		return(NULL);
	}
	
	next = q->PQArray[1];
	move = q->PQArray[q->count];

	q->count -= 1;
	current = 1;
	child = 2*current;

	if(q->count == 0)
	{
		return(q->PQArray[1]);
	}
	else if(q->count == 1)
	{
		q->PQArray[1] = move;
		return(next);	
	}
	else
	{
		while(child <= q->count)
		{
			if(child < q->count)
			{
				if(q->PQArray[child+1]->event_time < q->PQArray[child]->event_time)
				{
					child += 1;
				}
			}
			if(q->PQArray[child]->event_time >= move->event_time)
			{
				q->PQArray[current] = move;
				return(next);
			}
			else
			{
				q->PQArray[current] = q->PQArray[child];
				current = child;
				q->PQArray[child] = move;
				child = current * 2;
			}
		}
		return(next);
	}
	
	return(NULL);			
}

/* return non-zero if the priority queue is empty */
int priority_empty(priority_t *q){
	return(q->count == 0);
}

/* return nono-zero if the priority queue is full; This may be trivial using a linked implementation*/
int priority_full(priority_t *q){
	return(q->count == QSZ);
}

/* free all resources used by the priority queue then free the queue itself */
void priority_finalize(priority_t *q){
	free(q);
}

