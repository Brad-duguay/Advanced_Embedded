#include <stdio.h>
#include <stdlib.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <unistd.h>




void main(){
	int escape = 9;
	int arr[] = {0,1,4,5,2,3,6,7};//array for bit value order of keyboard LED's as they are not in the "right" order
	int num;

do{
	scanf("%d",&num);
	if(num !=escape){
		ioctl (1,KDSETLED,arr[num]);
		}
	
	}while (num !=escape);
}
