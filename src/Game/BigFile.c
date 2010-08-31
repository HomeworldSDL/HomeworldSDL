// =============================================================================
//  BigFile.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created May 1998 by Darren Stone.
// =============================================================================

#ifndef _WIN32
/* If it is not Windows, it must be POSIX... right?
   Unfortunately POSIX does not define a way to test whether a system is
   POSIX-compatible. */
#include <fnmatch.h>
#endif

#include "BigFile.h"

#include "BitIO.h"
#include "LZSS.h"
#include "standard_library.h"


#ifdef BF_HOMEWORLD
    #include "Memory.h"
    #include "Debug.h"
    #include "File.h"
#endif


//
// The following value sets the threshold used by the bigfile system to decide which
// files don't compress well enough to bother.  Files with compression ratios below
// (better than) or equal to this value will be compressed.
//
// BUG: at present, BigFileAdd compresses everything or nothing, and ignores this define
//

#define MINIMUM_COMPRESSION_RATIO 0.950f


#ifdef BF_HOMEWORLD

//  on a file-by-file basis, use the most recent file
//  out of either the bigfile(s) or the filesystem
//  NOTE: This slows the initial (blackscreen) load of the
//  game a bit, and we could probably eliminate it for
//  the retail version.
bool CompareBigfiles =
#ifdef HW_BUILD_FOR_DISTRIBUTION
    FALSE
#else
    TRUE
#endif
;

//  allows you to force loading from regular filesystem,
//  even if files are available in a bigfile
bool IgnoreBigfiles = FALSE;

//  allows you to create a text file of all data file loads
//  (useful for ordering or creating a bigfile)
bool LogFileLoads = FALSE;

#endif


bigFileConfiguration bigFilePrecedence[] = {
    {
        "HomeworldSDL.big",         // bigFileName
        FALSE,                      // required
        NULL,                       // filePtr
        UNINITIALISED_BIG_TOC,      // tableOfContents
        NULL,                       // localFileRelativeAge
    },
    {
#if defined(HW_GAME_RAIDER_RETREAT) && defined(_MACOSX)
        // rename allows this to live alongside 1.05 patch's Update.big in standard installation
        "OEM_Update.big",
#else
        "Update.big",
#endif
        TRUE,
        NULL,
        UNINITIALISED_BIG_TOC,
        NULL,
    },
    {
#ifdef HW_GAME_DEMO
        "HomeworldDL.big",
#else
        "Homeworld.big",
#endif
        TRUE,
        NULL,
        UNINITIALISED_BIG_TOC,
        NULL,
    },
};

#define NUMBER_CONFIGURED_BIG_FILES  (sizeof(bigFilePrecedence) / sizeof(bigFileConfiguration))

static udword numOpenedBigFiles = 0;

//
//  1. convert all slashes to either forward slashes or backslashes based on
//     the platform (use backslashes on Win32, else use forward).
//  2. reduce multiple consecutive slashes to single slashes
//
//  (operate on the string in place)
//
static void filenameSlashMassage(char *filename, bool bLocalPath)
{
    udword i, j;
    char slash_ch, not_slash_ch;
    udword len = strlen(filename);

#ifdef _WIN32
    slash_ch     = '\\';
    not_slash_ch = '/';
#else
    if (bLocalPath)
    {
        slash_ch     = '/';
        not_slash_ch = '\\';
    }
    else
    {
        slash_ch     = '\\';
        not_slash_ch = '/';
    }
#endif  /* _WIN32 */

    for (i = 0; i < len; ++i)
    {
        if (filename[i] == not_slash_ch)
        {
            filename[i] = slash_ch;
        }
    }

    i = 0;
    while (i < strlen(filename) - 1)
    {
        if (filename[i] == slash_ch && filename[i+1] == slash_ch)
        {
            for (j = i; j < strlen(filename) - 1; ++j)
            {
                filename[j] = filename[j+1];
            }
            
            filename[j] = 0;
        }
        ++i;
    }
}

/*-----------------------------------------------------------------------------
    Name        : PrettyFilesize
    Description : Format a file size (in bytes) attractively)
    Inputs      : bytes - file size
    Outputs     : buf - an attractive numeric string, like "1,326,719"
    Return      : buf
----------------------------------------------------------------------------*/
static char *PrettyFilesize(long bytes, char *buf)
{
    char temp[32], tempRev[32], *p, *r;
    int count;

    sprintf(temp, "%0ld", bytes);
    // copy backwards, by 3's into tempRev
    p = temp + strlen(temp) - 1;
    r = tempRev;
    count = 0;
    while (1)
    {
        *r = *p;
        ++count;
        if (p == temp)
            break;
        if (!(count % 3))
            *(++r) = ',';
        --p;
        ++r;
    }
    *(r+1) = 0;
    // copy backwards into buf
    r = tempRev + strlen(tempRev) -1;
    p = buf;
    while (1)
    {
        *p = *r;
        if (r == tempRev)
            break;
        --r;
        ++p;
    }
    *(p+1) = 0;
    return buf;
}

/*-----------------------------------------------------------------------------
    Name        : PrettyDateTime
    Description : Format a time stamp attractively
    Inputs      : dt - time stamp
    Outputs     : pretty - an attractive numeric string, like "1998/05/11 05:32:12"
    Return      : pretty;
----------------------------------------------------------------------------*/
static char *PrettyDateTime(time_t dt, char *pretty)
{
    struct tm *stamp;

    stamp = localtime(&dt);
    strftime(pretty, 31, "%Y/%m/%d %H:%M:%S", stamp);
    return pretty;
}

/*-----------------------------------------------------------------------------
    Name        : bigFileExists
    Description : Determine if a file exists
    Inputs      : filename - file you're looking for
    Outputs     : nothing
    Return      : 1 if the file exists, 0 otherwise
----------------------------------------------------------------------------*/
static int bigFileExists(char *filename)
{
    struct stat fileInfo;

    return (stat(filename, &fileInfo) != -1);
}

/*-----------------------------------------------------------------------------
    Name        : bigFilenameRetrieve
    Description : Pull filename from file data portion of bigfile.
                  This is a fairly slow operation because of the seeking
                  and reading required -- use it only when necessary!
    Inputs      : toc - table of contents of bigfile
                  fileNum - ordinal location of file
                  fp - filepointer into bigfile
    Outputs     : filename is set
                  filepointer in fp is restored to original position
    Return      : 1 on success, 0 on failure
----------------------------------------------------------------------------*/
static int bigFilenameRetrieve(bigTOC *toc, int fileNum, FILE *fp, char *filename)
{
    long fpos = ftell(fp);

    fseek(fp, (toc->fileEntries + fileNum)->offset, SEEK_SET);
    fread(filename, 1, (toc->fileEntries + fileNum)->nameLength+1, fp);
    fseek(fp, fpos, SEEK_SET);
    bigFilenameDecrypt(filename, (toc->fileEntries + fileNum)->nameLength);
    return 1;
}

/*-----------------------------------------------------------------------------
    Name        : bigCopy File
    Description : Copy a file.  Like windows CopyFile().
    Inputs      : existingFilename - for source file
                  newFilename - for (new) dest file
                  failIfExists - 1/0
    Outputs     : file will be copied
    Return      : 1 on succes, 0 on failure
----------------------------------------------------------------------------*/
static int bigCopyFile(char *existingFilename, char *newFilename, int failIfExists)
{
    FILE *oldfp, *newfp;
    char ch;

    oldfp = fopen(existingFilename, "rb");
    if (!oldfp)
    {
        return 0;
    }

// On platforms using mkstemp(), the temp file has already been created for us
// so this check will always fail
#ifdef _WIN32
    if (failIfExists && bigFileExists(newFilename))
    {
        return 0;
    }
#endif

    newfp = fopen(newFilename, "wb");
    if (!newfp)
    {
        return 0;
    }

    while (1)
    {
        // horribly inefficient!  change this!
        ch = (char)getc(oldfp);
        if (feof(oldfp))
            break;
        putc(ch, newfp);
    }
    
    fclose(oldfp);
    fclose(newfp);
    return 1;
}

/*-----------------------------------------------------------------------------
    Name        : bigFilenameExtract
    Description : Extract the name-only portion from a full pathname
    Inputs      : filenameonly - where you want a new string
                  fullpathname - full pathname to operate on
    Outputs     : filenameonly will be filled in
    Return      : nothing
----------------------------------------------------------------------------*/
static void bigFilenameExtract(char *filenameonly, char *fullpathname)
{
    char *p = fullpathname + strlen(fullpathname) - 1;

    // work backwards through the fullpathname, until we get to the first
    // backslash or the start of the string
    while (p != fullpathname)
    {
#ifdef _WIN32
        if (*p == '\\')
#else
        if (*p == '\\' || *p == '/')
#endif
        {
            ++p;
            break;
        }
        --p;
    }

    strcpy(filenameonly, p);
}

static void bigTOCFileEntryWrite(bigTOCFileEntry *fileEntry, FILE *bigFilePtr)
{
// fiddle for writing
#if FIX_ENDIAN
    fileEntry->nameCRC1     = FIX_ENDIAN_INT_32( fileEntry->nameCRC1     );
    fileEntry->nameCRC2     = FIX_ENDIAN_INT_32( fileEntry->nameCRC2     );
    fileEntry->nameLength   = FIX_ENDIAN_INT_16( fileEntry->nameLength   );
    fileEntry->storedLength = FIX_ENDIAN_INT_32( fileEntry->storedLength );
    fileEntry->realLength   = FIX_ENDIAN_INT_32( fileEntry->realLength   );
    fileEntry->offset       = FIX_ENDIAN_INT_32( fileEntry->offset       );
    fileEntry->timeStamp    = FIX_ENDIAN_INT_32( fileEntry->timeStamp    );
#endif

    fwrite((void *)fileEntry, sizeof(bigTOCFileEntry), 1, bigFilePtr);

// put it back the way it was
#if FIX_ENDIAN
    fileEntry->nameCRC1     = FIX_ENDIAN_INT_32( fileEntry->nameCRC1     );
    fileEntry->nameCRC2     = FIX_ENDIAN_INT_32( fileEntry->nameCRC2     );
    fileEntry->nameLength   = FIX_ENDIAN_INT_16( fileEntry->nameLength   );
    fileEntry->storedLength = FIX_ENDIAN_INT_32( fileEntry->storedLength );
    fileEntry->realLength   = FIX_ENDIAN_INT_32( fileEntry->realLength   );
    fileEntry->offset       = FIX_ENDIAN_INT_32( fileEntry->offset       );
    fileEntry->timeStamp    = FIX_ENDIAN_INT_32( fileEntry->timeStamp    );
#endif
}

/*-----------------------------------------------------------------------------
    Name        : bigTOCWrite
    Description : Write table of contents & file entries to bigfile .
                  File pointer must be positioned right after header
    Inputs      : fp - file pointer of bigfile
                  toc - table of contents struct
    Outputs     : toc & file entries are written,
                  leaving file pointer right after it
    Return      : 1 on succes, 0 on failure
----------------------------------------------------------------------------*/
static int bigTOCWrite(FILE *fp, bigTOC *toc)
{
    int i;
#if FIX_ENDIAN
	int numFiles = FIX_ENDIAN_INT_32( toc->numFiles );
	int flags    = FIX_ENDIAN_INT_32( toc->flags );
#else
	int numFiles = toc->numFiles;
	int flags    = toc->flags;
#endif

    fwrite((void *)&numFiles, sizeof(toc->numFiles), 1, fp);
    fwrite((void *)&flags,    sizeof(toc->flags),    1, fp);

    for (i = 0; i < toc->numFiles; ++i)
    {
        bigTOCFileEntry *fileEntry = &toc->fileEntries[i];

        bigTOCFileEntryWrite(fileEntry, fp);
    }

    return 1;
}

