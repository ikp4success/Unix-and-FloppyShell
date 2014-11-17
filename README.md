Project2: A Floppy Disk Shell (Due at 11:59:59pm on 11/15/2014 (EST))

New Group Arrangment.

Questions? Please first check our FAQ link before you submit your inquiries.

Based on the floppy disk program you have developed in project, you are asked to change the program to a floppy shell that supports Linux shell commands in addition to your existing floppy disk related commands. You are expected to use either C or C++ programming language. Your implementation must work on Linux machine in BU004b lab.

Required Modules:

(20 points) Your shell should repeatedly display a prompt and allow the user to enter a command to run. Your shell is supposed to read the input from system standard input, parse the line with command and arguments, and execute. You may want to use fork() and exec*() system calls.
Your shell should find the build-in command (see part2) in your current working directory first. If not found, it should search the directories in the shell's pathname environment (see part2).
You are not allowed to use system(), as it invokes the system's /bin/sh shell. You should not use execlp() or execvp(), because your shell has its own path variable (explained in part2).
By convention, the command arguments are seperated by white spaces. Please describe your customized argument seperation rules in your README document if you have special arrangement (not recommended though). Your shell does not need to handle special characters, like ",", "?", "\", except the redirection operators (<, >) in part3 and the pipeline operator (|) required in part4.
(30 points) In addition to those built-in commands you implemented in project2, implement the following two additional build-in commands (running in the main process rather than the child process).
cd: is a command, obviously, to change directories. You may want to use the chdir system call.
path: is not only a command to show the current command searching pathnames (if no argument is provided), but also a utility to modify (either add or remove) the command searching pathnames.
path (without arguments) displays the pathnames currently set. e.g., "/bin:/sbin"
path + /abc/def appends the pathname "/abc/def" to the "path" variable. e.g., "/bin:/sbin:/abc/def".
path - /abc/def removes the pathname "/abc/def" from the "path" variable.
(30 points) Extend your shell with I/O redirection (<, >)
You may want to use open(), close(), dup2() system calls.
Please refer to Unix file descriptors for the mapping of stdin, stdout and stderr to 0, 1, 2.
Your shell does not need to handle rediction for build-in commands: exit, cd, path.
(20 points) Extend you shell with pipeline (|). Given the following command,
$ cmd1 | cmd2 | cmd3
the standard output of cmd1 is connected to the standard input of cmd2, and the standard output of cmd2 is connected to the standard input of cmd3.
You may want to use pipe() system call.
You may assume at most two pipeline operators (|) in my test command while grading. Actually, it is easy to remove this assumption and your shell should handle unlimited number of "|"s.
(10 points) This is optional (as a bonus) for undergrads, but is required for grads. Further extend your shell to support a mixture of pipelines and redirections like the following examples:
$ cmd1 | cmd2 > file1
$ cmd1 < file1 | cmd2 > file2
Provide a Makefile so make command would produce the executable.
Readme: you are required to write a README document (TXT only) that describes your project design detail and the execution sequence (with the commands). In particular, please explicitly state which part, if there is any, does not work and the possible reasons why that module does not work. For those working modules, please give a brief (in short) sample output.
Submission:

Create a folder with the name of your assigned group name, e.g., Group_A.
Copy all source code files/directories into the created folder.
Make sure you have sshed to grail.cba.csuohio.edu, and then type:
$ turnin -c cis340w -p proj3 Group_A






NOTES:

           Objective : 
To create a flop shell with commands to read from a floppy disk. Shell should have help, Fmount, Fumount, Structure,traverse,showfat,showsector,showfile, Output Redirection and quit.
The Shell - uses a while(1) loop, then uses fgets to read chracters from stream, when it see a new line it stops reading.
Fmount- mounts the floppy by assinging the variable to 'floppyLoc'
Fumount- it clears out the 'floppyloc' 
Showsector- show the content of the specified sector number, so it uses the open function the read the interger contents into fseek then i used the while loop to arrange the content.
Help - Show the help information to use the shell
Structure - show the list of structure of floppydisk using fseek and fread to get the number of bytes of it s structure
OutPut Redirection - works like Unix redirection.

Path(parent) - to show the current command searching pathnames (if no argument is provided), but also a utility to modify (either add or remove) the command searching pathnames.
cd(parent) - to change directories.
pipe (| cmd | cmd |) - implement multiple commands at the sametime display their input .



                    What Works:
The Shell, Help, Fmount, Fumount, showsector, showfat, showfile, quit, OutPut and input Redirection , cd, *path, pipe (i.e | cmd | cmd |).

                    What Work Partially:
** pipe question--> cmd | cmd does not work , you will have to this way-->   | cmd | cmd | .
                    What does not Work
** The Optional(bonus) question e.g cmd < file | cmd > file couldn't do this question.
**Redirection does not work for fmount or fumount or quit.
** Pipe question won't work for quit, fmount , fumount. i.e quit | fmount  -- won't work.

                    How To Use It:
**You have to fmount media/floppy.img for example before you can use any other commands, except help,path,cd,exit and quit.
**Any other commands typed not given in class material will not work.





















