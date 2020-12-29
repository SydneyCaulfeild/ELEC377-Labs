/*+
 * Module:  lab2.c
 *
 * Purpose: Use the /proc file system to get the PID, user ID, total amount of virtual memory and amount of virtual
 * memory that is in RAM of the current state for the processes on the system.
 *
-*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

static struct task_struct * firstTask, *theTask;

int cnt;

//+
// Function      my_read_proc
//
// Purpose:      Read the process id, user id, total amount of vm and amount of vm in the ram for a process     

// Parameters:   
//    page       A pointer to the kernel buffer.
//    start      A pointer to the memory to be used.
//    fpos       The current position of the file.
//    blen       The length of the buffer.
//    eof        Pointer to the end of the request. 
//    data       A pointer to data that tracks multiple processes reading the file at once.

// Returns:      An integer representing the number of characters that were written into the buffer.
//-
int my_read_proc(char * page, char **start, off_t fpos, int blen, int * eof, void * data){
    // Initialize return value, which is the number of characters you wrote into the buffer.
    int numChars;
    // Check if we are at the beginning of the file.
    if (fpos == 0){
	// Write the headers.
	numChars = sprintf(page, "PID\t");
	numChars += sprintf(page + numChars, "UID\t");
	numChars += sprintf(page + numChars, "VSZ\t");
	numChars += sprintf(page + numChars, "RSS\n");
      
	// Find the first task.
	theTask = &init_task;
	while(theTask->pid == 0) {
	    theTask = theTask->next_task;
	}
        // Write the first task.
	firstTask = theTask;
	numChars += sprintf(page + numChars, "%d\t",theTask->pid); 
	numChars += sprintf(page + numChars, "%d\t",theTask->uid); 
	// Check if process is not using virtual memory. If so, print zeros.
	if (theTask->mm == NULL) {
	    numChars += sprintf(page + numChars, "0\t");
	    numChars += sprintf(page + numChars, "0\n");
	}       
	else {
	    // Adjust pageSize to get the size in K.
	    int pageSize = PAGE_SIZE>>10;
	    numChars += sprintf(page + numChars, "%d\t",((theTask->mm->total_vm)*pageSize));
	    numChars += sprintf(page + numChars, "%d\n",((theTask->mm->rss)*pageSize)); 
	}

        // Advance to next task.
	do {
	    theTask = theTask->next_task;
	} while (theTask->pid == 0);     
    } 
    else {
	// If we have looped back to the firstTask, return 0.
	if (theTask==firstTask){
	    *eof = 0;
	    *start = page;
	    return 0;
	}
	
	// Write task info for one task.
	numChars = sprintf(page, "%d\t",theTask->pid); 
	numChars += sprintf(page + numChars, "%d\t",theTask->uid); 
	if (theTask->mm == NULL) {
	    numChars += sprintf(page + numChars, "0\t");
	    numChars += sprintf(page + numChars, "0\n");
	}       
	else {
	    int pageSize = PAGE_SIZE>>10;
	    numChars += sprintf(page + numChars, "%d\t",((theTask->mm->total_vm)*pageSize));
	    numChars += sprintf(page + numChars, "%d\n",((theTask->mm->rss)*pageSize)); 
	}

	// Advance to the next task.
	do {
	    theTask = theTask->next_task;
	} while (theTask->pid == 0);     
    }
    *eof = 1;
    *start = page;
    return numChars;
}

//+
// Function      init_module 
//
// Purpose:      Function executed by the kernel when the module is loaded.     
//
// Returns:      Returns -1 if unsuccessful in finding the data structure allocated by the kernel, otherwise returns 0. 
//-
int init_module(){
    struct proc_dir_entry * proc_entry;
    proc_entry = create_proc_entry("lab2",0444,NULL);
    if (proc_entry == NULL) {
	return -1;
    }
    else {
	proc_entry->read_proc = my_read_proc;
	return 0;
    }
}

//+
// Function      cleanup_module
//
// Purpose:      Called by the kernel when the module is removed to remove the /proc file. 
//-
void cleanup_module(){
    remove_proc_entry("lab2", NULL);
}
