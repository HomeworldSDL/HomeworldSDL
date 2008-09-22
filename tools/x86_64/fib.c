
#include "FEFlow.h"
#include "fib.h"
#include "file.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

int 
main (int argc, char *argv[])
{

  int i;
  int j;
  int k;
  int offset;

  char oldfile[80];
  char newfile[80];
  
  fibfileheader *header = NULL; 

  fescreen_disk *oldfescreen = NULL;
  fescreen *newfescreen = NULL;

  void * oldrootptr = NULL;
  void * newrootptr = NULL;

  void * oldptr = NULL;
  void * newptr = NULL;

  struct tagfelink_disk * oldfelink = NULL;
  struct tagfelink * newfelink = NULL;

  struct tagfeatom * newfeatom = NULL;
  struct tagfeatom_disk * oldfeatom = NULL;
  

  int oldlength;
  Uint64 newlength;


//  printf("args = %d\n",argc);

  if (argc < 2 ){
    printf("usage %s <inputfile>\n", argv[0]);
    return 1;
  }

  for (k=1; k <argc; k++){

    strcpy (oldfile, argv[k]);
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
  
    header = oldptr = oldrootptr;
  

    oldfescreen = oldrootptr + sizeof(fibfileheader); 
  
//    newfescreen = newptr = newrootptr = malloc(sizeof(fescreen) * header->nScreens); 
    newrootptr = malloc(2* oldlength); 
    newfescreen = newrootptr +sizeof(fibfileheader);

    newptr = (void *)((sizeof(fescreen) * header->nScreens)+ newrootptr+ sizeof(fibfileheader));
    memset(newrootptr, 0,2*oldlength);
    memcpy(newrootptr, oldrootptr, sizeof(fibfileheader));

    for (i = 0; i<header->nScreens; i++, oldfescreen++, newfescreen++){
      newfescreen->name = (char *)((Uint64)newptr - (Uint64)newrootptr );
      newptr = stpcpy(newptr,  (char *)((Uint64)oldfescreen->name + oldrootptr) );
      newptr = (void*)(((Uint64)newptr + 8) & ((Uint64) ~7)); 
      newfescreen->flags = oldfescreen->flags ;
      newfescreen->nLinks = oldfescreen->nLinks ;
      newfescreen->nAtoms = oldfescreen->nAtoms ;
      newfescreen->links = (struct tagfelink *)((Uint64)newptr - (Uint64)newrootptr);

      newfelink = (struct tagfelink*) newptr;
      newptr += sizeof(struct tagfelink) * oldfescreen->nLinks;

      oldfelink= (struct tagfelink_disk *)((Uint64)oldfescreen->links + oldrootptr);

      for(j=0; j<oldfescreen->nLinks;j++,newfelink++,oldfelink++){
        newfelink->name = (char *)((Uint64)newptr - (Uint64)newrootptr );
        newptr = stpcpy(newptr,  (char *)((Uint64)oldfelink->name + oldrootptr) );
        newptr = (void*)(((Uint64)newptr + 8) & ((Uint64) ~7)); 
        
        newfelink->flags = oldfelink->flags;

        newfelink->linkToName = (char *)((Uint64)newptr - (Uint64)newrootptr );
        newptr = stpcpy(newptr,  (char *)((Uint64)oldfelink->linkToName + oldrootptr) );
        newptr = (void*)(((Uint64)newptr + 8) & ((Uint64) ~7)); 
      }

      newfescreen->atoms = (struct tagfeatom *)((Uint64)newptr - (Uint64)newrootptr);
      newfeatom = ( struct tagfeatom*) newptr;
      oldfeatom = (struct tagfeatom_disk*) ((Uint64)oldfescreen->atoms + oldrootptr);
      newptr += sizeof(struct tagfeatom) * oldfescreen->nAtoms;

      for(j=0; j<oldfescreen->nAtoms;j++,newfeatom++,oldfeatom++){
        if (oldfeatom->name != 0){
          newfeatom->name = (char *)((Uint64)newptr - (Uint64)newrootptr );
          newptr = stpcpy(newptr,  (char *)((Uint64)oldfeatom->name + oldrootptr) );
          newptr = (void*)(((Uint64)newptr + 8) & ((Uint64) ~7)); 
        }
        newfeatom->flags = oldfeatom->flags;
        newfeatom->status = oldfeatom->status;
        newfeatom->type = oldfeatom->type;
        newfeatom->borderWidth = oldfeatom->borderWidth;
        newfeatom->tabstop = oldfeatom->tabstop;
        newfeatom->borderColor = oldfeatom->borderColor;
        newfeatom->contentColor = oldfeatom->contentColor;
        newfeatom->x = oldfeatom->x;
        newfeatom->loadedX = oldfeatom->loadedX;
        newfeatom->y = oldfeatom->y;
        newfeatom->loadedY = oldfeatom->loadedY;
        newfeatom->width = oldfeatom->width;
        newfeatom->loadedWidth = oldfeatom->loadedWidth;
        newfeatom->height = oldfeatom->height;
        newfeatom->loadedHeight = oldfeatom->loadedHeight;
        if (oldfeatom->pData != 0){
          newfeatom->pData = (char *)((Uint64)newptr - (Uint64)newrootptr );
          newptr = stpcpy(newptr,  (char *)((Uint64)oldfeatom->pData + oldrootptr) );
          newptr = (void*)(((Uint64)newptr + 8) & ((Uint64) ~7)); 
        }
        if (oldfeatom->attribs != 0){
          newfeatom->attribs = (char *)((Uint64)newptr - (Uint64)newrootptr );
          newptr = stpcpy(newptr,  (char *)((Uint64)oldfeatom->attribs + oldrootptr) );
          newptr = (void*)(((Uint64)newptr + 8) & ((Uint64) ~7)); 
        }
//        strncpy(newfeatom->hotKeyModifiers, oldfeatom->hotKeyModifiers, 1);
        newfeatom->hotKeyModifiers = oldfeatom->hotKeyModifiers;
        strncpy(newfeatom->hotKey, oldfeatom->hotKey, FE_NumberLanguages);
        newfeatom->drawstyle[0] = oldfeatom->drawstyle[0];
        newfeatom->drawstyle[1] = oldfeatom->drawstyle[1];
        if ((oldfeatom->region != 0) && (oldfeatom->region !=0xcdcdcdcd)){
          newfeatom->region = (char *)((Uint64)newptr - (Uint64)newrootptr );
          newptr = stpcpy(newptr,  (char *)((Uint64)oldfeatom->region + oldrootptr) );
          newptr = (void*)(((Uint64)newptr + 8) & ((Uint64) ~7)); 
        }




      }

    }

    newfescreen=newrootptr;
    oldfescreen=oldrootptr + sizeof(fibfileheader);

    oldfelink= (struct tagfelink_disk *)((Uint64)oldfescreen->links + oldrootptr);
    newfelink = (struct tagfelink*) newptr;

// assert(0);
    newlength = newptr - newrootptr;

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
  
