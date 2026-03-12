#include <stdio.h>
#include <unistd.h>
int main() {
  FILE *id;
  int loop, operation;
  loop = 1;
  while (loop == 1) {
    // DELAY ONCE PER LOOP //
    // SO PRINTS CAN BE SEEN //
    usleep(500000);
    id = fopen("1", "r");
    if (id != NULL) {
      printf("BACKGROUND : Operation 1 complete\n");
      fclose(id);
    }
    id = fopen("2", "r");
    if (id != NULL) {
      printf("BACKGROUND : Operation 2 complete\n");
      fclose(id);
    }
    id = fopen("3", "r");
    if (id != NULL) {
      printf("BACKGROUND : Operation 3 complete\n");
      fclose(id);
    }
    id = fopen("4", "r");
    if (id != NULL) {
      printf("BACKGROUND : Operation 4 complete\n");
      fclose(id);
      loop = 0;
    }
  }
}
