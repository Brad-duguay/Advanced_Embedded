/*
 * file:even.c
 * author:Bradley Duguay W0516067
 * date:March 7th 2026
 * description:file designed to be used in conjunction with odd.c
 *             prints out an even number, waits until the flag for odd.c's
 *             print command appears, then prints out the next even number
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  int i;
  FILE *id = NULL;

  for (i = 0; i < 60; i = i + 1) {
    if ((i & 1) == 0) {  // Checks if i is even
      printf("%d\n", i); // \n terminates line and flushes buffer //
      id = fopen("0", "w");
      fclose(id);

    } else {

      do {

        id = fopen("1", "r"); //checks for the odd.c number print flag
                              //if it exists, wait before printing out next even number
        if (id == NULL) {
         
          sleep(1);
        } else {
          fclose(id);
          remove("1");
        }
      } while (id == NULL);
    }

    sleep(1);
  }
}
