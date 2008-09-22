
#include "Mesh.h"
#include "geo.h"
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
  
  GeoFileHeader_disk *oldHeader = NULL;
  GeoFileHeader *newHeader = NULL;

  int lenDiff;
  memsize offset=0;
  memsize loopsize=0;

  void * oldptr = NULL;
  void * newptr = NULL;
  void * oldbase = NULL;
  void * newbase = NULL;
  
  struct polygonobject_disk * oldpolobj = NULL;
  struct polygonobject * newpolobj = NULL;

    struct tagmaterialentry_disk *oldmaterial = NULL;
    struct tagmaterialentry *newmaterial = NULL;
    struct tagmaterialentry *newmaterialbase = NULL;

  int oldLength;
  int newLength;
    int numPolygons;

  lenDiff = sizeof(GeoFileHeader) - sizeof(GeoFileHeader_disk);
//  offset=lenDiff;


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

    newLength = 3*oldLength;

    newbase = malloc(3*oldLength);
    newHeader = newbase;
    newptr =  (void*) newbase;

    memset (newbase, 0 ,3*oldLength);
  
    strcpy(newHeader->identifier,oldHeader->identifier);
    newHeader->version = oldHeader->version;
    if (oldHeader->pName !=0){
//      newHeader->pName = (char *)(((Uint64)oldHeader->pName) +offset);
//      strcpy(newHeader + offset,oldHeader + (Uint64)oldHeader->pName);
//      offset += strlen(oldHeader + (Uint64)oldHeader->pName);
    }
    newHeader->fileSize = oldHeader->fileSize;
    newHeader->localSize = oldHeader->localSize;
    newHeader->nPublicMaterials = oldHeader->nPublicMaterials;
    newHeader->nLocalMaterials = oldHeader->nLocalMaterials;
//    newHeader->oPublicMaterial = oldHeader->oPublicMaterial;
//    newHeader->oLocalMaterial = oldHeader->oLocalMaterial;
    newHeader->nPolygonObjects = oldHeader->nPolygonObjects;
    
    memcpy (newHeader->reserved, oldHeader->reserved, 24); 

// fixup polygonobject

    numPolygons = oldHeader->nPolygonObjects;

    oldpolobj = (struct polygonobject_disk *) (((void *) sizeof(GeoFileHeader_disk)) + (Uint64)oldptr);

    newpolobj = (struct polygonobject *) (newbase + sizeof(GeoFileHeader));
    newptr += sizeof(GeoFileHeader);
    offset += sizeof(GeoFileHeader);

    for (i=0; i<numPolygons; i++){

    printf("polyobj[%d]-> %d@%x %d@%x %d@%x\n",i, oldpolobj[i].nVertices,oldpolobj[i].pVertexList,
        oldpolobj[i].nFaceNormals,oldpolobj[i].pNormalList,
        oldpolobj[i].nPolygons,oldpolobj[i].pPolygonList);
    

        newpolobj[i].flags = oldpolobj[i].flags;
        newpolobj[i].iObject = oldpolobj[i].iObject;
        newpolobj[i].nameCRC = oldpolobj[i].nameCRC;
        newpolobj[i].nVertices = oldpolobj[i].nVertices;
        newpolobj[i].nFaceNormals = oldpolobj[i].nFaceNormals;
        newpolobj[i].nVertexNormals = oldpolobj[i].nVertexNormals;
        newpolobj[i].nPolygons = oldpolobj[i].nPolygons;
        newpolobj[i].localMatrix = oldpolobj[i].localMatrix;

        if (oldpolobj[i].pDaughter != NULL){
            newpolobj[i].pDaughter = ( oldpolobj[i].pDaughter - sizeof(GeoFileHeader_disk)) /
                 sizeof(struct polygonobject_disk) * sizeof(struct polygonobject) + sizeof(GeoFileHeader);
        }
        if (oldpolobj[i].pMother != NULL){
            newpolobj[i].pMother = ( oldpolobj[i].pMother - sizeof(GeoFileHeader_disk)) /
                 sizeof(struct polygonobject_disk) * sizeof(struct polygonobject) + sizeof(GeoFileHeader);
        }
        if (oldpolobj[i].pSister != NULL){
            newpolobj[i].pSister = ( oldpolobj[i].pSister - sizeof(GeoFileHeader_disk)) /
                 sizeof(struct polygonobject_disk) * sizeof(struct polygonobject) + sizeof(GeoFileHeader);
        }
    
        newptr += sizeof(struct polygonobject);
        offset += sizeof(struct polygonobject);

    }

    newHeader->oPublicMaterial = offset;
    newHeader->oLocalMaterial = offset;


    loopcount = oldHeader->nPublicMaterials + oldHeader->nLocalMaterials;
    
    oldmaterial = (struct tagmaterialentry_disk *) ((Uint64)oldbase + oldHeader->oPublicMaterial);


    newmaterialbase = (struct tagmaterialentry *) newptr;

    newmaterial = newmaterialbase;

    loopcount = oldHeader->nPublicMaterials + oldHeader->nLocalMaterials;

    for (loopvar=0; loopvar < loopcount; loopvar++) {
        if (oldmaterial->pName != 0){
            printf("oldmaterial->pName = %x \n",oldmaterial->pName);
        }
        newmaterial->ambient = oldmaterial->ambient;    
        newmaterial->diffuse = oldmaterial->diffuse;    
        newmaterial->specular = oldmaterial->specular;    
        newmaterial->kAlpha = oldmaterial->kAlpha;    
        if (oldmaterial->texture != 0){
            printf("oldmaterial->texture = %x \n",oldmaterial->texture);
        }
        newmaterial->flags = oldmaterial->flags;    
        newmaterial->nFullAmbient = oldmaterial->nFullAmbient;    
        newmaterial->bTexturesRegistered = oldmaterial->bTexturesRegistered;    
        if (oldmaterial->textureNameSave != 0xabababab){
            printf("oldmaterial->textureNameSave = %x \n",oldmaterial->textureNameSave);
        }

        newmaterial++;
        oldmaterial++;
        offset += sizeof(struct tagmaterialentry);


    }

    newptr = (void*)newmaterial;

