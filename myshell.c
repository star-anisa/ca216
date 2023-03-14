// will change credits to say that it was modified from the original creators code

/* 
strtokeg - skeleton shell using strtok to parse command line
usage:
strtokeg
reads in a line of keyboard input at a time, parsing it into
tokens that are separated by white spaces (set by #define SEPARATORS).
can use redirected input if the first token is a recognised internal command, 
then that command is executed. otherwise the tokens are printed on the display.
internal commands:
clear - clears the screen
quit - exits from the program
********************************************************************
version: 1.0
date:    December 2003
author:  Ian G Graham
School of Information Technology
Griffith University, Gold Coast
ian.graham@griffith.edu.au
copyright (c) Ian G Graham, 2003. All rights reserved.
This code can be used for teaching purposes, but no warranty,
explicit or implicit, is provided.
*******************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 200                            // max # args
#define SEPARATORS " \t\n"                     // token separators

extern char **environ;

void display_prompt(){
    char prompt[1024] = "";
	char *arrow = " ==>";                	// shell prompt
	char *pwd = getenv("PWD");
	strcat(prompt, pwd);
	strcat(prompt, arrow);
	fputs (prompt, stdout); 				// write prompt
}

void set_shell_env(char ** argv){
	char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) == NULL) // gets the name of the current working directory and stores it in cwd
      perror("getcwd() error");
    else{
		strcat(cwd, "/");
		strcat(cwd, argv[0]);
		if (setenv("SHELL", cwd, 1) == -1) { // swets the eniron name SHELL to the value in cwd
			perror("setenv");
			exit(1);
		}
	}
}

int dir(char * args[MAX_ARGS]){ // 'Dir' command, lists files in current directory
	DIR *dir;
    struct dirent *entry;

	if (args[1] == NULL){
    	// Open the directory that you are in right now
    	dir = opendir(".");
	}
	else {
		dir = opendir(args[1]);
	}
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    // Read the directory entries
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dir);
}

void pause_process(){ // 'Pause' command
	printf("Program paused.\nPress ENTER key to Continue\n");  
	getchar(); // Program will only continue if the enter key is pressed
}

void get_environs(){

	for (int i = 0; environ[i] != NULL; i++){ // lists out each of the environ variables and their values one per line
    	printf("%s\n",environ[i]);
	}
}

void commands(char * args[MAX_ARGS], char ** arg){
	/* check for internal/external command */
	if (!strcmp(args[0],"clr")) { // "clear" command
		system("clear");
	}

	else if (!strcmp(args[0],"quit")){   // "quit" command
		// free everything
		exit(0);                    // break out of 'while' loop
	}

	else if (!strcmp(args[0],"dir")){   //"dir" command
		dir(args);
	}

	else if (!strcmp(args[0],"pause")){ // "pause" command
		pause_process();
	}

	else if (!strcmp(args[0],"environ")){ // "environ" command
		get_environs();
	}

	else{/* else pass command onto OS (or in this instance, print them out) */
		arg = args;
		while (*arg) {
			fprintf(stdout,"%s ",*arg++);
			fputs ("\n", stdout);
		}
	}
}


int main (int argc, char ** argv)
{
    char buf[MAX_BUFFER];                      // line buffer
    char * args[MAX_ARGS];                     // pointers to arg strings
    char ** arg;                               // working pointer thru args

    set_shell_env(argv);

    /* keep reading input until "quit" command or eof of redirected input */
    while (!feof(stdin)) { 
        /* get command line from input */
		display_prompt();

        if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenise input

            while ((*arg++ = strtok(NULL,SEPARATORS)));

            // last entry will be NULL 
            if (args[0]) {                     // if there's anything there
				commands(args, arg);
            }
        }
    }
    return 0; 
}