
#ifndef flop_H_   /* Include guard */
#define flop_H_ 

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <errno.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include "minsc.h"
  #include "parentF.h"
  #include <stdbool.h>
  //#include <sys/

  #ifdef _WIN32
  #include <windows.h>
  //#define chdir _chdir

  #else
  #include <unistd.h>
  #endif

  #define MAX_LENGTH 1024
  #define DELIMS " \t\r\n"




struct Fat12Entry *entry;
struct Fat12Boot boot;





char *finame = '\0';


FILE *  OpenFile(char *name) {
 FILE * file;
 //finame = (char *)malloc(strlen(name) + 1);
 // strcpy(finame, name);
 if (!(file = fopen(name, "rb"))) {
  fprintf(stderr, "Unable to open %s. Exiting... \n", name);
  exit(1);
}

printf("Mounted  %s\n" ,name);
     //fchmod(fileno(file), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH);
return file;
}


struct Fat12Entry {
  unsigned char FILENAME[8];
  unsigned char EXT[3];
  unsigned char ATTRIBUTES[1];
  unsigned char RESERVED[2];
  unsigned short CREATION_TIME;
  unsigned short CREATION_DATE;
  unsigned short LAST_ACCESS_DATE;
  unsigned short MODIFY_TIME;
  unsigned short MODIFY_DATE;
  unsigned short START_CLUSTER;
  unsigned long FILE_SIZE;

} Fat12Entry_t;

struct Fat12Boot {
  unsigned int BYTES_PER_SECTOR;
  unsigned int SECTORS_PER_CLUSTER;
  unsigned int RESERVED_SECTORS;
  unsigned int NUM_OF_FATS;
  unsigned int MAX_ROOT_DIRS;
  unsigned int TOTAL_SECTORS;
  unsigned int SECTORS_PER_FAT;
  unsigned int SECTORS_PER_TRACK;
  unsigned int NUM_OF_HEADS;
  unsigned int VOLUME_ID;
  unsigned char VOLUME_LABEL;
} Fat12Boot_t;




