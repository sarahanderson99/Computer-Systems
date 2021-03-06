//Sarah Anderson and Denise Linn

/* event.h */

#ifndef EVENT_H
#define EVENT_H
#include "priority.h"
#include "sim.h"
#include "queue.h"

typedef struct event_s event_t;

struct event_s
{
   int event_type;    	/* type of event – see below */
   struct queue_s *queue;     	/* queue passenger is waiting in */
   //I think this is where it needs to be updated (according to the note above)
   double event_time; 	/* sim time when event occurs */
   passenger_t *passenger;/* passenger related to this event */
};
 
/* initializes events, creates a priority queue */
void event_init(int size);
 
/* frees up all event space, including space in the priority
   queue */
void event_fini();
 
/* allocate a fresh event with empty fields */
event_t *event_create();
 
/* free an event */
void event_destroy(event_t *e);
 
/* insert the event into the priority queue.  The key value is the current sim time plus the event_time in event.  Update the event time to the key value. */
void event_schedule(event_t *e);
 
/* remove the next event from the priority and return it to the program for execution */
event_t *event_cause();

int event_empty();

#endif
