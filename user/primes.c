#include "kernel/types.h"
#include "user/user.h"
//to avoid the overflow of the number of file descriptors
void 
redirect(int indexIO,int p[]){
    close(indexIO);
    dup(p[indexIO]);//duplicate p[indexIO] to indexIO
    //discard pipe file descriptor
    close(p[0]);
    close(p[1]);
}

void printPrimes(int p[]){  
    int num;
    if(read(0, &num, sizeof(int))){
        printf("prime %d\n", num);
        int pToright[2];
        pipe(pToright);
        int pid = fork();
        if(pid == 0){
            //child process
            redirect(0,pToright);
            printPrimes(pToright);
        }
        else{
            //parent process
            redirect(1,pToright);
            int prime;
            while(read(0, &prime,sizeof(int))){
                if(prime % num != 0)
                    write(1,&prime,sizeof(int));
            }
            // very tricky!!
            //the last pipe(write from the last but one and read from the last) is empty.
            //to return 0 from read syscall of the last process,it must make write end of the pipe close. 
            close(1);
            wait(0);
            exit(0);
        }
    }
    //if missed ,error:usertrap(): unexpected scause 0x0000000000000002 pid=15
    exit(0);
}

int 
main(int argc,char *argv[]){
    int pToright[2];//feed to the right pipe
    pipe(pToright);

    int pid = fork();
    if(pid == 0){
       // child process
       redirect(0,pToright);
       printPrimes(pToright);
     
    }
    else{
        //parent process
        redirect(1, pToright);
        for(int i = 2;i<=35;++i){
            write(1, &i, sizeof(int));
        }
        //to be explored
        //I don't know why there must be close() func.
        //if missed ,the shell will lose control
        close(1);
        wait(0);
        exit(0);
    }
    return 1;
}