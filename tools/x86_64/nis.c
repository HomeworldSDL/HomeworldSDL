
#include "nis.h"

#include "MeshAnim.h"
#include "file.h"
#include "texreg.h"

#include <assert.h>
#include <stdio.h>
#include <strings.h>

int main(int argc, char *argv[]) {

    int i;
    int j;

    int loopvar;
    int loopcount;

    char oldFile[80];
    char newFile[80];

    nisheader_disk *oldHeader = NULL;
    nisheader *newHeader = NULL;

    spaceobjpath_disk *oldObjPath = NULL;
    spaceobjpath *newObjPath = NULL;

    camerapath_disk *oldCamPath = NULL;
    camerapath *newCamPath = NULL;

    memsize offset = 0;
    memsize loopsize = 0;

    void *oldptr = NULL;
    void *newptr = NULL;
    void *oldbase = NULL;
    void *newbase = NULL;

    int oldLength;
    int newLength;

    if (argc < 2) {
        printf("usage %s <inputfile>\n", argv[0]);
        return 1;
    }

    for (i = 1; i < argc; i++) {
        strcpy(oldFile, argv[i]);
        sprintf(newFile, "%s.64", oldFile);

        if (fileExists(oldFile, 0) != 0) {
            printf("File does not exist.\n");
            return 1;
        }

        printf("Converting %s to %s\n", oldFile, newFile);

        oldbase = fileloadalloc(oldFile, &oldLength, 0);
        oldHeader = oldptr = oldbase;
        if (oldLength == 0) {
            printf("Error loading file.\n");
            return 1;
        }

        printf("%s size is %d\n", oldFile, oldLength);

        newLength = 2 * oldLength;

        newbase = malloc(2 * oldLength);
        newHeader = newbase;
        newptr = (void *)newbase;

        memset(newbase, 0, 2 * oldLength);

        offset = 0;

        strncpy(newHeader->identifier, oldHeader->identifier, 12);
        newHeader->version = oldHeader->version;
        newHeader->flags = oldHeader->flags;
        newHeader->stringBlockLength = oldHeader->stringBlockLength;
        newHeader->length = oldHeader->length;
        newHeader->loop = oldHeader->loop;
        newHeader->nObjectPaths = oldHeader->nObjectPaths;
        newHeader->nCameraPaths = oldHeader->nCameraPaths;
        newHeader->nLightPaths = oldHeader->nLightPaths;
        newHeader->nEvents = oldHeader->nEvents;

        offset += sizeof(nisheader);
        newptr += sizeof(nisheader);

        newHeader->objectPath = offset;

        oldObjPath = (spaceobjpath_disk *)((void *)oldHeader + sizeof(nisheader_disk));
        newObjPath = (spaceobjpath *)newptr;

        offset += sizeof(spaceobjpath) * newHeader->nObjectPaths;

        newHeader->cameraPath = offset;

        offset += sizeof(camerapath) * newHeader->nCameraPaths;
        offset += sizeof(lightpath) * newHeader->nLightPaths;
        offset += sizeof(nisevent) * newHeader->nEvents;

        newptr = (void *)((memsize)newbase + offset);

        printf("offest of objects: %x -- %d:%d:%d:%d\n", offset, newHeader->nObjectPaths, newHeader->nCameraPaths,
               newHeader->nLightPaths, newHeader->nEvents);

        loopcount = newHeader->nObjectPaths;
        for (loopvar = 0; loopvar < loopcount; loopvar++) {
            newObjPath[loopvar].instance = offset;
            newObjPath[loopvar].type = oldObjPath[loopvar].type;
            newObjPath[loopvar].parentIndex = oldObjPath[loopvar].parentIndex;
            newObjPath[loopvar].race = oldObjPath[loopvar].race;
            newObjPath[loopvar].nSamples = oldObjPath[loopvar].nSamples;
            memcpy(newptr, oldptr + oldObjPath[loopvar].instance, 4 * oldObjPath[loopvar].nSamples);
            offset += 4 * newObjPath[loopvar].nSamples;
            newptr += 4 * newObjPath[loopvar].nSamples;
            newObjPath[loopvar].timeOffset = oldObjPath[loopvar].timeOffset;

            newObjPath[loopvar].times = offset;
            memcpy(newptr, oldptr + oldObjPath[loopvar].times, 4 * oldObjPath[loopvar].nSamples);
            offset += 4 * newObjPath[loopvar].nSamples;
            newptr += 4 * newObjPath[loopvar].nSamples;

            newObjPath[loopvar].parameters = offset;
            memcpy(newptr, oldptr + oldObjPath[loopvar].parameters, sizeof(tcb) * oldObjPath[loopvar].nSamples);
            offset += sizeof(tcb) * newObjPath[loopvar].nSamples;
            newptr += sizeof(tcb) * newObjPath[loopvar].nSamples;

            for (j = 0; j < 6; j++) {
                newObjPath[loopvar].curve[j] = offset;
                memcpy(newptr, oldptr + oldObjPath[loopvar].curve[j], sizeof(real32) * oldObjPath[loopvar].nSamples);
                offset += sizeof(real32) * newObjPath[loopvar].nSamples;
                newptr += sizeof(real32) * newObjPath[loopvar].nSamples;
            }
        }

        //    newHeader->cameraPath = offset;

        oldCamPath = (camerapath_disk *)&oldObjPath[loopvar]; // seqential objects!
        newCamPath = (camerapath *)&newObjPath[loopvar];      // seqential objects!

        loopcount = newHeader->nCameraPaths;
        for (loopvar = 0; loopvar < loopcount; loopvar++) {
            newCamPath[loopvar].oLength = offset;
            memcpy(newptr, oldptr + oldCamPath[loopvar].oLength, 4 * oldCamPath[loopvar].nSamples);
            offset += 4 * newCamPath[loopvar].nSamples;
            newptr += 4 * newCamPath[loopvar].nSamples;

            newCamPath[loopvar].nSamples = oldCamPath[loopvar].nSamples;

            memcpy(newptr, oldptr + oldCamPath[loopvar].oLength, 4 * oldCamPath[loopvar].nSamples);
            offset += 4 * newCamPath[loopvar].nSamples;
            newptr += 4 * newCamPath[loopvar].nSamples;

            newCamPath[loopvar].times = offset;
            memcpy(newptr, oldptr + oldCamPath[loopvar].times, 4 * oldCamPath[loopvar].nSamples);
            offset += 4 * newCamPath[loopvar].nSamples;
            newptr += 4 * newCamPath[loopvar].nSamples;

            newCamPath[loopvar].parameters = offset;
            memcpy(newptr, oldptr + oldCamPath[loopvar].parameters, sizeof(tcb) * oldCamPath[loopvar].nSamples);
            offset += sizeof(tcb) * newCamPath[loopvar].nSamples;
            newptr += sizeof(tcb) * newCamPath[loopvar].nSamples;

            for (j = 0; j < 6; j++) {
                newCamPath[loopvar].curve[j] = offset;
                memcpy(newptr, oldptr + oldCamPath[loopvar].curve[j], sizeof(real32) * oldCamPath[loopvar].nSamples);
                offset += sizeof(real32) * newCamPath[loopvar].nSamples;
                newptr += sizeof(real32) * newCamPath[loopvar].nSamples;
            }
        }

        memcpy(newptr, oldptr + (udword)oldHeader->stringBlock, oldHeader->stringBlockLength);
        newHeader->stringBlock = offset;

        offset += oldHeader->stringBlockLength;

        if (fileSave(newFile, newbase, offset) == 0) {
            return 1;
        }
        printf("%s size is %d\n", newFile, offset);
    }

    free(oldbase);
    free(newbase);

    return 0;
}
