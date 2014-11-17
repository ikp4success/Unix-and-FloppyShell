#ifndef minsc_H_   /* Include guard */
#define minsc_H_ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void welcomeMessage() {
	printf("\n*******WELCOME TO SHELL(parent)*******\n");
	printf("\nPlease go over the read me before using this shell\n");
}
void welcomeMessageFlop() {
	printf("\n//*******WELCOME TO FLOPPY SHELL(child)*******\n");
	printf("\nPlease go over the read me before using this shell\n");
}

void shellLabel() {
	char cwd[1024];
	if((getcwd(cwd, sizeof(cwd)))!=NULL){
		getcwd(cwd, sizeof(cwd));
		printf("[%s]shell$ ", cwd);
	}
	else{
		perror("command failed");
	}
}

void wipeTerminalScreen() {
	printf("\033[H\033[J");
}

void printError() {
	printf("Error:- Please Mount Floppy first");
	printf("\n");
}



int equals(char * str1, char *str2) {
	int i;
	for (i = 0; i < strlen(str1); i++) {
		if (str1[i] != str2[i])
			return 0;
	}
	return 1;
}



void help(char * line2){




	if(strstr(line2,">") != NULL){

		char *fm;
		fm = strtok(line2,"help >");


		while(fm !=NULL){
           // printf("test4 %s\n",fm );



			if (fork() == 0)
			{
              // child
				int fd = open(fm, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

              //fflush(stdout); close(out);
              //fflush(stderr); close(err);

                 dup2(fd, 1);   // make stdout go to file
                 dup2(fd, 2);   // make stderr go to file - you may choose to not do this

                 printf("\n\nClient Environment Floppy Console\n\n");
                 printf("help: shows the commands supported in the floppy shell\n\n");
                 printf("fmount argument: mount a local floppy disk, where arguments is the floppy device path name, e.g, /dev/floppy\n\n");
                 printf("fumount: unmount the mounted floppy disk.\n\n");
                 printf("structure: to list the structure of the floppy disk.\n\n");
                 printf("traverse: list the content in the root directory.\n\n");
                 printf("showfat: show the content of the FAT tables.\n\n");
                 printf("showsector [sector number]: show the content of the specified sector number(516 bytes for each sector).\n\n");
                 printf("show file [filename]: show the content of the target file(in the hex dump)\n\n");
                 printf("Addtional Commands Path + , Path -\n\n");

              close(fd);     // fd no longer needed - the dup'ed handles are sufficient

          }

          fm= strtok(NULL," ");

        //showsector(fm);



      }
  }else if(strstr(line2,"<") != NULL){

  }else{
  	printf("\n\nClient Environment Floppy Console\n\n");
  	printf("help: shows the commands supported in the floppy shell\n\n");
  	printf("fmount argument: mount a local floppy disk, where arguments is the floppy device path name, e.g, /dev/floppy\n\n");
  	printf("fumount: unmount the mounted floppy disk.\n\n");
  	printf("structure: to list the structure of the floppy disk.\n\n");
  	printf("traverse: list the content in the root directory.\n\n");
  	printf("showfat: show the content of the FAT tables.\n\n");
  	printf("showsector [sector number]: show the content of the specified sector number(516 bytes for each sector).\n\n");
  	printf("show file [filename]: show the content of the target file(in the hex dump)\n\n");
  	printf("(Parent) Addtional Commands  Path,Path +(add path) , Path -(remove path)\n\n");
  	printf("(Parent) Addtional Commands Change directory cd \n\n");
  }




}

#endif
