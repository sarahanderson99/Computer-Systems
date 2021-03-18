//Sarah Anderson and Denise Linn

/*event.c*/
#include "priority.h"
#include "event.h"
#include <stdlib.h>
#include "queue.h"
#include "sim.h"
#include <string.h>
#include <stdio.h>
#include "time.h"

static priority_t *PQ;

/* initializes events, creates a priority queue */
void event_init(int size){

	//struct event_s *event;
	//priority_t *PQ;

	//event = (struct event_s *)malloc(sizeof(struct event_s));
	PQ = priority_init(size);
}

/* frees up all event space, including space in the priority queue */
void event_fini(){

//	event_t *event;	

	//while(!priority_empty(PQ))
	//{
	//	event = event_cause();
	//	event_destroy(event);
	//}
	//call event_cause and event_destroy on all events in pq
	priority_finalize(PQ);
}

/* allocate a fresh event with empty fields */
event_t *event_create(){

	event_t *event;
	event = (struct event_s *)malloc(sizeof(struct event_s));
	memset(event, 0, sizeof(event_t));

	return(event);
}

/* free an event */
void event_destroy(event_t *e){
	
	free(e);
	
}

/* insert the event into the priority queue.  The key value is the current sim time plus the event_time in event.  Update the event time to the key value. */
void event_schedule(event_t *e){

	double time;
	int test;
	
	time = time_get();
	e->event_time += time;
	//e->event_type += 1;
	test = priority_insert(PQ, e);
	if(test == -1)
	{
		printf("event was not able to be scheduled");
	}
}

/* remove the next event from the priority and return it to the program for execution */
event_t *event_cause(){

	event_t *event;	
	
	event = (event_t *)priority_remove(PQ);
	time_set(event->event_time);

	return(event);
}

int event_empty()
{
	return(priority_empty(PQ));
}



