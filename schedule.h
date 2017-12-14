#include <pthread.h>
#include <sys/types.h>

#ifndef OK_SUCCESS
#define OK_SUCCESS 112
#endif
typedef struct job{
	char * query;
	int version;
}job;

typedef struct queue{
	int queue_capacity;
	int queue_used;
	job * jobs;
}queue;


typedef struct JobScheduler{
int execution_threads; // number of execution threads
queue* q; // a queue that holds submitted jobs / tasks
p_thread_t* tids; // execution threads
//....
// mutex, condition variable, ...
}JobScheduler;

JobScheduler* initialize_scheduler( int execution_threads);

void submit_job( JobScheduler* sch, job* j);

void execute_all_jobs( JobScheduler* sch);

void wait_all_tasks_finish( JobScheduler* sch); //waits all submitted tasks to finish

OK_SUCCESS destroy_scheduler( JobScheduler* sch);
