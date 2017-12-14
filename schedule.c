#include "schedule.h"

extern int threads_quantity;

void * get_a_job(void* q) {
//	while(1){
printf("a function in a thread\n");
//	}
	return NULL;
}

JobScheduler* initialize_scheduler(int execution_threads){
	int i;
	JobScheduler * Scheduler = malloc(sizeof(JobScheduler));
	Scheduler->execution_threads = execution_threads;
	Scheduler->q = malloc(sizeof(Queue));
	Scheduler->q->queue_capacity = 20;
	Scheduler->q->queue_used = 0;
	Scheduler->q->jobs = malloc(Scheduler->q->queue_capacity*sizeof(Job));
	Scheduler->tids = malloc(execution_threads*sizeof(pthread_t*));
//	char * str = "Margarita";
	for(i=0;i<execution_threads;i++){
		Scheduler->tids[i] = pthread_create(&Scheduler->tids[i], NULL, get_a_job, NULL);
	}
	return Scheduler;
}

void extend_queue(Queue * q){
	q->queue_capacity *=2;
	q->jobs = realloc(q->jobs,q->queue_capacity*sizeof(Job));
}

void submit_job( JobScheduler* sch, Job* j){

	if(sch->q->queue_capacity==sch->q->queue_used)
		extend_queue(sch->q);
	sch->q->jobs[sch->q->queue_used] = j;
	sch->q->queue_used++;
}

void execute_all_jobs( JobScheduler* sch){
	initialize_scheduler(threads_quantity);
}

void wait_all_tasks_finish( JobScheduler* sch){ //waits all submitted tasks to finish
}
/*
OK_SUCCESS destroy_scheduler( JobScheduler* sch){


}
*/