void load(FILE * in) {
    fseek(in, 11, SEEK_SET); //skip 11 bytes

    //BYTES_PER_SECTOR (2 bytes)
    fread(&boot.BYTES_PER_SECTOR, 2, 1, in);

    //SECTORS_PER_CLUSTER (2 bytes)
    fread(&boot.SECTORS_PER_CLUSTER, 1, 1, in);

    //RESERVED_SECTORS (2 bytes)
    fread(&boot.RESERVED_SECTORS, 2, 1, in);

    //NUM_OF_FATS (1 byte)
    fread(&boot.NUM_OF_FATS, 1, 1, in);

    //MAX_ROOT_DIRS (2 bytes)
    fread(&boot.MAX_ROOT_DIRS, 2, 1, in);

    //Initialize 'entry'
    entry = (struct Fat12Entry *)malloc(sizeof(struct Fat12Entry) * boot.MAX_ROOT_DIRS);

    //TOTAL_SECTORS (2 bytes)
    fread(&boot.TOTAL_SECTORS, 2, 1, in);

    fseek(in, 1, SEEK_CUR); //skip 1 byte

    //SECTORS_PER_FAT (2 bytes)
    fread(&boot.SECTORS_PER_FAT, 2, 1, in);

    //SECTORS_PER_TRACK (2 bytes)
    fread(&boot.SECTORS_PER_TRACK, 2, 1, in);

    //NUM_OF_HEADS (2 bytes)
    fread(&boot.NUM_OF_HEADS, 2, 1, in);

    fseek(in, 11, SEEK_CUR); //skip 11 bytes

    //VOUME_ID (4 bytes)
    fread(&boot.VOLUME_ID, 4, 1, in);

    //VOLUME_LABEL
    fread(&boot.VOLUME_LABEL, 11, 1, in);

    //Move to beginning of ROOT DIRECTORY
    fseek(in, ((boot.NUM_OF_FATS * boot.SECTORS_PER_FAT) + 1) * boot.BYTES_PER_SECTOR, SEEK_SET);
    int i = 0;
    for (i = 0; i < boot.MAX_ROOT_DIRS; i++) {
        //FILENAME (8 bytes)
      fread(&entry[i].FILENAME, 8, 1, in);

        //EXT (3 bytes)
      fread(&entry[i].EXT, 3, 1, in);

        //ATTRIBUTES (1 byte)
      fread(&entry[i].ATTRIBUTES, 1, 1, in);

        //RESERVED (10 bytes)
      fread(&entry[i].RESERVED, 2, 1, in);

        //CREATION_TIME (2 bytes)
      fread(&entry[i].CREATION_TIME, 2, 1, in);

        //CREATION_DATE (2 bytes)
      fread(&entry[i].CREATION_DATE, 2, 1, in);

        //LAST_ACCESS_DATE (2 bytes)
      fread(&entry[i].LAST_ACCESS_DATE, 2, 1, in);

        fseek(in, 2, SEEK_CUR); //skip 2 bytes

        //MODIFY_TIME (2 bytes)
        fread(&entry[i].MODIFY_TIME, 2, 1, in);

        //MODIFY_DATE (2 bytes)
        fread(&entry[i].MODIFY_DATE, 2, 1, in);

        //START_CLUSTER (2 bytes)
        fread(&entry[i].START_CLUSTER, 2, 1, in);

        //FILE_SIZE (4 bytes)
        fread(&entry[i].FILE_SIZE, 4, 1, in);
      }
    }


    int num = 0;

    void showSector(FILE * in,char *line2) {
     if(strstr(line2,">") != NULL){

      char *fm;
      fm = strtok(line2,"showsector");


      while(fm !=NULL){
         // printf("test4 %s\n",fm );



        if (fork() == 0)
        {
            // child
          int fw = open(fm, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

            //fflush(stdout); close(out);
            //fflush(stderr); close(err);

               dup2(fw, 1);   // make stdout go to file
               dup2(fw, 2);   // make stderr go to file - you may choose to not do this

               unsigned char in2;
            //   Fat12BootSector pt;

               int i ;

               printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");
               char *sm;
               sm = strtok(line2," ");
               while(sm !=NULL){
                 if(strcmp(sm, "showsector") != 0){
                  int smi = atoi( sm );
                  fseek(in, boot.BYTES_PER_SECTOR * smi, SEEK_SET);
                  for (i = 0; i < boot.BYTES_PER_SECTOR; i++) {
                    if (i % 16 == 0 || i == 0) {
                      printf("\n");
                      printf("%4x", i);
                    }
                    fread(&in2, 1, 1, in);
                    printf("%5x", in2);
                  }

                  printf("\n");
                }
                sm= strtok(NULL," ");
              }



            close(fw);     // fd no longer needed - the dup'ed handles are sufficient

          }

          fm= strtok(NULL," ");

        }




      }else if(strstr(line2,"<") != NULL){

      }


      else{
       unsigned char in2;
     //  Fat12BootSector pt;

       int i ;

       printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");
       char *sm;
       sm = strtok(line2," ");
       while(sm !=NULL){
         if(strcmp(sm, "showsector") != 0){
          int smi = atoi( sm );
          fseek(in, boot.BYTES_PER_SECTOR * smi, SEEK_SET);
          for (i = 0; i < boot.BYTES_PER_SECTOR; i++) {
            if (i % 16 == 0 || i == 0) {
              printf("\n");
              printf("%4x", i);
            }
            fread(&in2, 1, 1, in);
            printf("%5x", in2);
          }

          printf("\n");
        }
        sm= strtok(NULL," ");
      }
      



    }
  }

  void structure(FILE * in,char * line2){
    if(strstr(line2,">") != NULL){

      char *fm;
      fm = strtok(line2,"structure -l >");


      while(fm !=NULL){

       //if(strcmp(fm, "") != 0){
        // char fname[100];
        // printf("Enter floppy Image Location: \n"); 
        //             scanf("%s", fname);// i used this because i could not get fopen to work with fmount media/floppy.img
        if (fork() == 0)
        {
              // child
          int fd = open(fm, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

              //fflush(stdout); close(out);
              //fflush(stderr); close(err);

                 dup2(fd, 1);   // make stdout go to file
                 dup2(fd, 2);   // make stderr go to file - you may choose to not do this



                 
                    //FILE * in = fopen(fname, "rb");
                 // FILE *in;
                 // in = OpenFile(floppyLoc);


                 int i;



                 for(i=0; i<boot.NUM_OF_FATS; i++) {
                  printf("Number of Fat:      %d\n", boot.NUM_OF_FATS);
                  printf("Number of Sectors used by FAT:      %d\n", boot.NUM_OF_FATS);
                  printf("Number of Sector Per Clusters:      %d\n", boot.SECTORS_PER_FAT);
                  printf("Number of Root Entries:     %d\n", boot.MAX_ROOT_DIRS);
                  printf("Number of bytes Per sector:     %d\n", boot.BYTES_PER_SECTOR);
                  printf("        ---Sector #---      ---Sector Types---\n");
                  printf("             0                    BOOT\n");
                  printf("          %02d -- %02d                FAT%d\n", (boot.SECTORS_PER_FAT * i) + 1,boot.SECTORS_PER_FAT * (i + 1), i);
                  printf("          %02d -- %02d                ROOT DIRECTORY\n", boot.SECTORS_PER_FAT * boot.NUM_OF_FATS, (boot.MAX_ROOT_DIRS / 16) + (boot.SECTORS_PER_FAT * boot.NUM_OF_FATS)); 
                }





              close(fd);     // fd no longer needed - the dup'ed handles are sufficient

            }

         // }

            fm= strtok(NULL," ");
          }
        }else{



         // char fname[100];
         // printf("Enter floppy Image Location: \n"); 
               // scanf("%s", floppyLoc);// i used this because i could not get fopen to work with fmount media/floppy.img
                //FILE * in = fopen(fname, "rb");
                // FILE *in;
                // in = OpenFile(floppyLoc);


          int i;
          for(i=0; i<boot.NUM_OF_FATS; i++) {
            printf("Number of Fat:      %d\n", boot.NUM_OF_FATS);
            printf("Number of Sectors used by FAT:      %d\n", boot.NUM_OF_FATS);
            printf("Number of Sector Per Clusters:      %d\n", boot.SECTORS_PER_FAT);
            printf("Number of Root Entries:     %d\n", boot.MAX_ROOT_DIRS);
            printf("Number of bytes Per sector:     %d\n", boot.BYTES_PER_SECTOR);
            printf("        ---Sector #---      ---Sector Types---\n");
            printf("             0                    BOOT\n");
            printf("          %02d -- %02d                FAT%d\n", (boot.SECTORS_PER_FAT * i) + 1,boot.SECTORS_PER_FAT * (i + 1), i);
            printf("          %02d -- %02d                ROOT DIRECTORY\n", boot.SECTORS_PER_FAT * boot.NUM_OF_FATS, (boot.MAX_ROOT_DIRS / 16) + (boot.SECTORS_PER_FAT * boot.NUM_OF_FATS)); 
          }




        }


      }

      void traverse(char * line2){
        if(strstr(line2,">") != NULL){

          char *fm;
          fm = strtok(line2,"traverse >");
          

          while(fm !=NULL){

           if (fork() == 0)
           {
              // child
            int fd = open(fm, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

              //fflush(stdout); close(out);
              //fflush(stderr); close(err);

                 dup2(fd, 1);   // make stdout go to file
                 dup2(fd, 2);   // make stderr go to file - you may choose to not do this



                 printf("    *****************************\n");
                 printf("    ** FILE ATTRIBUTE NOTATION **\n");
                 printf("    **                         **\n");
                 printf("    ** R ------ READ ONLY FILE **\n");
                 printf("    ** S ------ SYSTEM FILE    **\n");
                 printf("    ** H ------ HIDDEN FILE    **\n");
                 printf("    ** A ------ ARCHIVE FILE   **\n");
                 printf("    *****************************\n");
                 printf("\n");
                 int i=0;
                 while(i < boot.MAX_ROOT_DIRS) {
                  if (entry[i].FILENAME[0] != 0x00 && entry[i].START_CLUSTER != 0) {
                    char attr[6] = {'-', '-', '-', '-', '-'};
                    unsigned char a = entry[i].ATTRIBUTES[0];
                    if (a == 0x01)
                      attr[0] = 'R';
                    if (a == 0x02)
                      attr[1] = 'H';
                    if (a == 0x04)
                      attr[2] = 'S';
                    if (a == 0x20)
                      attr[5] = 'A';
                    if (a == 0x10) {
                      int j= 0;
                      while(j < 6){
                        attr[j] = '-';
                        j++;
                      }
                    }

                    if (entry[i].ATTRIBUTES[0] == 0x10) {
                      printf("%.6s    %d %d       < DIR >      /%.8s                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILENAME, entry[i].START_CLUSTER);
                      printf("%.6s    %d %d       < DIR >      /%.8s/.                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILENAME, entry[i].START_CLUSTER);
                      printf("%.6s    %d %d       < DIR >      /%.8s/..                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILENAME, 0);
                    } else {
                      printf("%.6s    %d %d       %lu      /%.8s.%.3s                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILE_SIZE, entry[i].FILENAME, entry[i].EXT, entry[i].START_CLUSTER);
                    }

                  }
                  i++;


                }


              close(fd);     // fd no longer needed - the dup'ed handles are sufficient

            }

            fm= strtok(NULL," ");


          }

        } else if(strstr(line2,"<") != NULL){

        }else  if(strstr(line2,"l") != NULL){

          printf("    *****************************\n");
          printf("    ** FILE ATTRIBUTE NOTATION **\n");
          printf("    **                         **\n");
          printf("    ** R ------ READ ONLY FILE **\n");
          printf("    ** S ------ SYSTEM FILE    **\n");
          printf("    ** H ------ HIDDEN FILE    **\n");
          printf("    ** A ------ ARCHIVE FILE   **\n");
          printf("    *****************************\n");
          printf("\n");
          int i=0;
          while(i < boot.MAX_ROOT_DIRS) {
            if (entry[i].FILENAME[0] != 0x00 && entry[i].START_CLUSTER != 0) {
              char attr[6] = {'-', '-', '-', '-', '-'};
              unsigned char a = entry[i].ATTRIBUTES[0];
              if (a == 0x01)
                attr[0] = 'R';
              if (a == 0x02)
                attr[1] = 'H';
              if (a == 0x04)
                attr[2] = 'S';
              if (a == 0x20)
                attr[5] = 'A';
              if (a == 0x10) {
                int j= 0;
                while(j < 6){
                  attr[j] = '-';
                  j++;
                }
              }

              if (entry[i].ATTRIBUTES[0] == 0x10) {
                printf("%.6s    %d %d       < DIR >      /%.8s                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILENAME, entry[i].START_CLUSTER);
                printf("%.6s    %d %d       < DIR >      /%.8s/.                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILENAME, entry[i].START_CLUSTER);
                printf("%.6s    %d %d       < DIR >      /%.8s/..                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILENAME, 0);
              } else {
                printf("%.6s    %d %d       %lu      /%.8s.%.3s                 %d\n", attr, entry[i].CREATION_DATE, entry[i].CREATION_TIME, entry[i].FILE_SIZE, entry[i].FILENAME, entry[i].EXT, entry[i].START_CLUSTER);
              }

            }
            i++;


          }


        }

        else{
          int i;
          char attr[6] = {'-', '-', '-', '-', '-'};
          for (i = 0; i < boot.MAX_ROOT_DIRS; i++) {
           if (entry[i].FILENAME[0] != 0x00 && entry[i].START_CLUSTER != 0) {
            if (entry[i].ATTRIBUTES[0] == 0x10) {
              printf("/%.8s                       < DIR >\n", entry[i].FILENAME);
              printf("/%.8s/.                     < DIR >\n", entry[i].FILENAME);
              printf("/%.8s/..                    < DIR >\n", entry[i].FILENAME);
            } else {
              printf("/%.8s.%.3s\n", entry[i].FILENAME, entry[i].EXT);
            }
          }
        }
      }
      

      

    }

    void showfat(char * floppyLoc,char * line2) {
     if(strstr(line2,">") != NULL){

      char *fm;
      fm = strtok(line2,"showfat >");


      while(fm !=NULL){
           // printf("test4 %s\n",fm );



        if (fork() == 0)
        {
              // child
          int fw = open(fm, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

              //fflush(stdout); close(out);
              //fflush(stderr); close(err);

                 dup2(fw, 1);   // make stdout go to file
                 dup2(fw, 2);   // make stderr go to file - you may choose to not do this


                 printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");

                 unsigned char buff[512];
                 int fd = open(floppyLoc, O_RDONLY);
                 lseek(fd,512*512,512*512);
                 read(fd,buff,512);
                 int i=0;
                 int k=0;
                 while(i<512){


                  int j=0;



                  while (j<16){
                    printf("%02x    ", buff[k]);
                    j++;
                    k++;
                  }
                  printf("\n");

                  i++;
                }





              close(fw);     // fd no longer needed - the dup'ed handles are sufficient

            }

            fm= strtok(NULL," ");

          }




        }else if(strstr(line2,"<") != NULL){

        }else{


          printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");

          unsigned char buff[512];
          int fd = open(floppyLoc, O_RDONLY);
          lseek(fd,512*512,512*512);
          read(fd,buff,512);
          int i=0;
          int k=0;
          while(i<512){


            int j=0;



            while (j<16){
              printf("%02x    ", buff[k]);
              j++;
              k++;
            }
            printf("\n");

            i++;
          }


        }

        

      }

      void showfile(FILE * in,char *line2) {



       if(strstr(line2,">") != NULL){

        char *fm;
        fm = strtok(line2,"help >");


        while(fm !=NULL){
           // printf("test4 %s\n",fm );



          if (fork() == 0)
          {
              // child
            int fd = open(fm, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

            char *fm;
            fm = strtok(line2," ");


            while(fm !=NULL){



              char *filename = '\0';

              char *ext = '\0';
              char *full_filename;
              char *p;
              
  //Parse filename
              p = strtok(fm, ".");
              if (p)
                filename = p;
              
  //Parse extension
              p = strtok(NULL, ".");
              if (p)
                ext = p;
              
              full_filename = (char *)malloc(strlen(filename) + strlen(ext) + 1);
              strcpy(full_filename, filename);
              strcat(full_filename, ext);
              int i;
              
              for (i = 0; i < boot.MAX_ROOT_DIRS; i++) {
    if (entry[i].FILENAME != 0x00 && entry[i].START_CLUSTER != 0) { //Ignore invalid files
      if(equals(full_filename, (char *)entry[i].FILENAME)) {
        unsigned char in2;
        
        //Move to the first byte of the file
        fseek(in, ((boot.MAX_ROOT_DIRS / 16) + (boot.SECTORS_PER_FAT * boot.NUM_OF_FATS) - 1) + (boot.BYTES_PER_SECTOR * entry[i].START_CLUSTER), SEEK_SET);
        int j;
        printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");
        for (j = 0; j < entry[i].FILE_SIZE; j++) {
          if (j % 16 == 0 || j == 0) {
            printf("\n");
            printf("%4x", j);
          }
          fread(&in2, 1, 1, in);
          printf("%5x", in2);
        }

        printf("\n");
      }
    }
  }


  fm= strtok(NULL," ");

}


              close(fd);     // fd no longer needed - the dup'ed handles are sufficient

            }

            fm= strtok(NULL," ");

        //showsector(fm);



          }
        }else if(strstr(line2,"<") != NULL){

        }else{

          char *fm;
          fm = strtok(line2," ");


          while(fm !=NULL){



            char *filename = '\0';

            char *ext = '\0';
            char *full_filename;
            char *p;
            
  //Parse filename
            p = strtok(fm, ".");
            if (p)
              filename = p;
            
  //Parse extension
            p = strtok(NULL, ".");
            if (p)
              ext = p;
            
            full_filename = (char *)malloc(strlen(filename) + strlen(ext) + 1);
            strcpy(full_filename, filename);
            strcat(full_filename, ext);
            int i;
            
            for (i = 0; i < boot.MAX_ROOT_DIRS; i++) {
    if (entry[i].FILENAME != 0x00 && entry[i].START_CLUSTER != 0) { //Ignore invalid files
      if(equals(full_filename, (char *)entry[i].FILENAME)) {
        unsigned char in2;
        
        //Move to the first byte of the file
        fseek(in, ((boot.MAX_ROOT_DIRS / 16) + (boot.SECTORS_PER_FAT * boot.NUM_OF_FATS) - 1) + (boot.BYTES_PER_SECTOR * entry[i].START_CLUSTER), SEEK_SET);
        int j;
        printf("        0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");
        for (j = 0; j < entry[i].FILE_SIZE; j++) {
          if (j % 16 == 0 || j == 0) {
            printf("\n");
            printf("%4x", j);
          }
          fread(&in2, 1, 1, in);
          printf("%5x", in2);
        }

        printf("\n");
      }
    }
  }


  fm= strtok(NULL," ");

}
}

}


void exepipelineChild(bool isMounted,char line2[],char * floppyLoc,FILE * in){
  char *fm;
  fm = strtok(line2," | ");



  while(fm !=NULL){
    if (strcmp(fm, "help") == 0) {
      help(line2);
    }else if (strcmp(fm, "showsector") == 0) {

      if(isMounted){

        showSector(in,line2);
      }else{
        printError();
      }
    }else if (strcmp(fm, "structure") == 0 ) {
      if(isMounted){
        structure(in,line2);

      }else{
        printError();
      }
    }else if (strcmp(fm, "traverse") == 0 ) {
      if(isMounted){
        traverse(line2);
      }else{
        printError();
      }
    }else if (strcmp(fm, "showfat") == 0 ) {
      if(isMounted){
        showfat(floppyLoc,line2);

      }else{
        printError();
      }
    }else if (strcmp(fm, "showfile") == 0 ) {
      if(isMounted){
        structure(in,line2);

      }else{
        printError();
      }
    }



    fm= strtok(NULL," | ");





  }
}









#endif
