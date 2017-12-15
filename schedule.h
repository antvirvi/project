#include <pthread.h>
#include <errno.h>
#include "libraries.h"
#include <sys/types.h>

typedef void(*Operation)(char * text); //a pointer to a function returning void, and taking a string as param

typedef struct Job{
	char * text;
	Operation opt;
}Job;

typedef struct Queue{
	int queue_capacity;
	int queue_used;
	Job * jobs;
}Queue;


typedef struct JobScheduler{
int execution_threads; // number of execution threads
Queue* q; // a queue that holds submitted jobs / tasks
pthread_t* tids; // execution threads
//....
// mutex, condition variable, ...
}JobScheduler;

pthread_mutex_t lock;
pthread_cond_t condition_var;

void pr(char * str);

JobScheduler* initialize_scheduler( int execution_threads);

void submit_job( JobScheduler* sch, Job* j);

void execute_all_jobs( JobScheduler* sch);

void wait_all_tasks_finish( JobScheduler* sch); //waits all submitted tasks to finish

//OK_SUCCESS destroy_scheduler( JobScheduler* sch);
