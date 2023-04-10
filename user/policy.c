#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc != 2)
        exit(1, "Invalid number of arguments\n");
    int type = atoi(argv[1]);
    if(set_policy(type) < 0)
        exit(1, "Illegal argument type\n");
    exit(0,"");
}
