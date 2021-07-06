#include "kernel/types.h"
#include "user/user.h"

int 
main(int agrc ,char *argv[]){
    int parTochild[2];//pipe    
    int childToParent[2];
    pipe(parTochild); //parent process send messaage to child
    pipe(childToParent);
    char buf[1];           //the byte transported between parent and child process
    int pid = fork();


    if(pid < 0) exit(1);
    else if(pid == 0){
        //child process
        close(parTochild[1]);
        close(childToParent[0]);
        write(childToParent[1],buf,1);
        int n = read(parTochild[0],buf,1);
        if(n == 1){
            printf("%d: received ping\n",getpid());
        }
        close(childToParent[1]);
        close(parTochild[0]);
        exit(0);
    }
    else{
        //parent process
        close(parTochild[0]);
        close(childToParent[1]);
        write(parTochild[1], buf,1);
        ///////////////////////////////
        //attention!!!!
        //wait(int*) is to wait for exit of the child process of the current process
        // just to make sure that "ping" is printed before "pong"
        //if lost,the sentence printed will be disordered
        //the exact reason for disorder is to be explored further 
        wait(0);

        int n  = read(childToParent[0],buf,1);
        if(n == 1){
            printf("%d: received pong\n",getpid());
        }
        close(childToParent[0]);
        close(parTochild[1]);
        
        exit(0);
    }

}