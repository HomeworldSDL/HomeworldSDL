
#include "lif.h"
#include "texreg.h"
#include "file.h"

#include <stdio.h>
#include <strings.h>
#include <assert.h>

int 
main (int argc, char *argv[])
{

  int i;

  char oldfile[80];
  char newfile[80];
  
  lifheader_disk *oldheader = NULL;
  lifheader *newheader = NULL;

  int lendiff;

  void * oldptr = NULL;
  void * newptr = NULL;
  

  int oldlength;
  int newlength;

  lendiff = sizeof(lifheader) - sizeof(lifheader_disk);

//  printf("args = %d\n",argc);

  if (argc < 2 ){
    printf("usage %s <inputfile>\n", argv[0]);
    return 1;
  }

  for (i=1; i<argc; i++) {
    strcpy (oldfile, argv[i]);
    sprintf(newfile, "%s.64", oldfile);

    if (fileExists(oldfile,0) != 0){
      printf("File does not exist.\n");
      return 1;
    }


    printf("Converting %s to %s\n",oldfile, newfile);

    oldptr = fileloadalloc(oldfile, &oldlength, 0);
    if (oldlength == 0){
      printf("Error loading file.\n");
      return 1;
    }

    printf("%s size is %d\n",oldfile, oldlength);

    oldheader = oldptr;

    newlength = oldlength + lendiff;

    newptr = malloc(newlength);
    newheader = newptr;
  
    strcpy(newheader->ident,oldheader->ident);
    newheader->version = oldheader->version;
    newheader->flags = oldheader->flags;
    newheader->width = oldheader->width;
    newheader->height = oldheader->height;
    newheader->paletteCRC = oldheader->paletteCRC;
    newheader->imageCRC = oldheader->imageCRC;
    newheader->data = (ubyte *)((Uint64)oldheader->data + lendiff);
    newheader->palette = (color*)((Uint64)oldheader->palette + lendiff);
    newheader->teamEffect0 = (ubyte *)((Uint64)oldheader->teamEffect0 + lendiff);
    newheader->teamEffect1 = (ubyte *)((Uint64)oldheader->teamEffect1 + lendiff);
    
    memcpy (newptr+sizeof(lifheader), oldptr + sizeof(lifheader_disk), oldlength - sizeof(lifheader_disk)); 
  
    if ( fileSave(newfile, newptr, newlength) == 0)
    {
      return 1;
    }
    printf("%s size is %d\n",newfile, newlength);
  }

  free(oldptr);
  free(newptr);

  return 0;
}

