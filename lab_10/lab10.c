/*
* File: lab10.c
* Author: Bradley Duguay W0516067
* Date: 2026/03/22
* Description: 
*/
#include <math.h>
#include <stdlib.h>
#include <threads.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define thrd_1 0
#define thrd_2 1
#define r_amt 500
#define upper 10000
#define lower 1
#define divider 1000

int id [2];

int gen(void *arg);
int std(void *arg);

int main(){
	int vals[r_amt];
	thrd_t thread [2];
	if(thrd_create(&thread[thrd_1], gen, &vals)){
		printf("error thread 1 creation \n");
		return 1;
	}
	if(thrd_create(&thread[thrd_2], std, &vals)){
		printf("error thread 2 creation \n");
		return 1;
	}
	for(int j = 0; j <= 1; j++){
		thrd_join(thread[j], &vals[j]);
	}
	
	return 0;	
}
/*
* Description: generates random number values
* @param a: void ptr to int array
* side effects: creates two random characters before printing mean and standard deviation
*/
int gen(void *arg){
	int *arr = arg;
	srand(time(NULL));
		
	for(int i = 0; i <= r_amt; i++){
		arr[i] = (lower + rand()) % upper;
	}
	ssize_t data = write(id[1],arr,sizeof(arr));
	if (data == -1) {
		printf("Thread 1: Error writing to pipe\n");
		return  1;
	}
	close(id[1]);
	return 0;
}
/*
* Description: calculates mean and standard deviation from generated numbers
* @param a: void ptr to int array
* side effects: creates random characters before printing mean and standard deviation
*/
int std(void *arg){
	
	int *d_int = arg;
	float stand;
	float mean = 0;
	float d_fl[r_amt];
	ssize_t r_data = read(id[0], d_int, sizeof(d_int));
		if (r_data == -1) {
			printf("Thread 2: Error reading from pipe\n");
			return  1;
		}
	for(int i = 0; i < r_amt; i++){
		d_fl[i] = d_int[i]/divider;
		mean = mean + d_fl[i];
	}
	mean = mean / r_amt;
	
	for(int i = 0; i < r_amt; i++){
		stand += pow((d_fl[i]) - mean, 2);
	}
	stand = sqrt(stand / r_amt);
	
	printf("mean = %.3lf\n", mean);
	printf("SD = %.3lf\n", stand);
	close(id[0]);
	return 0;
}
