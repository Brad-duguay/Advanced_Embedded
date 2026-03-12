#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void fgnd_func(int);
void bgnd_func();

FILE *id;

int main() {

  char operation = '0';
  int num_op = 0;

  pid_t pid = fork();
  do {

    if (pid == 0) {
      bgnd_func();

    } else {
      scanf("%c", &operation);
      if (operation == 'q' || operation == 'Q') {
	printf("Quitting Program \n");
	break;
      } else {
	num_op = atoi(&operation);
        fgnd_func(num_op);
      }
    }
  } while (operation != 'q' || operation != 'Q');
  return 0;
}

void fgnd_func(int num_operation) {
  do {
    if (num_operation == 1) {
      id = fopen("1", "w");

      if (id == NULL) {
        printf("ERROR : File failed to open");
      }
      fclose(id);
    } else if (num_operation == 2) {
      id = fopen("2", "w");

      if (id == NULL) {
        printf("ERROR : File failed to open");
      }
      fclose(id);
    } else if (num_operation == 3) {
      id = fopen("3", "w");

      if (id == NULL) {
        printf("ERROR : File failed to open");
      }
      fclose(id);
    } else if (num_operation == 4) {
      id = fopen("4", "w");

      if (id == NULL) {
        printf("ERROR : File failed to open");
      }
      fclose(id);
    } else {

      printf("ERROR : Illegal input");
    }

  } while (getchar() != '\n');
  num_operation = 0;
}
void bgnd_func() {
  id = fopen("1", "r");
  if (id != NULL) {
    printf("BACKGROUND : Operation 1 complete\n");
    fclose(id);
    remove("1");
  }
  id = fopen("2", "r");
  if (id != NULL) {
    printf("BACKGROUND : Operation 2 complete\n");
    fclose(id);
    remove("2");
  }
  id = fopen("3", "r");
  if (id != NULL) {
    printf("BACKGROUND : Operation 3 complete\n");
    fclose(id);
    remove("3");
  }
  id = fopen("4", "r");
  if (id != NULL) {
    printf("BACKGROUND : Operation 4 complete\n");
    fclose(id);
    remove("4");
  }
  if (id == NULL) {
    sleep(1);
  }
}
