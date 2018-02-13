#include "kernel_ken.h"
#include "monitor.h"
#include "error.h"

/*  ##############################################################################
     __  __      ___ 
    |__)|__)||\ | |  
    |   | \ || \| |
    
    You can simply have your implementation of
    the functions wrapper the sys_call_monitor* functions that are provided
    as part of the initital kernel code.
*/

void print(const char *string)
{
    monitor_write(string);
}

    // Output a null-terminated ASCII string to the monitor.

void print_hex(unsigned int n)
{
    monitor_write_hex(n);
}

    //Output an HEX integer to the monitor
    

void print_dec(unsigned int n)
{
    monitor_write_dec( n);
}

    // Output an integer to the monitor

/*  ##############################################################################
          __            __   __                    
    |\/| |_  |\/| |\/| /  \ |__) \_/               
    |  | |__ |  | |  | \__/ | \   |                
                                                   
                         __   __       __      ___ 
    |\/|  /\  |\ |  /\  / _  |_  |\/| |_  |\ |  |  
    |  | /--\ | \| /--\ \__) |__ |  | |__ | \|  |  
*/
void *alloc(unsigned int size, unsigned char page_align)
{
    return NULL;
}

    // Allocates a contiguous region of memory 'size' in the user heap. If
    // page_align==1, it creates that block starting on a page boundary.


void free(void *p)
{
    return;
}

    // Releases a block from the user heap allocated with 'alloc'.
    // p is a reference to the memory to be deallocated. 

/*  ##############################################################################
     __   __   __   __  __  __  __                 
    |__) |__) /  \ /   |_  (_  (_                  
    |    | \  \__/ \__ |__ __) __)                 
                                                   
                         __   __       __      ___ 
    |\/|  /\  |\ |  /\  / _  |_  |\/| |_  |\ |  |  
    |  | /--\ | \| /--\ \__) |__ |  | |__ | \|  |  
                                                   
    Processes will be scheduled according to priority. Priorities range from 1 to
    10. 1 is the highest. To prevent against high priority processes starving low
    priority processes, you are to implement dynamic priorities. At the expiration
    of each quantum, the scheduler can decrease the priority of the current 
    running process (thereby penalizing it for taking that much CPU time). 
    Eventually its priority will fall below that of the next highest process and
    that process will be allowed to run.
    
    Another approach is to have the scheduler keep track of low priority
    processes that do not get a chance to run and increase their priority so that
    eventually the priority will be high enough so that the processes will get
    scheduled to run. Once it runs for its quantum, the priority can be brought
    back to the previous low level.
    
    This periodic boosting of a process’ priority to ensure it gets a chance to
    run is called process aging. A simple way to implement aging is to simply
    increase every process’ priority and then have them get readjusted as they
    execute.
*/
void initialise_tasking()
{
    return;
}

    // Initialises the tasking system. Any data structures or initialization of
    // the system that is necessary before your first call to fork() must be
    // done in this function. For example, establishing your ready queue.


int fork()
{
    return NULL;
}

    // Forks the current process, spawning a new one with a different
    // memory space. For the parent, the return value is the pid of
    // the new process. For the child, the return value is 0.


void exit()
{
    return;
}

    // Terminates the current process, cleaning up the resources allocated
    // to the process.


void yield()
{
    return;
}

    // Causes the process to surrender the CPU. The result is that the process
    // with the highest priority is assigned the CPU. Note: This may be the
    // current process *if* it is the highest priority.


int sleep(unsigned int secs)
{
    return NULL;
}

    // Causes the process to surrender the CPU and go to sleep for n seconds.
    // The function returns 0 if the requested time has elapsed, or the 
    // number of seconds left to sleep, if the call was interrupted by a
    // signal handler.


int getpid()
{
    return NULL;
}

    // Returns the pid of the current process.

int setpriority(int pid, int new_priority)
{
    return NULL;
}

    // Set the priority of the process. pid is the process id returned by
    // getpid(). newpriority is the new priority value between 1 and 10,
    // where 1 is highest priority. The return value is the resulting
    // priority of the pid. If the pid is invalid then the return value is 0.
    
/*  ##############################################################################
     __   __   __   __  __  __  __                                  
    |__) |__) /  \ /   |_  (_  (_                                   
    |    | \  \__/ \__ |__ __) __)                                  
                                                                    
     __           __       __   __         ___      ___    __       
    (_  \_/ |\ | /   |__| |__) /  \ |\ | |  _/  /\   |  | /  \ |\ | 
    __)  |  | \| \__ |  | | \  \__/ | \| | /__ /--\  |  | \__/ | \| 

    A semaphore must be initialized by open_sem() before it can be used.
    Processes waiting on a semaphore are resumed on a first-come first-served
    basis.
*/
int open_sem(int n)
{
    return NULL;
}

    // n is the number of processes that can be granted access to the critical
    // region for this semaphore simultaneously. The return value is a semaphore
    // identifier to be used by signal and wait. 0 indicates open_sem failed.


int wait(int s)
{
    return NULL;
}

    // The invoking process is requesting to acquire the semaphore, s. If the
    // internal counter allows, the process will continue executing after acquiring
    // the semaphore. If not, the calling process will block and release the
    // processor to the scheduler. Returns semaphore id on success of acquiring
    // the semaphore, 0 on failure.


int signal(int s)
{
    return NULL;
}

    // The invoking process will release the semaphore, if and only if the process
    // is currently holding the semaphore. If a process is waiting on
    // the semaphore, the process will be granted the semaphore and if appropriate
    // the process will be given control of the processor, i.e. the waking process
    // has a higher scheduling precedence than the current process. The return value
    // is the seamphore id on success, 0 on failure.


int close_sem(int s)
{
    return NULL;
}

    // Close the semaphore s and release any associated resources. If s is invalid then
    // return 0, otherwise the semaphore id.

/*  ##############################################################################
           ___  __  __   __   __   __   __  __  __  __      
    | |\ |  |  |_  |__) |__) |__) /  \ /   |_  (_  (_       
    | | \|  |  |__ | \  |    | \  \__/ \__ |__ __) __)      
                                                            
     __  __                         __      ___    __       
    /   /  \ |\/| |\/| /  \ |\ | | /    /\   |  | /  \ |\ | 
    \__ \__/ |  | |  | \__/ | \| | \__ /--\  |  | \__/ | \|

    pipes are first-in-first-out bounded buffers. Elements are read in the same
    order as they were written. When writes overtake reads, the first unread
    element will be dropped. Thus, ordering is always preserved.
    "read" and "write" on pipes are atomic, i.e., they are indivisible, and
    they are non-blocking. All pipes are of the same size.
*/
#define INVALID_PIPE -1

int open_pipe(){
    return NULL;
}

    // Initialize a new pipe and returns a descriptor. It returns INVALID_PIPE
    // when none is available.


unsigned int write(int fildes, const void *buf, unsigned int nbyte)
{
    return NULL;
}

    // Write the first nbyte of bytes from buf into the pipe fildes. The return value is the
    // number of bytes written to the pipe. If the pipe is full, no bytes are written.
    // Only write to the pipe if all nbytes can be written.


unsigned int read(int fildes, void *buf, unsigned int nbyte)
{
    return NULL;
}

    // Read the first nbyte of bytes from the pipe fildes and store them in buf. The
    // return value is the number of bytes successfully read. If the pipe is 
    // invalid, it returns -1.
    
    
int close_pipe(int fildes)
{
    return NULL;
}

    // Close the pipe specified by fildes. It returns INVALID_PIPE if the fildes
    // is not valid.


