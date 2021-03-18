//Sarah Anderson and Denise Linn

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sim.h"
#include "queue.h"
#include "randsim.h"
#include "time.h"
#include "priority.h"
#include "event.h"

//#define QSZ 1000000
#define MAX_SCAN 4

int main(int argc, char **argv)
{
	event_init(QSZ);
	
	int i = 0, k = 0;
	event_t *airQ_ev, *start_ev, *gate_ev, *newairQ_ev, *air_ev, *idQ_ev, *id_ev, *newidQ_ev, *scanQ_ev, *newscanQ_ev, *scan_ev, *train_ev, *trainQ_ev, *newtrainQ_ev;
	queue_t *airlineQ, *idQ, *scanQ[MAX_SCAN], *trainQ;
	
	airlineQ = queue_init(QSZ);
	idQ = queue_init(QSZ);
	for(i = 0; i < MAX_SCAN; i++){
		scanQ[i] = queue_init(QSZ);
	}
	trainQ = queue_init(QSZ);

	start_ev = event_create();
	start_ev->passenger = (passenger_t *)malloc(sizeof(passenger_t));
	num_passengers = 1;
	start_ev->passenger->pass_id = num_passengers;
	
	/* schedule EV_ARRIVE event at t=0 and put in event queue */
	start_ev->event_time = 0.0;
	start_ev->event_type = EV_ARRIVE;
	event_schedule(start_ev);

	/* run main loop */
	while(!event_empty()) 
	{
		//Removes next event from event queue and sets the sim time
		event_t *new_ev;
		event_t *arrive_ev;

		new_ev = event_cause();

		switch (new_ev->event_type)
		{
			case (EV_ARRIVE) :
				//Sets arrival time increases event type, sets the time to a new delta t and adds the event back into the priority queue
				new_ev->passenger->arrival_time = time_get();

				//new_ev->event_type = EV_AIRLINEQ;
				printf("Passenger %d has arrived at time %f.\n", new_ev->passenger->pass_id, new_ev->passenger->arrival_time);
        	
				arrive_ev = event_create();
				arrive_ev->passenger = new_ev->passenger;
				new_ev->passenger = NULL;
				arrive_ev->event_time = time_airlineQ(); //Need to make this delta t 
				arrive_ev->event_type = EV_AIRLINEQ;
				event_schedule(arrive_ev);
				/* create EV_ENQUEUE event for this passenger */
        			/* schedule EV_ENQUEUE event */
        			if ((MAX_PASS) > num_passengers++)
        			{
            				/* create new EV_ARRIVE event and passsenger */
					//Creates new event gives it type arrive and mallocs another passenger
					event_t *new1_ev;
					new1_ev = event_create(); 					

					new1_ev->event_type = EV_ARRIVE;
					new1_ev->passenger = (passenger_t *)malloc(sizeof(passenger_t));
					new1_ev->passenger->pass_id = num_passengers;
			
            				/* schedule EV_ARRIVE event */ 
					new1_ev->event_time = time_arrive();//Need to set as delta t
					//Adds event back into event queue
					event_schedule(new1_ev);
	 			}
        			break;

    			case (EV_AIRLINEQ) :
			//	event_t *airQ_ev;
				airQ_ev = event_create();
				airQ_ev->event_time = time_get();
				
				airQ_ev->passenger = new_ev->passenger;
				 new_ev->passenger = NULL;
				airQ_ev->passenger->airlineQ_time = time_get();

			//	printf("Passenger %d has entered the airline queue at time %f.\n", new_ev->passenger->pass_id, new_ev->passenger->airlineQ_time);

				//If the queue is full (but it will never be because it is a linked list)
				if(queue_full(airlineQ) == 0){ 
					queue_insert(airlineQ, airQ_ev->passenger);
                                	//IF the queue has one passenger in it
					if(queue_size(airlineQ) == 1){
						//New event (change the event type)
						airQ_ev->event_type = EV_AIRLINE;
						airQ_ev->event_time = time_airline();
						event_schedule(airQ_ev);
					}
					else{
						event_destroy(airQ_ev);
					}
				}
        			break;

    			case (EV_AIRLINE) :
				//Remove queue (special case for first passenger in queue)
				//person leaving airline person (i.e. will get a new random time) will cause the person to pop off of the airline queue
				air_ev = event_create();
				air_ev->event_time = time_get();
				
				air_ev->passenger = new_ev->passenger;
				new_ev->passenger = NULL;
				air_ev->passenger->airline_time = time_get();

				if(queue_size(airlineQ) != 0){
					queue_remove(airlineQ);
				}
				if(queue_size(airlineQ) != 0){
					newairQ_ev = event_create();
					newairQ_ev->event_time = time_airline();
					newairQ_ev->passenger = airlineQ->head->elem;
					newairQ_ev->event_type = EV_AIRLINE;
					event_schedule(newairQ_ev);
				}
				
				air_ev->event_type = EV_IDQ;
				air_ev->event_time = time_idQ();
			//	printf("Passenger %d has left the airline queue at %f.\n", new_ev->passenger->pass_id, new_ev->passenger->airline_time);
				event_schedule(air_ev);
        			break;

			case (EV_IDQ) :
				idQ_ev = event_create();
                                idQ_ev->event_time = time_get();

                                idQ_ev->passenger = new_ev->passenger;
                                idQ_ev->passenger->idQ_time = time_get();

                        //      printf("Passenger %d has entered the ID queue at time %f.\n", idQ_ev->passenger->pass_id, idQ_ev->passenger->idQ_time);

                                //If the queue is full (but it will never be because it is a linked list)
                                if(queue_full(idQ) == 0){
                                        queue_insert(idQ, idQ_ev->passenger);
                                        //IF the queue has one passenger in it
                                        if(queue_size(idQ) == 1){
                                                idQ_ev->event_type = EV_ID;
                                                idQ_ev->event_time = time_id();
                                                event_schedule(idQ_ev);
                                        }
                                        else{
                                                event_destroy(idQ_ev);
                                        }
                                }
                                break;

			case (EV_ID) :
				id_ev = event_create();
                                id_ev->event_time = time_get();

                                id_ev->passenger = new_ev->passenger;
                                new_ev->passenger = NULL;
				id_ev->passenger->id_time = time_get();
				 
				if(queue_size(idQ) != 0){
                                	queue_remove(idQ);
                                }
				
				if(queue_size(idQ) != 0){
                                        newidQ_ev = event_create();
                                        newidQ_ev->event_time = time_id();
                                        newidQ_ev->passenger = idQ->head->elem;
                                        newidQ_ev->event_type = EV_ID;
                                        event_schedule(newidQ_ev);
                                }

                                id_ev->event_type = EV_SCANQ;
                                id_ev->event_time = time_scanQ();
                        //      printf("Passenger %d has left the id queue at %f.\n", id_ev->passenger->pass_id, id_ev->passenger->id_time);
                                event_schedule(id_ev);
				break;

       		        case (EV_SCANQ) :
				scanQ_ev = event_create();
                                scanQ_ev->event_time = time_get();

                                scanQ_ev->passenger = new_ev->passenger;
                                 new_ev->passenger = NULL;
				scanQ_ev->passenger->scanQ_time = time_get();

                     //           printf("Passenger %d has entered the scan queue at time %f.\n", scanQ_ev->passenger->pass_id, scanQ_ev->passenger->scanQ_time);
                      
                                //If the queue is full (but it will never be because it is a linked list)
                                if((queue_full(scanQ[0]) == 0) || (queue_full(scanQ[1]) == 0)   || (queue_full(scanQ[2]) == 0) || (queue_full(scanQ[3]) == 0)){
					if((queue_size(scanQ[0]) < queue_size(scanQ[1])) || (queue_size(scanQ[0]) < queue_size(scanQ[2])) || (queue_size(scanQ[0]) < queue_size(scanQ[3]))){
						 queue_insert(scanQ[0], scanQ_ev->passenger);
					//	printf("Passenger %d has entered the scan queue 1 at time %f.\n", new_ev->passenger->pass_id, new_ev->passenger->scanQ_time);
						if(queue_size(scanQ[0]) == 1){
                                                	scanQ_ev->event_type = EV_SCAN;
							scanQ_ev->event_time = time_scan();
                                               		scanQ_ev->passenger->scan_id = 0;
							event_schedule(scanQ_ev);
 	                                       	}
        	                                else{   //queue has people waiting in it
							scanQ_ev->passenger->scan_id = 0;
                	                                event_destroy(scanQ_ev);
                        	                }
					}
					else if((queue_size(scanQ[1]) < queue_size(scanQ[0])) || (queue_size(scanQ[1]) < queue_size(scanQ[2])) || (queue_size(scanQ[1]) < queue_size(scanQ[3]))){
						queue_insert(scanQ[1], scanQ_ev->passenger);
					//	printf("Passenger %d has entered the scan queue 2 at time %f.\n", new_ev->passenger->pass_id, new_ev->passenger->scanQ_time);
						if(queue_size(scanQ[1]) == 1){
                                                        scanQ_ev->event_type = EV_SCAN;
							scanQ_ev->event_time = time_scan();
							scanQ_ev->passenger->scan_id = 1;
                                                        event_schedule(scanQ_ev);
                                                }
                                                else{   //queue has people waiting in it
							scanQ_ev->passenger->scan_id = 1;
                                                        event_destroy(scanQ_ev);
                                                }
                                        }
					else if((queue_size(scanQ[2]) < queue_size(scanQ[1])) || (queue_size(scanQ[2]) < queue_size(scanQ[0])) || (queue_size(scanQ[2]) < queue_size(scanQ[3]))){
						queue_insert(scanQ[2], scanQ_ev->passenger);
					//	printf("Passenger %d has entered the scan queue 3 at time %f.\n", new_ev->passenger->pass_id, new_ev->passenger->scanQ_time);
						 if(queue_size(scanQ[2]) == 1){
                                                        scanQ_ev->event_type = EV_SCAN;
							scanQ_ev->event_time = time_scan();
							scanQ_ev->passenger->scan_id = 2;
                                                        event_schedule(scanQ_ev);
                                                }
                                                else{   //queue has people waiting in it
							scanQ_ev->passenger->scan_id = 2;
                                                        event_destroy(scanQ_ev);
						}

                                        }
					else if((queue_size(scanQ[3]) < queue_size(scanQ[1])) || (queue_size(scanQ[3]) < queue_size(scanQ[2])) || (queue_size(scanQ[3]) < queue_size(scanQ[0]))){
						queue_insert(scanQ[3], scanQ_ev->passenger);
					//	printf("Passenger %d has entered the scan queue 4 at time %f.\n", new_ev->passenger->pass_id, new_ev->passenger->scanQ_time);
						 if(queue_size(scanQ[3]) == 1){
                                                        scanQ_ev->event_type = EV_SCAN;
							scanQ_ev->event_time = time_scan();
							scanQ_ev->passenger->scan_id = 3;
                                                        event_schedule(scanQ_ev);
                                                }
                                                else{   //queue has people waiting in it
							scanQ_ev->passenger->scan_id = 3;
                                                        event_destroy(scanQ_ev);
                                                }

                                        }
					else{
						queue_insert(scanQ[0], scanQ_ev->passenger);
                                          //      printf("Passenger %d has entered the scan queue 1 at time %f.\n", new_ev->passenger->pass_id, new_ev->passenger->scanQ_time);
                                                if(queue_size(scanQ[0]) == 1){
                                                        scanQ_ev->event_type = EV_SCAN;
							scanQ_ev->event_time = time_scan();
							scanQ_ev->passenger->scan_id = 0;
                                                        event_schedule(scanQ_ev);
                                                }
                                                else{   //queue has people waiting in it
							scanQ_ev->passenger->scan_id = 0;
                                                        event_destroy(scanQ_ev);
                                                }
					    }
                                }

				break;
        
			case (EV_SCAN) :

				scan_ev = event_create();
                                scan_ev->event_time = time_get();

                                scan_ev->passenger = new_ev->passenger;
                                new_ev->passenger = NULL;
				scan_ev->passenger->scan_time = time_get();
				
				if(queue_size(scanQ[scan_ev->passenger->scan_id]) == 0){
					break;
				}

				if(queue_size(scanQ[scan_ev->passenger->scan_id]) != 0){
            				queue_remove(scanQ[scan_ev->passenger->scan_id]);
				}
				if(queue_size(scanQ[scan_ev->passenger->scan_id]) != 0){
                                        newscanQ_ev = event_create();
                                        newscanQ_ev->passenger = scanQ[scan_ev->passenger->scan_id]->head->elem;
					newscanQ_ev->event_time = time_scan();
                                        newscanQ_ev->event_type = EV_SCAN;
                                        event_schedule(newscanQ_ev);
                                }
				
                                scan_ev->event_type = EV_TRAINQ;
                                scan_ev->event_time = time_trainQ();
                               // printf("Passenger %d has left the scan queue at %f.\n", new_ev->passenger->pass_id, new_ev->passenger->scan_time);
                                event_schedule(scan_ev);
				break;
        
			case (EV_TRAINQ) :
				trainQ_ev = event_create();
                                trainQ_ev->event_time = time_get();
                                trainQ_ev->passenger = new_ev->passenger;
				 new_ev->passenger = NULL;
                                trainQ_ev->passenger->trainQ_time = time_get();

                        //      printf("Passenger %d has entered the ID queue at time %f.\n", idQ_ev->passenger->pass_id, idQ_ev->passenger->idQ_time);

                                //If the queue is full (but it will never be because it is a linked list)
                                if(queue_full(trainQ) == 0){
                                        queue_insert(trainQ, trainQ_ev->passenger);
                                        //IF the queue has one passenger in it
                                        if(queue_size(trainQ) == 1){
                                                //New event (change the event type)
                                                trainQ_ev->event_type = EV_TRAIN;
                                                trainQ_ev->event_time = time_train();
                                                event_schedule(trainQ_ev);
                                        }
                                        else{
                                                event_destroy(trainQ_ev);
                                        }
                                }
            			break;
       
			case (EV_TRAIN) :
				train_ev = event_create();
                                train_ev->event_time = time_get();

                                train_ev->passenger = new_ev->passenger;
				 new_ev->passenger = NULL;
                                train_ev->passenger->train_time = time_get();
				
				 if(queue_size(trainQ) != 0){
                                	queue_remove(trainQ);
				}
                                if(queue_size(trainQ) != 0){
                                        newtrainQ_ev = event_create();
                                        newtrainQ_ev->event_time = time_train();
                                        newtrainQ_ev->passenger = trainQ->head->elem;
                                        newtrainQ_ev->event_type = EV_TRAIN;
                                        event_schedule(newtrainQ_ev);
                                }

                                train_ev->event_type = EV_GATE;
                                train_ev->event_time = time_gate();
                        //      printf("Passenger %d has left the id queue at %f.\n", id_ev->passenger->pass_id, id_ev->passenger->id_time);
                                event_schedule(train_ev);
				break;
        
			case (EV_GATE) :
				gate_ev = event_create();
                                gate_ev->event_time = time_get();

                                gate_ev->passenger = new_ev->passenger;
				new_ev->passenger = NULL;
                                gate_ev->passenger->gate_time = time_get();

				printf("Passenger %d has arrived at the gate at %f.\n", gate_ev->passenger->pass_id, gate_ev->passenger->gate_time);
            			//free(gate_ev->passenger);
				passenger_destroy(gate_ev->passenger);
				event_destroy(gate_ev);
				break;

    			/* remaining event types */    			
			default :
        		/* error */
        			break;
    		}
		event_destroy(new_ev);
	}
        /* free event*/
 	queue_finalize(airlineQ);
        queue_finalize(trainQ);
        queue_finalize(idQ);
	for (k = 0; k < MAX_SCAN; k++){
	       	queue_finalize(scanQ[k]);
	}
	event_fini();
	time_fini();	
	
	return(0);
}
