//+ 	ELEC 377, Lab 3
//
//   common.c contains routines to be used from both the
//   producer, and the  consumer
//   Mutual Exclusion routines will be here
//-

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <asm/system.h>


#include "common.h"

#define FALSE 0
#define TRUE 1

//+
// Purpose: Exchange and test the lock variable that will be used to ensure only one producer/consumer accesses shared variables at a time.
// Parameters: Pointer to an integer variable named lock that will be used by the mutex.
// Function: Use the compare and exchange assembly command to exchange and test the lock.
//-
int test_and_set(int * lock){
    return __cmpxchg(lock,0,1,4);
}

//+
// Purpose: Make the consumer/producer wait until it has access to the shared variables.
// Parameters: Pointer to an integer variable named lock that will be used by the mutex.
// Function: Pass the lock variable to the test_and_set function and loop until this returns 1.
//-
void getMutex(int *  lock){
	// this should not return until it has mutual exclusion. Note that many versions of 
	// this will probobly be running at the same time.
    while(test_and_set(lock));
}

//+
// Purpose: Free up the mutex once the consumer/producer is finished with the shared variables.
// Parameters: Pointer to an integer variable named lock that will be used by the mutex.
// Function: Set the value that lock is pointing to to zero using the defined FALSE constant.
void releaseMutex(int * lock){
	// set the mutex back to initial state so that somebody else can claim it
    *lock = FALSE;
}

