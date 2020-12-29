//+
//  common.h - Common definiton of the shared
//     memory segment and prototypes for semaphore functions
// 
//  Created by Thomas Dean
//  Copyright 2005 Queen's University.
//
//-

#define MEMSIZE 200
#define BUFFSIZE 5

struct shared {
    int  lock;   // Variable to dictate which process has access to shared data. 
    int count;   // Count of the number of items in the buffer.
    int inPtr;   // Index of the buffer's in pointer.
    int outPtr;  // Index of the buffer's out pointer.
    char buffer[BUFFSIZE];	// The buffer that will be accessed by producers and consumers.
    int numProducers;		// The number of current producers.
};


void getMutex(int * lock);
void releaseMutex(int * lock);
