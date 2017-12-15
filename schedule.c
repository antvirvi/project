#include "schedule.h"

extern int threads_quantity;



void * get_a_job(void* q) {
//	while(1){

printf("a function in a thread %.8x : %s %d\n",(char *)q);
//	}
	return NULL;
}

void pr(char * str){printf("Test pr %s\n",str);}

JobScheduler* initialize_scheduler(int execution_threads){
	int i;
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&condition_var, NULL);
	printf("Start JS init\n");
	JobScheduler * Scheduler = malloc(sizeof(JobScheduler));
	Scheduler->execution_threads = execution_threads;
	Scheduler->q = malloc(sizeof(Queue));
	Scheduler->q->queue_capacity = 20;
	Scheduler->q->queue_used = 0;
	Scheduler->q->jobs = malloc(Scheduler->q->queue_capacity*sizeof(Job));
	Scheduler->tids = malloc(execution_threads*sizeof(pthread_t*));
//	char * str = "Margarita";
	for(i=0;i<execution_threads;i++){
		Scheduler->tids[i] = pthread_create(&Scheduler->tids[i], NULL, get_a_job, "margarita");
	}
	printf("End JS init\n");
	return Scheduler;
}

void extend_queue(Queue * q){
	q->queue_capacity *=2;
	q->jobs = realloc(q->jobs,q->queue_capacity*sizeof(Job));
}

void submit_job( JobScheduler* sch, Job* j){
	printf("Submit Job init\n");
	if(sch->q->queue_capacity==sch->q->queue_used)
		extend_queue(sch->q);
	sch->q->jobs[sch->q->queue_used] = *j;
	sch->q->queue_used++;
	printf("Submit Job end\n");
}

void execute_all_jobs( JobScheduler* sch){
	printf("Execute Jobs init\n");
	initialize_scheduler(threads_quantity);
	printf("Execute Jobs end\n");
}

void wait_all_tasks_finish( JobScheduler* sch){ //waits all submitted tasks to finish
}
/*
OK_SUCCESS destroy_scheduler( JobScheduler* sch){


}
*/
