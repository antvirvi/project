#ifndef _SCHEDULEH_
#define _SCHEDULEH_
#include <pthread.h>
#include <errno.h>
#include "libraries.h"
#ifndef  _FUNCTIONSH_
#include "functions.h"
#endif
#include <sys/types.h>

typedef void(*Operation)(void); //a pointer to a function returning void, and taking a string as param
void * get_a_job(void* queue);
struct hash_layer;

typedef struct Job{
	struct hash_layer *hash;
	char **words;
	int number_of_words;
	topk *top;
	Operation opt;
}Job;

typedef struct Queue{
	int queue_capacity;
	int queue_used;
	int queue_ptr;
	Job * jobs;
}Queue;


typedef struct JobScheduler{
int execution_threads; // number of execution threads
Queue* q; // a queue that holds submitted jobs / tasks
pthread_t* tids; // execution threads
//....
// mutex, condition variable, ...
}JobScheduler;

pthread_mutex_t T; //stop other threads
pthread_mutex_t R; //ready for submit jobs
pthread_mutex_t L; //

pthread_cond_t tcv;
pthread_cond_t rcv;
pthread_cond_t lcv;

void pr(void);

JobScheduler* initialize_scheduler( int execution_threads);

void submit_job( JobScheduler* sch, Job* j);

void execute_all_jobs( JobScheduler* sch);

void wait_all_tasks_finish( JobScheduler* sch); //waits all submitted tasks to finish

//OK_SUCCESS destroy_scheduler( JobScheduler* sch);
#endif
