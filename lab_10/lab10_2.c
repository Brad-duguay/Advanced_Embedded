#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

#define thrd_1 0
#define thrd_2 1
#define r_amt 500
#define upper 10000
#define lower 1
#define divider 1000

int main() {
  //////////////////////
  // FILE IDs //
  // ID [0] = READ //
  // ID [1] = WRITE //
  //////////////////////
  int id[2];
  int pid;
  int error;
  float arr[r_amt];
  pid = fork();

  error = pipe(id);
  if (error < 0) {
    printf("error thread creation \n");
    return 1;
  }

  if (pid < 0) {
    printf("error thread creation \n");
    return 1;
  }

  if (pid == 0) { // child thread
    srand(time(NULL));
    for (int i = 0; i <= r_amt; i++) {
      arr[i] = (lower + rand()) % upper;
      printf("%f from child \n", arr[i]);
      printf("%ld arr size \n", sizeof(arr));

      write(id[1], arr, sizeof(arr));
      /////////////////////////////////////
      // WRITE THE MESSAGE TO THE PIPE //
      /////////////////////////////////////
	  memset (arr, '\0', sizeof (arr[i]));
      usleep(10);
    }

  } else { // parent thread

    //////////////////////////////////////
    // READ THE MESSAGE FROM THE PIPE //
    //////////////////////////////////////
    float stand;
    float mean = 0;
    float dec[r_amt];

    for (int i = 0; i < r_amt; i++) {

      read(id[0], dec, sizeof(dec));
      dec[i] = dec[i] / divider;
      mean = mean + dec[i];
      memset (arr, '\0', sizeof (arr[i]));

    }

    mean = mean / r_amt;

    for (int i = 0; i < r_amt; i++) {
      stand += pow((dec[i]) - mean, 2);
    }

    stand = sqrt(stand / r_amt);
    printf("mean = %.3lf\n", mean);
    printf("SD = %.3lf\n", stand);
    exit(0);
  }
}
