/*
* File: lab4.c
* Author: Bradley Duguay
* Date: 2026/01/29
* Description: This lab goes over control of system peripherals,
* in this case keyboard LED's in the startup terminal interface
*/
#include <stdio.h>
#include <stdlib.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <unistd.h>

void main(){
	int escape = 9; //key to press to end the program
	int arr[] = {0,1,4,5,2,3,6,7};//array for bit value order of keyboard LED's as they are not in the "right" order
	int num;
do{
	scanf("%d",&num);
	if(num !=escape){
		ioctl (1,KDSETLED,arr[num]);
		}	
	}while (num !=escape);
}
