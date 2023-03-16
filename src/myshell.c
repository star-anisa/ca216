// Anisa Hoxha, 21413586
/* 
I declare that this material, which I now submit for assessment,
is entirely my own work and has not been taken from the work of others
save and to the extent that such work 
has been cited and acknowledged within the text of my work.
*/

//////////////////////////////////////////////////

// Incudes 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>


// Global Variables
#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 200                           // max # args
#define SEPARATORS " \t\n"                     // token separators

extern char **environ;

void commands(char * args[MAX_ARGS]);

void echo(char *args[MAX_ARGS]){
	for (int i = 1; args[i] != NULL; i++){
		printf("%s\n", args[i]);
	}
}

int change_directory(char * args[MAX_ARGS]){
	char cwd[256];
	if(args[1] == NULL){
		printf("Current Working Directory: %s\n", getcwd(cwd, sizeof(cwd)));
	}
	else{
		if (chdir(args[1]) < 0){
			perror("chdir() error");
			return 1;
		}
		if (setenv("PWD", getcwd(cwd, sizeof(cwd)), 1) == -1) { // sets the eniron name PWD to the path in args[1]
			perror("setenv() error");
			return 1;
		}
	}
}
// code adapted from: https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-chdir-change-working-directory 


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
    if (getcwd(cwd, sizeof(cwd)) == NULL){ // gets the name of the current working directory and stores it in cwd
		perror("getcwd() error");
		return;
	}
    else{
		strcat(cwd, "/myshell");
		if (setenv("SHELL", cwd, 1) == -1) { // swets the eniron name SHELL to the value in cwd
			perror("setenv");
			return;
		}
	}
}


int dir(char * args[MAX_ARGS]){ // 'Dir' command, lists files in a given directory
	DIR *dir;					// If no directory given, defualts to current directory
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
// code adapted from: https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/


void pause_process(){ // 'Pause' command
	printf("Program paused.\nPress ENTER key to Continue\n");  
	getchar(); // Program will only continue if the enter key is pressed
}
// Pause program until enter key is pressed
// code adapted from: https://stackoverflow.com/questions/18801483/press-any-key-to-continue-function-in-c 


void get_environs(){

	for (int i = 0; environ[i] != NULL; i++){ // lists out each of the environ variables and their values one per line
    	printf("%s\n",environ[i]);
	}
}

void batchmode(char const *file){
    char *args[MAX_ARGS];
    char line[MAX_BUFFER];
    FILE *fptr;
    long pos = 0;

    if ((fptr = fopen(file, "r")) == NULL) {
        printf("Error! File cannot be opened.");
        // Program exits if the file pointer returns NULL.
        exit(1);
    }

    // reads text until newline is encountered
    while (fgets(line, MAX_BUFFER, fptr)) {
        args[0] = strtok(line, SEPARATORS);   // tokenise input

        int i = 0;
        while (args[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            args[i] = strtok(NULL, SEPARATORS);
        }

        if (args[0]) {                     // if there's anything there
            commands(args);
        }
    }

    fclose(fptr);
    return;
}

void commands(char * args[MAX_ARGS]){
	char **arg;
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

	else if (!strcmp(args[0],"cd")){ // "change directory" command
		change_directory(args);
	}

	else if (!strcmp(args[0],"echo")){ // "change directory" command
		echo(args);
	}

	else{/* else pass command onto OS (or in this instance, print them out) */
		arg = args;
		while (*arg) {
			fprintf(stdout,"%s ",*arg++);
			fputs ("\n", stdout);
		}
	}
}




/* 
The following code has been heavily modified from Ian G Graham's source code.
details below.
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

		if (argc > 1){
			batchmode(argv[1]);
			exit(0);
		}

        if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
            /* tokenize the input into args array */
            arg = args;
            *arg++ = strtok(buf,SEPARATORS);   // tokenise input

            while ((*arg++ = strtok(NULL,SEPARATORS)));

            // last entry will be NULL 
            if (args[0]) {                     // if there's anything there
				commands(args);
            }

        }
    }
    return 0; 
}