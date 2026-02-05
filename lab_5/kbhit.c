// P11 - PROGRAM KBHIT //
// DEMONSTRATION OF FLAG CHECKED, KEYBOARD //
// INTERRUPTS IN C //
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

// USE PROTOTYPES IF FUNCTIONS ARE PLACED AFTER MAINLINE //
void changemode(int);
int kbhit(void);
int main(void) {
 int ch;
 // SET KEYBOARD DRIVER TO NOT //
 // WAIT FOR RESPONSE //
 changemode(1);
 // EXECUTE AS LONG AS A //
 // KEY IS NOT PRESSED //
 while (!kbhit()) {
 // NORMALLY MAIN CODE GOES HERE //
 putchar('.');
 }
 // IF YOU WISH YOU CAN GET THE //
 // KEY PRESSED FROM THE BUFFER //
 ch = getchar();
 printf("\nGot %c\n", ch);
 // RETURN KEYBOARD TO //
 // NORMAL OPERATION //
 changemode(0);
}
// FUNCTION CHANGEMODE //
// TOGGLE OPERATION OF KEYBOARD FROM FLAG //
// CHECKING OPERATION TO WAITING FOR INPUT //
// TO COMPLETE. //
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
