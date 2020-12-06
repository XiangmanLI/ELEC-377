//+
//    ELEC 377,  Lab 3
//
//  consumer.c
// transfers each character from the queue of the output
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

    /*     Shared memory init     */
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

    // put your code here...
    
    getMutex(&sharedPtr->lock);
    int N = sharedPtr->numProducers;
    
    releaseMutex(&sharedPtr->lock);
    
    char A;//init a char A

    int readC = TRUE;//init readC to be true
    //+
    // when readC is true, beginning transferring 
    //-
    while (N > 0 && readC)
    {
        readC = FALSE;//set it to false
    
        while (N > 0 && !readC)
        {
            //get lock
            getMutex(&sharedPtr->lock);
            //+
            //put the element in A until count is 0
            //_
            if (sharedPtr->count > 0)//conditon is when count is positive
            {
                //put the value into char A
                A = sharedPtr->buffer[sharedPtr->out];
                //decreament the count by 1
                sharedPtr->count --;
                sharedPtr->out ++;
                
                readC = TRUE;
                if(sharedPtr->out == 5){
                        sharedPtr->out =0;
                }
                
            }else
            {
                //set the number of producers
                N = sharedPtr->numProducers;
                
            }
            //release lock
            releaseMutex(&sharedPtr->lock);
            
        }
        putchar(A);
    }
    return 0;
}