/*-----------------------------------------------------------------------------
    Name        : bigTOCRead
    Description : Read table of contents & file entries from bigfile .
                  File pointer must be positioned right after header
    Inputs      : fp - file pointer of bigfile
                  toc - table of contents struct
    Outputs     : toc & file entries are read,
                  leaving file pointer right after it
                  *** memory is left allocated for toc->fileEntries
                  -- remember to free this when you're done
    Return      : 1 on success, 0 on failure
----------------------------------------------------------------------------*/
static int bigTOCRead(FILE *fp, bigTOC *toc)
{
    fread((void *)&(toc->numFiles), sizeof(toc->numFiles), 1, fp);
    fread((void *)&(toc->flags),    sizeof(toc->flags),    1, fp);

#if FIX_ENDIAN
  	toc->numFiles = FIX_ENDIAN_INT_32( toc->numFiles );
	toc->flags    = FIX_ENDIAN_INT_32( toc->flags );
#endif

    if (toc->numFiles)
    {
        toc->fileEntries = malloc(sizeof(bigTOCFileEntry) * toc->numFiles);
        fread((void *)(toc->fileEntries), sizeof(bigTOCFileEntry), toc->numFiles, fp);

#if FIX_ENDIAN
        int i;
		for( i=0; i < toc->numFiles; ++i )
		{
			bigTOCFileEntry *e = &toc->fileEntries[i];
			e->nameCRC1     = FIX_ENDIAN_INT_32( e->nameCRC1 );
			e->nameCRC2     = FIX_ENDIAN_INT_32( e->nameCRC2 );
			e->nameLength   = FIX_ENDIAN_INT_16( e->nameLength );
			e->storedLength = FIX_ENDIAN_INT_32( e->storedLength );
			e->realLength   = FIX_ENDIAN_INT_32( e->realLength );
			e->offset       = FIX_ENDIAN_INT_32( e->offset );
			e->timeStamp    = FIX_ENDIAN_INT_32( e->timeStamp );
		}
#endif
    }
    
    return 1;
}

//
//  bigTOCCRC
//
//  returns the 32-bit CRC for the TOC
//
crc32 bigTOCCRC(bigTOC *toc)
{
    return crc32Compute((ubyte *)(toc->fileEntries), sizeof(bigTOCFileEntry)*(toc->numFiles));
}

/*-----------------------------------------------------------------------------
    Name        : bigHeaderWrite
    Description : Write header/version info to bigfile .
                  File pointer must be positioned at start of file
    Inputs      : fp - file pointer of bigfile
    Outputs     : header/version info is written,
                  leaving file pointer right after it
    Return      : 1 on succes, 0 on failure
----------------------------------------------------------------------------*/
static int bigHeaderWrite(FILE *fp)
{
    char header[16];

    sprintf(header, "%s%s", BF_FILE_HEADER, BF_VERSION);
    fwrite((void *)header, strlen(header), 1, fp);
    return 1;
}

/*-----------------------------------------------------------------------------
    Name        : bigHeaderVerify
    Description : Read header/version info from bigfile .
                  File pointer must be positioned at start of file
    Inputs      : fp - file pointer of bigfile
    Outputs     : header/version info is read,
                  leaving file pointer right after it
    Return      : 1 if the type & version are OK, 0 on failure
----------------------------------------------------------------------------*/
static int bigHeaderVerify(FILE *fp)
{
    char headerNeed[16], headerHave[16];
    int headerLength;

    sprintf(headerNeed, "%s%s", BF_FILE_HEADER, BF_VERSION);
    headerLength = strlen(headerNeed);

    fgets(headerHave, headerLength+1, fp);
    headerHave[headerLength] = 0;

    return !strcmp(headerHave, headerNeed);
}

//
//  sorts given bigfile
//  returns 1 on success
//  returns 0 on failure
//
static int bigSort(char *bigfilename)
{
    FILE *bigFP;
    bigTOC toc;

    bigFP = fopen(bigfilename, "r+b");
    if (!bigFP)
    {
        return 0;
    }
    
    if (!bigHeaderVerify(bigFP)) // ensure correct file type & version
    {
        fclose(bigFP);
        return 0;
    }
    
    bigTOCRead(bigFP, &toc);
    bigTOCSort(&toc);
    fseek(bigFP, strlen(BF_FILE_HEADER BF_VERSION), SEEK_SET);
    bigTOCWrite(bigFP, &toc);
    fclose(bigFP);
    return 1;
}

/*-----------------------------------------------------------------------------
    Name        : bigTOCFileExists
    Description : Determine if a file exists in a big file (from the CRC in the TOC)
    Inputs      : toc - pointer to table of contents
                  filename - what we're looking for
    Outputs     : fileNum - 0...numFiles, if it's found
    Return      : 1 if found, 0 otherwise
----------------------------------------------------------------------------*/
bool bigTOCFileExists(bigTOC *toc, char *filename, udword *fileNum)
{
    char filenamei[PATH_MAX] = "";
    bigTOCFileEntry target;
    int numFiles = toc->numFiles;
    unsigned int i;
    uword halfFilenameLength = 0;

    if (!numFiles)
    {
        return FALSE;
    }
    
    if ((filename == NULL) || (filename[0] == 0))
    {
        return FALSE;
    }

    // pretend the filename is in lowercase
    for (i = 0; (filenamei[i] = tolower(filename[i])); i++) { }

    // convert all slashes to backslashes and whittle multiple slashes down to a single one
    filenameSlashMassage(filenamei, FALSE);

    // There's a bug here - can you see it? See later if not. Unfortunately,
    // since the shipped .big files have this problem we can't fix it without
    // breaking everything, so we do a lookup the broken way (more likely to
    // find a match anyway) and then try the fixed version for newer .big files
    target.nameLength = strlen(filenamei);
    target.nameCRC1   = crc32Compute((ubyte *)filenamei,  target.nameLength/2);
    target.nameCRC2   = crc32Compute((ubyte *)filenamei + target.nameLength/2, target.nameLength/2);

    if (bigTOCFileExistsByCRC(toc, &target, fileNum))
    {
        return TRUE;
    }
    
    // The bug is that the CRC2 calculation is told to read namelength/2 bytes
    // starting from the middle of the filename. That sounds fine but as soon as
    // you start playing with integer arithmetic and filenames with an odd
    // number of characters, it all goes pear shaped. The last character is
    // ignored in these cases and so can incorrectly trigger a "duplicate entry"
    // match for similarly named files. Having determined the middle of the
    // filename you should then read (full_length - half_length) bytes to ensure
    // you don't miss a character.
    halfFilenameLength = target.nameLength / 2;

    target.nameCRC1 = crc32Compute((ubyte *)filenamei, halfFilenameLength);
    target.nameCRC2 = crc32Compute((ubyte *)filenamei + halfFilenameLength,
                                   (target.nameLength - halfFilenameLength));

    return bigTOCFileExistsByCRC(toc, &target, fileNum);
}

//
//  This is similar to bigTOCFileExists, but it assumes you have a fileEntry
//  (only the CRC of it is used) instead of a filename.
//
int bigTOCFileExistsByCRC(bigTOC *toc, bigTOCFileEntry *target, udword *fileNum)
{
    static int FileNum = 0;
    int startFileNum;
    int numFiles = toc->numFiles;
    int low, high;  // for binary search

    if (toc->flags & BF_FLAG_TOC_SORTED)
    {
        // sorted toc -- binary search
        low = 0;
        high = numFiles-1;
        while (low <= high)
        {
            *fileNum = (low+high)/2;  // middle
            if (bigCRC64EQ(target, toc->fileEntries + *fileNum))
                return 1;
            else if (bigCRC64GT(target, toc->fileEntries + *fileNum))
                low = *fileNum +1;
            else // if (b < a[*fileNum])
                high = *fileNum -1;  }
        return 0;
    }
    else
    {
        // unsorted toc -- linear search, but optimized to
        // start searching from wherever we left off last time
        // to potentially find sequentially ordered files faster
        startFileNum = FileNum;
        do
        {
            if (toc->fileEntries[FileNum].nameLength == target->nameLength &&
                toc->fileEntries[FileNum].nameCRC1 == target->nameCRC1 &&
                toc->fileEntries[FileNum].nameCRC2 == target->nameCRC2)
            {
                *fileNum = FileNum;
                return 1;
            }
            ++FileNum;
            if (FileNum >= numFiles)
                FileNum = 0;
        } while (FileNum != startFileNum);
    }

    return 0;
}

#if 0
//  64-bit CRC comparison
//  (pass pointers to bigTOCFileEntries)
//  return 1 if a's CRC is greater than b's
int bigCRC64GT(bigTOCFileEntry *a, bigTOCFileEntry *b)
{
    // this is a good candidate for a macro
    return ((a->nameCRC1 > b->nameCRC2) ||
            (a->nameCRC1 == b->nameCRC2 && a->nameCRC2 > b->nameCRC2));
}
#endif

void bigFilenameEncrypt(char *filename)
{
    ubyte maskch = 213;
    ubyte nextmask;
    while (*filename)
    {
        nextmask = *filename;
        *(filename++) ^= maskch;
        maskch = nextmask;
    }
}

//
//  since the encrypted string could contain 0's, we need to pass the length
//  (not including the terminating 0, which is not encrypted)
//
void bigFilenameDecrypt(char *filename, int len)
{
    ubyte maskch = 213;
    while (len--)
    {
        *filename ^= maskch;
        maskch = *(filename++);
    }
}

