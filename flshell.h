#ifndef flshell_H_   /* Include guard */
#define flshell_H_ 




#include <stdio.h>
#include <stdbool.h>
#include "minsc.h"
#include "flop.h"
#include "parentF.h"



void floppyShell(bool isMounted,char floppyLoc[MAX_LENGTH], FILE *in){


  

  welcomeMessageFlop();




  char *cmd;
  char line[MAX_LENGTH];
  char line2[MAX_LENGTH];
    //char *floppyLoc;
  //char floppyLoc[MAX_LENGTH] ="";
  //bool isMounted = false;
  const char* fmloc;
  const char *arg;
  const char * ty;

  
  // FILE *in;





  while (1) {
    char *p;
   printf("flop: ");
   
    if (!fgets(line, MAX_LENGTH, stdin)) break;
    strncpy(line2,line,MAX_LENGTH);




      // Parse and execute command
    if ((cmd = strtok(line, DELIMS))) {
        // Clear errors
      errno = 0;

      if (strcmp(cmd, "help") == 0) {
      	help(line2);
      }

      if (strcmp(cmd, "fmount") == 0) {



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
      }



    } else if (strcmp(cmd, "fumount") == 0) {
      if(isMounted && floppyLoc[0] !='\0'){
       printf("UnMounted  %s\n" ,floppyLoc);
        // *floppyLoc=0;
       isMounted = false;
       fclose(in);
       break;
     }else{
       printf("Error:- Floppy Cannot UnMount because its not Mounted in the first place ");
       printf("\n");


     }





   }else if (strcmp(cmd, "showsector") == 0) {

     if(isMounted){

     	showSector(in,line2);
     }else{
      printError();
    }
  }else if (strcmp(cmd, "structure") == 0 ) {
    if(isMounted){
      structure(in,line2);

    }else{
    	printError();
    }
  }else if (strcmp(cmd, "traverse") == 0 ) {
    if(isMounted){
      traverse(line2);

    }else{
    	printError();
    }
  }else if (strcmp(cmd, "showfat") == 0 ) {
    if(isMounted){
      showfat(floppyLoc,line2);

    }else{
    	printError();
    }
  }else if (strcmp(cmd, "showfile") == 0 ) {
    if(isMounted){
      structure(in,line2);

    }else{
    	printError();
    }
  }else  if(strstr(line,"|") != NULL) {
          exepipelineChild(isMounted,line2,floppyLoc,in);// execute | | command for child
        }

  else if (strcmp(cmd, "quit") == 0) {
    break;

  } 

  else{ 
    break;
    
  if (strcmp(cmd, "path") == 0 ) {
     
      execPath(line2);
    }else if (strcmp(cmd, "cd") == 0 ) {
      execchangeDir(line2);
    }


  printf("\n");
}
}


}


}

#endif
