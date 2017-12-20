#include "schedule.h"
#include <unistd.h>
#include <sys/wait.h>

extern int threads_quantity;

//στην function.c tha metraw posa Q exw kai tha kanw malloc Q*sizeof Job mia fora stin functions. tha pernaw ayton ton pinaka stin submit jobs kai ola cool.

void * get_a_job(void* queue) {
Queue * q;
q = (Queue*)queue;
//printf(YELLOW"Sharknado  %d\n"RESET,q->queue_used);
	while(1)
	{
//		printf("thread\n");
//		pthread_id_np_t   tid;
//		tid = pthread_getthreadid_np();
//		pthread_t         self;
//		self = pthread_self();
//		pthread_getunique_np(&self, &tid);
// printf("thread_self: %lu\n", pthread_self());
	//	Job * j;
		//	printf("Cap %d\n",q->queue_capacity);
		if(!pthread_mutex_lock(&T)){
			pthread_cond_wait(&tcv,&T);
			//pthread_cond_wait(&proceed_threads,&T);
			printf(RED"Sharknado  %d\n"RESET,q->queue_used);
			if(q->queue_used>0){
				printf("Queue used > 0\n");
				//j = &q->jobs[q->queue_ptr];
//				j->opt(j->hash,j->words,j-> number_of_words,j->top);
//				j->opt(j->hash,j->words,j->number_of_words,j->top);
				q->jobs[q->queue_ptr].opt;
				printf(GREEN"Queue used+ %d\n"RESET,q->queue_used);
				q->queue_used--;
				printf(RED"Queue used- %d\n"RESET,q->queue_used);
				if(q->queue_used==0){

					q->queue_ptr=0;	
					printf(GREEN"++++++++++++++++++++++++++++++++++++++++++++\n"RESET);
					pthread_mutex_unlock(&R);
					pthread_cond_signal(&rcv);
				}				
				else
					q->queue_ptr++;
				pthread_mutex_unlock(&T);
				pthread_cond_signal(&tcv);
				return NULL;
			}
			else {
				printf("Queue used <= 0\n");
				printf("Thread dies\n");
				pthread_mutex_unlock(&T);
				pthread_cond_signal(&tcv);
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
	pthread_mutex_init(&T, NULL);
	pthread_mutex_init(&R, NULL);
	pthread_mutex_init(&L, NULL);
	pthread_cond_init(&tcv, NULL);
	pthread_cond_init(&rcv, NULL);
	pthread_cond_init(&lcv, NULL);
	JobScheduler * Scheduler = malloc(sizeof(JobScheduler));
	Scheduler->execution_threads = execution_threads;
	Scheduler->q = malloc(sizeof(Queue));
	Scheduler->q->queue_capacity = 20;
	Scheduler->q->queue_used = 0;
	Scheduler->q->queue_ptr = 0;
	Scheduler->q->jobs = malloc(Scheduler->q->queue_capacity*sizeof(Job ));
	Scheduler->tids = malloc(execution_threads*sizeof(pthread_t*));
printf("Margarita2 %p %p\n",&Scheduler->q,Scheduler->q);
	for(i=0;i<execution_threads;i++){
//	printf("sharknado5_ %p\n",&Scheduler->q);
		Scheduler->tids[i] = pthread_create(&Scheduler->tids[i], NULL, get_a_job, Scheduler->q);
	}
	//pthread_cond_signal(&tcv);
	printf("End JS init\n");
	return Scheduler;
}

void extend_queue(Queue * q){
	q->queue_capacity *=2;
	q->jobs = realloc(q->jobs,q->queue_capacity*sizeof(Job ));
}

void submit_job(JobScheduler* sch, Job* j){
//	printf("Submit Job init\n");
//	printf("Catnado %p\n",&sch->q);
	if(sch->q->queue_capacity==sch->q->queue_used)
		extend_queue(sch->q);
//	sch->q->jobs[sch->q->queue_used] = malloc(sizeof(Job));
//	printf("sharknado_ %d %p\n",sch->q->queue_used,(void *)&sch->q->queue_used);
	sch->q->jobs[sch->q->queue_used] = *j;
	sch->q->queue_used++;
//	printf("Submit Job end\n");
}

void execute_all_jobs( JobScheduler* sch){
	printf("Execute Jobs init\n");
	printf("Submitted jobs: %d\n",sch->q->queue_used);
	pthread_cond_signal(&tcv);
//	initialize_scheduler(threads_quantity);
	printf("Execute Jobs end\n");
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
void wait_all_tasks_finish( JobScheduler* sch){ //waits all submitted tasks to finish
}
/*
OK_SUCCESS destroy_scheduler( JobScheduler* sch){


}
*/
