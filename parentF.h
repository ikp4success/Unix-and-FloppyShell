#ifndef parentF_H_   /* Include guard */
#define parentF_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "flop.h"

void execPath(char * line2){

	char *cwd_f;

	if (getenv("PATH") != NULL){
		cwd_f = getenv("PATH");
		
		if(strstr(line2,"+") != NULL){


			char *fm;
			fm = strtok(line2,"+ ");



			while(fm !=NULL){

				if(strcmp(fm, "path") != 0){
					if(strstr(fm,"+") == NULL){

						strcat(cwd_f, ":");
						strcat(cwd_f,fm);



					}
				}

				fm= strtok(NULL," ");





			}
			setenv("PATH", cwd_f, 1);
			fprintf(stdout, "%s\n", cwd_f);




		}else if(strstr(line2,"-") != NULL){
			char *fm;
			fm = strtok(line2,"- ");
			char *path;


			while(fm !=NULL){

				if(strcmp(fm, "path") != 0){
					if(strstr(fm,"-") == NULL){
						path = (char *)malloc((sizeof(char) * strlen(getenv("PATH"))) - (sizeof(char) * strlen(fm)));

						printf("testminus%s\n",fm);

						int i = 0; char *p;
						for (i = 0, p = strtok(getenv("PATH"), ":"); p != NULL; i++, p = strtok(NULL, ":")) {
							if (strcmp(p, fm) == 0) {
								continue;
							} else {
								if (i != 0) {
									strcat(path, ":");
								}
								strcat(path, p);
							}
						}


					}
				}
				fm= strtok(NULL," ");





			}
			setenv("PATH", path, 1);
			fprintf(stdout, "%s\n", path);


		}else{
			printf("%s\n",getenv("PATH"));
		}
	}
}


void execchangeDir(char * line2){
	//printf("Change Directory Command under construction\n");



	
	if(strstr(line2," ") != NULL){
	char *chgdir;
	char *fm;
	fm = strtok(line2," ");
	while(fm !=NULL){
		chgdir = fm;
		int i =1;
    	sscanf (fm,"%s %*s %d",chgdir,&i);
		fm= strtok(NULL," ");

	}
	

	//strcpy(chgdir,fm);
	int check;
	//printf("dirstuff%s\n", chgdir);
	
	check =  chdir(chgdir);

	if(check !=0){
		perror("Error:");
	}else{
		//setenv("PATH", chgdir, 1);
	}
	}else{
		chdir(getenv("HOME"));
	}

}


void exepipeParent(bool isMounted,char line2[],char * floppyLoc,FILE * in){

	char *fm;
	fm = strtok(line2," | ");



	while(fm !=NULL){
		

		
		if (strcmp(fm, "help") == 0) {
			help(line2);
		}if (strcmp(fm, "path") == 0) {
			execPath(line2);
		}else if (strcmp(fm, "cd") == 0 ) {
			execchangeDir(line2);
		}else{
			exepipelineChild(isMounted,line2,floppyLoc,in);// execute | | command for child if command not in parent
		}
		

		fm= strtok(NULL," | ");





	}
}
#endif 
