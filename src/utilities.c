// Anisa Hoxha, 21413586
/* 
I declare that this material, which I now submit for assessment,
is entirely my own work and has not been taken from the work of others
save and to the extent that such work 
has been cited and acknowledged within the text of my work.
*/

//////////////////////////////////////////////////

// Includes
#include "myshell.h"

void display_prompt(){
	char prompt[1024] = "";
	char *arrow = " ==>";                	// shell prompt
	char *pwd = getenv("PWD");
	strcat(prompt, pwd);
	strcat(prompt, arrow);
	fputs (prompt, stdout); 				// write prompt
}

void set_shell_env(){
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

/* 
The following code for the functions 'tokenise' and 'batchmode' 
has been heavily modified from Ian G Graham's source code.
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

void batchmode(char const *file){
	char buf[MAX_BUFFER];
	FILE *fptr;
	char * args[MAX_ARGS];                     // pointers to arg strings
	char ** arg; 

	if ((fptr = fopen(file, "r")) == NULL) {
		printf("Error! File cannot be opened.");
		// Program exits if the file pointer returns NULL.
		exit(1);
	}

	// tokenising input from file
	if (fgets (buf, MAX_BUFFER, fptr)) { // read a line from file
		/* tokenize the input into args array */
		arg = args;
		*arg++ = strtok(buf,SEPARATORS);   // tokenise input

		while ((*arg++ = strtok(NULL,SEPARATORS)));

		// last entry will be NULL 
		if (args[0]) {                     // if there's anything there
			args[strcspn(*args, "\n")] = '\0'; // make newline charcter at the end of a line into a null character
			//printf("%s\n", *args);
			commands(args);
		}
	}
}

void tokenise(){    
	char buf[MAX_BUFFER];                      // line buffer
	char * args[MAX_ARGS];                     // pointers to arg strings
	char ** arg;                               // working pointer thru args

	if (fgets (buf, MAX_BUFFER, stdin )) { // read a line
		/* tokenize the input into args array */
		arg = args;
		*arg++ = strtok(buf,SEPARATORS);   // tokenise input

		while ((*arg++ = strtok(NULL,SEPARATORS)));

		// last entry will be NULL 
		if (args[0]) {                     // if there's anything there
			args[strcspn(*args, "\n")] = '\0';
			//printf("\n%s\n", *args);
			commands(args);
		}

	}
}

void commands(char * args[MAX_ARGS]){
	//char **arg;

	/* check for internal/external command */
	if (!strcmp(args[0],"clr")) { // "clear" command
		clear_terminal();
	}

	else if (!strcmp(args[0],"quit")){   // "quit" command
		quit_process();
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

	else if (!strcmp(args[0],"echo")){ // "echo" command
		echo(args);
	}

	else if (!strcmp(args[0],"help")){ // "help" command
		help();
	}

	else{// else pass command onto OS
		int pid = fork();
		if (pid < 0){
			fprintf(stderr, "Could not Fork\n");
			return;
		}
		else if (pid == 0){
			//printf("\n");
			execvp(args[0], args);
		}
	}
}

// The following are all the internal commands
int change_directory(char * args[MAX_ARGS]){ // 'cd' command
	char cwd[256];
	if(args[1] == NULL){ // prints current working directory if no other argument given
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
	return 0;
}
// code adapted from: https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-chdir-change-working-directory 

void clear_terminal(){
	system("clear");
}

int dir(char * args[MAX_ARGS]){ // 'Dir' command
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
	return 0;
}
// code adapted from: https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/

void get_environs(){ //'Environ' command

	for (int i = 0; environ[i] != NULL; i++){ // lists out each of the environ variables and their values one per line
		printf("%s\n",environ[i]);
	}
}

void echo(char *args[MAX_ARGS]){ // 'Echo' command
	for (int i = 1; args[i] != NULL; i++){ // prints each argument after 'echo', one per line
		printf("%s\n", args[i]);
	}
}

int help(){
	char line[1000]; // Define a buffer for reading lines
	int line_count = 0; // Initialize the line count to zero

	FILE *file = fopen("../manual/readme", "r"); // Open the file for reading
	if (file == NULL) {
		printf("Unable to open file\n");
		return 1;
	}

	while (fgets(line, sizeof(line), file)) {
		printf("%s", line); // Print the current line

		line_count++; // Increment the line count
	}

	fclose(file); // Close the file
	return 0;
}

void pause_process(){ // 'Pause' command
	printf("Program paused.\nPress ENTER key to Continue\n");
	while(getchar()!='\n'); // Program will only continue if the enter key is pressed
}
// Pause program until enter key is pressed
// code adapted from: https://stackoverflow.com/questions/18801483/press-any-key-to-continue-function-in-c 

void quit_process(){ // 'Quit' command
	exit(0);
}