//fixups

    newpolobj->pPolygonList = (polyentry*)(Uint64)offset;
    

    for (i=0; i < numPolygons; i++) {
    
        loopsize= oldpolobj[i].nPolygons * sizeof(struct tagpolygon);
        memcpy(newptr, oldbase + oldpolobj[i].pPolygonList, loopsize);
        newpolobj[i].pPolygonList = (polyentry *)offset;        
        newptr += loopsize;
        offset += loopsize;  

        loopsize= oldpolobj[i].nVertices * sizeof(struct tagvertex);
        memcpy(newptr, oldbase + oldpolobj[i].pVertexList, loopsize);
        newpolobj[i].pVertexList = (vertexentry*)offset;        
        newptr += loopsize;
        offset += loopsize;  
        
        loopsize= (oldpolobj[i].nFaceNormals + oldpolobj[i].nVertexNormals) * sizeof(struct tagnormal);
        memcpy(newptr, oldbase + oldpolobj[i].pNormalList, loopsize);
        newpolobj[i].pNormalList = (normalentry *)offset;        
        newptr += loopsize;
        offset += loopsize;  
    }
        
    for (i=0; i < numPolygons; i++) {
        if (oldpolobj[i].pName != NULL) {
         strcpy(newptr, oldbase + oldpolobj[i].pName);
         loopsize=strlen(oldbase + oldpolobj[i].pName) + 1;
         newpolobj[i].pName = offset;

         newptr += loopsize;
         offset += loopsize;  
        }
    }


    oldmaterial = (struct tagmaterialentry_disk *) ((Uint64)oldbase + oldHeader->oPublicMaterial);
    newmaterial = newmaterialbase;

    loopcount = oldHeader->nPublicMaterials + oldHeader->nLocalMaterials;

    for (loopvar=0; loopvar < loopcount; loopvar++) {
        if (oldmaterial->texture != 0){
            printf("oldmaterial->texture = %x - %s \n",oldmaterial->texture, oldbase + oldmaterial->texture);
            strcpy(newptr, oldbase + oldmaterial->texture);
            loopsize=strlen(oldbase + oldmaterial->texture) + 1;
            newmaterial->texture = offset; 
        }

        newmaterial++;
        oldmaterial++;
        newptr += loopsize;
        offset += loopsize;  

    }



    newHeader->fileSize= offset - sizeof (struct tagGeoFileHeader_disk);
//    newHeader->localSize = 


//    }



// Fixups
//

//    newHeader->pName 
//    newpolobj->pName
//    newpolobj->pVertexList
//    newpolobj->pNormalList
//    newpolobj->pPolygonList  **
//    newpolobj->pMother
//    newpolobj->pDaughter
//    newpolobj->pSister
//
//    material  pName
//    material texture
//    material textureNameSave





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

