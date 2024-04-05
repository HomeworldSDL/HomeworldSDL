// =============================================================================
//  BigFile.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
// =============================================================================

#ifndef ___BIGFILE_H
#define ___BIGFILE_H

#include "CRC32.h"
#include "standard_library.h"
#include "Types.h"


//  Version Notes:
//  0.90    1998/05/12  Darren Stone
//          Created.  Basic add operations.
//  0.91    1998/05/15  Darren Stone
//          View operation added.
//  0.92    1998/05/19  Darren Stone
//          Changed name storage to CRC method.
//  0.93    1998/08/06  Darren Stone
//          Wildcard args now supported.
//          Added header verification during -a
//  0.94    1998/08/07  Darren Stone
//          Better file-not-found handling for -a
//  0.95    1998/08/10  Darren Stone
//          Added @filelist.txt capability
//          Added -f (fast-create)
//          1998/08/11  Darren Stone
//          Removed filename verification for FileLoad (for speed)
//  1.10    1998/08/11  Darren Stone
//          Made filename comparisons (for CRC purposes) lowercase
//  1.11    1998/08/11  Darren Stone
//          Fixed some lowercase filename bugs
//          1998/08/12  Darren Stone
//          Optimized TOC file find
//  1.12    1998/08/12  Darren Stone
//          Added duplicate filename checking to -f
//          Fixed blank-line problem with filelists
//  1.13    1998/08/12  Darren Stone
//          Fixed bad file count during -f (with dupes)
//  1.14    1998/09/15  Darren Stone
//          Turned all filename slashes into backslashes and reduced multiple slashes down to singles
//  1.15    1998/09/15  Darren Stone
//          Do the above slash massaging for TOC storage also (just in case)
//  1.16    1998/09/15  Darren Stone
//          Slash massaging bug fix
//  1.17    1998/09/15  Darren Stone
//          Slash massaging bug fix
//  1.18    1998/10/06  Darren Stone
//          Added compression support
//  1.19    1998/10/12  Darren Stone
//          Full in-game support for compression now
//          Bigfile fast-create option will now fall back to non-compressed if compression buys us nothing
//  1.20    1998/10/13  Darren Stone
//          Fixed bug in "revert to uncompressed" feature of fast-create.
//  1.21    1998/10/19  Darren Stone
//          Added option for sorted TOC and binary search.
//  1.22    1998/10/20  Darren Stone
//          Encrypted filenames.
//  1.23    1998/11/23  Darren Stone
//          Fixed fast-create sorting bug.
// Gary changed interface of bigCRC function

// keep these strings the same length
#define BF_VERSION     "1.23"   // increment this when the file format changes

#define BIGGIE_VERSION "3.00"   // biggie: .BIG file extractor tool (see /tools)

// some things don't get compiled into the command line tool
#if defined(HW_BUILD_FOR_DEBUGGING) || defined(HW_BUILD_FOR_DISTRIBUTION)
    #define BF_HOMEWORLD
#endif

#define BF_FILE_HEADER "RBF"   // don't change this!

#define BF_MAX_FILENAME_LENGTH 128
#define BF_FLAG_TOC_SORTED       1

#define bigCRC64EQ(a, b)  (((a)->nameCRC1 == (b)->nameCRC1) && ((a)->nameCRC2 == (b)->nameCRC2))
#define bigCRC64GT(a, b)  (((a)->nameCRC1 > (b)->nameCRC1) || ((a)->nameCRC1 == (b)->nameCRC1 && (a)->nameCRC2 > (b)->nameCRC2))
#define bigCRC64LT(a, b)  (((a)->nameCRC1 < (b)->nameCRC1) || ((a)->nameCRC1 == (b)->nameCRC1 && (a)->nameCRC2 < (b)->nameCRC2))


