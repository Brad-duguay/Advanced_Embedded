// P12 - PROGRAM TIMING //
// DEMONSTRATION OF TIMING OPERATIONS //
#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>
int main(void) {
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
 printf("Thousandths of a second: %d\n", T2 - T1);
 }
 return 0;
}
