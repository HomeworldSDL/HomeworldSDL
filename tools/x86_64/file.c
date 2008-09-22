
#include <Types.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <unistd.h>


sdword fileExists(char *_fileName, udword flags)
{

  struct stat fileDetails;

  return (stat (_fileName,&fileDetails));



}



/*-----------------------------------------------------------------------------
    Name        : fileSizeGet
    Description : Finds the length, in bytes, of a file.
    Inputs      : fileName - name of file to find the length of.
                  flags - where to search for file (dosk or in .BIG files)
    Outputs     : ..
    Return      : Length of file.  Generates a fatal error if file doesn't exist.
    Note        : Because this may actually open the file to find it's length,
        it should never be called on files which are already open.
----------------------------------------------------------------------------*/
sdword fileSizeGet(char *_fileName, udword flags)
{
    FILE *file;
    sdword length;
    char *fileName;


    if ((file = fopen(_fileName, "rb")) == NULL)              //open the file
    {
	printf("unable to open file for sizing.\n");
	return 0;
    }
    length = fseek(file, 0, SEEK_END);                      //get length of file
    length = ftell(file);
    fclose(file);
    return(length);
}



/*-----------------------------------------------------------------------------
    Name        : fileLoadAlloc
    Description : Loads specified file into memory after allocating memory for it.
    Inputs      : fileName - path and name of file to be loaded.
                  address - location to store the allocated pointer.
                  flags - NonVolatile, Pyrophoric or file access flags
    Outputs     : *address - pointer to newly allocated and loaded file.
    Return      : number of bytes allocated and loaded.
    Note        : Generates a fatal error if file doesn't exist.  If you don't
        like this behavior, call fileExists() first.
----------------------------------------------------------------------------*/
void * fileloadalloc(char *_fileName, int *length, udword flags)
{
    char  *memoryName = NULL,
          *fileName   = NULL;
    FILE  *inFile     = NULL;

    void *address     = NULL;


    udword nameLength    = 0,
           lengthRead    = 0,
           bigFileIndex  = 0;
    sdword bigfileResult = 0;


    //  try to load from bigfile

    nameLength = strlen(_fileName);                          //set memory name to the

    *length = fileSizeGet(_fileName, flags);                 //get size of file

    address = malloc(*length);//allocate the memory for this file
    if (address == NULL)                                   //if couldn't allocate enough
    {                                                       //generate a fatal error
	return 0;
    }

    if ((inFile = fopen(_fileName, "rb")) == NULL)           //open the file
    {
	return 0;
    }

    lengthRead = fread(address, 1, *length, inFile);        //read the file


    fclose(inFile);

    return address;
}

/*-----------------------------------------------------------------------------
    Name        : fileSave
    Description : Saves data of length, address to fileName
    Inputs      : fileName, address, length
    Outputs     :
    Return      : length
----------------------------------------------------------------------------*/
sdword fileSave(char *_fileName, void *address, sdword length)
{
    FILE *outFile;
    char *fileName;
    sdword lengthWrote;


    if ((outFile = fopen(_fileName, "wb")) == NULL)           //open the file
    {
	printf("Unable to create output file for writing.\n");
	return 0;
    }

    lengthWrote = fwrite(address,1,length,outFile);


    fclose(outFile);

    printf("fileSave: saved %d bytes of '%s' to 0x%x\n", length, _fileName, address);

    return lengthWrote;
}


