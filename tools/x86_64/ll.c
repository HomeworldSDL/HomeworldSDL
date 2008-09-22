
#include "texreg.h"
#include "ll.h"
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
  
  llfileheader *oldHeader = NULL;
  llfileheader *newHeader = NULL;

    llelement_disk *oldElement = NULL;
    llelement *newElement = NULL;

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
  

    offset=0;

    strncpy(newHeader->ident, oldHeader->ident,8);
    newHeader->version = oldHeader->version;
    newHeader->nElements = oldHeader->nElements;
    newHeader->stringLength = oldHeader->stringLength;
    newHeader->sharingLength = oldHeader->sharingLength;
    newHeader->totalLength = oldHeader->totalLength + oldHeader->sharingLength;

    offset +=sizeof(llfileheader);
    newptr +=sizeof(llfileheader);

    tmpptr=oldbase + 28 + ( newHeader->nElements * sizeof(llelement_disk));

    oldElement= oldbase+ /*sizeof(llfileheader)*/ 28/*old llfileheader*/; 
    newElement= newbase+ sizeof(llfileheader); 

    for (i=0;i<newHeader->nElements;i++){
        newElement->textureName = (char*)((memsize)oldElement->textureName); 
        newElement->width = oldElement->width; 
        newElement->height = oldElement->height; 
        newElement->flags = oldElement->flags; 
        newElement->imageCRC = oldElement->imageCRC; 
        newElement->nShared = oldElement->nShared; 
        if (oldElement->sharedTo != -1){    
    //        printf("%x: pointer to %x\n",i,oldElement->sharedTo);
            newElement->sharedTo = (sdword*)((memsize)oldElement->sharedTo); 
        }
        else {
            newElement->sharedTo = -1;
        }
        if (oldElement->sharedFrom != -1){    
     //       printf("%x:pointer to parent %x\n",i,oldElement->sharedFrom);
            newElement->sharedFrom = oldElement->sharedFrom; 
        }
        else{
            newElement->sharedFrom = -1;
        }

        printf("%5d: %s: shared:%2d sharedTo: %5d sharedFrom:%5d\n",i, (memsize)tmpptr + (memsize)newElement->textureName, newElement->nShared,  newElement->sharedTo, newElement->sharedFrom); 

        offset += sizeof(llelement);
        newptr += sizeof(llelement);
        oldElement++;
        newElement++;

    }

    oldptr = (void*)oldElement;
    memcpy(newptr,oldptr,newHeader->stringLength);
    offset+=newHeader->stringLength;
    newptr+=newHeader->stringLength;

    loopcount= oldHeader->sharingLength / 4;

    
    oldShare=(udword*)((memsize)oldptr +newHeader->stringLength) ;
    newShare=(sdword*)newptr;

    for(loopvar = 0; loopvar<loopcount; loopvar++){
        newShare[loopvar] = oldShare[loopvar] ;
    }

    offset+=newHeader->sharingLength;
    newptr+=newHeader->sharingLength;

    newHeader->totalLength = offset - sizeof(llfileheader);

    if ( fileSave(newFile, newbase, offset) == 0)
    {
      return 1;
    }
    printf("%s size is %d\n",newFile, offset);
  }

  free(oldbase);
  free(newbase);

  return 0;
}

