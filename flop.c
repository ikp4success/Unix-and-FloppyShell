//Names:George Immanuel , Henderson Creighton ,Rutledge Brandon
//Group: H
//Project: 2
//Objective:  to develop a floppy shell for floppy disk program

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <errno.h>
  #include <unistd.h>
  #include <fcntl.h>
  //#include <sys/

  #ifdef _WIN32
  #include <windows.h>
  #define chdir _chdir

  #else
  #include <unistd.h>
  #endif

  #define MAX_LENGTH 1024
  #define DELIMS " \t\r\n"

struct Fat12Entry *entry;
struct Fat12Boot boot;



FILE *  OpenFile( char *name) {
 FILE * file;
 if (!(file = fopen(name, "rb"))) {
  fprintf(stderr, "Unable to open %s. Exiting... \n", name);
  exit(1);
}
     //fchmod(fileno(file), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH);
return file;
}

typedef struct {
 unsigned char jmp[3];
 char oem[8];
 unsigned short sector_size;
 unsigned char sectors_per_cluster;
 unsigned short reserved_sectors;
 unsigned char number_of_fats;
 unsigned short root_dir_entries;
 unsigned short total_sectors_short; 
 unsigned char media_descriptor;
 unsigned short fat_size_sectors;
 unsigned short sectors_per_track;
 unsigned short number_of_heads;
 unsigned long hidden_sectors;
 unsigned long total_sectors_long;

 unsigned char drive_number;
 unsigned char current_head;
 unsigned char boot_signature;
 unsigned long volume_id;
 char volume_label[11];
 char fs_type[8];
 char boot_code[448];
 unsigned short boot_sector_signature;
 unsigned char first_byte;
 unsigned char start_chs[3];
 unsigned char partition_type;
 unsigned char end_chs[3];
 unsigned long start_sector;
 unsigned long length_sectors;
} __attribute((packed)) Fat12BootSector;

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


