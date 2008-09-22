
#include "font.h"
#include "font_l.h"
#include "file.h"

#include <stdio.h>
#include <strings.h>
#include <assert.h>

int 
main (int argc, char *argv[])
{

  int i;
  int j;
  int offset;

  char oldfile[80];
  char newfile[80];
  
  fontfileheader_disk *oldfontfileheader = NULL;
  fontfileheader *newfontfileheader = NULL;

  fontheader_disk *oldfontheader = NULL;
  fontheader *newfontheader = NULL;

  charheader_disk *oldcharheader = NULL;
  charheader *newcharheader = NULL;

  int fontfileheaderdiff;
  int fontheaderdiff;
  int charheaderdiff;

  sdword no_of_chars = 0;

  void * oldrootptr = NULL;
  void * newrootptr = NULL;

  void * oldptr = NULL;
  void * newptr = NULL;
  

  int oldlength;
  int newlength;

  fontfileheaderdiff = sizeof(fontfileheader) - sizeof(fontfileheader_disk);
  fontheaderdiff = sizeof(fontheader) - sizeof(fontheader_disk);
  charheaderdiff = sizeof(charheader) - sizeof(charheader_disk);

  offset = fontheaderdiff + 256 * charheaderdiff;

//  printf("args = %d\n",argc);

  if (argc < 2 ){
    printf("usage %s <inputfile>\n", argv[0]);
    return 1;
  }

  for (i=1; i <argc; i++){

    strcpy (oldfile, argv[i]);
    sprintf(newfile, "%s.64", oldfile);
  
    if (fileExists(oldfile,0) != 0){
      printf("File does not exist.\n");
      return 1;
    }
  
  
    printf("Converting %s to %s\n",oldfile, newfile);
  
    oldrootptr = fileloadalloc(oldfile, &oldlength, 0);
    if (oldlength == 0){
      printf("Error loading file.\n");
      return 1;
    }
  
    printf("%s size is %d\n",oldfile, oldlength);
  
    oldfontfileheader = oldptr = oldrootptr;
  
 //   newlength = oldlength + lendiff;
  
    newfontfileheader = newptr = newrootptr = malloc(2*oldlength); //too much, but better than not enough. :)
    memset(newrootptr, 0,2*oldlength);


//    memcpy(newptr,oldptr,8+2+2); // Copy first part of fontfileheader
    strncpy(newfontfileheader->identification, oldfontfileheader->identification, 8);
    newfontfileheader->version = oldfontfileheader->version;
    newfontfileheader->flags = oldfontfileheader->flags;
    
  
    oldfontheader = &oldfontfileheader->header;
    newfontheader = &newfontfileheader->header;

    no_of_chars = newfontheader->nCharacters = oldfontheader->nCharacters;
    newfontheader->spacing = oldfontheader->spacing;
    newfontheader->fullHeight = oldfontheader->fullHeight;
    newfontheader->baseLine = oldfontheader->baseLine;
    if (oldfontheader->name != 0){
      newfontheader->name = (char *)((Uint64)oldfontheader->name +  fontfileheaderdiff);
    }
    newfontheader->imageWidth = oldfontheader->imageWidth;
    newfontheader->imageHeight = oldfontheader->imageHeight;
    newfontheader->nColors = oldfontheader->nColors;
    newfontheader->palette = (color *)((Uint64)oldfontheader->palette +  fontfileheaderdiff);
    newfontheader->image = (ubyte *)((Uint64)oldfontheader->image +  fontfileheaderdiff);
    newfontheader->glFont = (void *)((Uint64)oldfontheader->glFont +  fontfileheaderdiff);
    memcpy(newfontheader->reserved, newfontheader->reserved, 12);
    for (j=0; j<256; j++)
    {
      if (oldfontheader->character_disk[j] != 0){
        newfontheader->character[j] = (charheader *)((Uint64)oldfontheader->character_disk[j] +  fontfileheaderdiff);
      }
    }
      
    memcpy(newrootptr + sizeof(fontfileheader), oldrootptr + sizeof(fontfileheader_disk), oldlength - sizeof(fontfileheader_disk));

    
    newlength = oldlength + fontfileheaderdiff;
  
    if ( fileSave(newfile, newrootptr, newlength) == 0)
    {
      return 1;
    }
    printf("%s size is %d\n",newfile, newlength);
//assert (0);
  
    free(oldrootptr);
    free(newrootptr);
  }
  
  return 0;
} 
  
