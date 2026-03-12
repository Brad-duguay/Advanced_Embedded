/*File:Submission.c
 *Author:Dennis Meyers
 *Last Edited: Bradley Duguay
 *Date: Feburary 15th 2026
 *Description: Keyboard LED Cycling
 */


#include <linux/kd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

void changemode(int);
int kbhit(void);
void time_delay();

struct timeb t;

int main(void) {
// long long start_time;
// ftime(&t);
// start_time = t.millitm;
 int ch = 'l';
 int bitval = 1; // values used are 1, 2 & 4 to control LEDS

 changemode(1);
 while (ch != 'q'){
	 while (!kbhit()) {
		time_delay();
		printf("out of time loop\n");
		if (ch == 'l'){
			bitval = bitval >> 1;
			if (bitval > 4){
				bitval = 1;
			}
			printf("led #%x",bitval);
			ioctl(1,KDSETLED,bitval);
		}
		else if (ch == 'r'){
			bitval = bitval << 1;
			if (bitval < 1){
				bitval = 4;
			}
			printf("led #%x",bitval);
			ioctl(1,KDSETLED,bitval);
		}
	 }

	 ch = getchar();
	 printf("\nGot %c\n", ch);
	 if(ch == 'q'){
	 	printf("Quitting program");
	 }
 	 changemode(0);
 }
 return 0;
}


void changemode(int dir) {
 static struct termios oldt, newt;
 if (dir == 1) {
 tcgetattr(STDIN_FILENO, &oldt);
 newt = oldt;
 newt.c_lflag &= ~(ICANON | ECHO);
 tcsetattr(STDIN_FILENO, TCSANOW, &newt);
 } else {
 tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
 }
}

void time_delay(){
 long long starttime;
ftime(&t);
starttime = t.millitm;
 long long time_since;
 do{
// printf("%f/n",start_time);
 ftime(&t);
 
 time_since = t.millitm;
 }while(time_since - starttime < 500);
}

int kbhit(void) {
 struct timeval tv;
 fd_set rdfs;
 tv.tv_sec = 0;
 tv.tv_usec = 0;
 FD_ZERO(&rdfs);
 FD_SET(STDIN_FILENO, &rdfs);
 select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
 return FD_ISSET(STDIN_FILENO, &rdfs);
}