/*-----------------------------------------------------------------------------
    Name        : bigTOCSort
    Description : Sort the TOC in ascending CRC order.
    Inputs      : toc - pointer to table of contents
    Outputs     : toc - newly sorted
    Return      :
    Note        : sorts in place (there are a few types of sorts implemented here...)
----------------------------------------------------------------------------*/
void bigTOCSort(bigTOC *toc)
{
    // a very ignorant bubble sort!
#if 0
    int noExchanges, first, pass;
    int numFiles = toc->numFiles;
    bigTOCFileEntry saveEntry;

    if (!numFiles || (toc->flags & BF_FLAG_TOC_SORTED))
        return;  // nothing to do

    if (numFiles < 2)
    {
        toc->flags |= BF_FLAG_TOC_SORTED;  // toc is already "sorted"
        return;
    }

    pass = 1;
    do
    {
        noExchanges = 1;
        for (first = 0; first < numFiles - pass; ++first)
        {
            if (bigCRC64GT(&toc->fileEntries[first], &toc->fileEntries[first+1]))
            {
                saveEntry = toc->fileEntries[first];
                toc->fileEntries[first] = toc->fileEntries[first+1];
                toc->fileEntries[first+1] = saveEntry;
                //memcpy(&saveEntry, &toc->fileEntries[first], sizeof(bigTOCFileEntry));
                //memcpy(&toc->fileEntries[first], &toc->fileEntries[first+1], sizeof(bigTOCFileEntry));
                //memcpy(&toc->fileEntries[first+1], &saveEntry, sizeof(bigTOCFileEntry));
                noExchanges = 0;
            }
        }
        ++pass;
    } while (!noExchanges);
    toc->flags |= BF_FLAG_TOC_SORTED;  // toc is now sorted
#endif

    //
    // shell sort
#if 1
    int i, j, n, h;
    int numFiles = toc->numFiles;
    int ub, lb;
    bigTOCFileEntry saveEntry;

    if (!numFiles || (toc->flags & BF_FLAG_TOC_SORTED))
        return;  // nothing to do

    if (numFiles < 2)
    {
        toc->flags |= BF_FLAG_TOC_SORTED;  // toc is already "sorted"
        return;
    }

    // shell sort
    ub = numFiles-1;
    lb = 0;

    /* compute largest increment */
    n = ub - lb + 1;
    h = 1;
    if (n < 14)
        h = 1;
    //else if (sizeof(tblIndex) == 2 && n > 29524)  i think this is unnecessary
    //  h = 3280;
    else
    {
        while (h < n)
            h = 3*h + 1;
            h /= 3;
            h /= 3;
    }
    while (h > 0)
    {        /* sort-by-insertion in increments of h */
        for (i = lb + h; i <= ub; i++)
        {
            memcpy(&saveEntry, &toc->fileEntries[i], sizeof(bigTOCFileEntry));
            for (j = i-h; j >= lb && bigCRC64GT(&toc->fileEntries[j], &saveEntry); j -= h)
                memcpy(&toc->fileEntries[j+h], &toc->fileEntries[j], sizeof(bigTOCFileEntry));
            memcpy(&toc->fileEntries[j+h], &saveEntry, sizeof(bigTOCFileEntry));
        }
        /* compute next increment */
        h /= 3;
    }
    toc->flags |= BF_FLAG_TOC_SORTED;  // toc is now sorted
#endif

// insertion sort
#if 0
    int numFiles = toc->numFiles;
    int i, j;
    bigTOCFileEntry saveEntry;

    if (!numFiles || (toc->flags & BF_FLAG_TOC_SORTED))
        return;  // nothing to do

    for (i = 1; i < numFiles; ++i)
    {
        memcpy(&saveEntry, &toc->fileEntries[i], sizeof(bigTOCFileEntry));
        for (j = i-1; j >= 0 && bigCRC64GT(&toc->fileEntries[j], &saveEntry); --j)
            memcpy(&toc->fileEntries[j+1], &toc->fileEntries[j], sizeof(bigTOCFileEntry));
        memcpy(&toc->fileEntries[j+1], &saveEntry, sizeof(bigTOCFileEntry));
    }
    toc->flags |= BF_FLAG_TOC_SORTED;  // toc is now sorted
#endif
}

/*-----------------------------------------------------------------------------
    Name        : bigCreateNew
    Description : Create a new (empty) bigfile
    Inputs      : filename - bigfile to create
    Outputs     : new file is created, with nothing in it except headers, version info, etc.
    Return      : 1 on succes, 0 on failure
----------------------------------------------------------------------------*/
static int bigCreateNew(char *filename)
{
    FILE *fp;
    bigTOC toc;

    fp = fopen(filename, "wb");
    if (!fp)
        return 0;

    bigHeaderWrite(fp);

    toc.numFiles = 0;
    toc.flags = 0;  // unsorted TOC by default
    bigTOCWrite(fp, &toc);

    fclose(fp);

    return 1;
}

//
//  Create a "patch" bigfile from all the files in the newbigfile that have
//  have been changed or added since the oldbigfile.  A byte-for-byte comparison
//  is done for each file.  Date/time-stamps are ignored.
//
int bigPatch(char *oldfilename, char *newfilename, char *patchfilename, int consoleOutput)
{
    FILE *oldFP, *newFP, *patchFP;
    bigTOC oldTOC, newTOC, patchTOC;
    bigTOCFileEntry *newEntry, *oldEntry, *patchEntry;
    udword fileNum;
    udword filesNewer = 0, filesChanged = 0;
    udword curPatchOffset;
    sdword i, j;
    sdword toBePatched;
    udword bytesCompared = 0;

    // open old (read)
    if (bigFileExists(oldfilename))
    {
        oldFP = fopen(oldfilename, "rb");
        if (!oldFP)
        {
            if (consoleOutput)
                printf("ERROR: Can't open %s\n", oldfilename);
            return 0;
        }
        // ensure correct file type & version
        if (!bigHeaderVerify(oldFP))
        {
            if (consoleOutput)
                printf("ERROR: Incompatible file %s\n", oldfilename);
            fclose(oldFP);
            return 0;
        }
    }
    else
    {
        if (consoleOutput)
            printf("ERROR: Can't find %s\n", oldfilename);
        return 0;
    }
    bigTOCRead(oldFP, &oldTOC);

    // open new (read)
    if (bigFileExists(oldfilename))
    {
        newFP = fopen(newfilename, "rb");
        if (!newFP)
        {
            if (consoleOutput)
                printf("ERROR: Can't open %s\n", newfilename);
            return 0;
        }
        // ensure correct file type & version
        if (!bigHeaderVerify(newFP))
        {
            if (consoleOutput)
                printf("ERROR: Incompatible file %s\n", newfilename);
            fclose(newFP);
            return 0;
        }
    }
    else
    {
        if (consoleOutput)
            printf("ERROR: Can't find %s\n", newfilename);
        return 0;
    }
    bigTOCRead(newFP, &newTOC);

    // open patch (write)
    if (bigFileExists(patchfilename))
    {
        unlink(patchfilename);
        if (bigFileExists(patchfilename))
        {
            if (consoleOutput)
                printf("ERROR: Can't overwrite %s\n", patchfilename);
            return 0;
        }
        if (consoleOutput)
            printf("Overwriting");
    }
    else if (consoleOutput)
        printf("Creating");
    if (consoleOutput)
        printf(" %s (update from %s to %s)\n", patchfilename, oldfilename, newfilename);

    patchFP = fopen(patchfilename, "wb");
    if (!patchFP)
    {
        if (consoleOutput)
            printf("ERROR: Can't create %s\n", patchfilename);
        return 0;
    }
    bigHeaderWrite(patchFP);
    patchTOC.numFiles = 0;
    patchTOC.flags = 0;
    patchTOC.fileEntries = malloc(sizeof(bigTOCFileEntry) * newTOC.numFiles);
    curPatchOffset = strlen(BF_FILE_HEADER BF_VERSION)
                        + sizeof(patchTOC.numFiles)
                        + sizeof(patchTOC.flags);

    if (consoleOutput)
        printf("Comparing files\n");

    // compare each file in the new bigfile
    for (i = 0; i < newTOC.numFiles; ++i)
    {
        toBePatched = 0;
        newEntry = &newTOC.fileEntries[i];
        if (bigTOCFileExistsByCRC(&oldTOC, newEntry, &fileNum))
        {
            // add to patch if it is different
            oldEntry = &oldTOC.fileEntries[fileNum];
            // quick size compare
            if (newEntry->storedLength != oldEntry->storedLength ||
                newEntry->realLength != oldEntry->realLength ||
                newEntry->compressionType != oldEntry->compressionType ||
                newEntry->nameLength != oldEntry->nameLength)
            {
                toBePatched = 1;
                ++filesChanged;
            }
            else
            {
                // byte-by-byte compare
                fseek(oldFP, oldEntry->offset + oldEntry->nameLength+1, SEEK_SET);
                fseek(newFP, newEntry->offset + newEntry->nameLength+1, SEEK_SET);
                bytesCompared = 0;
                while (bytesCompared < newEntry->storedLength)
                {
                    if (fgetc(oldFP) != fgetc(newFP))
                    {
                        toBePatched = 1;
                        ++filesChanged;
                        break;
                    }
                    ++bytesCompared;
                }
            }
        }
        else
        {
            // it's new, so add it
            toBePatched = 1;
            ++filesNewer;
        }

        if (toBePatched)
        {
            patchEntry = &patchTOC.fileEntries[patchTOC.numFiles];
            patchEntry->nameCRC1 = newEntry->nameCRC1;
            patchEntry->nameCRC2 = newEntry->nameCRC2;
            patchEntry->nameLength = newEntry->nameLength;
            patchEntry->storedLength = newEntry->storedLength;
            patchEntry->realLength = newEntry->realLength;
            patchEntry->timeStamp = newEntry->timeStamp;
            patchEntry->compressionType = newEntry->compressionType;
            patchEntry->offset = curPatchOffset;
            curPatchOffset += patchEntry->storedLength + patchEntry->nameLength+1;
            ++patchTOC.numFiles;
        }
    }

    // update offsets, now that we know the size of the TOC
    for (i = 0; i < patchTOC.numFiles; ++i)
        patchTOC.fileEntries[i].offset += sizeof(bigTOCFileEntry) * patchTOC.numFiles;

    if (consoleOutput)
    {
        printf("%d new file%s\n", filesNewer, filesNewer == 1 ? "" : "s");
        printf("%d updated file%s\n", filesChanged, filesChanged == 1 ? "" : "s");
    }

    if (consoleOutput)
        printf("Writing data\n");

    fseek(patchFP, strlen(BF_FILE_HEADER BF_VERSION), SEEK_SET);

    // TOC
    bigTOCWrite(patchFP, &patchTOC);

    // file data
    for (i = 0; i < patchTOC.numFiles; ++i)
    {
        bigTOCFileExistsByCRC(&newTOC, &patchTOC.fileEntries[i], &fileNum);
        fseek(newFP, newTOC.fileEntries[fileNum].offset, SEEK_SET);
        j = patchTOC.fileEntries[i].storedLength +
            patchTOC.fileEntries[i].nameLength + 1;
        while (j--)
            fputc(fgetc(newFP), patchFP);
    }

    fclose(oldFP);
    fclose(newFP);
    fclose(patchFP);

    if (consoleOutput)
        printf("Sorting table of contents\n");
    if (!bigSort(patchfilename))
    {
        if (consoleOutput)
            printf("WARNING: Can't sort bigfile %s\n", patchfilename);
        return 0;
    }

    return 1;
}

