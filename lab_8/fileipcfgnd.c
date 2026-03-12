#include <stdio.h>
int main() {
  FILE *id;
  int loop, operation;
  loop = 1;
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
