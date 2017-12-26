#include "schedule.h"
#include <unistd.h>
#include <sys/wait.h>

extern int threads_quantity;

//στην function.c tha metraw posa Q exw kai tha kanw malloc Q*sizeof Job mia fora stin functions. tha pernaw ayton ton pinaka stin submit jobs kai ola cool.

void * get_a_job(void* queue) {
Queue * q;
q = (Queue*)queue;
	while(1)
	{ printf(GREEN"Panos 1\n"RESET);
		if(!pthread_mutex_lock(&T)){
			pthread_cond_wait(&tcv,&T);
			if(q->queue_used==0) {

//				printf("0\n");
				q->queue_ptr=0;	
				pthread_mutex_unlock(&R);
				//printf("R unlocked\n");
				pthread_cond_signal(&rcv); //it unlocks the R mtx-cv and not T so that only main can run and not any thread 	
//				printf("R signaled\n");
				}
			else if(q->queue_used>0){
				void *args=(void *)q->jobs[q->queue_ptr]->arguments;
				q->jobs[q->queue_ptr]->opt(args);
				printf("ptr = %d\n",q->queue_ptr);
//				q->jobs[q->queue_ptr]->opt();
	//printf("Queue used %d\n",q->queue_used);
				q->queue_used--;
				q->queue_ptr++;
				pthread_mutex_unlock(&T);
				pthread_cond_signal(&tcv);
			 			
				//return NULL;
				}
			else if(q->queue_used==-1){
				printf("Thread dies\n"); 
				pthread_mutex_unlock(&T);
				pthread_cond_signal(&tcv);
				return NULL;
				}
			else {printf("ODD\n");}
printf(RED"Panos 1\n"RESET);
		}

	}
}
//void pr(char * str){printf("Test pr %s\n",str);}

void pr(void){printf("TEST PRINT\n");}


JobScheduler* initialize_scheduler(int execution_threads){
	int i;
	printf("Start JS init\n");
	pthread_mutex_init(&T, NULL);
	pthread_cond_init(&tcv, NULL);
	pthread_mutex_init(&R, NULL);
	pthread_cond_init(&rcv, NULL);

	JobScheduler * Scheduler = malloc(sizeof(JobScheduler));
	Scheduler->execution_threads = execution_threads;
	Scheduler->q = malloc(sizeof(Queue));
	Scheduler->q->queue_capacity = 20;
	Scheduler->q->queue_used = 0;
	Scheduler->q->queue_ptr = 0;
	Scheduler->q->jobs = malloc(Scheduler->q->queue_capacity*sizeof(Job * ));
	Scheduler->tids = malloc(execution_threads*sizeof(pthread_t*));
	printf("Thread %d\n",execution_threads);
	for(i=0;i<execution_threads;i++){
//	printf("sharknado5_ %p\n",&Scheduler->q);
		Scheduler->tids[i] = pthread_create(&Scheduler->tids[i], NULL, get_a_job, Scheduler->q);
	}
	printf("End JS init\n");
	return Scheduler;
}

void extend_queue(Queue * q){
	q->queue_capacity *=2;
	q->jobs = realloc(q->jobs,q->queue_capacity*sizeof(Job ));
}

void submit_job(JobScheduler* sch, Job* j){
	//printf("Submit Job init\n");
	//j->opt();
	if(sch->q->queue_capacity==sch->q->queue_used)
		extend_queue(sch->q);
//	sch->q->jobs[sch->q->queue_used] = malloc(sizeof(Job));
	sch->q->jobs[sch->q->queue_used] = j;

	sch->q->queue_used++;
	//sch->q->jobs[sch->q->queue_used]->opt();
//	printf("Submit Job end %d\n",sch->q->queue_used);
}

void execute_all_jobs( JobScheduler* sch){
	//printf("Execute Jobs init\n");
	//printf("Submitted jobs count: %3d\n",sch->q->queue_used);
	wait_all_tasks_finish();
	//printf("Execute Jobs end\n");
	return ;
}

void destroy_threads( JobScheduler* sch){
	//printf("Destroy threads\n");
//	printf("Submitted jobs count: %3d\n",sch->q->queue_used);
	sch->q->queue_used = -1;
int i;
	pthread_mutex_unlock(&T);
	pthread_cond_signal(&tcv);

	for(i=0;i<sch->execution_threads;i++)
		pthread_join(sch->tids[i], NULL);

	printf("Destroy threads end\n");
	return ;
}

/*
void empty_jobscheduler(JobScheduler * sch){

	int i;
	pthread_mutex_lock(&T, NULL);
	pthread_cond_wait(&tcv, NULL);
	Scheduler->q->queue_used = 0;
	Scheduler->q->queue_ptr = 0;
}
*/
void wait_all_tasks_finish( /*JobScheduler* sch*/ void){ //waits all submitted tasks to finish
printf("wait all tasks finish start\n");

	pthread_mutex_unlock(&T);
	pthread_cond_signal(&tcv);

while(1){
	if(pthread_mutex_trylock(&R)){
	pthread_cond_wait(&rcv,&R); 	//this set of mtx-cv is exclusively for this function, so that it can run only when jobs have finished. 
	printf("wait all tasks finish end\n");
	return ;	
	}
}
//return ;
}
/*
OK_SUCCESS destroy_scheduler( JobScheduler* sch){


}
*/