/*-----------------------------------------------------------------------------
    Name        : bigAdd
    Description : Add files to a bigfile
    Inputs      : bigfilename - filename of bigfile (to add or append to)
                  numFiles - # of filenames in following array
                  filenames - names of files to add
                  optCompression - 1|0 compress
                  optNewer - 1|0 add files only if newer
                  optMove - 1|0 move (not just copy) files
                  optPathnames - 1|0 store full pathnames
                  consoleOutput - 1|0 = output to stdout (yes/no)
    Outputs     : New (or updated) bigfile.
                  Possibly deleted (moved) files.
                  Output to stdout (if enabled).
    Return      : 1 on success, 0 on failure
----------------------------------------------------------------------------*/
int bigAdd(char *bigfilename, int numFiles, char *filenames[], int optCompression, int optNewer, int optMove, int optPathnames, int consoleOutput)
{
#ifdef _WIN32
    char tempfilename[L_tmpnam+1];
#else
    /* To be safe... */
    char tempfilename[PATH_MAX];
#endif
    char tempshortfilename[BF_MAX_FILENAME_LENGTH+1];
    int filesAdded = 0, filesUpdated = 0, filesUnchanged = 0;
    int f;
    int res;
    int *moveFiles;
    FILE *fp = NULL, *filelistFP = NULL;
    int filelist;
    char *filelistName = NULL;
    char filelistLine[BF_MAX_FILENAME_LENGTH+1];
    char filename[BF_MAX_FILENAME_LENGTH+1];

    // temp file in case we need to abort
#ifdef _WIN32
    if (!tmpnam(tempfilename))
#else
    strcpy(tempfilename, P_tmpdir);
    strcat(tempfilename, "/hwXXXXXX");
    if (mkstemp(tempfilename) == -1)    // NB: also creates/opens the temporary file...
#endif
    {
        if (consoleOutput)
        {
            printf("ERROR: Can't create temporary filename\n");
        }
        return 0;
    }

    // either update or create
    if (bigFileExists(bigfilename))
    {
        fp = fopen(bigfilename, "rb");
        if (!fp)
        {
            if (consoleOutput)
            {
                printf("ERROR: Can't open '%s'\n", bigfilename);
            }
            return 0;
        }
        // ensure correct file type & version
        if (!bigHeaderVerify(fp))
        {
            if (consoleOutput)
            {
                printf("ERROR: Incompatible file %s\n", bigfilename);
            }
            fclose(fp);
            return 0;
        }
        fclose(fp);
        if (consoleOutput)
        {
            printf("Updating %s\n", bigfilename);
        }
    }
    else
    {
        if (consoleOutput)
        {
            printf("Creating %s\n", bigfilename);
        }
        bigCreateNew(bigfilename);
    }

    // move original file to safe, temporary place
    if (!bigCopyFile(bigfilename, tempfilename, 1))
    {
        if (consoleOutput)
        {
            printf("ERROR: Can't create %s\n", tempfilename);
        }
        return 0;
    }

    // keep track of files that need to be moved/deleted
    moveFiles = (int *)malloc(sizeof(int) * numFiles);

    // add each file
    for (f = 0; f < numFiles; ++f)
    {
        filelist = 0;
        if (filenames[f][0] == '@')
        {
            filelistName = filenames[f] + 1;
            // clean up messy slashes in filenames
            filenameSlashMassage(filelistName, TRUE);
            filelist = 1;
            filelistFP = fopen(filelistName, "r");
            if (!filelistFP)
            {
                if (consoleOutput)
                    printf("ERROR: Can't open filelist: %s\n", filelistName);
                unlink(tempfilename);
                free(moveFiles);
                return 0;
            }
            if (optMove && consoleOutput)
            {
                printf("WARNING: Can't (re)move files from filelist: %s\n", filelistName);
            }
        }

        do  // loop in case we have a filelist to process for this one "filename"
        {
            if (filelist)
            {
                if (feof(filelistFP))
                {
                    break;
                }
                // get next filename
                fgets(filelistLine, BF_MAX_FILENAME_LENGTH, filelistFP);
                filelistLine[BF_MAX_FILENAME_LENGTH] = 0;
                if (sscanf(filelistLine, "%s", filename) != 1 || !strlen(filename))
                {
                    continue;
                }
            }
            else
            {
                strcpy(filename, filenames[f]);
            }

            // clean up messy slashes for the TOC
            filenameSlashMassage(filename, FALSE);

            // use simple filename or full path
            if (!optPathnames)
                bigFilenameExtract(tempshortfilename, filename);
            else
                strcpy(tempshortfilename, filename);

            if (consoleOutput)
                printf("  %s ", filename);

            res = bigAddFile(tempfilename, filename, tempshortfilename, optCompression, optNewer, consoleOutput);
            if (!res)
            {
                //if (consoleOutput)
                //  printf("\nERROR: Can't add %s\n", filename);
                unlink(tempfilename);
                free(moveFiles);
                return 0;
            }
            else
            {
                if (res & BF_ADD_RES_ADDED)
                {
                    ++filesAdded;
                    if (consoleOutput)
                        printf("added");
                    moveFiles[f] = optMove && !filelist;
                }
                else if (res & BF_ADD_RES_UPDATED)
                {
                    ++filesUpdated;
                    if (consoleOutput)
                        printf("updated");
                    moveFiles[f] = optMove && !filelist;
                }
                else if (res & BF_ADD_RES_OLD)
                {
                    ++filesUnchanged;
                    if (consoleOutput)
                        printf("unchanged");
                    moveFiles[f] = 0;
                }
                else
                    moveFiles[f] = 0;

                if (strcasecmp(tempshortfilename, filename) && consoleOutput)
                {
                    printf(" (%s)", tempshortfilename);
                }

                if (consoleOutput)
                    printf("\n");
            }
        } while (filelist && !feof(filelistFP));

        if (filelist)
            fclose(filelistFP);
    }

    // "move" (delete) files if necessary
    for (f = 0; f < numFiles; ++f)
        if (moveFiles[f])
        {
            if (unlink(filenames[f]) == -1)
                if (consoleOutput)
                    printf("ERROR: Can't (re)move %s\n", filenames[f]);
        }

    // sort
    if (filesAdded)
    {
        if (consoleOutput)
            printf("Sorting table of contents\n");
        if (!bigSort(tempfilename))
        {
            if (consoleOutput)
                printf("WARNING: Can't sort bigfile %s\n", tempfilename);
            return 0;
        }
    }

    // summary
    if (consoleOutput)
    {
        if (!(filesAdded + filesUpdated))
            printf("Nothing to do.\n");
        else
        {
            if (filesAdded)
                printf("%d file%s added%s\n", filesAdded, ((filesAdded == 1) ? "" : "s"),
                    optMove ? " (moved)":"");
            if (filesUpdated)
                printf("%d file%s updated%s\n", filesUpdated, ((filesUpdated == 1) ? "" : "s"),
                    optMove ? " (moved)":"");
        }
    }

    // replace original file
    if (!bigCopyFile(tempfilename, bigfilename, 0))
    {
        if (consoleOutput)
            printf("ERROR: Can't overwrite %s with new file (%s)\n", bigfilename, tempfilename);
        free(moveFiles);
        return 0;
    }
    if(unlink(tempfilename) == -1)
    {
        if (consoleOutput)
            printf("ERROR: Can't remove temporary file %s\n", tempfilename);
        free(moveFiles);
        return 0;
    }

    free(moveFiles);
    return 1;
}

