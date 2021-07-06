#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

//get name from the full name 
// char* fmtname(char *path)
// {
//   static char buf[DIRSIZ+1];
//   char *p;

//   // Find first character after last slash.
//   for(p=path+strlen(path); p >= path && *p != '/'; p--)
//     ;
//   p++;

//   // Return blank-padded name.
//   if(strlen(p) >= DIRSIZ)
//     return p;
//   memmove(buf, p, strlen(p));
//   memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
//   return buf;
// }

void find(char *path, char *name){
    int fd;//file descriptor
    struct stat st;//the status of file
    char buf[512], *p;
    struct dirent de;//a node of dir file

    if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

    //get the status of dir from fd
   if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  //find the target file from the dir
  if(st.type == T_DIR ){
    if(strlen(path) + 1+ DIRSIZ + 1 > sizeof(buf)){
      printf("find: path too long\n");
      close(fd);
      return;
    }
    strcpy(buf, path);
    p = buf+ strlen(buf);//p points to the end of buf
    *p++ = '/';
    while (read(fd, &de,sizeof(de)) == sizeof(de)){
      if(de.inum == 0 ) 
          continue;
      //buf is the full name of the file 
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      } 
      switch(st.type){
        case T_FILE:
           if(strcmp(de.name, name) == 0){
            printf("%s\n", buf);
          }
          break;

        case T_DIR:
          //close(fd);
          if(strcmp(de.name,".") != 0 && strcmp(de.name, "..") != 0)
              find(buf, name);
          break;
        
      }
    }
  }
  close(fd);
}
int 
main(int argc, char *argv[]){
    if(argc != 3){
        printf("ERROR: You need to input two arguments ");
        exit(1);
    }
    else{
        char *path = argv[1];
        char *name = argv[2];
        find(path,name);
        exit(0);
    }
    
}