#include <stdio.h>
#include <unistd.h>
int main() {
  int cpid;
  FILE *id;
  int loop, operation;
  loop = 1;
  cpid = fork();
  if (cpid != 0 && cpid != -1)

  {
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

    usleep(500000);
  } else if (cpid == 0)

  {
    while (loop == 1) {
      scanf("%d", &operation);
      if (operation == 1) {
        id = fopen("1", "w");
        if (id == NULL) {
          printf("ERROR : File failed to open");
        }
        fclose(id);
      } else if (operation == 2) {
        id = fopen("2", "w");
        if (id == NULL) {
          printf("ERROR : File failed to open");
        }
        fclose(id);
      } else if (operation == 3) {
        id = fopen("3", "w");
        if (id == NULL) {
          printf("ERROR : File failed to open");
        }
        fclose(id);
      } else if (operation == 4) {
        id = fopen("4", "w");
        if (id == NULL) {
          printf("ERROR : File failed to open");
        }
        fclose(id);
        loop = 0;
      } else {
        printf("ERROR : Illegal input");
      }
    }
  }

  else {
    printf("Child was not created\n");
  }
  return 0;
}