/*-----------------------------------------------------------------------------
    Name        : bigFastCreate
    Description : Create a new bigfile quickly
    Inputs      : bigfilename - filename of bigfile to create
                  numFiles - # of filenames in following array
                  filenames - names of files to add
                  optCompression - 1|0 compress
                  optNewer - 1|0 add files only if newer
                  optMove - 1|0 move (not just copy) files
                  optPathnames - 1|0 store full pathnames
                  consoleOutput - 1|0 = output to stdout (yes/no)
    Outputs     : New bigfile.
                  Possibly deleted (moved) files.
                  Output to stdout (if enabled).
    Return      : 1 on success, 0 on failure
----------------------------------------------------------------------------*/
int bigFastCreate(char *bigfilename, int numFiles, char *filenames[], int optCompression, int optNewer, int optMove, int optPathnames, int consoleOutput)
{
    char tempshortfilename[BF_MAX_FILENAME_LENGTH+1];
    char tempshortfilenamei[BF_MAX_FILENAME_LENGTH+1];
    char ch;
    int filesAdded = 0, dupesSkipped = 0;
    int pass;
    int f, i, j, index;
    int *moveFiles = NULL;
    FILE *bigFP = NULL, *filelistFP = NULL, *dataFP = NULL;
    int filelist;
    char *filelistName = NULL;
    char filelistLine[BF_MAX_FILENAME_LENGTH+1];
    char filename[BF_MAX_FILENAME_LENGTH+1];
    bigTOC toc;
    struct stat findData;
    bigTOCFileEntry fileEntry;
    unsigned long curOffset = 0;
    int compressedSize = 0;
    crc32 *crcAdded1 = NULL, *crcAdded2 = NULL;
    int dupe = 0;
    uword halfFilenameLength = 0;

    // either update or create
    if (bigFileExists(bigfilename))
    {
        unlink(bigfilename);
        if (bigFileExists(bigfilename))
        {
            if (consoleOutput)
            {
                printf("ERROR: Can't overwrite %s\n", bigfilename);
            }
            
            return 0;
        }
        
        if (consoleOutput)
        {
            printf("Fast-creating %s (overwriting old file)\n", bigfilename);
        }
    }
    else if (consoleOutput)
    {
        printf("Fast-creating %s\n", bigfilename);
    }

    bigFP = fopen(bigfilename, "wb");
    if (!bigFP)
    {
        if (consoleOutput)
        {
            printf("ERROR: Can't create %s\n", bigfilename);
        }
        
        return 0;
    }

    bigHeaderWrite(bigFP);

    toc.numFiles = 0;
    toc.flags = 0;

    // keep track of files that need to be moved/deleted
    moveFiles = (int *)malloc(sizeof(int) * numFiles);

    //  do 4 passes : count files, write filecount, TOC headers, data
    for (pass = -1; pass < 3; ++pass)
    {
        // add each file
        for (f = 0; f < numFiles; ++f)
        {
            filelist = 0;
            if (filenames[f][0] == '@')
            {
                filelistName = filenames[f] + 1;
                filelist = 1;
                filelistFP = fopen(filelistName, "r");
                
                if (!filelistFP)
                {
                    if (consoleOutput)
                    {
                        printf("ERROR: Can't open filelist: %s\n", filelistName);
                    }
                    
                    free(moveFiles);
                    return 0;
                }
                if (pass == -1 && optMove && consoleOutput)
                {
                    printf("WARNING: Can't (re)move files from filelist: %s\n", filelistName);
                }
            }

            do  // loop in case we have a filelist to process for this one "filename"
            {
                if (filelist)
                {
                    if (feof(filelistFP))
                    {
                        break;
                    }
                    
                    // get next filename
                    fgets(filelistLine, BF_MAX_FILENAME_LENGTH, filelistFP);
                    filelistLine[BF_MAX_FILENAME_LENGTH] = 0;
                    
                    if (sscanf(filelistLine, "%s", filename) != 1 || !strlen(filename))
                    {
                        continue;
                    }
                }
                else
                {
                    strcpy(filename, filenames[f]);
                }

                // clean up messy slashes for the TOC
                filenameSlashMassage(filename, FALSE);

                // use simple filename or full path
                if (!optPathnames)
                {
                    bigFilenameExtract(tempshortfilename, filename);
                }
                else
                {
                    strcpy(tempshortfilename, filename);
                }

                // pretend name is lowercase for consistent CRCs
                for (j = 0; tempshortfilename[j]; j++)
                {
                    tempshortfilenamei[j] = tolower(tempshortfilename[j]);
                }
                
                tempshortfilenamei[j] = '\0';

                // convert all slashes to backslashes and whittle multiple slashes down to a single one
                filenameSlashMassage(tempshortfilenamei, FALSE);

                if (pass > 1 && consoleOutput)
                {
                    printf("  %s ", filename);
                }

                // watch for dupes
                fileEntry.nameLength = (unsigned short)strlen(tempshortfilenamei);
                halfFilenameLength   = fileEntry.nameLength / 2;

                fileEntry.nameCRC1 = crc32Compute((ubyte *)tempshortfilenamei, halfFilenameLength);
                fileEntry.nameCRC2 = crc32Compute((ubyte *)tempshortfilenamei + halfFilenameLength,
                                                  (fileEntry.nameLength - halfFilenameLength));

                fileEntry.compressionType = optCompression;

                if (pass != -1)
                {
                    dupe = 0;
                    for (i = 0; i < filesAdded; ++i)
                    {
                        if (fileEntry.nameCRC1 == crcAdded1[i] &&
                            fileEntry.nameCRC2 == crcAdded2[i])
                        {
                            dupe = 1;
                            break;
                        }
                    }
                }
                
                // revert back to local path variant
                filenameSlashMassage(filename, TRUE);

                switch (pass)
                {
                    case -1:
                        // count everything, even dupes
                        break;
                        
                    case 0:
                        if (dupe)
                        {
                            continue;
                        }
                        break;
                        
                    case 1:
                        // used to write TOC header entry here, but because
                        // of compression possibility (ie, unknown stored size),
                        //  we do it in stage 2 now
                        if (dupe)
                        {
                            continue;
                        }
                        break;
                        
                    case 2:
                        // TOC header entry & data

                        if (dupe)
                        {
                            ++dupesSkipped;
                            if (consoleOutput)
                            {
                                printf("skipped (duplicate)\n");
                            }
                            continue;
                        }

                        if (stat(filename, &findData) == -1)
                        {
                            if (consoleOutput)
                            {
                                printf("\nERROR: Can't find %s\n", filename);
                            }
                            
                            fclose(bigFP);
                            unlink(bigfilename);
                            free(moveFiles);
                            return 0;
                        }
                        fileEntry.realLength = findData.st_size;
                        fileEntry.timeStamp = findData.st_mtime;

                        // data
                        fseek(bigFP, curOffset, SEEK_SET);
                        bigFilenameEncrypt(tempshortfilename);
                        fwrite((void *)(tempshortfilename), 1, fileEntry.nameLength+1, bigFP);
                        bigFilenameDecrypt(tempshortfilename, fileEntry.nameLength);
                        dataFP = fopen(filename, "rb");
                        if (!dataFP)
                        {
                            if (consoleOutput)
                            {
                                printf("ERROR: Can't read %s\n", filename);
                            }
                            fclose(bigFP);
                            unlink(bigfilename);
                            free(moveFiles);
                            return 0;
                        }
                        if (fileEntry.compressionType)
                        {
                            // compress and append
                            BitFile *bitFile;
                            bitFile = bitioFileAppendStart(bigFP);
                            lzssCompressFile(dataFP, bitFile);
                            compressedSize = bitioFileAppendStop(bitFile);
                            //  revert to uncompressed if the file won't compress
                            if ((float)compressedSize/(float)fileEntry.realLength > MINIMUM_COMPRESSION_RATIO)
                            {
                                fileEntry.compressionType = 0;
                                // theoretically, this could leave some harmless data at the end of a bigfile
                                fseek(bigFP, curOffset + strlen(tempshortfilename)+1, SEEK_SET);
                                fseek(dataFP, 0, SEEK_SET);
                            }
                        }
                        if (!fileEntry.compressionType)
                        {
                            // raw, uncompressed copy
                            while (1)
                            {
                                // inefficient!
                                ch = (char)getc(dataFP);
                                if (feof(dataFP))
                                {
                                    break;
                                }
                                putc(ch, bigFP);
                            }
                        }
                        fclose(dataFP);

                        // TOC header entry
                        fseek(bigFP,   // overwrite placeholder from previous step
                              strlen(BF_FILE_HEADER BF_VERSION) +
                                sizeof(toc.numFiles) +
                                sizeof(toc.flags) +
                                (filesAdded * sizeof(bigTOCFileEntry)), SEEK_SET);

                        fileEntry.storedLength = fileEntry.compressionType ? compressedSize : findData.st_size;
                        fileEntry.offset = curOffset;

                        bigTOCFileEntryWrite(&fileEntry, bigFP);

                        curOffset += fileEntry.nameLength + 1 + fileEntry.storedLength;

                        moveFiles[f] = optMove && !filelist;

                        break;
                }

                if (pass != -1)
                {
                    // keep track to prevent dupes
                    crcAdded1[filesAdded] = fileEntry.nameCRC1;
                    crcAdded2[filesAdded] = fileEntry.nameCRC2;
                }
                ++filesAdded;

                if (pass == 2 && consoleOutput)
                {
                    //printf("[pass %d of 2]", pass);  // don't mention the first passes
                    if (strcasecmp(tempshortfilename, filename))
                    {
                        printf("(%s) ", tempshortfilename);
                    }
                    printf("added\n");
                }

            } while (filelist && !feof(filelistFP));

            if (filelist)
            {
                fclose(filelistFP);
            }
        }

        switch (pass)
        {
            case -1:
                crcAdded1 = (crc32 *)malloc(sizeof(crc32) * filesAdded);
                crcAdded2 = (crc32 *)malloc(sizeof(crc32) * filesAdded);
                filesAdded = 0;
                break;

            case 0:
#if FIX_ENDIAN
                filesAdded = FIX_ENDIAN_INT_32( filesAdded );
#endif
                fwrite((void *)&(filesAdded), sizeof(filesAdded), 1, bigFP);
                filesAdded = 0;
                
#if FIX_ENDIAN
                toc.flags = FIX_ENDIAN_INT_32( toc.flags );
#endif

                fwrite((void *)&(toc.flags), sizeof(toc.flags), 1, bigFP);

// reset back again
#if FIX_ENDIAN
                toc.flags = FIX_ENDIAN_INT_32( toc.flags );
#endif
                break;

            case 1:
                // placeholders for real TOC header entries (to be updated later)
                for (index = 0; index < filesAdded; index++)
                {
                    bigTOCFileEntryWrite(&fileEntry, bigFP);
                }
                curOffset = strlen(BF_FILE_HEADER BF_VERSION)
                        + sizeof(toc.numFiles)
                        + sizeof(toc.flags)
                        + (filesAdded * sizeof(bigTOCFileEntry));
                filesAdded = 0;  // reset for final pass, to keep track of dupes
                break;
        }
    }

    fclose(bigFP);

    if (consoleOutput)
    {
        printf("Sorting table of contents");
    }
    
    if (!bigSort(bigfilename))
    {
        printf("\nERROR: Can't sort bigfile %s\n", bigfilename);
        unlink(bigfilename);
        free(moveFiles);
        return 0;
    }
    
    if (consoleOutput)
    {
        printf("\n");
    }
    
    // "move" (delete) files if necessary
    for (f = 0; f < numFiles; ++f)
    {
        if (moveFiles[f])
        {
            if (unlink(filenames[f]) == -1)
            {
                if (consoleOutput)
                {
                    printf("ERROR: Can't (re)move %s\n", filenames[f]);
                }
            }
        }
    }

    // summary
    if (consoleOutput)
    {
        if (!filesAdded && !dupesSkipped)
        {
            printf("Nothing to do\n");
        }
        else
        {
            if (filesAdded)
            {
                printf("%d file%s added to new bigfile\n", filesAdded, ((filesAdded == 1) ? "" : "s"));
            }
            if (dupesSkipped)
            {
                printf("%d duplicate%s skipped\n", dupesSkipped, ((dupesSkipped == 1) ? "" : "s"));
            }
        }
    }

    free(crcAdded1);
    free(crcAdded2);
    free(moveFiles);
    return 1;
}

