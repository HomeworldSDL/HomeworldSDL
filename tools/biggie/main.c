//
//    BIGGIE - Bigfile support utility
//
//    Copyright (C) 1998 Relic Entertainment Inc.
//
//    Created May 1998 by Darren Stone.
//    See bigfile.h for version notes.
//

#include <stdio.h>

#include "../../src/Game/BigFile.h"
#include "options.h"

extern char OptCommand;
extern int  OptCompression;
extern int  OptPathnames;
extern int  OptNewer;
extern int  OptOverwrite;
extern int  OptMove;

void display_version(void) {
    printf("Biggie - version %s  [%s%s]\n", BIGGIE_VERSION, BF_FILE_HEADER, BF_VERSION);
}

void usage(void) {
    printf("\n*.big file support utility.  Copyright (C)1998 Relic Entertainment Inc.\n\n");

    printf("Usage:  biggie <options> <bigfile> [<files...>]\n\n");

    printf("Primary options:\n");
    printf("-a  Add/update files to bigfile\n");
    printf("-f  Fast-create a bigfile\n");
    printf("-d  Delete files from bigfile\n");
    printf("-u  Create a patch bigfile (see special usage, below)\n");
    printf("-v  View bigfile contents\n");
    printf("-x  Extract files from bigfile\n");

    printf("\nAdditional options, as they apply to the primary options:\n");
    printf("         AFDUVX\n");
    printf("-c[0|1]  **      Compress files               (default: 1)\n");
    printf("-m[0|1]  **   *  Move files to/from bigfile   (default: 0)\n");
    printf("-n[0|1]  *    *  Only newer files             (default: 0)\n");
    printf("-o[0|1]       *  Overwrite existing files     (default: 1)\n");
    printf("-p[0|1]  **   *  Store/restore full pathnames (default: 1)\n");

    printf("\n----------------------------------------------------------\n\n");

    printf("Example usage:\n\n");
    printf("    biggie -a -n1 -p0 fonts.big \\fonts\\*.hff \\data\\*.hff\n\n");

    printf("File lists (files with one filename per line) may be used in\n");
    printf("place of filenames by using an @ symbol to differentiate them:\n\n");
    
    printf("    biggie -a -n1 data.big dmp*.lif @otherfiles.txt\n\n");

    printf("Creating a \"patch\" bigfile has the following usage:\n\n");
    
    printf("    biggie -u oldbigfile newbigfile patchbigfile\n\n");
    
    printf("Files that have changed or have been added in newbigfile\n");
    printf("will be placed in the patchbigfile. A byte-for-byte compare\n");
    printf("is performed on every file (i.e.: date/time-stamps are ignored).\n");
    printf("Additional options do not apply to patching.\n");
    
    exit(-1);
}

int main(int argc, char *argv[]) {
    char   bigfilename[BF_MAX_FILENAME_LENGTH + 1];
    char **filenames = NULL;
    int    numOpts   = 0;
    int    numFiles  = 0;

    display_version();

    if (argc < 3) {
        usage();
    }

    // establish defaults for command line options
    optDefaultsSet();

    // override defaults with user options
    while ((numOpts+1) < argc && 
            optProcessArgument(argv[numOpts+1])) {
        ++numOpts;
    }

    if (!numOpts) {
        printf("ERROR: No options specified.\n\n");
        usage();
    }

    strcpy(bigfilename, argv[numOpts + 1]);
    numFiles  = argc - (numOpts + 2);
    filenames = argv + (numOpts + 2);

    // execute primary command, with any applicable options
    switch (toupper(OptCommand)) {
        case 'A':
            bigAdd(bigfilename, numFiles, filenames, 
                OptCompression, OptNewer, OptMove, OptPathnames, 1);
            break;
            
        case 'D':
            bigDelete(bigfilename, numFiles, filenames, 1);
            break;
                        
        case 'F':
            bigFastCreate(bigfilename, numFiles, filenames, 
                OptCompression, OptNewer, OptMove, OptPathnames, 1);
            break;
            
        case 'U':
            {   // private block to declare these arrays under plain C
                char oldfilename[  BF_MAX_FILENAME_LENGTH + 1],
                     newfilename[  BF_MAX_FILENAME_LENGTH + 1],
                     patchfilename[BF_MAX_FILENAME_LENGTH + 1];

                if (numOpts != 1 || argc != 5) {
                    printf("ERROR: Invalid patch option\n");
                    return 0;
                }
                
                strcpy(oldfilename,   argv[numOpts+1]);
                strcpy(newfilename,   argv[numOpts+2]);
                strcpy(patchfilename, argv[numOpts+3]);
                
                bigPatch(oldfilename, newfilename, patchfilename, 1);
            }
            break;

        case 'V':
            bigView(bigfilename, 1);
            break;
        
        case 'X':
            bigExtract(bigfilename, numFiles, filenames, 
                OptNewer, OptMove, OptPathnames, OptOverwrite, 1);
            break;
        
        default:
            printf("ERROR: Unrecognized option: %s\n", OptCommand);
            usage();
            break;
    }

    return 0;
}
