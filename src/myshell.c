// Anisa Hoxha, 21413586
/* 
I declare that this material, which I now submit for assessment,
is entirely my own work and has not been taken from the work of others
save and to the extent that such work 
has been cited and acknowledged within the text of my work.
*/

//////////////////////////////////////////////////

// Incudes 
#include "utilities.c"


int main (int argc, char ** argv)
{
    set_shell_env(argv);

	if (argc > 1){
		batchmode(argv[1]);
		exit(0);
	}

    /* keep reading input until "quit" command or eof of redirected input */
    while (!feof(stdin)) { 
        /* get command line from input */
		display_prompt();
		tokenise();

    }
    return 0; 
}