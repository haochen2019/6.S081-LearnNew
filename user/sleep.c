#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc , char *argv[]){
    if(argc != 2){
        fprintf(2,"ERROR: sleep time needed\n");
    }
    //call syscall "sleep"
    sleep(atoi(argv[1]));
    exit(0);
}