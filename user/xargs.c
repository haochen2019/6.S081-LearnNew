#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int 
main(int argc, char *argv[]){
   if(argc < 2){
       printf("xargs: arguments WRONG");
       exit(1);
   }

   char *childArgv[MAXARG];//arguments passed to the child process
   for(int i = 1;i<argc;++i){
       //ignore xargs
       childArgv[i-1] = argv[i];
   }

    while(1){
        char c;
        char buf[1024];//store the string read from input
        int bufBegin = 0; 
        int bufEnd = 0;
        int argCursor = argc-1; 
        //read from standard input
        //stat is used to judge two conditions:
        //1. the end of the whole program
        //2. newline
        int stat = read(0, &c, sizeof(char));
        while(stat){
            if(c == '\n' || c == ' '){
                buf[bufEnd++] = 0;
                childArgv[argCursor++] = &buf[bufBegin];
                bufBegin = bufEnd;
                if( c == '\n')
                    break;
            }
            else{
                buf[bufEnd++] = c;
            }
            stat = read(0, &c, sizeof(char));
        }
        if(stat == 0) 
            exit(0);
        childArgv[argCursor] = 0;
        if(fork() == 0){
            exec(childArgv[0],childArgv);
        }
        else{
            wait(0);
        }
    }
 
}