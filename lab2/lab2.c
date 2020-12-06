/*+
 * Module:  lab2.c
 *
 * Purpose: Skeleton solution to ELEC 377 Lab2.
 *
-*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#define ENTERING_READ_PROC "<> entering my_read_proc, fpos = %d\n"
#define PageSize 1024*4

static struct task_struct *firstTask, *theTask;

int cnt;


int my_read_proc(char * page, char **start, off_t fpos, int blen, int * eof, void * data){

    int numChars;
    printk(ENTERING_READ_PROC,fpos);
    if (fpos == 0){
       //step 1
         numChars = sprintf(page, "Hello");
         numChars += sprintf(page + numChars, " World\n");

        //step2
	    // // write headers
        numChars += sprintf(page + numChars, "PID\tUID\tVSZ\tRSS\t\n");
	    // find first task
        theTask = &init_task;
        while (theTask->pid == 0){
            theTask = theTask->next_task;
        }
        // write first task
        firstTask = theTask;
        //start iterate all the list
        numChars += sprintf(page + numChars, "%d\t%d\t", theTask->pid,theTask->uid);

        
        if (theTask->mm == NULL){
            //if mm is NULL, and if so, use sprintf to add two 0s to the buffer
            numChars += sprintf(page + numChars," %4d\t%4d\t\n",00,00);
        }else{
            //Otherwise add the total_vm and the rss fields of the mm field multiplied
			//by your variable with the page size to the buffer
            numChars += sprintf(page + numChars," %4d\t",
                                (theTask->mm->total_vm * PageSize)>>10);
            numChars += sprintf(page + numChars," %4d\t\n",
                                (theTask->mm->rss *PageSize)>>10);
        }

        // advance to next task
        do{
            theTask = theTask->next_task;
        } while (theTask->pid == 0);
    } else {

        if (theTask == firstTask){
            *eof = 0;
            *start = page;
            return 0;
        }

        numChars = sprintf(page, "%d\t%d\t", theTask->pid,theTask->uid);

        if (theTask->mm == NULL){
            //if mm is NULL, and if so, use sprintf to add two 0s to the buffer
            numChars += sprintf(page + numChars," %4d\t%4d\t\n",00,00);
        }else{
            //Otherwise add the total_vm and the rss fields of the mm field multiplied
			//by your variable with the page size to the buffer
            numChars += sprintf(page + numChars," %4d\t",
                                (theTask->mm->total_vm * PageSize)>>10);
            numChars += sprintf(page + numChars," %4d\t\n",
                                (theTask->mm->rss *PageSize)>>10);
        }

	    // advance to next task
        do{
            theTask = theTask->next_task;
        } while (theTask->pid == 0);

    }
    *eof = 1;
    *start = page;
    return numChars;
}

int init_module(){

   struct proc_dir_entry * proc_entry;
   proc_entry = create_proc_entry("lab2",0444,NULL);
   	//if it returns NULL, there is no point in continuing, so you return a non - zero value from init_module
   if (proc_entry == NULL){
       return -1;
   }
	// If it was successful, then you set the read_proc entry of the proc_entry data structure to your read proc function.
   proc_entry->read_proc = my_read_proc;
   return 0;
}

void cleanup_module(){
    remove_proc_entry("lab2",NULL);

}
