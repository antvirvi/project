#include "schedule.h"
#include <unistd.h>
#include <sys/wait.h>

extern int threads_quantity;

//στην function.c tha metraw posa Q exw kai tha kanw malloc Q*sizeof Job mia fora stin functions. tha pernaw ayton ton pinaka stin submit jobs kai ola cool.

void * get_a_job(void* queue) {
Queue * q;
q = (Queue*)queue;
int my_job;
void *args;
pthread_t self;
self=pthread_self();
printf("I have pid %d\n",self);
//pthread_exit(NULL);
	while(1)
	{		
			printf("waiting to be full\n");
			pthread_mutex_lock(&pool_mutex);
			while(q->queue_used==0){
				q->queue_ptr=0;
				printf("broadcasted empty\n");
				pthread_cond_broadcast(&pool_is_empty);
				pthread_cond_wait(&pool_is_full,&pool_mutex);
				//pthread_mutex_lock(&pool_mutex);
			}
			printf("done waiting\n");
			if(q->queue_used>0){
				my_job=q->queue_ptr;
				printf("Queue used %d\n",q->queue_used);
				q->queue_used--;
				q->queue_ptr++;
				pthread_mutex_unlock(&pool_mutex);
				pthread_cond_broadcast(&pool_is_full);
			 	args=(void *)q->jobs[my_job]->arguments;
				q->jobs[my_job]->opt(args);
				free(q->jobs[my_job]->arguments);
				free(q->jobs[my_job]);
				printf("Done job = %d\n",my_job);		
				//return NULL;
				}
			else if(q->queue_used==-1){
				printf("Thread dies\n");
				fflush(stdout);
				printf("%d unlocked pool\n",self);	 
				pthread_mutex_unlock(&pool_mutex);
				pthread_cond_broadcast(&pool_is_full);
				pthread_exit(0);
				return 0;
				}
			else {printf("ODD with %d\n",q->queue_used);}

		

	}
}
//void pr(char * str){printf("Test pr %s\n",str);}

void pr(void){printf("TEST PRINT\n");}


JobScheduler* initialize_scheduler(int execution_threads){
	int i;
	//printf("Start JS init\n");
	pthread_mutex_init(&pool_mutex, NULL);
	pthread_cond_init(&pool_is_empty, NULL);
	//pthread_mutex_init(&R, NULL);
	pthread_cond_init(&pool_is_full, NULL);

	JobScheduler * Scheduler = malloc(sizeof(JobScheduler));
	Scheduler->execution_threads = execution_threads;
	Scheduler->q = malloc(sizeof(Queue));
	Scheduler->q->queue_capacity = 20;
	Scheduler->q->queue_used = 0;
	Scheduler->q->queue_ptr = 0;
	Scheduler->q->jobs = malloc(Scheduler->q->queue_capacity*sizeof(Job * ));
	Scheduler->tids = malloc(execution_threads*sizeof(pthread_t*));
	//printf("Thread %d\n",execution_threads);
	for(i=0;i<execution_threads;i++){
//	printf("sharknado5_ %p\n",&Scheduler->q);
		Scheduler->tids[i] = pthread_create(&Scheduler->tids[i], NULL, get_a_job, Scheduler->q);
	}
	//printf("End JS init\n");
	pthread_mutex_lock(&pool_mutex);
	return Scheduler;
}

void extend_queue(Queue * q){
	q->queue_capacity *=2;
	q->jobs = realloc(q->jobs,q->queue_capacity*sizeof(Job ));
}

void submit_job(JobScheduler* sch, Job* j){
	//printf("Submit Job init\n");\
	//j->opt();
	if(sch->q->queue_capacity==sch->q->queue_used)
		extend_queue(sch->q);
//	sch->q->jobs[sch->q->queue_used] = malloc(sizeof(Job));
	Job *temp_job=malloc(sizeof(Job));
	memcpy(temp_job,j,sizeof(Job));
	sch->q->jobs[sch->q->queue_used]=temp_job;
	q_args *args=(q_args*)temp_job->arguments;
	char **ptr=*(args->words);
	//printf("submited job with first word %s in %d\n",ptr[0],sch->q->queue_used);
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
	//pthread_mutex_lock(&T);
	sch->q->queue_used = -1;
	int i;
	printf("signal to destroy \n");
	pthread_mutex_unlock(&pool_mutex);
	printf("main unlocked pool\n");
	pthread_cond_broadcast(&pool_is_full);
	int error;
	for(i=0;i<sch->execution_threads;i++)
		error=pthread_join(sch->tids[i], NULL);
		if(error!=0){
			printf("ERROR %d\n",error);		
		}

	
	//free(sch);
	//printf("Destroy threads end\n");
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
	//printf("main unlocked T\n");
	pthread_mutex_unlock(&pool_mutex);
	printf("main unlocked pool\n");
	printf("broadcasting full\n");
	pthread_cond_broadcast(&pool_is_full);
	printf("main waits for empty pool\n");
	pthread_cond_wait(&pool_is_empty,&pool_mutex);
	//	if(pthread_mutex_trylock(&pool_mutex)==0){
	//		printf("pool lock aquired\n");
	//		break;
	//}
	//printf("wait all tasks finish end\n");
	printf("wait all tasks finish end\n");

}

