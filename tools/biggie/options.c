#include <stdio.h>
#include "options.h"

char OptCommand;	    // a|d|v|x (add|delete|view|extract)

int  OptCompression;	// true/false
int  OptPathnames;      // true/false
int  OptNewer;	        // true/false
int  OptMove;           // true/false
int  OptOverwrite;		// true/false

void optDefaultsSet(void) {
	OptCommand     = 'a';
	OptCompression = 1;
	OptMove        = 0;
	OptNewer       = 0;
	OptOverwrite   = 1;
	OptPathnames   = 1;
}

//	override OptXXXX variables with user argument
//
//	return 1 if arg is actually an argument (ie, if it
//	starts with a '-'.  return 0 otherwise.
int optProcessArgument(char *arg)
{
	if (arg[0] != '-') {
		return 0;
    }

	switch (arg[1])	{
        case 'a':
        case 'd':
        case 'v':
        case 'x':
        case 'f':
        case 'u':
            OptCommand = arg[1];
            break;
            
        case 'c':
            optSetBoolean(arg, &OptCompression);
            break;
            
        case 'm':
            optSetBoolean(arg, &OptMove);
            break;
            
        case 'n':
            optSetBoolean(arg, &OptNewer);
            break;
            
        case 'o':
            optSetBoolean(arg, &OptOverwrite);
            break;
            
        case 'p':
            optSetBoolean(arg, &OptPathnames);
            break;
            
        default:
            printf("WARNING: Undefined option \"%s\"\n", arg);
            break;
	}

	return 1;
}

void optSetBoolean(char *arg, int *option) {
    // arg = "-"<char><0|1>
    char flag  = arg[1];
    char value = arg[2];

    if (value == '0' || value == '1') {
        *option = (int)(value - '0');
    }
    else {
        printf("WARNING: Invalid option setting \"%s\"; using \"-%c0\"\n", arg, flag);
        *option = 0;
    }
}
