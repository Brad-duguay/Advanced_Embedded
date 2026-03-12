// P11 - PROGRAM KBHIT //
// DEMONSTRATION OF FLAG CHECKED, KEYBOARD //
// INTERRUPTS IN C //
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>

#include <linux/kd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

// USE PROTOTYPES IF FUNCTIONS ARE PLACED AFTER MAINLINE //
void changemode(int);
int kbhit(void);
int num = 0;
int bitval = 0x01;
int main(void) {
  int ch='l';

  changemode(1);
    int i, T1, T2;
    struct timeb t;
    ftime(&t);
    // Loop for 2 times for a change in the measurement //
    T1 = t.millitm;
    for (i = 1; i < 3; i = i + 1) {
      ftime(&t);
      T2 = t.millitm;
      // ACCOUNT FOR T2 ROLL OVER //
      if (T2 < T1) {
        T2 = T2 + 1000;
      }
    } // maytbe delete this bracket


  while (ch != 'q') {

  do{
	if(ch == 'l'){
  changemode(1);
      ioctl(1, KDSETLED, (bitval<<=1));
      sleep(1);
      if (!bitval & 8) {
        bitval = 0x01;
      }
  }
  else if (ch == 'r') {

  changemode(1);
      ioctl(1, KDSETLED, (bitval>>=1));
      sleep(1);
      if (!bitval & 8) {
       bitval = 0x03;
      }
}
 ch = getchar();
  }while(ch !='q');

  } if (ch == 'q') {

    printf("\nGot %c\n", ch);
    changemode(0);
    return 0;
     }
    
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
// FUNCTION KBHIT //

// CHECK SYSTEM FLAG FOR KEY PRESS. //
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