/*-----------------------------------------------------------------------------
    Name        : bigAddFile
    Description : Add a file to a bigfile
    Inputs      : bigFilename - filename of bigfile (to add or update to)
                  filename - file to add (or update)
                  storedFilename - name to store (could be shortr, with no path)
                                   (this will have its slashes massaged in place -- possibly
                                    resulting in a shorter name)
                  optCompression - 1|0 compress
                  optNewer - 1|0 add file only if newer
                  consoleOutput - 1|0 = output to stdout (yes/no)
    Outputs     : Updated bigfile.
    Return      : 0 on failure, or...
                  BF_ADD_RES_ADDED
                  BF_ADD_RES_UPDATED
                  BF_ADD_RES_OLD
    Note        : If 0 is returned, this routine must display an error message
                  to the console.
----------------------------------------------------------------------------*/
int bigAddFile(char *bigFilename, char *filename, char *storedFilename, int optCompression, int optNewer, int consoleOutput)
{
    char tempshortfilenamei[BF_MAX_FILENAME_LENGTH+1];
    FILE *oldfp, *newfp, *datafp;
    bigTOC oldtoc, newtoc;
    bigTOCFileEntry fileEntry;
#ifdef _WIN32
    char tempFilename[L_tmpnam], compressedTempFilename[L_tmpnam];
#else
    char tempFilename[PATH_MAX], compressedTempFilename[PATH_MAX];
#endif
    udword fileNum;
    int res = 0;
    sdword i;
    sdword offsetDelta;
    udword oldStoredLength;
    struct stat findData, findDataCompressed;
    uword halfFilenameLength = 0;

    newtoc.fileEntries = (bigTOCFileEntry *)0;

    // temp file in case we need to abort
#ifdef _WIN32
    if (!tmpnam(tempFilename) || (optCompression && !tmpnam(compressedTempFilename)))
#else
    strcpy(tempFilename, P_tmpdir);
    strcat(tempFilename, "/hwXXXXXX");
    strcpy(compressedTempFilename, tempFilename);
    if ((mkstemp(tempFilename) == -1) || (optCompression && (mkstemp(compressedTempFilename) == -1)))
#endif
    {
        if (consoleOutput)
            printf("\nERROR: Can't create temporary filename\n");
        return 0;
    }

    // open current bigfile
    oldfp = fopen(bigFilename, "rb");
    if (!oldfp)
    {
        if (consoleOutput)
            printf("\nERROR: Can't open bigfile %s\n", bigFilename);
        return 0;
    }
    // ensure correct file type & version
    if (!bigHeaderVerify(oldfp))
    {
        if (consoleOutput)
            printf("\nERROR: Incompatible file %s\n", bigFilename);
        fclose(oldfp);
        return 0;
    }

    // start new bigfile
    newfp = fopen(tempFilename, "wb");
    if (!newfp || !bigHeaderWrite(newfp))
    {
        if (consoleOutput)
            printf("\nERROR: Can't create temporary file %s\n", tempFilename);
        goto abort;
    }

    // read current toc
    bigTOCRead(oldfp, &oldtoc);

    // clean up messy slashes for the TOC
    filenameSlashMassage(filename, TRUE);
    filenameSlashMassage(storedFilename, FALSE);

    // fill in fileentry
    if (stat(filename, &findData) == -1)
    {
        if (consoleOutput)
            printf("\nERROR: Can't find %s\n", filename);
        goto abort;
    }

    // compress file to temp file if required
    if (optCompression)
    {
        BitFile *bf = bitioFileOpenOutput(compressedTempFilename);
        if (!bf)
        {
            if (consoleOutput)
                printf("\nERROR: Can't create temporary file %s\n", compressedTempFilename);
            goto abort;
        }
        datafp = fopen(filename, "rb");
        if (!datafp)
        {
            if (consoleOutput)
                printf("\nERROR: Can't open %s\n", filename);
            goto abort;
        }
        lzssCompressFile(datafp, bf);
        fclose(datafp);
        bitioFileCloseOutput(bf);
        if (stat(compressedTempFilename, &findDataCompressed) == -1)
        {
            if (consoleOutput)
                printf("\nERROR: Can't find %s\n", filename);
            goto abort;
        }
    }

    // pretend name is lowercase for consistent CRCs
    for (i = 0; (tempshortfilenamei[i] = tolower(storedFilename[i])); i++) { }

    // convert all slashes to backslashes and whittle multiple slashes down to a single one
    filenameSlashMassage(tempshortfilenamei, FALSE);

    //strcpy(fileEntry.name, storedFilename);
    fileEntry.nameLength = (unsigned short)strlen(tempshortfilenamei);
    halfFilenameLength   = fileEntry.nameLength / 2;

    fileEntry.nameCRC1 = crc32Compute((ubyte *)tempshortfilenamei, halfFilenameLength);
    fileEntry.nameCRC2 = crc32Compute((ubyte *)tempshortfilenamei + halfFilenameLength,
                                      (fileEntry.nameLength - halfFilenameLength));
    fileEntry.storedLength = optCompression ? findDataCompressed.st_size : findData.st_size;
    fileEntry.realLength = findData.st_size;
    fileEntry.timeStamp = findData.st_mtime;
    fileEntry.compressionType = optCompression;
    // fileEntry.offset will be calculated later

    // update existing file
    if (bigTOCFileExists(&oldtoc, storedFilename, &fileNum))
    {
        char compareFilename[BF_MAX_FILENAME_LENGTH];

        // important: check right now if we happen to have two filenames
        // with the same CRCs
        if (!bigFilenameRetrieve(&oldtoc, fileNum, oldfp, compareFilename))
        {
            goto abort;
        }
        
        if (strcasecmp(storedFilename, compareFilename))
        {
            if (consoleOutput)
            {
                printf("\nERROR: Can't add %s (same CRC as %s)\n",
                    storedFilename, compareFilename);
            }
            
            goto abort;
        }

        // if required, ensure file is newer
        if (optNewer && fileEntry.timeStamp <= (oldtoc.fileEntries + fileNum)->timeStamp)
        {
            fclose (newfp);
            fclose (oldfp);
            unlink(tempFilename);
            return BF_ADD_RES_OLD;
        }

        // update fileEntry
        oldStoredLength = (oldtoc.fileEntries + fileNum)->storedLength;
        offsetDelta = fileEntry.storedLength - oldStoredLength;
        fileEntry.offset = (oldtoc.fileEntries + fileNum)->offset;
        memcpy(oldtoc.fileEntries + fileNum, &fileEntry, sizeof(bigTOCFileEntry));

        // all files past the updated one are offset
        for (i = fileNum + 1; i < oldtoc.numFiles; ++i)
            (oldtoc.fileEntries + i)->offset += offsetDelta;

        // write updated TOC (unsorted, still)
        bigTOCWrite(newfp, &oldtoc);

        // copy first fileNum-1 file data
        i = (oldtoc.fileEntries)->offset;
        while ((unsigned long)i < fileEntry.offset)
        {
            fputc(fgetc(oldfp), newfp);
            ++i;
        }

        // write new file data
        datafp = fopen(optCompression ? compressedTempFilename : filename, "rb");
        if (!datafp)
        {
            if (consoleOutput)
                printf("\nERROR: Can't open %s\n", optCompression ? compressedTempFilename : filename);
            goto abort;
        }
        bigFilenameEncrypt(storedFilename);
        fwrite(storedFilename, 1, fileEntry.nameLength+1, newfp); // filename
        bigFilenameDecrypt(storedFilename, fileEntry.nameLength);
        i = fileEntry.storedLength;
        while (i--)
            fputc(fgetc(datafp), newfp);
        fclose(datafp);

        // skip old data
        fseek(oldfp, oldStoredLength + fileEntry.nameLength + 1, SEEK_CUR);

        // copy all subsequent file data
        i = fileEntry.offset + fileEntry.storedLength + fileEntry.nameLength + 1;
        while ((unsigned long)i < (oldtoc.fileEntries + oldtoc.numFiles - 1)->offset +
            (oldtoc.fileEntries + oldtoc.numFiles - 1)->storedLength +
            (oldtoc.fileEntries + oldtoc.numFiles - 1)->nameLength +1)
        {
            fputc(fgetc(oldfp), newfp);
            ++i;
        }

        // already sorted

        // write updated TOC
        //bigTOCWrite(newfp, &oldtoc);

        res = BF_ADD_RES_UPDATED;
    }
    // add new file
    else
    {
        // add new toc entry at end and write the toc
        newtoc.numFiles = oldtoc.numFiles+1;
        newtoc.flags = oldtoc.flags & (~BF_FLAG_TOC_SORTED);  // mark TOC as unsorted now
        newtoc.fileEntries = malloc(sizeof(bigTOCFileEntry)*(newtoc.numFiles));
        memcpy(newtoc.fileEntries, oldtoc.fileEntries, sizeof(bigTOCFileEntry)*oldtoc.numFiles);
        if (oldtoc.numFiles)
        {
            // offset each file data offset to make room for the TOCFileEntry
            for (i = 0; i < newtoc.numFiles-1; ++i)
            {
                (newtoc.fileEntries + i)->offset += sizeof(bigTOCFileEntry);
            }
            
            fileEntry.offset = (newtoc.fileEntries + oldtoc.numFiles - 1)->offset +
                (newtoc.fileEntries + oldtoc.numFiles - 1)->storedLength +
                (newtoc.fileEntries + oldtoc.numFiles - 1)->nameLength + 1;
        }
        else
        {
            // first entry
            fileEntry.offset = strlen(BF_FILE_HEADER BF_VERSION) + sizeof(oldtoc.numFiles) + sizeof(oldtoc.flags) +
                sizeof(bigTOCFileEntry);
        }
        
        memcpy(newtoc.fileEntries + oldtoc.numFiles, &fileEntry, sizeof(bigTOCFileEntry));

        bigTOCWrite(newfp, &newtoc); // (unsorted)

        // copy all the old file data
        if (oldtoc.numFiles)
        {
            i = (newtoc.fileEntries)->offset;
            while ((unsigned long)i < fileEntry.offset)
            {
                fputc(fgetc(oldfp), newfp);
                ++i;
            }
        }

        // write the new file data
        datafp = fopen(optCompression ? compressedTempFilename : filename, "rb");
        if (!datafp)
        {
            if (consoleOutput)
            {
                printf("\nERROR: Can't open %s\n", optCompression ? compressedTempFilename : filename);
            }
            
            goto abort;
        }
        bigFilenameEncrypt(storedFilename);
        fwrite(storedFilename, 1, fileEntry.nameLength+1, newfp); // filename
        bigFilenameDecrypt(storedFilename, fileEntry.nameLength);
        i = fileEntry.storedLength;
        while (i--)
        {
            fputc(fgetc(datafp), newfp);
        }
        fclose(datafp);

        res = BF_ADD_RES_ADDED;
    }


    fclose (newfp);
    fclose (oldfp);

#if 0
    // sort
    if (!bigSort(tempFilename))
    {
        if (consoleOutput)
            printf("\nWARNING: Can't sort bigfile %s\n", tempFilename);
        return 0;
    }
#endif

    // copy new file to old file
    if (!bigCopyFile(tempFilename, bigFilename, 0))
    {
        if (consoleOutput)
        {
            printf("\nERROR: Can't overwrite %s with new file (%s)\n", bigFilename, tempFilename);
        }
        
        return 0;
    }

    // remove tempfile
    if(unlink(tempFilename) == -1)
    {
        if (consoleOutput)
        {
            printf("\nERROR: Can't remove temporary file %s\n", tempFilename);
        }
        
        return 0;
    }

    if (optCompression)
    {
        if(unlink(compressedTempFilename) == -1)
        {
            if (consoleOutput)
            {
                printf("\nERROR: Can't remove temporary file %s\n", compressedTempFilename);
            }
            
            return 0;
        }
    }

    return res;

abort:
    if (newtoc.fileEntries)
    {
        free(newtoc.fileEntries);
    }
    
    free(oldtoc.fileEntries);
    fclose(newfp);
    fclose(oldfp);
    
    return 0;
}

/*-----------------------------------------------------------------------------
    Name        : bigDelete
    Description : Delete files from a bigfile
    Inputs      : bigfilename - filename of bigfile (to delete from)
                  numFiles - # of filenames in following array
                  filenames - names of files to delete
                  consoleOutput - 1/0 = output to stdout (yes/no)
    Outputs     : Updated bigfile.
                  Output to stdout (if enabled).
    Return      : 1 on success, 0 on failure
----------------------------------------------------------------------------*/
int bigDelete(char *bigfilename, int numFiles, char *filenames[], int consoleOutput)
{
    if (consoleOutput)
        printf("ERROR: Delete operation not supported yet.\n");
    return 1;
}

/*-----------------------------------------------------------------------------
    Name        : bigView
    Description : View contents of a bigfile
    Inputs      : bigfilename - filename of bigfile to view
                  consoleOutput - 1/0 = output to stdout (yes/no)
    Outputs     : Output to stdout (if enabled).
    Return      : 1 on success, 0 on failure
----------------------------------------------------------------------------*/
int bigView(char *bigFilename, int consoleOutput)
{
    FILE *fp;
    bigTOC toc;
    int f;
    unsigned long totalCompressed = 0, totalExpanded = 0;
    int displayCompressed = 0;
    char pRealSize[32], pStoredSize[32], pTimestamp[32], pRatio[16];
    struct stat findData;
    char filename[BF_MAX_FILENAME_LENGTH+1];

    // open bigfile
    fp = fopen(bigFilename, "rb");
    if (!fp)
    {
        if (consoleOutput)
            printf("ERROR: Can't open %s\n", bigFilename);
        return 0;
    }
    // ensure correct file type & version
    if (!bigHeaderVerify(fp))
    {
        if (consoleOutput)
            printf("ERROR: Incompatible file %s\n", bigFilename);
        goto abort;
    }

    // read current toc
    bigTOCRead(fp, &toc);

    if (!toc.numFiles)
        printf("No files in %s\n", bigFilename);
    else if (consoleOutput)
    {
        stat(bigFilename, &findData);

        printf("\nContents of %s ", bigFilename);
        printf(  "(%s bytes, ", PrettyFilesize(findData.st_size, pStoredSize));
        printf(  "%s):\n", (toc.flags & BF_FLAG_TOC_SORTED)?"CRC-sorted TOC":"unsorted TOC");

        printf("\n             CRC   Stored Size     Real Size  Ratio        Date     Time  Filename\n");
        printf("----------------  ------------  ------------  -----  ---------- --------  ------------\n");
        for (f = 0; f < toc.numFiles; ++f)
        {
            bigFilenameRetrieve(&toc, f, fp, filename);
            if ((toc.fileEntries + f)->compressionType)
                sprintf(pRatio, "%05.3f",
                    ((toc.fileEntries + f)->realLength ?
                     (float)(toc.fileEntries + f)->storedLength/(float)(toc.fileEntries + f)->realLength
                     : 0.0));
            else
                strcpy(pRatio, "  na ");
            printf("%08x%08x %13s %13s  %s  %s  %s\n",
                (toc.fileEntries + f)->nameCRC1,
                (toc.fileEntries + f)->nameCRC2,
                PrettyFilesize((toc.fileEntries + f)->storedLength, pStoredSize),
                PrettyFilesize((toc.fileEntries + f)->realLength, pRealSize),
                pRatio,
                PrettyDateTime((toc.fileEntries + f)->timeStamp, pTimestamp),
                filename);
            totalCompressed += (toc.fileEntries + f)->storedLength;
            totalExpanded += (toc.fileEntries + f)->realLength;
            if ((toc.fileEntries + f)->storedLength != (toc.fileEntries + f)->realLength)
                displayCompressed = 1;
        }
        // totals
        if (toc.numFiles)
        {
            printf("                  ------------  ------------  -----\n");
            if (totalCompressed == totalExpanded)
            {
                PrettyFilesize(totalExpanded, pRealSize),
                strcpy(pStoredSize, "");
                strcpy(pRatio, "  na ");
            }
            else
            {
                PrettyFilesize(totalCompressed, pStoredSize),
                PrettyFilesize(totalExpanded, pRealSize),
                sprintf(pRatio, "%05.3f",
                    (totalExpanded ? (float)totalCompressed/(float)totalExpanded : 0.0));
            }
            printf("                 %13s %13s  %s\n",
                pStoredSize, pRealSize, pRatio);
        }
    }

    fclose(fp);

    return 1;

abort:
    fclose(fp);
    return 0;
}

