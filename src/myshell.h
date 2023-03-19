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

// Function Prototypes
void display_prompt(); // displays prompt of the pwd and arrow on terminal
void set_shell_env(); // sets shell environment to [pwd]/myshell

void batchmode(char const *file); // reads lines from file on commandline and passes to commands

void tokenise(); // makes standard input into tokens
void commands(char * args[MAX_ARGS]); // uses strcmp to see which command is being invoked and passing to the appropriate function

void echo(char *args[MAX_ARGS]); // echos arguments one per line
int change_directory(char * args[MAX_ARGS]); // changes directory to argument, if non given prints cwd
int dir(char * args[MAX_ARGS]); // lists contents of argument, if no given defaults to current directory
void pause_process(); // pauses shell until the enter key is pressed
void get_environs(); // prints all the environment variables
void quit_process(); // quites the shell
void clear_terminal(); // clears the terminal
int help(); // displays manual