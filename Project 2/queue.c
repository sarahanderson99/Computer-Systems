//Sarah Anderson and Denise Linn

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "sim.h"

//#define QSZ 100

//initializes the queue that holds size int size number of nodes 
queue_t *queue_init(int size){
	queue_t *q;
	q = malloc(sizeof(*q));
            if (!q){
                return NULL;
	}
           q->tail = NULL;
           q->head = NULL;
	   q->size = 0;

           return q;
}

/* insert an item into the queue; return 0 if successful, -1 otherwise */
int queue_insert(queue_t *q, passenger_t *c){
	qnode *new = malloc(sizeof(*new));

        if (!new)
                return -1;
      
        q->size += 1;
        new->elem = c; 
	
        new->next = NULL;
     //   new->prev = NULL;
        if (q->tail == NULL && q->head == NULL){
                q->tail = new;
		q->head = new;
	}
	else{
		q->tail->next = new;
		q->tail = new;
	}

//        q->tail = new;
  //      if (!q->head)
//		 q->head = new;
        return 0;
}

/* returns the next item in the queue but do not remove it; return NULL if the queue is empty or an error */
passenger_t *queue_peek(queue_t *q){
	qnode *temp;
	if (!q){
               return NULL;
        }
        else{
	    temp = q->head->next;
	    return(temp->elem);  //next item
        }
}
		
/* remove the next item from the queue and return it, return NULL if there is an error */
passenger_t *queue_remove(queue_t *q){
	passenger_t *temp;
	qnode *other;
 
        if ((q->head == NULL) && (q->tail == NULL)) //If the queue is emty
                return NULL;
        else{
		 q->size -= 1;
	
		 temp = q->head->elem;
		 other = q->head;
		 q->head = q->head->next; 
		 free(other);
		// q->head = temp;
		 if(q->head == NULL){		
			q->tail = NULL;
		 }
	}
	
	return(temp);
}

/* return the number of items in the queue; You can see if queue is empty with this */
int queue_size(queue_t *q){
	return(q->size);
}


/* return nono-zero if the queue is full; This may be trivial using a linked implementation */
int queue_full(queue_t *q){
	if(q->size == QSZ){
		return -1; //If it is full
	}
	else{
		return 0;  //if it isnt full
	}
}

/* free all resources used by the queue then free the queue itself */
void queue_finalize(queue_t *q){
//        qnode *prev;
        //qelem_t *Nelem;
/*
        if ((q->head == NULL) && (q->tail == NULL)) {  //If the queue is empty
                
	}
	else{
		//qelem_t *Nelem;

	       // prev = q->head->prev;
       		//Nelem = q->head->elem;
      		free(q->head);
        	//q->head = prev;

        	while((q->head != NULL) && (q->tail != NULL)){
			queue_finalize(q);
		}
       	
       		while((q->head == NULL) && (q->tail == NULL)){
			free(q);
       		}
	}
*/
    qnode *rover = q->head;
    qnode *item = rover;
    while(rover)
    {
        item = rover;
        rover = rover->next;
        free(item);
    }
    free(q);

}

int queue_empty(queue_t *q){
	if((q->head == NULL) && (q->tail == NULL)){
		return (-1);
	}
	else{
		return(0);
	}
} 
