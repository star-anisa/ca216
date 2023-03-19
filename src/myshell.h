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
void display_prompt();
void set_shell_env(char ** argv);

void batchmode(char const *file);

void tokenise();
void commands(char * args[MAX_ARGS]);

void echo(char *args[MAX_ARGS]);
int change_directory(char * args[MAX_ARGS]);
int dir(char * args[MAX_ARGS]);
void pause_process();
void get_environs();
void quit_process();
void clear_terminal();