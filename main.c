  /*
  *Name:George Immanuel
  *Project: 3
  *Deadline: Mon Nov 17,2014
  *Objective:  To develop a floppy mini unix shell to mount and access floppy disk.
  */





  #include <stdio.h>
  #include <stdbool.h>
  #include "parentF.h"
  #include "minsc.h"
  #include "flshell.h"
  #include "flop.h"




  int main(int args, char *argv[]){



    wipeTerminalScreen();
    welcomeMessage();




    char *cmd;
    char line[MAX_LENGTH];
    char line2[MAX_LENGTH];
      //char *floppyLoc;
    char floppyLoc[MAX_LENGTH] ="";
    bool isMounted = false;
    const char* fmloc;
    const char *arg;
    const char * ty;

    FILE * oImg;
    FILE *in;





    while (1) {
      char *p;
     // printf("flop: ");
      shellLabel();
      if (!fgets(line, MAX_LENGTH, stdin)) break;
      strncpy(line2,line,MAX_LENGTH);




        // Parse and execute command
      if ((cmd = strtok(line, DELIMS))) {
          // Clear errors
        errno = 0;


        if (strcmp(cmd, "path") == 0 ) {

          execPath(line2);
        }else if (strcmp(cmd, "cd") == 0 ) {
          execchangeDir(line2);
        }


        else if (strcmp(cmd, "exit") == 0) {
          break;

        } else  if(strcmp(line, "|") == 0) {
          exepipeParent(isMounted,line2,floppyLoc,in);
        }

        else{ // if command not in parent search child

          if (strcmp(cmd, "help") == 0) {
            help(line2);
          }
          else if (strcmp(cmd, "fmount") == 0) {



            if(isMounted){

              printf("Mounted   %s already, please unmount first to mount new floppy\n" ,floppyLoc);

            }else{
             isMounted = true;


             char *fm;
             fm = strtok(line2," ");


             while(fm !=NULL){
               if(strcmp(fm, "fmount") != 0){

            //floppyLoc[0]='\0';
           //strncat(floppyLoc,fm,sizeof(floppyLoc));
           // floppyLoc =(char *)malloc(strlen(fm) + 1);



                strcpy(floppyLoc,fm);

                int i =0;
                sscanf (fm,"%s %*s %d",floppyLoc,&i);

              }

              fm= strtok(NULL," ");

            }



            printf("Mounting  %s\n" ,floppyLoc);
            in = OpenFile(floppyLoc);
            load(in);
            floppyShell(isMounted,floppyLoc,in);
          }



        }else if (strcmp(cmd, "fumount") == 0) {
          if(isMounted && floppyLoc[0] !='\0'){
           printf("UnMounted  %s\n" ,floppyLoc);
          // *floppyLoc=0;
           isMounted = false;
           fclose(in);
         }else{
           printf("Error:- Floppy Cannot UnMount because its not Mounted in the first place ");
           printf("\n");

         }





       }else if (strcmp(cmd, "showsector") == 0) {
        printError();
      }else if (strcmp(cmd, "traverse") == 0) {
        printError();
      }else if (strcmp(cmd, "showfat") == 0) {
        printError();
      }else if (strcmp(cmd, "showfile") == 0) {
        printError();
      }else if (strcmp(cmd, "structure") == 0) {
        printError();
      }else{
       printf("Command does not exist, type help for available commands for floppy. Also only path and cd exist because this is Parent process");
       if (errno)
       {
        perror("command failed");
      }

      printf("\n");
    }
  }
}


}
return 0;

}
