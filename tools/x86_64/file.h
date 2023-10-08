// Is this file somehow supposed to be a stripped-down version of src/Game/File.h ??

#include <Types.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <unistd.h>


sdword fileExists(char *_fileName, udword flags);
sdword fileSizeGet(char *_fileName, udword flags);
void * fileloadalloc(char *_fileName, int *length, udword flags);
sdword fileSave(char *fileName, void *address, sdword length);
