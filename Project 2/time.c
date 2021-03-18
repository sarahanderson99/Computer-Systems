//Sarah Anderson and Denise Linn
/*time.c*/
#include "randsim.h"
#include "time.h"
#include <math.h>
#include <stdlib.h>

double Global_Time;

/* creates a double for using as the global sim time */
void time_init(){
	Global_Time = 0.0;
	randsim_init();
}

/* cleans up the time system as needed */
void time_fini(){

	//return(0);	
}

/* returns the current sim time */
double time_get(){
	return(Global_Time);
}
/* sets the current sim time */
void time_set(double t){
	Global_Time = t;
	//return(0);
}

/* these return a random time which is used to set the   time each passenger takes to make each of these steps   these times are set into the corresponding event   before it is scheduled. These should be moved from   randsim.c to time.c */
double time_arrive()    // passenger inter-arrival time
{
	return randsim_exp(180.0);
}

double time_airlineQ() // passenger enters airline queue
{
	return randsim_gauss(480, 60);
}

double time_airline()  // time airline takes to ticket and bag
{    
	return randsim_exp(60.0);
}

double time_idQ()      // passenger enters security queue
{    
	return randsim_gauss(240, 30);
}

double time_id()       // time security takes reviewing ID
{    
	return randsim_exp(60.0);
}

double time_scanQ()    // passenger enters a scanner queue
{    
	return randsim_gauss(240, 30);
}

double time_scan()     // time it takes to scan passenger
{    
	return randsim_exp(60.0);
}

double time_trainQ()   // passenger enters a queue for train
{    
	return randsim_gauss(240, 30);
}

double time_train()    // times for train to get to gate area
{    
	return randsim_exp(60.0);
}

double time_gate()     // time for passenger to get to gate
{    
	return randsim_exp(60.0);
}
