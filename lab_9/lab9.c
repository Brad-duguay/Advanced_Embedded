/*
* File: lab9.c
* Author: Bradley Duguay W0516067
* Date: 2026/03/15
* Description: 
*/
#include <math.h>
#include <stdlib.h>
#include <threads.h>
#include <stdio.h>
#include <unistd.h>

#define row 3
#define collumn 3
#define thrd_amt 3

int calc(void *arg);

//mtx_t* mutex;
typedef struct _matrice {
  int matrix_a[row][collumn];
  int matrix_b[row];
  int ans[row];
}matrice;
	matrice m1 = {{{1,2,3},{0,-1,1},{3,2,1}}, {1, 3, 2}};
	

int main(){	


//	mtx_init(mutex, mtx_plain);

	thrd_t thread[thrd_amt];

	for(int i = 0; i < thrd_amt; i++){
		
		if(thrd_create(&thread[i],calc,&i)){
			printf("error thread creation \n");
			return 1;
		}
		sleep(1);
	}
	for(int j = 0; j < thrd_amt; j++){
		thrd_join(thread[j], &m1.ans[j]);
	}
	printf("result = \n[%d\n%d\n%d]",m1.ans[0],m1.ans[1],m1.ans[2]);
	//sleep(3);
	return 0;
}
/*
* Description: multiplies the array/matrix values together
* @param a: the row of matrix A/the thread associated with the calculation
* 	//void ptr to the argument in thrd_create necessary for function to operate
* @return: the product of the matrix multiplication
* side effects: N/A
*/
int calc(void *arg){
//	mtx_lock(mutex);
	int id = *(int*)arg;
	int result = 0;
	for(int i = 0; i < collumn; i++){
		result += (m1.matrix_a[id][i])*(m1.matrix_b[i]);
	//	printf("%d\n",result);
		//m1.ans[i] = result;
	}
//	mtx_unlock(mutex);//mutex is an issue
	sleep(1);
	return result;
}
