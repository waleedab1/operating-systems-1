#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char* msg = "Hello World xv6\n";
  write(1, msg, strlen(msg));
  exit(0, "");
}
