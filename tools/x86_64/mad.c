
#include "Mesh.h"
#include "MeshAnim.h"
#include "mad.h"
#include "file.h"

#include <stdio.h>
#include <strings.h>
#include <assert.h>

int 
main (int argc, char *argv[])
{

  int i;
  int j;

  int loopvar;
  int loopcount;

  char oldFile[80];
  char newFile[80];

  int oldLength;
  int newLength;

    int madOpArgs=0;
    int madOpArgsThis=0;

  
  madheader_disk *oldHeader = NULL;
  madheader *newHeader = NULL;

    madanimation_disk *oldAnim = NULL;
    madanimation *newAnim = NULL;

    madobjpath_disk *oldMOP = NULL;
    madobjpath *newMOP = NULL;

    int madobjloop = 0;

  memsize offset=0;
  memsize loopsize=0;

  void * oldptr = NULL;
  void * newptr = NULL;
  void * oldbase = NULL;
  void * newbase = NULL;
  

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

    offset = newLength;

    newbase = malloc(2*oldLength);
    newHeader = newbase;
    newptr =  (void*) newbase;

    memset (newbase, 0 ,2*oldLength);
  

    strncpy(newHeader->identifier,oldHeader->identifier, 8);
    newHeader->version = oldHeader->version;
    newHeader->stringBlockLength = oldHeader->stringBlockLength;
    newHeader->length = oldHeader->length;
    newHeader->framesPerSecond = oldHeader->framesPerSecond;
    newHeader->nObjects = oldHeader->nObjects;
    newHeader->nAnimations = oldHeader->nAnimations;

    oldAnim=(madanimation_disk*)&oldHeader->anim;    
    newAnim=(madanimation*)&(newHeader->anim);    

    for (loopvar = 0; loopvar < newHeader->nAnimations; loopvar++){
        newAnim->name = (char*)(memsize)oldAnim->name;
        newAnim->startTime = oldAnim->startTime;
        newAnim->endTime = oldAnim->endTime;
        newAnim->flags = oldAnim->flags;

        newAnim++;
        oldAnim++;
    }
        
    newHeader->objPath=(madobjpath*)((memsize) newAnim - (memsize)newbase);

    oldMOP = (madobjpath_disk*) oldAnim;
    newMOP = (madobjpath*) newAnim;

    oldptr = oldbase + oldMOP->times;

    newptr = (void*) newMOP;
    newptr += sizeof(madobjpath) * newHeader->nObjects;
    offset = (memsize) ( (memsize)(newptr) - (memsize)(newbase));

    madOpArgs=sizeof(udword) /*times*/ + sizeof(tcb) /*parameters*/
                + sizeof(real32[6]) /*path*/;

printf("sizeof madOpArgs= %d\n",madOpArgs);

    for (madobjloop=0; madobjloop <newHeader->nObjects ; madobjloop++){

        madOpArgsThis = oldMOP->nKeyframes * madOpArgs;

        oldptr = oldbase + oldMOP->times;

        memcpy (newptr,oldptr,madOpArgsThis);

        newMOP->name = (char*)(memsize)oldMOP->name;
        newMOP->nameCRC = oldMOP->nameCRC;
        newMOP->animationBits = oldMOP->animationBits;
        newMOP->nKeyframes = oldMOP->nKeyframes;

        newMOP->times = (real32*)offset;
        newMOP->parameters = (tcb*)( offset + sizeof(real32) * oldMOP->nKeyframes);
        for (j=0; j<6 ; j++){
        
            newMOP->path[j] = (real32*) (offset + 
            (sizeof(tcb) + sizeof(real32) +(sizeof(real32) * j)) 
            * oldMOP->nKeyframes );
        }

    
        oldMOP++;
        newMOP++;
        offset +=madOpArgsThis;
        newptr +=madOpArgsThis;
    }
 
    newHeader->stringBlock = (char*)offset;

    memcpy(newptr, oldbase + oldHeader->stringBlock, oldHeader->stringBlockLength);

    newptr += oldHeader->stringBlockLength;
    offset += oldHeader->stringBlockLength;



//    newheader->stringBlockLength = 
//    newheader->stringBlock = 


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