typedef struct
{
    // together, the CRCs and nameLength make up a very unique identifier for
    // the filename (so we don't have to do string compares when searching or
    // or store long names in the TOC)
    crc32 nameCRC1, nameCRC2;  // CRC of 1st and 2nd halves of name
    unsigned short nameLength;

    //char name[BF_MAX_FILENAME_LENGTH];  for speed & space concerns, these now preceed the datafile portions of the bigfile

    udword storedLength;
    udword realLength;
    udword offset;
//    time_t timeStamp;
    udword timeStamp;
    char compressionType;  // 0/1
} bigTOCFileEntry;

typedef struct
{
    // together, the CRCs and nameLength make up a very unique identifier for
    // the filename (so we don't have to do string compares when searching or
    // or store long names in the TOC)
    crc32 nameCRC1, nameCRC2;  // CRC of 1st and 2nd halves of name
    unsigned short nameLength;

    //char name[BF_MAX_FILENAME_LENGTH];  for speed & space concerns, these now preceed the datafile portions of the bigfile

    udword storedLength;
    udword realLength;
    udword offset;
//    time_t timeStamp;
    udword timeStamp;
    udword unknown; // why would someone change the file format and not change the version number. It says right here in the file!
    char compressionType;  // 0/1
} bigTOCFileEntryRemaster;

typedef struct {
    int numFiles;
    int flags;
    bigTOCFileEntry *fileEntries;
} bigTOC;

#define UNINITIALISED_BIG_TOC  {0, 0, NULL}

typedef enum
{
    LOCAL_FILE_DOES_NOT_EXIST = 0,
    LOCAL_FILE_IS_OLDER,
    LOCAL_FILE_IS_NEWER,
}
bigLocalFileAgeComparison;

typedef struct
{
    char   *bigFileName;
    bool32    required;
    FILE   *filePtr;
    bigTOC  tableOfContents;
    bigLocalFileAgeComparison  *localFileRelativeAge;   // array with TOC file number as index
}
bigFileConfiguration;


enum {
    BF_ADD_RES_ADDED   = 1,
    BF_ADD_RES_UPDATED = 2 ,
    BF_ADD_RES_OLD     = 4,
};

int bigAdd(char *bigfilename, int numFiles, char *filenames[], int optCompression, int optNewer, int optMove, int optPathnames, int consoleOutput);
int bigFastCreate(char *bigfilename, int numFiles, char *filenames[], int optCompression, int optNewer, int optMove, int optPathnames, int consoleOutput);
int bigPatch(char *oldfilename, char *newfilename, char *patchfilename, int consoleOutput);
int bigAddFile(char *bigFilename, char *filename, char *storedFilename, int optCompression, int optNewer, int consoleOutput);
int bigDelete(char *bigfilename, int numFiles, char *filenames[], int consoleOutput);
int bigView(char *bigfilename, int consoleOutput);
int bigExtract(char *bigfilename, int numFiles, char *filenames[], int optFreshen, int optMove, int optPathnames, int optOverwrite, int consoleOutput);
bool32 bigTOCFileExists(bigTOC *toc, char *filename, udword *fileNum);
int bigTOCFileExistsByCRC(bigTOC *toc, bigTOCFileEntry *target, udword *fileNum);
void bigTOCSort(bigTOC *toc);
void bigFilenameEncrypt(char *filename);
void bigFilenameDecrypt(char *filename, int length);
crc32 bigTOCCRC(bigTOC *toc);

// not used in command line utility, only in the game
#ifdef BF_HOMEWORLD
    bool32 bigOpenAllBigFiles(void);
    void bigCloseAllBigFiles(void);

    void bigCRC(udword *bigCRCArray, udword arraySize);
    void bigFilesystemCompare(char *baseDirectory, char *directory);
    bool32 bigFindFile(char *filename, bigFileConfiguration **whereFound, udword *fileIndex);

    sdword bigFileLoadAlloc(bigTOC *toc, FILE *bigFP, char *filename, udword fileNum, void **address);
    sdword bigFileLoad(bigTOC *toc, FILE *bigFP, udword fileNum, void *address);
#endif

#endif