/*-----------------------------------------------------------------------------
    Name        : bigExtract
    Description : Extract files from a bigfile
    Inputs      : bigfilename - filename of bigfile to extract from
                  numFiles - # of filenames in following array
                  filenames - names of files to extract
                  optNewer - 1|0 extract files only if newer
                  optMove - 1|0 move (not just copy) files out of bigfile
                  optPathnames - 1|0 extract to subdir pathnames stored
                  optOverwrite - 1|0 overwrite existing files
                  consoleOutput - 1/0 = output to stdout (yes/no)
    Outputs     : Possibly updated bigfile.
                  New files.
                  Output to stdout (if enabled).
    Return      : 1 on success, 0 on failure
----------------------------------------------------------------------------*/
int bigExtract(char *bigFilename, int numFiles, char *filenames[], int optFreshen, int optMove, int optPathnames, int optOverwrite, int consoleOutput)
{
#ifdef _WIN32
    if (consoleOutput) {
        printf("ERROR: Extract operation not supported yet.\n");
    }
#else

    FILE   *fp;
    bigTOC  toc;

    // open bigfile
    fp = fopen(bigFilename, "rb");
    if (!fp)
    {
        if (consoleOutput)
            printf("ERROR: Can't open %s\n", bigFilename);
        return 0;
    }
    // ensure correct file type & version
    if (!bigHeaderVerify(fp))
    {
        if (consoleOutput)
            printf("ERROR: Incompatible file %s\n", bigFilename);
        goto abort;
    }

    // read table of contents for .big file
    bigTOCRead(fp, &toc);

    if (!toc.numFiles)
        printf("No files in %s\n", bigFilename);
    else
    {
        int i;
        char *ptr = NULL;
        
        FILE *extractFp = fp;                            // take a copy of the main BigFile file pointer
        bigTOCFileEntry *fileEntry = NULL;               // an entry in the table of contents
        char filename[BF_MAX_FILENAME_LENGTH] = {'\0'};  // filename stored in the .big file

        char *compressedFile   = NULL,                   // memory buffers for the compressed...
             *uncompressedFile = NULL;                   //     and uncompressed files

        char outdir[PATH_MAX];                           // directory output files are written to
        FILE *output;                                    // file writing file handle
        char outfilename[BF_MAX_FILENAME_LENGTH];        // the path/filename of the output file
        
        // output directory name will be something like "Homeworld.big.contents"
        strcpy(outdir, bigFilename);
        strcat(outdir, ".contents");
        
        printf("\nExtracting %d files from:\n    %s\nto:\n    %s\n\n",
               toc.numFiles, bigFilename, outdir);
        
        // go through the table of contents
        for (i = 0; i < toc.numFiles; i++)
        {
            int j = 0;
            bool matches_args = 0;

            // get the entry in the TOC for the next file
            fileEntry = toc.fileEntries + i;

            // seek to the start of the data for the file
            fseek(extractFp, fileEntry->offset, SEEK_SET);
            
            // first thing is the filename - this includes a NULL terminator
            fread(filename, 1, fileEntry->nameLength + 1, extractFp);
            bigFilenameDecrypt(filename, fileEntry->nameLength);


#ifdef _WIN32
            /* Does anyone know what Windows has instead of fnmatch?
               Please fix this. */
            matches_args = 1;
#else
            // BUG: filelists (@-notation) are not supported.
            for (; j < numFiles; j++)
            {
                switch (fnmatch(filenames[j], filename, 0))
                {
                default:
                    /* POSIX defines this mechanism for indicating errors
                       but leaves open what kind of errors might occur
                       and how to distinguish between them. */
                    printf("Error matching pattern %s to filename %s\n",
                           filenames[j], filename);
                    // fall through
                case FNM_NOMATCH:
                    continue;
                case 0:
                    matches_args = 1;
                }
                break;
            }
#endif
            if (numFiles > 0 && !matches_args)
                continue;

            // the rest is the (possibly compressed) file
            printf("Extracting: %s", filename);
            
            // uncompress file if necessary
            if (fileEntry->compressionType == '\1')
            {
                compressedFile   = (char *) malloc(fileEntry->storedLength);
                uncompressedFile = (char *) malloc(fileEntry->realLength);
                
                if (compressedFile != NULL && uncompressedFile != NULL)
                {
                    fread(compressedFile, 1, fileEntry->storedLength, extractFp);
                    if (lzssExpandBuffer(compressedFile, fileEntry->storedLength,
                                         uncompressedFile, fileEntry->realLength) == -1)
                    {
                        printf(" - DECOMPRESSION FAILED!\n");
                        
                        free(compressedFile);
                        compressedFile = NULL;
                        
                        free(uncompressedFile);
                        uncompressedFile = NULL;

                        continue;
                    }
                }
                else
                {
                    printf(" - MEMORY ALLOCATION FAILED!\n");
                    
                    // either or both allocations may have failed so check both
                    if (compressedFile != NULL)
                    {
                        free(compressedFile);
                        compressedFile = NULL;
                    }
                    if (uncompressedFile != NULL)
                    {
                        free(uncompressedFile);
                        uncompressedFile = NULL;
                    }

                    continue;
                }
            }
            else
            {
                uncompressedFile = (char *) malloc(fileEntry->realLength);

                if (uncompressedFile != NULL)
                {
                    fread(uncompressedFile, 1, fileEntry->realLength, extractFp);
                }
                else
                {
                    printf(" - MEMORY ALLOCATION FAILED!\n");
                    continue;
                }

            }
            
            // this isn't strictly necessary but Homeworld.big will create a
            // hell of a mess otherwise; semi-bug
            strcpy(outfilename, outdir);
            strcat(outfilename, "/");
            
            // fiddle directory delimiters
            strcat(outfilename, filename);
            ptr = outfilename;
            while (*ptr != '\0')
            {
                if (*ptr == '\\')
                {
                    *ptr = '/';
                }
                
                ptr++;
            }
            
            // what's the function call in C to create directories?
            // system() isn't the way to be doing this...
            ptr = strrchr(outfilename, '/');
            if (ptr != NULL)
            {
                char mkdir[1024];
                *ptr = '\0';
                
                // quoting path to ensure characters like spaces are parsed correctly
                strcpy(mkdir, "/bin/mkdir -p '");
                strcat(mkdir, outfilename);
                strcat(mkdir, "'");
                
                // actually create the directory
                system(mkdir);
                
                // put the string back the way it was before
                *ptr = '/';
            }
            
            // open the new file
            if ((output = fopen(outfilename, "wb")) == NULL)
            {
                printf(" - UNABLE TO OPEN OUTPUT FILE %s!\n", outfilename);
                
                if (compressedFile != NULL)
                {
                    free(compressedFile);
                    compressedFile = NULL;
                }
                if (uncompressedFile != NULL)
                {
                    free(uncompressedFile);
                    uncompressedFile = NULL;
                }

                continue;
            }
            else
            {
                // write the file
                fwrite(uncompressedFile, 1, fileEntry->realLength, output);
                fclose(output);

                if (compressedFile != NULL)
                {
                    free(compressedFile);
                    compressedFile = NULL;
                }
                if (uncompressedFile != NULL)
                {
                    free(uncompressedFile);
                    uncompressedFile = NULL;
                }
            }
            
            printf("\n");
        }
    }

abort:
    fclose(fp);

#endif

    return 1;
}

//
//  the following routines are for in-game compilation only -- not for the command-line tool
//
#ifdef BF_HOMEWORLD

bool bigOpenAllBigFiles(void)
{
    udword bigfile_i = 0;
    char bigFilePath[PATH_MAX] = "";

    for (bigfile_i = 0; bigfile_i < NUMBER_CONFIGURED_BIG_FILES; ++bigfile_i)
    {
        strcpy(bigFilePath,
               filePathPrepend(bigFilePrecedence[bigfile_i].bigFileName, FF_HomeworldDataPath));
        
        if (!fileExists(bigFilePath, FF_IgnorePrepend))
        {
            if (bigFilePrecedence[bigfile_i].required)
            {
                dbgFatalf(DBG_Loc, "Unable to find required .big file: %s", bigFilePath);
            }
        }
        
        // can't fileOpen them because fileOpen wraps the .big archives themselves
        bigFilePrecedence[bigfile_i].filePtr = fopen(bigFilePath, "rb");
        
        if (bigFilePrecedence[bigfile_i].filePtr == NULL)
        {
            if (bigFilePrecedence[bigfile_i].required)
            {
                dbgFatalf(DBG_Loc, "Unable to open required .big file: %s", filePathTempBuffer);
            }
            else
            {
                continue;
            }
        }

        if (!bigHeaderVerify(bigFilePrecedence[bigfile_i].filePtr))
        {
            if (bigFilePrecedence[bigfile_i].required)
            {
                dbgFatalf(DBG_Loc, "Incompatible version for required .big file: %s", filePathTempBuffer);
            }
        }

        if (bigFilePrecedence[bigfile_i].filePtr != NULL)
        {
            numOpenedBigFiles++;

            bigTOCRead(bigFilePrecedence[bigfile_i].filePtr,
                      &bigFilePrecedence[bigfile_i].tableOfContents);
            
            dbgMessagef("%8d files found in %s",
                bigFilePrecedence[bigfile_i].tableOfContents.numFiles,
                bigFilePrecedence[bigfile_i].bigFileName);
        
            bigFilePrecedence[bigfile_i].localFileRelativeAge
                = memAlloc(bigFilePrecedence[bigfile_i].tableOfContents.numFiles,
                           "bigLocalFileRelativeAge", 0);                   // 0 => no memAlloc flags
                           
            memset(bigFilePrecedence[bigfile_i].localFileRelativeAge,
                0, bigFilePrecedence[bigfile_i].tableOfContents.numFiles);  // 0 = LOCAL_FILE_DOES_NOT_EXIST
        }
    }
    
    bigFilesystemCompare(fileOverrideBigPath, "");

    return TRUE;
}

