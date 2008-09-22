
//#include "soundcmn.h"
#include "bank.h"
#include "file.h"

#include <stdio.h>
#include <strings.h>
#include <assert.h>

int 
main (int argc, char *argv[])
{

  int i;

  int loopvar;
  int loopcount;

  char oldFile[80];
  char newFile[80];
  
  BANK *oldHeader = NULL;
  BANK *newHeader = NULL;

    PATCH_DISK *oldPatch = NULL;
    PATCH *newPatch = NULL;

  memsize offset=0;
  memsize loopsize=0;

  void * oldptr = NULL;
  void * newptr = NULL;
  void * oldbase = NULL;
  void * newbase = NULL;
  void * tmpptr = NULL;
  
  int oldLength;
  int newLength;

    udword *oldShare = NULL;
    udword *newShare = NULL;

  if (argc < 2 ){
    printf("usage %s <inputfile>\n", argv[0]);
    return 1;
  }

  for (i=1; i<argc; i++) {
    strcpy (oldFile, argv[i]);
    sprintf(newFile, "%s.64", oldFile);

    if (fileExists(oldFile,0) != 0){
      printf("File does not exist.\n");
      return 1;
    }


    printf("Converting %s to %s\n",oldFile, newFile);

    oldbase = fileloadalloc(oldFile, &oldLength, 0);
    oldHeader = oldptr = oldbase;
    if (oldLength == 0){
      printf("Error loading file.\n");
      return 1;
    }

    printf("%s size is %d\n",oldFile, oldLength);

    newLength = 2*oldLength;

    newbase = malloc(2*oldLength);
    newHeader = newbase;
    newptr =  (void*) newbase;

    memset (newbase, 0 ,2*oldLength);
  

    newHeader->id, oldHeader->id;
    newHeader->checksum = oldHeader->checksum;
    newHeader->numpatches = oldHeader->numpatches;

    oldPatch = (PATCH_DISK*)&oldHeader->firstpatch;
    newPatch = (PATCH*)&newHeader->firstpatch;


    offset=newHeader->numpatches * (sizeof(PATCH) - sizeof(PATCH_DISK));
    printf("New data offset =  +%d\n",offset);

    loopsize=newHeader->numpatches;

    for (i=0;i<loopsize;i++){
        printf("%3d: %d: dataoffset:%ld datasize: %ld loopstart: %ld loopend: %ld\n",i, oldPatch->id , oldPatch->dataoffset, oldPatch->datasize, oldPatch->loopstart, oldPatch -> loopend); 
        newPatch->id = oldPatch->id;
        newPatch->priority = oldPatch->priority;
        newPatch->pitch = oldPatch->pitch;
        newPatch->dataoffset = oldPatch->dataoffset + offset;
        newPatch->datasize = oldPatch->datasize;
        newPatch->loopstart = oldPatch->loopstart;
        newPatch->loopend = oldPatch->loopend;
        newPatch->bitrate = oldPatch->bitrate;
        newPatch->flags = oldPatch->flags;
        newPatch->volume = oldPatch->volume;
        newPatch->pan = oldPatch->pan;
        newPatch->waveformat.format = oldPatch->waveformat.format;
        newPatch->waveformat.channels = oldPatch->waveformat.channels;
        newPatch->waveformat.frequency = oldPatch->waveformat.frequency;
        newPatch->waveformat.avgBytesPerSecond = oldPatch->waveformat.avgBytesPerSecond;
        newPatch->waveformat.blockAlign = oldPatch->waveformat.blockAlign;
        newPatch->wavepad = oldPatch->wavepad;
        printf(" becomes dataoffset:%ld \n", newPatch->dataoffset); 

    oldPatch++;
    newPatch++;
    }

    newLength = oldLength + offset;
    
    offset = (memsize)oldptr + oldLength - (memsize) oldPatch;

    memcpy (newPatch, oldPatch, offset);


    if ( fileSave(newFile, newbase, newLength) == 0)
    {
      return 1;
    }
    printf("%s size is %d\n",newFile, newLength);
  }

  free(oldbase);
  free(newbase);

  return 0;
}

