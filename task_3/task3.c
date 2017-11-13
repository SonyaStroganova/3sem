
//a = 2147483647;
#include <pthread.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h> 


struct int_params{
	float x1, x2, y;
	int n_points;
	int res_id;
	int shmid;
};

void integral(struct int_params *params){
	printf("integral begin ... %d\n", params->res_id);
	float x1 = params->x1;
	float x2 = params->x2;
	float y  = params->y;
	int n_points = params->n_points;
	float rx, ry, s1 = 0;
	for(int i=0; i<n_points; i++) {
		rx = (rand()/2147483647.)*(x2-x1) + x1;
		ry = (rand()/2147483647.)*(y);
		if(ry <= rx*rx) s1++;
	}
	float *array = (float*)shmat(params->shmid, NULL, 0);

	array[0] = ((float)s1)/n_points;
	printf("Thread #%d result %f\n", params->res_id, array[0]);

	return NULL;
}

int main()
{
	int N_threads, n_points;
	float x1, x2, y;
	printf("введите N нитей, n точек\n");
	scanf("%d %d", &N_threads, &n_points);
	printf("введите x1, x2, y\n");
	scanf("%f %f %f", &x1, &x2, &y);

	pthread_t tids[100];
	struct int_params params[100];
	int shmid[100];
	for(int i = 0; i < N_threads; i++){
		params[i].x1 = x1;
		params[i].x2 = x2;
		params[i].y  = y;
		params[i].n_points  = n_points;
		params[i].res_id = i;
		shmid[i] = shmget(IPC_PRIVATE, sizeof(float), 0666|IPC_CREAT);
		params[i].shmid = shmid[i];
		pthread_create(&(tids[i]), NULL, (void*)&integral, &(params[i]));
	}
	for(int i = 0; i < N_threads; i++) pthread_join(tids[i], NULL);

	float total_result = 0;
	for(int i = 0; i < N_threads; i++){
		float *array = (float*)shmat(shmid[i], NULL, 0);
		printf("CORE#%d RESULT = %f\n",i,*array);
		total_result += *array;
	} 

	printf("TOTAL RESULT = %f\n",total_result/N_threads);
	return 0;

}


