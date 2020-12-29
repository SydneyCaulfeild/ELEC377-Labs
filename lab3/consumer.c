//+
//    ELEC 377,  Lab 3
//
//  consumer.c
//-

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>

#include "common.h"

#define FALSE 0
#define TRUE 1

int main (int argc, char *argv[]){

	// initialize the shared memory, load in the initial array's, spawn the worker
	// processes.

	key_t   key;
	struct shared    *sharedPtr;
	int shmid;
    
	/*	 Shared memory init 	*/
	key = ftok(".", 'S');
	if((shmid = shmget(key, MEMSIZE, IPC_CREAT|0666)) == -1 ){
		if( (shmid = shmget(key, MEMSIZE, 0)) == -1){
			printf("Error allocating shared memory. \n");
			exit(1);
		}
	}

	// now map the region..
	if((int)(sharedPtr = (struct shared *) shmat(shmid, 0, 0)) == -1){
	    printf("Couldn't map the memory into our process space.\n");
	    exit(1);
	}
    int c; 
    // Get the shared memory.
    getMutex(&sharedPtr->lock);
    int numProd = sharedPtr->numProducers;
    releaseMutex(&sharedPtr->lock);
    int charRead = TRUE;
    while (charRead == TRUE && numProd !=0) {
    	charRead = FALSE;
	while (charRead == FALSE && numProd !=0) {
	    getMutex(&sharedPtr->lock);
	    // Check if there there are elements in the buffer.
	    if (sharedPtr->count > 0) {
		charRead = TRUE;
		c = sharedPtr->buffer[sharedPtr->outPtr];
		sharedPtr->count--;
		sharedPtr->outPtr = (sharedPtr->outPtr + 1) % BUFFSIZE;	
	    } else {
		numProd = sharedPtr->numProducers;
	    }
	    releaseMutex(&sharedPtr->lock);
	}
    putchar(c);
    }
	return 0;
}


