#include "schedule.h"

int jobs_quantity = 64;

void pr(void){printf("this is a thread test function\n");}

JobScheduler* initialize_scheduler(int execution_threads){
	JobScheduler * Scheduler = malloc(sizeof(JobScheduler));
	Scheduler->execution_threads = execution_threads;
	Scheduler->queue = malloc(sizeof(queue));
	return Scheduler;
}

void submit_job( JobScheduler* sch, job* j){

}

void execute_all_jobs( JobScheduler* sch){

}

void wait_all_tasks_finish( JobScheduler* sch){ //waits all submitted tasks to finish
}

OK_SUCCESS destroy_scheduler( JobScheduler* sch){


}
