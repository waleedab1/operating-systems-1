#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("Before Allocation: %d\n", memsize());
  char* arr = malloc(20000);
  printf("After Allocation: %d\n", memsize());
  free(arr);
  printf("After Freeing Allocation: %d\n", memsize());
  exit(0, "");
}
