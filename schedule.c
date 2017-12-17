#include "schedule.h"

extern int threads_quantity;

//στην function.c tha metraw posa Q exw kai tha kanw malloc Q*sizeof Job mia fora stin functions. tha pernaw ayton ton pinaka stin submit jobs kai ola cool.

void * get_a_job(void* queue) {
Queue * q;
q = (Queue *) queue;


	while(1)
	{
		printf("thread\n");
		Job * j;
		//	printf("Cap %d\n",q->queue_capacity);
		if(!pthread_mutex_lock(&lock)){
		pthread_cond_wait(&condition_var,&lock);
			if(q->queue_used>0){
				*j = q->jobs[q->queue_ptr];
//				j->opt(j->hash,j->words,j-> number_of_words,j->top);
//				j->opt(j->hash,j->words,j->number_of_words,j->top);
				j->opt;
				q->queue_used--;
				q->queue_ptr++;
				pthread_mutex_unlock(&lock);
				pthread_cond_signal(&condition_var);
return NULL;
			}
			else {
				printf("Thread dies\n");
				pthread_mutex_unlock(&lock);
				pthread_cond_signal(&condition_var);
				return NULL;
			}

		}
	}
}

//void pr(char * str){printf("Test pr %s\n",str);}

void pr(void){printf("TEST PRINT\n");}


JobScheduler* initialize_scheduler(int execution_threads){
	int i;
	printf("Start JS init\n");
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&condition_var, NULL);
	JobScheduler * Scheduler = malloc(sizeof(JobScheduler));
	Scheduler->execution_threads = execution_threads;
	Scheduler->q = malloc(sizeof(Queue));
	Scheduler->q->queue_capacity = 20;
	Scheduler->q->queue_used = 0;
	Scheduler->q->queue_ptr = 0;
//	printf("sharknado %d %p\n",Scheduler->q->queue_used,(void *)&Scheduler->q->queue_used);
	Scheduler->q->jobs = malloc(Scheduler->q->queue_capacity*sizeof(Job ));
	Scheduler->tids = malloc(execution_threads*sizeof(pthread_t*));
//	char * str = "Margarita";
	for(i=0;i<2;i++){
		Scheduler->tids[i] = pthread_create(&Scheduler->tids[i], NULL, get_a_job, Scheduler->q);
	}
	//pthread_cond_signal(&condition_var);
	printf("End JS init\n");
	return Scheduler;
}

void extend_queue(Queue * q){
	q->queue_capacity *=2;
	q->jobs = realloc(q->jobs,q->queue_capacity*sizeof(Job ));
}

void submit_job(JobScheduler* sch, Job* j){
	printf("Submit Job init\n");
	if(sch->q->queue_capacity==sch->q->queue_used)
		extend_queue(sch->q);
//	sch->q->jobs[sch->q->queue_used] = malloc(sizeof(Job));
//	printf("sharknado_ %d %p\n",sch->q->queue_used,(void *)&sch->q->queue_used);
	sch->q->jobs[sch->q->queue_used] = *j;
	sch->q->queue_used++;
	printf("Submit Job end\n");
}

void execute_all_jobs( JobScheduler* sch){
	printf("Execute Jobs init\n");
	printf("Submitted jobs: %d\n",sch->q->queue_used);
	return ;
	pthread_cond_signal(&condition_var);
//	initialize_scheduler(threads_quantity);
	printf("Execute Jobs end\n");
}

void wait_all_tasks_finish( JobScheduler* sch){ //waits all submitted tasks to finish
}
/*
OK_SUCCESS destroy_scheduler( JobScheduler* sch){


}
*/