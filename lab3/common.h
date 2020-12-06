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
    
    int lock;
    int numProducers;
    char buffer[BUFFSIZE];
    int in;
    int out;
    int count;
};


void getMutex(int * lock);
void releaseMutex(int * lock);
