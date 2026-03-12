/*
 * file:odd.c
 * author:Bradley Duguay W0516067
 * date:March 7th 2026
 * description:file designed to be used in conjunction with even.c
 *             prints out an odd number, waits until the flag for even.c's
 *             print command appears, then prints out the next odd number
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int i;
  FILE *id = NULL;

  for (i = 0; i < 60; i = i + 1) {

    if ((i & 1) == 1) {  // Checks if i is odd
      printf("%d\n", i); // \n terminates line and flushes buffer //
      id = fopen("1", "w");
      fclose(id);

    } else {

      do {
        id = fopen("0", "r"); // checks for the even.c activation flag,
                              // if it exists, do not print the next even number
        if (id == NULL) {
          
          sleep(1);
        } else {
          fclose(id);
          remove("0");
        }
      } while (id == NULL);
    }

    sleep(1);
  }
}