int main(int argc, char *argv[]) {
  char *cmd;
  char line[MAX_LENGTH];
  char line2[MAX_LENGTH];
    //char *floppyLoc;
  char floppyLoc[100] ="";

      //floppyLoc = (char *)malloc(sizeof(char) * 128);

  while (1) {
    printf("flop: ");
    if (!fgets(line, MAX_LENGTH, stdin)) break;
    strncpy(line2,line,MAX_LENGTH);




      // Parse and execute command
    if ((cmd = strtok(line, DELIMS))) {
        // Clear errors
      errno = 0;

      if (strcmp(cmd, "fmount") == 0) {


        char *fm;
        fm = strtok(line2," ");


        while(fm !=NULL){
         if(strcmp(fm, "fmount") != 0){

          floppyLoc[0]='\0';
          strncat(floppyLoc,fm,sizeof(floppyLoc));


        }

        fm= strtok(NULL," ");

        //showsector(fm);

      }
    }




    else if (strcmp(cmd, "help") == 0) {
        //printf("test%s\n",line2 );
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

              close(fd);     // fd no longer needed - the dup'ed handles are sufficient

            }

            fm= strtok(NULL," ");

        //showsector(fm);



          }
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
       }



     }
     else if (strcmp(cmd, "fumount") == 0 && floppyLoc[0] !='\0') {
      if(floppyLoc[0] =='\0'){
        printf("Error:- Floppy Cannot UnMount because its not Mounted in the first place ");
        printf("\n");

      }else{


       *floppyLoc=0;
     }





   }else if (strcmp(cmd, "structure") == 0 ) {
    if(floppyLoc[0] =='\0'){
      printf("Error:- Please Mount Floppy first");
      printf("\n");

    }else{
                //printf("here\n");

     if(strstr(line2,">") != NULL){

      char *fm;
      fm = strtok(line2,"structure -l >");


      while(fm !=NULL){

       //if(strcmp(fm, "") != 0){
        char fname[100];
        printf("Enter floppy Image Location: \n"); 
                    scanf("%s", fname);// i used this because i could not get fopen to work with fmount media/floppy.img
                    if (fork() == 0)
                    {
              // child
                      int fd = open(fm, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

              //fflush(stdout); close(out);
              //fflush(stderr); close(err);

                 dup2(fd, 1);   // make stdout go to file
                 dup2(fd, 2);   // make stderr go to file - you may choose to not do this



                 
                    //FILE * in = fopen(fname, "rb");
                 FILE *in;
                 in = OpenFile(fname);


                 int i;
                 Fat12BootSector pt[4];

                 fseek(in, 512 * pt[i].start_sector, SEEK_SET);
                 fread(&pt, sizeof(Fat12BootSector), 1, in);

                 for(i=0; i<1; i++) {
                  printf("Number of Fat:      %d\n", pt[i].number_of_fats);
                  printf("Number of Sectors used by FAT:      %d\n", pt[i].fat_size_sectors);
                  printf("Number of Sector Per Clusters:      %d\n", pt[i].sectors_per_cluster);
                  printf("Number of Root Entries:     %d\n", pt[i].root_dir_entries);
                  printf("Number of bytes Per sector:     %d\n", pt[i].sector_size);
                }

                fclose(in);



              close(fd);     // fd no longer needed - the dup'ed handles are sufficient

            }

         // }

            fm= strtok(NULL," ");
          }
        }else{



          char fname[100];
          printf("Enter floppy Image Location: \n"); 
                scanf("%s", fname);// i used this because i could not get fopen to work with fmount media/floppy.img
                //FILE * in = fopen(fname, "rb");
                FILE *in;
                in = OpenFile(fname);


                int i;
                Fat12BootSector pt[4];

                fseek(in, 512 * pt[i].start_sector, SEEK_SET);
                fread(&pt, sizeof(Fat12BootSector), 1, in);

                for(i=0; i<1; i++) {
                  printf("Number of Fat:      %d\n", pt[i].number_of_fats);
                  printf("Number of Sectors used by FAT:      %d\n", pt[i].fat_size_sectors);
                  printf("Number of Sector Per Clusters:      %d\n", pt[i].sectors_per_cluster);
                  printf("Number of Root Entries:     %d\n", pt[i].root_dir_entries);
                  printf("Number of bytes Per sector:     %d\n", pt[i].sector_size);
                }

                fclose(in);

              }


            }




          }else if (strcmp(cmd, "traverse") == 0) {
           if(floppyLoc[0] =='\0'){
            printf("Error:- Please Mount Floppy first");
            printf("\n");

          }else{
           if(strstr(line2,">") != NULL){

            char *fm;
            fm = strtok(line2,"traverse >");
            char fname[100];
            printf("Enter floppy Image Location: \n"); 
            scanf("%s", fname);// i used this because i could not get fopen to work with fmount media/floppy.img


            while(fm !=NULL){

             if (fork() == 0)
             {
              // child
              int fd = open(fm, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

              //fflush(stdout); close(out);
              //fflush(stderr); close(err);

                 dup2(fd, 1);   // make stdout go to file
                 dup2(fd, 2);   // make stderr go to file - you may choose to not do this

                 FILE *img = OpenFile(fname);

          fseek(img, 11, SEEK_SET); //skip 11 bytes

          //BYTES_PER_SECTOR (2 bytes)
          fread(&boot.BYTES_PER_SECTOR, 2, 1, img);

          //SECTORS_PER_CLUSTER (2 bytes)
          fread(&boot.SECTORS_PER_CLUSTER, 1, 1, img);

          //RESERVED_SECTORS (2 bytes)
          fread(&boot.RESERVED_SECTORS, 2, 1, img);

          //NUM_OF_FATS (1 byte)
          fread(&boot.NUM_OF_FATS, 1, 1, img);

          //MAX_ROOT_DIRS (2 bytes)
          fread(&boot.MAX_ROOT_DIRS, 2, 1, img);

          //Initialize 'entry'
          entry = (struct Fat12Entry *)malloc(sizeof(struct Fat12Entry) * boot.MAX_ROOT_DIRS);

          //TOTAL_SECTORS (2 bytes)
          fread(&boot.TOTAL_SECTORS, 2, 1, img);

          fseek(img, 1, SEEK_CUR); //skip 1 byte

          //SECTORS_PER_FAT (2 bytes)
          fread(&boot.SECTORS_PER_FAT, 2, 1, img);

          //SECTORS_PER_TRACK (2 bytes)
          fread(&boot.SECTORS_PER_TRACK, 2, 1, img);

          //NUM_OF_HEADS (2 bytes)
          fread(&boot.NUM_OF_HEADS, 2, 1, img);

          fseek(img, 11, SEEK_CUR); //skip 11 bytes

          //VOUME_ID (4 bytes)
          fread(&boot.VOLUME_ID, 4, 1, img);

          //VOLUME_LABEL
          fread(&boot.VOLUME_LABEL, 11, 1, img);

          //Move to beginning of ROOT DIRECTORY
          fseek(img, ((boot.NUM_OF_FATS * boot.SECTORS_PER_FAT) + 1) * boot.BYTES_PER_SECTOR, SEEK_SET);
          int h = 0;
          while(h < boot.MAX_ROOT_DIRS) {
              //FILENAME (8 bytes)
            fread(&entry[h].FILENAME, 8, 1, img);

              //EXT (3 bytes)
            fread(&entry[h].EXT, 3, 1, img);

              //ATTRIBUTES (1 byte)
            fread(&entry[h].ATTRIBUTES, 1, 1, img);

              //RESERVED (10 bytes)
            fread(&entry[h].RESERVED, 2, 1, img);

              //CREATION_TIME (2 bytes)
            fread(&entry[h].CREATION_TIME, 2, 1, img);

              //CREATION_DATE (2 bytes)
            fread(&entry[h].CREATION_DATE, 2, 1, img);

              //LAST_ACCESS_DATE (2 bytes)
            fread(&entry[h].LAST_ACCESS_DATE, 2, 1, img);

              fseek(img, 2, SEEK_CUR); //skip 2 bytes

              //MODIFY_TIME (2 bytes)
              fread(&entry[h].MODIFY_TIME, 2, 1, img);

              //MODIFY_DATE (2 bytes)
              fread(&entry[h].MODIFY_DATE, 2, 1, img);

              //START_CLUSTER (2 bytes)
              fread(&entry[h].START_CLUSTER, 2, 1, img);

              //FILE_SIZE (4 bytes)
              fread(&entry[h].FILE_SIZE, 4, 1, img);
              h++;
            }

//entry.FILENAME="floppy.img";

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

        }else{

         char fname[100];
         printf("Enter floppy Image Location: \n"); 
            scanf("%s", fname);// i used this because i could not get fopen to work with fmount media/floppy.img

            FILE *img = OpenFile(fname);

          fseek(img, 11, SEEK_SET); //skip 11 bytes

          //BYTES_PER_SECTOR (2 bytes)
          fread(&boot.BYTES_PER_SECTOR, 2, 1, img);

          //SECTORS_PER_CLUSTER (2 bytes)
          fread(&boot.SECTORS_PER_CLUSTER, 1, 1, img);

          //RESERVED_SECTORS (2 bytes)
          fread(&boot.RESERVED_SECTORS, 2, 1, img);

          //NUM_OF_FATS (1 byte)
          fread(&boot.NUM_OF_FATS, 1, 1, img);

          //MAX_ROOT_DIRS (2 bytes)
          fread(&boot.MAX_ROOT_DIRS, 2, 1, img);

          //Initialize 'entry'
          entry = (struct Fat12Entry *)malloc(sizeof(struct Fat12Entry) * boot.MAX_ROOT_DIRS);

          //TOTAL_SECTORS (2 bytes)
          fread(&boot.TOTAL_SECTORS, 2, 1, img);

          fseek(img, 1, SEEK_CUR); //skip 1 byte

          //SECTORS_PER_FAT (2 bytes)
          fread(&boot.SECTORS_PER_FAT, 2, 1, img);

          //SECTORS_PER_TRACK (2 bytes)
          fread(&boot.SECTORS_PER_TRACK, 2, 1, img);

          //NUM_OF_HEADS (2 bytes)
          fread(&boot.NUM_OF_HEADS, 2, 1, img);

          fseek(img, 11, SEEK_CUR); //skip 11 bytes

          //VOUME_ID (4 bytes)
          fread(&boot.VOLUME_ID, 4, 1, img);

          //VOLUME_LABEL
          fread(&boot.VOLUME_LABEL, 11, 1, img);

          //Move to beginning of ROOT DIRECTORY
          fseek(img, ((boot.NUM_OF_FATS * boot.SECTORS_PER_FAT) + 1) * boot.BYTES_PER_SECTOR, SEEK_SET);
          int h = 0;
          while(h < boot.MAX_ROOT_DIRS) {
              //FILENAME (8 bytes)
            fread(&entry[h].FILENAME, 8, 1, img);

              //EXT (3 bytes)
            fread(&entry[h].EXT, 3, 1, img);

              //ATTRIBUTES (1 byte)
            fread(&entry[h].ATTRIBUTES, 1, 1, img);

              //RESERVED (10 bytes)
            fread(&entry[h].RESERVED, 2, 1, img);

              //CREATION_TIME (2 bytes)
            fread(&entry[h].CREATION_TIME, 2, 1, img);

              //CREATION_DATE (2 bytes)
            fread(&entry[h].CREATION_DATE, 2, 1, img);

              //LAST_ACCESS_DATE (2 bytes)
            fread(&entry[h].LAST_ACCESS_DATE, 2, 1, img);

              fseek(img, 2, SEEK_CUR); //skip 2 bytes

              //MODIFY_TIME (2 bytes)
              fread(&entry[h].MODIFY_TIME, 2, 1, img);

              //MODIFY_DATE (2 bytes)
              fread(&entry[h].MODIFY_DATE, 2, 1, img);

              //START_CLUSTER (2 bytes)
              fread(&entry[h].START_CLUSTER, 2, 1, img);

              //FILE_SIZE (4 bytes)
              fread(&entry[h].FILE_SIZE, 4, 1, img);
              h++;
            }

//entry.FILENAME="floppy.img";

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
        }







      }else if (strcmp(cmd, "showfat") == 0) {
        if(floppyLoc[0] =='\0'){
          printf("Error:- Please Mount Floppy first");
          printf("\n");

        }else{

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







    }else if (strcmp(cmd, "showsector") == 0) {

     if(floppyLoc[0] =='\0'){
      printf("Error:- Please Mount Floppy first");
      printf("\n");

    }else{

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

               printf("       0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");
               char *sm;
               sm = strtok(line2," ");
               while(sm !=NULL){
                 if(strcmp(sm, "showsector") != 0){
                  int smi = atoi( sm );
              //printf("%d\n", smi);
                  unsigned char buff[512];
                  int fd = open(floppyLoc, O_RDONLY);
                  lseek(fd,smi*512,(smi+1)*512);
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

              sm= strtok(NULL," ");
            }



            close(fw);     // fd no longer needed - the dup'ed handles are sufficient

          }

          fm= strtok(NULL," ");

        }




      }


      else{
       printf("       0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f\n");
       char *sm;
       sm = strtok(line2," ");
       while(sm !=NULL){
         if(strcmp(sm, "showsector") != 0){
          int smi = atoi( sm );
              //printf("%d\n", smi);
          unsigned char buff[512];
          int fd = open(floppyLoc, O_RDONLY);
          lseek(fd,smi*512,(smi+1)*512);
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

      sm= strtok(NULL," ");
    }

  }
}


} else if (strcmp(cmd, "showfile") == 0) {

 if(floppyLoc[0] =='\0'){
  printf("Error:- Please Mount Floppy first");
  printf("\n");

}else{
  printf("ShowFile Under Contruction\n");

}



}

else if (strcmp(cmd, "fumount") == 0 && floppyLoc[0] !='\0') {
  if(floppyLoc[0] =='\0'){
    printf("Error:- Floppy Cannot UnMount because its not Mounted in the first place ");
    printf("\n");

  }else{


   *floppyLoc=0;
 }





}
else if (strcmp(cmd, "quit") == 0) {
  break;

} 

else{ 
 printf("Command does not exist please use Help to see available commands");
 if (errno)
 {
  perror("command failed");
}

printf("\n");
}
}
}

return 0;
}