//
//  Allocate memory and load a file into it.
//  If there's any other problem, return -1.  Otherwise, return
//  the number of bytes allocated and loaded.
//
sdword bigFileLoadAlloc(bigTOC *toc, FILE *bigFP, char *filename, udword fileNum, void **address)
{
    sdword length;
    bigTOCFileEntry *entry;
    char *memoryName;
    int expandedSize, storedSize;
    BitFile *bitFile;

    if (IgnoreBigfiles)
    {
        return -1;
    }
    
    // go there
    entry = toc->fileEntries + fileNum;
    fseek(bigFP, entry->offset + entry->nameLength+1, SEEK_SET);

    // allocate
    length = entry->realLength;
    dbgAssertOrIgnore(length > 0);
    if (entry->nameLength > MEM_NameLength)
    {
        memoryName = &filename[entry->nameLength - MEM_NameLength];
    }
    else
    {
        memoryName = filename;
    }
    
    *address = memAllocAttempt(length, memoryName, NonVolatile | MBF_String);
    if (*address == NULL)
    {
        dbgFatalf(DBG_Loc, "bigFileLoadAlloc: couldn't allocate %d bytes for %s", length, filename);
    }
    
    if (entry->compressionType)
    {
        // expand compressed file data directly into memory
        bitFile = bitioFileInputStart(bigFP);
        expandedSize = lzssExpandFileToBuffer(bitFile, *address, length);
        storedSize = bitioFileInputStop(bitFile);
        dbgAssertOrIgnore(expandedSize == length);
        dbgAssertOrIgnore(storedSize == entry->storedLength);
    }
    else
    {
        // load uncompressed file data
        fread(*address, 1, length, bigFP);
    }

    return length;
}

//
//  Load a file into memory.
//  If there's any problem, return -1.  Otherwise, return
//  the number of bytes loaded.
//
sdword bigFileLoad(bigTOC *toc, FILE *bigFP, udword fileNum, void *address)
{
    sdword length;
    bigTOCFileEntry *entry;
    int expandedSize, storedSize;
    BitFile *bitFile;

    if (IgnoreBigfiles)
    {
        return -1;
    }
    
    // go there
    entry = toc->fileEntries + fileNum;
    fseek(bigFP, entry->offset + entry->nameLength+1, SEEK_SET);

    length = entry->realLength;
    dbgAssertOrIgnore(length > 0);

    if (entry->compressionType)
    {
        // expand compressed file data directly into memory
        bitFile = bitioFileInputStart(bigFP);
        expandedSize = lzssExpandFileToBuffer(bitFile, address, length);
        storedSize = bitioFileInputStop(bitFile);
        dbgAssertOrIgnore(expandedSize == length);
        dbgAssertOrIgnore(storedSize == entry->storedLength);
    }
    else
    {
        // load uncompressed file data
        fread(address, 1, length, bigFP);
    }

    return length;
}

//
//  baseDirectory
//      directory to recursively scan for newer files than what's in the bigfile
//      "C:\homeworld\data", for example
//  directory
//      subdir to scan
//      "", or "Missions\SinglePlayer", for example
//  mainTOC
//      table of contents for the bigfile (for reading only)
//  updateTOC
//      table of contents for the update bigfile (for reading only) (or NULL)
//  mainNewerAvailable
//      array to be set (same number of entries as toc)
//      2 = newer file exists in filesystem
//      1 = older file exists in filesystem
//      0 = file doesn't exist in filesystem
//  updateNewerAvailable (or NULL)
//      array to be set (same number of entries as toc)
//      2 = newer file exists in filesystem
//      1 = older file exists in filesystem
//      0 = file doesn't exist in filesystem
//
//  NOTE:  Assumes the CRC64 name check is sufficient for uniqueness.
//  NOTE:  Start with newerAvailable array(s) set to all 0's.
//  NOTE:  It is assumed that the stored filenames in the bigfile will
//         not have the baseDirectory but will have the subdirectories
//         on their names.
//
void bigFilesystemCompare(char *baseDirectory, char *directory)
{
#ifdef _WIN32
    struct _finddata_t findData;
    sdword hFile;
#else
    struct stat file_stat;
    DIR *dp;
    struct dirent* dir_entry;
    char fullStatPath[PATH_MAX] = ""; 
#endif  // _WIN32
    char filespec[PATH_MAX];
    char subpath[PATH_MAX];
    udword fileNum;
    udword bigfile_i = 0;
    static udword compared = 0;
    static udword numOverriddenLocal[NUMBER_CONFIGURED_BIG_FILES] = {0,0,0};

    if (IgnoreBigfiles || !CompareBigfiles)
    {
        return;
    }
    
    if (!directory[0])
    {
        dbgMessagef("Scanning for newer files in %s", baseDirectory);
    }
    
#ifdef _WIN32

    sprintf(filespec, "%s\\%s%s*.*", baseDirectory, directory, directory[0] ? "\\" : "");
    hFile = _findfirst(filespec, &findData);
    if (hFile == -1)
    {
        return;
    }
    
    do
    {
        if (!strcmp(findData.name, ".") || !strcmp(findData.name, ".."))
        {
            continue;
        }
        
        sprintf(subpath, "%s%s%s", directory, directory[0] ? "\\" : "", findData.name);
        if (findData.attrib & _A_SUBDIR)
        {
            // recurse subdirectories
            bigFilesystemCompare(baseDirectory, subpath);
            --compared;
        }
        else
        {
            udword bigfile_i = 0;
            
            for (bigfile_i = 0; bigfile_i < NUMBER_CONFIGURED_BIG_FILES; ++bigfile_i)
            {
                if (bigTOCFileExists(&bigFilePrecedence[bigfile_i].tableOfContents,
                                     subpath, &fileNum))
                {
                    bigFilePrecedence[bigfile_i].localFileRelativeAge[fileNum]
                        = (bigFilePrecedence[bigfile_i].tableOfContents.fileEntries[fileNum].timeStamp < findData.time_write)
                        ? LOCAL_FILE_IS_NEWER
                        : LOCAL_FILE_IS_OLDER
                        ;
                        
                    numOverriddenLocal[bigfile_i]++;
                    
#ifdef HW_BUILD_FOR_DEBUGGING
                    if (bigFilePrecedence[bigfile_i].localFileRelativeAge[fileNum] == LOCAL_FILE_IS_NEWER)
                    {
                        dbgMessagef("    %s", subpath);
                    }
#endif
                }
            }
        }
        ++compared;
    } while (_findnext(hFile, &findData) == 0);

    _findclose(hFile);

#else  /* !_WIN32 */

    /* Form path name. */
    sprintf(filespec, "%s/%s/", baseDirectory, directory);
    filenameSlashMassage(filespec, TRUE);

    /* Open the directory. */
    dp = opendir(filespec);
    if (!dp)
    {
        return;
    }

    /* Read each directory entry. */
    while ((dir_entry = readdir(dp)))
    {
        if (dir_entry->d_name[0] == '.')
        {
            continue;
        }
        
        // the relative path that would be within the .big
        snprintf(subpath, PATH_MAX, "%s%s%s", directory, directory[0] ? "/" : "", dir_entry->d_name);
        
        // the relative path from the current directory to the actual file
        snprintf(fullStatPath, PATH_MAX, "%s/%s", baseDirectory, subpath);
        stat(fullStatPath, &file_stat);
        
        if (S_ISDIR(file_stat.st_mode))
        {
            // recurse subdirectories
            bigFilesystemCompare(baseDirectory, subpath);
            --compared;
        }
        else
        {
            udword bigfile_i = 0;
            
            for (bigfile_i = 0; bigfile_i < NUMBER_CONFIGURED_BIG_FILES; ++bigfile_i)
            {
                if (bigTOCFileExists(&bigFilePrecedence[bigfile_i].tableOfContents,
                                     subpath, &fileNum))
                {
                    bigFilePrecedence[bigfile_i].localFileRelativeAge[fileNum]
                        = (bigFilePrecedence[bigfile_i].tableOfContents.fileEntries[fileNum].timeStamp < file_stat.st_mtime)
                        ? LOCAL_FILE_IS_NEWER
                        : LOCAL_FILE_IS_OLDER
                        ;
                        
                    numOverriddenLocal[bigfile_i]++;
                    
#ifdef HW_BUILD_FOR_DEBUGGING
                    if (bigFilePrecedence[bigfile_i].localFileRelativeAge[fileNum] == LOCAL_FILE_IS_NEWER)
                    {
                        dbgMessagef("    %s", subpath);
                    }
#endif
                }
            }
        }
        
        ++compared;
    }

    closedir(dp);

#endif  // _WIN32

    if (!directory[0])
    {
        dbgMessagef("Compared %d filesystem files with %d .big files:",
                    compared, numOpenedBigFiles);
        
        for (bigfile_i = 0; bigfile_i < NUMBER_CONFIGURED_BIG_FILES; ++bigfile_i)
        {
            if (bigFilePrecedence[bigfile_i].filePtr != NULL)
            {
                dbgMessagef("%20s - %d newer files found on filesystem",
                    bigFilePrecedence[bigfile_i].bigFileName, numOverriddenLocal[bigfile_i]);
            }
        }
    }
}

bool bigFindFile(char *filename, bigFileConfiguration **whereFound, udword *fileIndex)
{
    udword bigfile_i = 0;
    
    if (IgnoreBigfiles)
    {
        return FALSE;
    }
    
    for (bigfile_i = 0; bigfile_i < NUMBER_CONFIGURED_BIG_FILES; ++bigfile_i)
    {
        if (bigTOCFileExists(&(bigFilePrecedence[bigfile_i].tableOfContents), filename, fileIndex))
        {
            if (bigFilePrecedence[bigfile_i].localFileRelativeAge[*fileIndex] != LOCAL_FILE_IS_NEWER)
            {
                *whereFound = &bigFilePrecedence[bigfile_i];
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

//
//  Close the main bigfile (and update bigfile, if present).
//
void bigCloseAllBigFiles(void)
{
    udword bigfile_i = 0;
    
    for (bigfile_i = 0; bigfile_i < NUMBER_CONFIGURED_BIG_FILES; ++bigfile_i)
    {
        if (bigFilePrecedence[bigfile_i].filePtr != NULL)
        {
            fclose(bigFilePrecedence[bigfile_i].filePtr);
        }
    }
}

//
//  bigCRC
//
//  Returns the 64-bit CRC of all open bigfile(s).
//
//  NOTE: call AFTER bigOpen
//
void bigCRC(udword *bigCRCArray, udword arraySize)
{
    udword  bigfile_i = 0,
            crcCount  = 1,
           *crcPtr    = bigCRCArray;
    
    for (bigfile_i = 0; bigfile_i < NUMBER_CONFIGURED_BIG_FILES; ++bigfile_i)
    {
        if (crcCount > arraySize)
        {
            break;
        }
        
        *(crcPtr++) = (bigFilePrecedence[bigfile_i].filePtr != NULL)
                    ? bigTOCCRC(&bigFilePrecedence[bigfile_i].tableOfContents)
                    : 0;

        crcCount++;
    }
}

#endif  // BF_HOMEWORLD
