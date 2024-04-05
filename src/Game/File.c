/*=============================================================================
    File.c: Functions for reading and saving files, including .BIG files.

    Created June 1997 by Luke Moloney
    Updated August 1998 by Darren Stone - added bigfile support
=============================================================================*/

#include <limits.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "BitIO.h"
#include "Debug.h"
#include "File.h"
#include "LZSS.h"
#include "Memory.h"

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <direct.h>
#else
    #include <dirent.h>
    #include <ctype.h>
#endif

#if defined _MSC_VER
    #define stat _stat
    #define S_ISDIR(mode) ((mode) & _S_IFDIR)
#endif


#ifdef HW_BUILD_FOR_DEBUGGING
    #define FILE_ERROR_CHECKING     1       // control error checking
    #define FILE_OPEN_LOGGING       1       // display file names as they are opened
    #define FILE_SEEK_WARNING       1       // display warnings as seeks are required
    #define FILE_TEST               0       // test the file module
    #define FILE_VERBOSE_LEVEL      1       // control level of verbose info
#else
    #define FILE_ERROR_CHECKING     0
    #define FILE_OPEN_LOGGING       0
    #define FILE_SEEK_WARNING       0
    #define FILE_TEST               0
    #define FILE_VERBOSE_LEVEL      0
#endif


struct stat fileStat;

// bigfile externs -- options and structures from bigfile.c
extern bool32 IgnoreBigfiles;
extern bool32 CompareBigfiles;
extern bool32 LogFileLoads;

/*=============================================================================
    Data:
=============================================================================*/

// filehandles are an index into this array
// (NOTE:  the first entry is wasted -- since a filehandle of 0
//  usually indicates an error, we don't want to use it.  And having
//  to add one to the index all the time seems like an invitation
//  for bugs.)
fileOpenInfo filesOpen[MAX_FILES_OPEN+1];


// where all the data files are kept (*.big, *.vce, *.wxe)
char fileHomeworldDataPath [PATH_MAX] = "";
 
// local directory mimicking the .big archive hierarchy and overrides
// the contents of those archives. (Mainly used for testing.)
char fileOverrideBigPath   [PATH_MAX] = "";

// the user's personal configuration/private file area
// (config/screenshots/savegames)
char fileUserSettingsPath  [PATH_MAX] = "";

// where the CD ROM drive is mounted
char fileCDROMPath         [PATH_MAX] = "";

// temporary path manipulation working area
char filePathTempBuffer    [PATH_MAX] = "";


//  space required for opening and reading from compressed streams within a bigfile
#define decompWorkspaceIncrement 65536;
static char  *decompWorkspaceP     = NULL;
static sdword decompWorkspaceSize  = 0;
static sdword decompWorkspaceInUse = FALSE;


/*=============================================================================
    Functions:
=============================================================================*/

/*-----------------------------------------------------------------------------
    Name        : fileNameReplaceSlashes
    Description : Replaces all backward slashes (\) in the file name with
                  forward slashes (/) for use with local filesystem
                  operations.
    Inputs      : fileName - Path and name of file.
    Outputs     : fileName - Modified file name.
    Return      : (None.)
    Note        : This is not needed for use with BIG files (slashes are
                  replaced as needed within BIG file functions).  Slashes are
                  also not condensed as with filenameSlashMassage() in
                  BigFile.c.

                  Windows is the only platform currently supported that does
                  not typically use forward slashes to separate components in
                  a path, but its file routines work just fine with forward
                  slashes.
----------------------------------------------------------------------------*/
void fileNameReplaceSlashesInPlace(char *filePath)
{
    char *ptr = NULL;

#ifdef WIN32
    #define PATH_DELIMITER     '\\'
    #define REPLACE_DELIMITER  '/'
#else
    #define PATH_DELIMITER     '/'
    #define REPLACE_DELIMITER  '\\'
#endif

    while ((ptr = strchr(filePath, REPLACE_DELIMITER)) != NULL)
    {
        *ptr = PATH_DELIMITER;
    }
}


/*-----------------------------------------------------------------------------
    Name        : fileNameReducePath
    Description : Removes extraneous slashes in the given path and reduces
                  occurences of "." and "..".
    Inputs      : pathName - File path.
    Return      : (None.)
----------------------------------------------------------------------------*/
static void fileNameReducePath (char* pathName)
{
    size_t pathLen;

    char* pathElem[PATH_MAX];
    udword pathElemCount;

    size_t searchLoc;
    bool8 isPathAbsolute;

    udword i;

    dbgAssertOrIgnore(pathName != NULL);

    pathLen = strlen(pathName);

    /* Reduce slashes. */
    for (i = 1; i < pathLen; i++)
    {
        char chPrev = pathName[i - 1];
        char chCurr = pathName[i];
        if ((chPrev == '/' || chPrev == '\\') &&
            (chCurr == '/' || chCurr == '\\'))
        {
            memmove(pathName + i - 1, pathName + i, pathLen - i + 1);
            pathLen--;
            i--;
        }
    }

    /* Remove trailing slashes. */
    while (pathName[pathLen - 1] == '/' ||
           pathName[pathLen - 1] == '\\')
    {
        pathLen--;
        pathName[pathLen] = '\0';
    }

    /* Split up the path string into an array containing each path
       component. */
    pathElem[0] = pathName;
    pathElemCount = 1;

    searchLoc = strcspn(pathName, "/\\");
    while (searchLoc != pathLen)
    {
        size_t searchLocPrev = searchLoc;

        pathName[searchLoc] = '\0';

        pathElem[pathElemCount] = pathName + searchLoc + 1;
        pathElemCount++;

        searchLoc = strcspn(pathName + searchLocPrev + 1, "/\\");
        searchLoc += searchLocPrev + 1;
    }

#ifdef _WIN32
    isPathAbsolute = (isalpha(pathName[0]) && pathName[1] == ':' &&
        pathName[2] == '\0');
#else
    isPathAbsolute = (pathName[0] == '\0');
#endif

    if (isPathAbsolute)
    {
        /* Skip over the root path element, we don't need to look at it for
           now. */
        pathElemCount--;
        if (pathElemCount != 0)
        {
            memmove(pathElem, pathElem + 1, sizeof(char*) * pathElemCount);
        }
    }

    /* Using our array of path components, reduce any occurences of "." and
       "..". */
    for (i = 0; i < pathElemCount; i++)
    {
        char* currentElem = pathElem[i];

        if (currentElem[0] != '.')
            continue;

        if (currentElem[1] == '\0')
        {
            /* The current path element refers to the current directory ("."),
               so we can remove it. */
            pathElemCount--;
            if (i < pathElemCount)
            {
                memmove(
                    pathElem + i, pathElem + i + 1,
                    sizeof(char*) * pathElemCount - i);
            }

            i--;

            continue;
        }

        if (!(currentElem[1] == '.' && currentElem[2] == '\0'))
            continue;

        /* The current path element refers to the parent directory (".."). */
        if (i == 0)
        {
            if (isPathAbsolute)
            {
                dbgMessagef("fileNameReducePath(): Attempted to reach a "
                            "parent directory of the root directory.");

                /* Attempting to reach a directory below the root directory, so
                   just get rid of the current element. */
                pathElemCount--;
                if (pathElemCount != 0)
                {
                    memmove(
                        pathElem, pathElem + 1,
                        sizeof(char*) * pathElemCount);
                }

                i--;
            }

            continue;
        }

        if (!strcmp(pathElem[i - 1], ".."))
        {
            /* Previous path element also represents the previous directory, so
               keep this element and allow further traversal up the directory
               tree. */
            continue;
        }

        /* Remove the current and previous path elements. */
        dbgAssertOrIgnore(pathElemCount >= 2);
        pathElemCount--;
        if (i < pathElemCount)
        {
            memmove(
                pathElem + i - 1, pathElem + i + 1,
                sizeof(char*) * pathElemCount - i);
        }

        pathElemCount--;
        i -= 2;
    }

    /* Rebuild the path string. */
    pathLen = 0;

    if (isPathAbsolute)
    {
#ifdef _WIN32
        pathName[2] = '/';
        pathLen = 3;
#else
        pathName[0] = '/';
        pathLen = 1;
#endif
    }

    if (pathElemCount == 0)
    {
        pathName[pathLen] = '\0';
        return;
    }

    for (i = 0; i < pathElemCount; i++)
    {
        char* currentElem = pathElem[i];
        size_t elemLen = strlen(currentElem);

        memmove(
            pathName + pathLen, currentElem, sizeof(char) * elemLen);

        pathLen += elemLen;
        pathName[pathLen] = '/';
        pathLen++;
    }

    /* Remove the trailing slash. */
    pathName[pathLen - 1] = '\0';
}


/*-----------------------------------------------------------------------------
    Name        : fileNameCorrectCase
    Description : Perform a case-insensitive search for the given file or
                  directory, modifying the path string to represent its case
                  on the filesystem.
    Inputs      : fileName - Path and name of file.
    Outputs     : fileName - The modified file name.  This will not be
                    modified if the file is not found.
    Return      : TRUE if the file was found, FALSE if not.
----------------------------------------------------------------------------*/
#if FILE_CASE_INSENSITIVE_SEARCH

static bool8 fileNameCorrectCase (char* fileName)
{
    char fileNameCopy[PATH_MAX + 1];
    char* pChar;
    udword pathComponentCount;
    bool8 isPathAbsolute;
    udword i;

    dbgAssertOrIgnore(fileName);

    /* Make a copy of the file name string to abuse as we wish. */
    dbgAssertOrIgnore(strlen(fileName) <= PATH_MAX);
    strncpy(fileNameCopy, fileName, PATH_MAX);
    fileNameCopy[PATH_MAX] = '\0';

    /* Compress any directory changes in the path so we can easily search for
       directory names while ignoring case. */
    fileNameReducePath(fileNameCopy);
    if (fileNameCopy[0] == '\0')
        return FALSE;

    /* Check if the file name is just a root directory reference. */
#ifdef _WIN32
    if (isalpha(fileNameCopy[0]) && strcmp(fileNameCopy + 1, ":/") == 0)
    {
        struct stat fileInfo;

        /* Make sure the drive specified exists. */
        if (stat(fileNameCopy, &fileInfo) != 0)
        {
            return FALSE;
        }

        dbgAssertOrIgnore(strlen(fileNameCopy) <= strlen(fileName));
        strcpy(fileName, fileNameCopy);

        return TRUE;
    }
#else
    if (strcmp(fileNameCopy, "/") == 0)
    {
        /* We should always have a root directory... */
        fileName[0] = '/';
        fileName[1] = '\0';

        return TRUE;
    }
#endif

    /* Split the path name components into separate strings. */
    pathComponentCount = 1;
    while ((pChar = strrchr(fileNameCopy, '/')))
    {
        *pChar = '\0';
        pathComponentCount++;
    }

    /* Find the first path component that is not a reference to the parent
       directory. */
    pChar = fileNameCopy;
    for (i = 0; i < pathComponentCount; i++)
    {
        if (strcmp(pChar, "..") != 0)
            break;

        pChar += strlen(pChar);

        /* Merge all parent directory references with the next path
           component. */
        if (i != pathComponentCount - 1)
        {
            *pChar = '/';
            pChar++;
        }
    }

    dbgAssertOrIgnore(i < pathComponentCount);
    if (i == pathComponentCount)
    {
        /* Apparently every element of the path is a parent directory
           reference.  Whatever...let's at least check if it exists. */
        struct stat fileInfo;

        dbgAssertOrIgnore(pathComponentCount == 1);

        if (stat(fileNameCopy, &fileInfo) != 0)
            return FALSE;

        /* The entry exists, so copy the reduced file name string into the
           original string. */
        dbgAssertOrIgnore(strlen(fileNameCopy) <= strlen(fileName));
        strcpy(fileName, fileNameCopy);

        return TRUE;
    }

    /* Check if the path name is an absolute path, merging the first two path
       components (the root directory reference and the first path component
       following it) if so. */
    isPathAbsolute = FALSE;
#ifdef _WIN32
    if (isalpha(fileNameCopy[0]) && strcmp(fileNameCopy + 1, ":") == 0)
    {
        fileNameCopy[2] = '/';

        dbgAssertOrIgnore(pathComponentCount > 1);
        pathComponentCount--;

        isPathAbsolute = TRUE;
    }
#else
    if (fileNameCopy[0] == '\0')
    {
        fileNameCopy[0] = '/';

        dbgAssertOrIgnore(pathComponentCount > 1);
        pathComponentCount--;

        isPathAbsolute = TRUE;
    }
#endif

    /* Now we start the search for the given file.  For each path component,
       we first do an exact check to see if it exists using the case given
       case.  If that fails, we then perform a case-insensitive comparison
       against each entry in the directory in which the path element resides.
       If a match is found, the file name of the matching entry is copied into
       the path string, and the search is continued with the next path
       component.  If a match is not found, the search is stopped, and we
       return a null pointer immediately. */
    for (i = 0; i < pathComponentCount; i++)
    {
#ifdef _WIN32
        char filespec[PATH_MAX + 1];
        struct _finddata_t findData;
        intptr_t hFile;
        bool32 foundResult;
#else
        struct dirent* pEntry;
        DIR* pDir;
#endif
        struct stat fileInfo;

        /* Check if the current entry exists in the given case. */
        if (stat(fileNameCopy, &fileInfo) != 0)
        {
            /* Temporarily remove the last path delimiter so we can separate
               the entry we're searching for from the directory in which it
               resides. */
            pChar = strrchr(fileNameCopy, '/');
            if (pChar)
            {
                if (i == 0 && isPathAbsolute)
                {
                    /* Special case for the first entry in an absolute path:
                       keep the trailing slash in the root directory name when
                       opening the root directory for searching. */
                    char tempChar;

                    pChar++;
                    tempChar = *pChar;
                    *pChar = '\0';

#ifdef _WIN32
                    filespec[PATH_MAX] = '\0';
                    strncpy(filespec, fileNameCopy, PATH_MAX);
                    strncat(filespec, "/*.*", PATH_MAX - strlen(filespec));
#else
                    pDir = opendir(fileNameCopy);
#endif

                    *pChar = tempChar;
                }
                else
                {
                    /* Remove the path delimiter and open the parent
                       directory. */
                    *pChar = '\0';
                    pChar++;

#ifdef _WIN32
                    filespec[PATH_MAX] = '\0';
                    strncpy(filespec, fileNameCopy, PATH_MAX);
                    strncat(filespec, "/*.*", PATH_MAX - strlen(filespec));
#else
                    pDir = opendir(fileNameCopy);
#endif
                }
            }
            else
            {
                /* The current entry we're searching for should be in the
                   current working directory. */
                pChar = fileNameCopy;

#ifdef _WIN32
                strcpy(filespec, "*.*");
#else
                pDir = opendir(".");
#endif
            }

#ifdef _WIN32
            /* Get the first entry in the directory. */
            hFile = _findfirst(filespec, &findData);
            if (hFile == -1)
                return FALSE;

            /* Search through each entry in the current directory, performing
               a case-insensitive check against the entry name. */
            foundResult = FALSE;
            do
            {
                if (_stricmp(pChar, findData.name) == 0)
                {
                    foundResult = TRUE;
                    break;
                }
            } while (_findnext(hFile, &findData) == 0);

            _findclose(hFile);

            /* Check if a match was found. */
            if (!foundResult)
                return FALSE;

            /* Copy over the entry name with the correct case. */
            strcpy(pChar, findData.name);
#else
            /* Make sure we could successfully open the directory. */
            if (!pDir)
                return FALSE;

            /* Search through each entry in the current directory, performing
               a case-insensitive check against the entry name. */
            while ((pEntry = readdir(pDir)))
            {
                if (strcasecmp(pChar, pEntry->d_name) == 0)
                    break;
            }

            /* Check if a match was found. */
            if (!pEntry)
            {
                closedir(pDir);
                return FALSE;
            }

            /* Copy over the entry name with the correct case. */
            strcpy(pChar, pEntry->d_name);
#endif

            /* Restore the full path up to the current entry. */
            if (pChar != fileNameCopy)
            {
                pChar--;
                *pChar = '/';
            }

#ifndef _WIN32
            closedir(pDir);
#endif
        }

        /* If this is not the last component of the path name, add a slash to
           the end of the current entry to form a full path to the next
           entry. */
        if (i != pathComponentCount - 1)
        {
            fileNameCopy[strlen(fileNameCopy)] = '/';
        }
    }

    /* Everything checks out with the file name, so copy the case-sensitive
       result back over into the original file name string. */
    dbgAssertOrIgnore(strlen(fileNameCopy) <= strlen(fileName));
    strcpy(fileName, fileNameCopy);

    return TRUE;
}

#else  /* FILE_CASE_INSENSITIVE_SEARCH */

/* If we're not manually performing case-insensitive file searches, merely
   make sure the file exists.  This should allow us to use this function even
   when we don't want to manually perform case-insensitive searches yet still
   give the same results on platforms that use case-insensitive file
   systems. */
static bool8 fileNameCorrectCase (char* fileName)
{
    char fileNameCopy[PATH_MAX + 1];
    struct stat fileInfo;

    dbgAssertOrIgnore(fileName);

    /* Make a copy of the file name string. */
    dbgAssertOrIgnore(strlen(fileName) <= PATH_MAX);
    strncpy(fileNameCopy, fileName, PATH_MAX);
    fileNameCopy[PATH_MAX] = '\0';

    /* Compress any directory changes in the path and convert all slashes to
       forward slashes. */
    fileNameReducePath(fileNameCopy);
    if (fileNameCopy[0] == '\0')
        return FALSE;

    /* Check if the file exists. */
    if (stat(fileNameCopy, &fileInfo) != 0)
        return FALSE;

    /* File exists, so copy the reduced path string into the original file
       name string. */
    dbgAssertOrIgnore(strlen(fileNameCopy) <= strlen(fileName));
    strcpy(fileName, fileNameCopy);

    return TRUE;
}

#endif  /* FILE_CASE_INSENSITIVE_SEARCH */


/*-----------------------------------------------------------------------------
    Name        : fileMakeDirectory
    Description : Creates the specified directory, creating parent directories
                  as necessary.
    Inputs      : directoryName - Name of the directory to create.
    Return      : TRUE if successful, FALSE if not.
----------------------------------------------------------------------------*/
bool8 fileMakeDirectory (const char* directoryName)
{
    char directoryCopy[PATH_MAX + 1];
    size_t directoryLen;

    char* pChar;

    dbgAssertOrIgnore(directoryName != NULL);
    dbgAssertOrIgnore(strlen(directoryName) <= PATH_MAX);

    /* Make a copy of the directory name with which we can modify as
       needed. */
    strncpy(directoryCopy, directoryName, PATH_MAX);
    directoryCopy[PATH_MAX] = '\0';

    /* Reduce the path name. */
    fileNameReducePath(directoryCopy);

    directoryLen = strlen(directoryCopy);
    if (directoryLen == 0)
        return TRUE;

    /* Add a trailing slash to our directory name. */
#ifdef _WIN32
    if (directoryCopy[directoryLen - 1] != '\\')
    {
        directoryCopy[directoryLen] = '\\';
        directoryLen++;
        directoryCopy[directoryLen] = '\0';
    }
#else
    if (directoryCopy[directoryLen - 1] != '/')
    {
        directoryCopy[directoryLen] = '/';
        directoryLen++;
        directoryCopy[directoryLen] = '\0';
    }
#endif

    /* Find the first path element that isn't the root directory or a parent
       directory delimiter. */
#ifdef _WIN32
    pChar = strchr(directoryCopy, '\\');
#else
    pChar = strchr(directoryCopy, '/');
#endif
    if (pChar)
    {
        *pChar = '\0';

#ifdef _WIN32
        if (isalpha(directoryCopy[0]) && directoryCopy[1] == ':' &&
            directoryCopy[2] == '\0')
#else
        if (directoryCopy[0] == '\0')
#endif
        {
#ifdef _WIN32
            *pChar = '\\';
            pChar = strchr(pChar + 1, '\\');
#else
            *pChar = '/';
            pChar = strchr(pChar + 1, '/');
#endif
            *pChar = '\0';
        }
    }

    /* Create each directory as needed. */
    while (pChar)
    {
        *pChar = 0;

        /* Check if the directory exists. */
        if (stat(directoryCopy, &fileStat) == 0)
        {
            /* A filesystem entry exists, so make sure it's a directory. */
            if (!S_ISDIR(fileStat.st_mode))
                return FALSE;
        }
        else
        {
            /* Attempt to create the directory. */
#if defined(_MSC_VER) || defined(__MINGW32__)
            if (mkdir(directoryCopy) == -1)
#else
            if (mkdir(directoryCopy, 0777) == -1)
#endif
                return FALSE;
        }
        /* Continue with the next path element. */
#ifdef _WIN32
        *pChar = '\\';
        pChar = strchr(pChar + 1, '\\');
#else
        *pChar = '/';
        pChar = strchr(pChar + 1, '/');
#endif
    }

    return TRUE;
}


/*-----------------------------------------------------------------------------
    Name        : fileMakeDestinationDirectory
    Description : Creates the directory in which the given file resides.
    Inputs      : fileName - path and name of file.
    Return      : TRUE if the directory exists or could be created, FALSE if
                  not.
----------------------------------------------------------------------------*/
bool8 fileMakeDestinationDirectory (const char* fileName)
{
    char directoryName[PATH_MAX + 1];
    char* pChar0;
    char* pChar1;

    dbgAssertOrIgnore(fileName);

    /* Make a copy of the directory name string, excluding the file itself. */
    strncpy(directoryName, fileName, PATH_MAX);
    directoryName[PATH_MAX] = '\0';

    pChar0 = strrchr(directoryName, '/');
    pChar1 = strrchr(directoryName, '\\');
    if (pChar1 > pChar0)
        pChar0 = pChar1;

    /* If the file is in the current directory, assume the directory
       exists. */
    if (!pChar0)
        return TRUE;

    /* Create the directory. */
    *pChar0 = '\0';

    return fileMakeDirectory(directoryName);
}


/*-----------------------------------------------------------------------------
    Name        : fileLoadAlloc
    Description : Loads specified file into memory after allocating memory for it.
    Inputs      : fileName - path and name of file to be loaded.
                  address - location to store the allocated pointer.
                  flags - NonVolatile, Pyrophoric or file access flags
    Outputs     : *address - pointer to newly allocated and loaded file.
    Return      : number of bytes allocated and loaded.
    Note        : Generates a fatal error if file doesn't exist.  If you don't
        like this behavior, call fileExists() first.
----------------------------------------------------------------------------*/
sdword fileLoadAlloc(char *_fileName, void **address, udword flags)
{
    char  *memoryName = NULL,
          *fileName   = NULL;
    FILE  *inFile     = NULL;
    
    bigFileConfiguration *whereFound = NULL;
    
    udword nameLength    = 0,
           length        = 0,
           lengthRead    = 0,
           bigFileIndex  = 0;
    sdword bigfileResult = 0;

    dbgAssertOrIgnore(address != NULL);

    //  try to load from bigfile
    if (!IgnoreBigfiles && !bitTest(flags, FF_CDROM|FF_IgnoreBIG|FF_UserSettingsPath))
    {
        if (bigFindFile(_fileName, &whereFound, &bigFileIndex))
        {
            bigfileResult = bigFileLoadAlloc(&(whereFound->tableOfContents), whereFound->filePtr, _fileName, bigFileIndex, address);
            if (bigfileResult != -1)
            {
                if (LogFileLoads)
                {
                    logfileLogf(FILELOADSLOG, "%s | %s\n", whereFound->bigFileName, _fileName);
                }
                
                return bigfileResult;
            }
        }
    }

    // filesystem load

    fileName = filePathPrepend(_fileName, flags);           //get full path
    fileNameCorrectCase(fileName);

    nameLength = strlen(fileName);                          //set memory name to the
    dbgAssertOrIgnore(nameLength > 1);                              //end of the filename if filename too long
    if (nameLength > MEM_NameLength)
    {
        memoryName = &fileName[nameLength - MEM_NameLength];
    }
    else
    {
        memoryName = fileName;
    }
    
    length = fileSizeGet(_fileName, flags);                 //get size of file
    dbgAssertOrIgnore(length > 0);                                  //and verify it
    *address = memAllocAttempt(length, memoryName, (flags & (NonVolatile | Pyrophoric)) | MBF_String);//allocate the memory for this file
    if (*address == NULL)                                   //if couldn't allocate enough
    {                                                       //generate a fatal error
        dbgFatalf(DBG_Loc, "fileLoadAlloc: couldn't allocate %d bytes for %s", length, fileName);
    }

    if ((inFile = fopen(fileName, "rb")) == NULL)           //open the file
    {
        dbgFatalf(DBG_Loc, "fileLoadAlloc: coundn't open file %s", fileName);
    }

    lengthRead = fread(*address, 1, length, inFile);        //read the file

#if FILE_ERROR_CHECKING
    if (lengthRead != length)                               //verify correct amount read
    {
        dbgFatalf(DBG_Loc, "fileLoadAlloc: expected %d bytes, read %d bytes", length, lengthRead);
    }
#endif

    fclose(inFile);

#if FILE_VERBOSE_LEVEL >= 2
    dbgMessagef("fileLoadAlloc: loaded %d bytes of '%s' to 0x%x from handle 0x%x", length, fileName, *address, inFile);
#endif

    if (LogFileLoads)
    {
        logfileLogf(FILELOADSLOG, "%-80s |       [F] |\n", _fileName);
    }

    return length;
}

/*-----------------------------------------------------------------------------
    Name        : fileLoad
    Description : Loads named file into specified address.
    Inputs      : fileName - path/name of file to load
                  address - address to load it to
                  flags - various load flags
    Outputs     : Reads the entire file into the buffer pointed to by address.
    Return      : number of bytes loaded
    Note        : Generates a fatal error if file doesn't exist.  If you don't
        like this behavior, call fileExists() first.
----------------------------------------------------------------------------*/
sdword fileLoad(char *_fileName, void *address, udword flags)
{
    char  *fileName   = NULL;
    FILE  *inFile     = NULL;
    
    bigFileConfiguration *whereFound = NULL;
    
    udword length        = 0,
           lengthRead    = 0,
           bigFileIndex  = 0;
    sdword bigfileResult = 0;

    //  try to load from bigfile
    if (!IgnoreBigfiles && !bitTest(flags, FF_CDROM|FF_IgnoreBIG|FF_UserSettingsPath))
    {
        if (bigFindFile(_fileName, &whereFound, &bigFileIndex))
        {
            bigfileResult = bigFileLoad(&(whereFound->tableOfContents), whereFound->filePtr, bigFileIndex, address);
            if (bigfileResult != -1)
            {
                if (LogFileLoads)
                {
                    logfileLogf(FILELOADSLOG, "%s | %s\n", whereFound->bigFileName, _fileName);
                }
                
                return bigfileResult;
            }
        }
    }

    // filesystem

    fileName = filePathPrepend(_fileName, flags);            //get full path
    fileNameCorrectCase(fileName);

    length = fileSizeGet(_fileName, flags);                  //get length of file

    if ((inFile = fopen(fileName, "rb")) == NULL)            //open the file
    {
        dbgFatalf(DBG_Loc, "fileLoadAlloc: couldn't open file %s", fileName);
    }
    lengthRead = fread(address, 1, length, inFile);         //read the file

#if FILE_ERROR_CHECKING
    if (lengthRead != length)                               //verify correct amount read
    {
        dbgFatalf(DBG_Loc, "fileLoadAlloc: expected %d bytes, read %d bytes", length, lengthRead);
    }
#endif
    fclose(inFile);

#if FILE_VERBOSE_LEVEL >= 2
    dbgMessagef("fileLoad: loaded %d bytes of '%s' to 0x%x", length, fileName, address);
#endif

    if (LogFileLoads)
    {
        logfileLogf(FILELOADSLOG, "%-80s |       [F] |\n", _fileName);
    }

    return(length);
}

/*-----------------------------------------------------------------------------
    Name        : fileSave
    Description : Saves data of length, address to fileName
    Inputs      : fileName, address, length
    Outputs     :
    Return      : length
----------------------------------------------------------------------------*/
sdword fileSave(char *_fileName, void *address, sdword length)
{
    FILE *outFile;
    char *fileName;
    sdword lengthWrote;

    fileName = filePathPrepend(_fileName, FF_UserSettingsPath);    //get full path
    fileNameCorrectCase(fileName);

    if (!fileMakeDestinationDirectory(fileName))
    {
        dbgFatalf(
            DBG_Loc,
            "fileSave: unable to create destination directory for file %s",
            fileName);
        return 0;
    }

    if ((outFile = fopen(fileName, "wb")) == NULL)           //open the file
    {
        dbgFatalf(DBG_Loc, "fileSave: couldn't open file %s", fileName);
    }

    lengthWrote = fwrite(address,1,length,outFile);

#if FILE_ERROR_CHECKING
    if (lengthWrote != length)                               //verify correct amount read
    {
        dbgFatalf(DBG_Loc, "fileSave: expected %d bytes, wrote %d bytes", length, lengthWrote);
    }
#endif

    fclose(outFile);

#if FILE_VERBOSE_LEVEL >= 2
    dbgMessagef("fileSave: saved %d bytes of '%s' to 0x%x", length, fileName, address);
#endif

    return lengthWrote;
}

/*-----------------------------------------------------------------------------
    Name        : fileExistsInBigFile
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
bool32 fileExistsInBigFile(char *fileName) {
    udword fileIndex = 0;
    bigFileConfiguration *whereFound = NULL;
    
    return bigFindFile(fileName, &whereFound, &fileIndex);
}

/*-----------------------------------------------------------------------------
    Name        : fileExists
    Description : Checks to see if a file exists
    Inputs      : fileName - name of file to find
                  flags - where to search (disk or in .BIG files)
    Outputs     : ..
    Return      : TRUE if file found, FALSE otherwise
----------------------------------------------------------------------------*/
bool32 fileExists(char *_fileName, udword flags) {
    char *fileName;

    if (!IgnoreBigfiles && !bitTest(flags, FF_CDROM|FF_IgnoreBIG|FF_UserSettingsPath))
    {
        if (fileExistsInBigFile(_fileName))
        {
            return TRUE;
        }
    }

    fileName = filePathPrepend(_fileName, flags);            //get full path

    if (fileNameCorrectCase(fileName))
    {
        return TRUE;
    }

    return FALSE;
}

/*-----------------------------------------------------------------------------
    Name        : fileSizeGet
    Description : Finds the length, in bytes, of a file.
    Inputs      : fileName - name of file to find the length of.
                  flags - where to search for file (dosk or in .BIG files)
    Outputs     : ..
    Return      : Length of file.  Generates a fatal error if file doesn't exist.
    Note        : Because this may actually open the file to find it's length,
        it should never be called on files which are already open.
----------------------------------------------------------------------------*/
sdword fileSizeGet(char *_fileName, udword flags)
{
    FILE *file;
    sdword length;
    char *fileName;

    if (!IgnoreBigfiles && !bitTest(flags, FF_CDROM|FF_IgnoreBIG|FF_UserSettingsPath))
    {
        bigFileConfiguration *whereFound = NULL;
        udword fileIndex = 0;
        
        if (bigFindFile(_fileName, &whereFound, &fileIndex))
        {
            length = (whereFound->tableOfContents.fileEntries + fileIndex)->realLength;

#if FILE_VERBOSE_LEVEL >= 3
            dbgMessagef("%s: '%s' is %d bytes in length (bigfile)", __func__, _fileName, length);
#endif
            return length;
        }
    }

    fileName = filePathPrepend(_fileName, flags);            //get full path
    fileNameCorrectCase(fileName);

    if ((file = fopen(fileName, "rb")) == NULL)              //open the file
    {
        dbgFatalf(DBG_Loc, "fileSizeGet: can't find file '%s'.", fileName);
    }
    length = fseek(file, 0, SEEK_END);                      //get length of file
    dbgAssertOrIgnore(length == 0);
    length = ftell(file);
    fclose(file);
#if FILE_VERBOSE_LEVEL >= 3
    dbgMessagef("fileSizeGet: '%s' is %d bytes in length", fileName, length);
#endif
    return(length);
}

void fileDelete(char *_fileName)
{
    char *fileName;

    fileName = filePathPrepend(_fileName, FF_IgnorePrepend);  // use unadulterated path
    fileNameReplaceSlashesInPlace(fileName);

    remove(fileName);
}

/*-----------------------------------------------------------------------------
    Name        : fileOpen
    Description : Open specified file for reading
    Inputs      : fileName - name of file to be opened
                  flags - flags controlling where to find it and how to open it.
    Outputs     : creates a fileOpenInfo for subsequent accesses
    Return      : returns a handle (which may be reference a FILE * or the bigfile)
 ----------------------------------------------------------------------------*/
filehandle fileOpen(char *_fileName, udword flags)
{
    FILE *file;
    char access[3];
    char *fileName;
    char localPath[PATH_MAX] = "";
    filehandle fh;
    bool32 usingBigfile = FALSE;
    bool32 firstBufUse = FALSE;
    bool32 localFileExists = FALSE;
    int expandedSize, storedSize;
    BitFile *bitFile;

    //  find next available filehandle
    fh = 1;
    while (fh <= MAX_FILES_OPEN)
    {
        if (!filesOpen[fh].inUse)
            break;
        ++fh;
    }
    if (fh > MAX_FILES_OPEN)
    {
        if (bitTest(flags, FF_ReturnNULLOnFail))
        {
            return 0;
        }
        dbgFatalf(DBG_Loc, "fileOpen: too many files open - cannot open file %s", _fileName);
    }

    strcpy(filesOpen[fh].path, _fileName);

    // if the filename starts with a slash it's a full path
    // which we should use as is
    if (_fileName[0] == '\\' || _fileName[0] == '/')
    {
        strcpy(localPath, _fileName);
    }
    // otherwise try a local filesystem version
    else
    {
        filePathPrepend(_fileName, FF_NoModifers);  // this ends up with the override path...
        strcpy(localPath, filePathTempBuffer);
    }
    
    // don't fiddle with the path any more - we should have an explicit full path
    // at this point; the question is does a file exist there?
    localFileExists = fileExists(localPath, FF_IgnorePrepend);

    //  try to load from bigfile
    if (!localFileExists && !IgnoreBigfiles && !bitTest(flags, FF_CDROM|FF_IgnoreBIG|FF_UserSettingsPath))
    {
        bigFileConfiguration *whereFound = NULL;
        udword fileIndex = 0;
        
        if (bigFindFile(_fileName, &whereFound, &fileIndex))
        {
            // no write support for bigfiles currently
            dbgAssertOrIgnore(!bitTest(flags, FF_AppendMode));
            dbgAssertOrIgnore(!bitTest(flags, FF_WriteMode));

            usingBigfile = TRUE;
            
            filesOpen[fh].usingBigfile  = TRUE;
            filesOpen[fh].bigFP         = whereFound->filePtr;
            filesOpen[fh].bigTOC        = &(whereFound->tableOfContents);
            filesOpen[fh].textMode      = bitTest(flags, FF_TextMode);
            filesOpen[fh].offsetStart   = (filesOpen[fh].bigTOC->fileEntries + fileIndex)->offset + (filesOpen[fh].bigTOC->fileEntries + fileIndex)->nameLength + 1;
            filesOpen[fh].offsetVirtual = 0;
            filesOpen[fh].length        = (filesOpen[fh].bigTOC->fileEntries + fileIndex)->realLength;
        }

        if (usingBigfile)  // common stuff, whether it's in the main or update bigfile
        {
            if ((filesOpen[fh].bigTOC->fileEntries + fileIndex)->compressionType)
            {
                // compressed file
                if (!decompWorkspaceInUse)
                {
                    if (decompWorkspaceSize < filesOpen[fh].length)  // workspace isn't big enough
                    {
                        // add a little extra room for the next possible reuse
                        // (an attempt to reduce too many memReallocs)
                        if (decompWorkspaceSize == 0)
                        {
                            firstBufUse = TRUE;
                        }
                        
                        decompWorkspaceSize = filesOpen[fh].length + decompWorkspaceIncrement;
                        decompWorkspaceP = (char *)memRealloc(decompWorkspaceP, decompWorkspaceSize,
                                                      "decompWorkspace", 0);
                        if (LogFileLoads)
                        {
                            logfileLogf(FILELOADSLOG, "%-80s", _fileName);

                            if (firstBufUse)
                            {
                                logfileLogf(FILELOADSLOG, "(decomp buffer created %dk)\n",
                                        decompWorkspaceSize/1024);
                            }
                            else
                            {
                                logfileLogf(FILELOADSLOG, "(decomp buffer increased to %dk)\n",
                                        decompWorkspaceSize/1024);
                            }
                        }
                    }
                    else  // reuse buffer
                    {
                        if (LogFileLoads)
                        {
                            logfileLogf(FILELOADSLOG, "%-80s", _fileName);
                            logfileLogf(FILELOADSLOG, "(decomp buffer reused)\n");
                        }
                    }
                    filesOpen[fh].decompBuf = decompWorkspaceP;
                    decompWorkspaceInUse = TRUE;
                }
                else // allocate a new, temporary buffer
                {
                    filesOpen[fh].decompBuf = (char *)memAlloc(filesOpen[fh].length, "decompBuf", 0);

                    if (LogFileLoads)
                    {
                        logfileLogf(FILELOADSLOG, "%-80s", _fileName);
                        logfileLogf(FILELOADSLOG, "(decomp buffer created %dk)\n",
                                    decompWorkspaceSize/1024);
                    }
                }
                // decompress from file directly into workspace
                fseek(filesOpen[fh].bigFP, filesOpen[fh].offsetStart, SEEK_SET);
                bitFile = bitioFileInputStart(filesOpen[fh].bigFP);
                expandedSize = lzssExpandFileToBuffer(bitFile, filesOpen[fh].decompBuf, filesOpen[fh].length);
                storedSize = bitioFileInputStop(bitFile);
                dbgAssertOrIgnore(expandedSize == filesOpen[fh].length);
                dbgAssertOrIgnore(storedSize == (filesOpen[fh].bigTOC->fileEntries + fileIndex)->storedLength);
            }
            else
            {
                filesOpen[fh].decompBuf = NULL;
                if (LogFileLoads)
                {
                    logfileLogf(FILELOADSLOG, "%-80s", _fileName);
                    logfileLogf(FILELOADSLOG, "(uncompressed file)\n");
                }
            }
#if FILE_VERBOSE_LEVEL >= 2
            dbgMessagef("fileOpen: '%s' (from bigfile) handle 0x%x", _fileName, fh);
#endif

            filesOpen[fh].inUse = TRUE;
            return fh;
        }
    }

    // resort to the good old disk filesystem

    fileName = filePathPrepend(_fileName, flags);            //get full path
    fileNameCorrectCase(fileName);

    if (bitTest(flags, FF_AppendMode))
    {
        access[0] = 'a';
    }
    else if (bitTest(flags, FF_WriteMode))
    {
        access[0] = 'w';
    }
    else
    {
        access[0] = 'r';
    }

#ifdef _WIN32
    /* Only open files in text mode on Windows.  Since text files are assumed
       to use DOS end-of-line markers, we will handle end-of-line conversions
       manually on other platforms. */
    if (bitTest(flags, FF_TextMode))
    {
        access[1] = 't';
    }
    else
#endif
    {
        access[1] = 'b';
    }

    access[2] = 0;

    if (LogFileLoads)
    {
        logfileLogf(FILELOADSLOG, "%-80s |       [F] |\n", _fileName);
    }

    if (bitTest(flags, FF_AppendMode | FF_WriteMode) &&
        !fileMakeDestinationDirectory(fileName))
    {
        dbgFatalf(
            DBG_Loc,
            "fileOpen: unable to create destination directory for file %s",
            fileName);
        return 0;
    }

    if ((file = fopen(fileName, access)) == NULL)
    {
        if (bitTest(flags, FF_ReturnNULLOnFail))
        {
            return 0;
        }
        dbgFatalf(DBG_Loc, "fileOpen: cannot open file %s", fileName);
    }
    
#if FILE_VERBOSE_LEVEL >= 2
    dbgMessagef("fileOpen: '%s' (from filesystem) handle 0x%x, FILE *0x%x", fileName, fh, file);
#endif

    filesOpen[fh].inUse = TRUE;
    filesOpen[fh].fileP = file;
    filesOpen[fh].usingBigfile = FALSE;
    filesOpen[fh].textMode = bitTest(flags, FF_TextMode);

    return fh;
}

/*-----------------------------------------------------------------------------
    Name        : fileClose
    Description : Closes a previously open file.
    Inputs      : handle - handle to the file to be closed
    Outputs     :
    Return      : void
    Note        : generates an error if the file not open and debugging enabled
----------------------------------------------------------------------------*/
void fileClose(filehandle handle)
{
    dbgAssertOrIgnore(handle);
    dbgAssertOrIgnore(filesOpen[handle].inUse);

    if (!filesOpen[handle].usingBigfile)
        fclose(filesOpen[handle].fileP);
    else if (filesOpen[handle].decompBuf)
    {
        if (filesOpen[handle].decompBuf == decompWorkspaceP)
            decompWorkspaceInUse = FALSE;
        else
            // free decompression buffer if it was in use and not the stock workspace
            // (ie, we must have allocated it during fileOpen())
            memFree(filesOpen[handle].decompBuf);
    }

    filesOpen[handle].inUse = FALSE;

#if FILE_VERBOSE_LEVEL >= 2
    dbgMessagef("fileClose: handle 0x%x", handle);
#endif
}

/*-----------------------------------------------------------------------------
    Name        : fileSeek
    Description : Seek to a new location in specified file.
    Inputs      : handle - handle to open file
                  offset - of new location
                  whence - where to seek from
    Outputs     : ..
    Return      : new location
    Note        :
----------------------------------------------------------------------------*/
sdword fileSeek(filehandle handle, sdword offset, sdword whence)
{
    sdword newLocation = 0;

    // dbgAssertOrIgnore(offset >= 0);
    dbgAssertOrIgnore(handle);
    dbgAssertOrIgnore(filesOpen[handle].inUse);

    /* We can seek to the start of a text file, but that's it. */
    dbgAssertOrIgnore(!filesOpen[handle].textMode || (whence == FS_Start && !offset));

    if (filesOpen[handle].usingBigfile)
    {
        switch (whence)
        {
            case FS_Start:
                newLocation =
                    (filesOpen[handle].offsetVirtual = offset);
                break;
            case FS_Current:
                newLocation =
                    (filesOpen[handle].offsetVirtual += offset);
                break;
            case FS_End:
                newLocation =
                    (filesOpen[handle].offsetVirtual = filesOpen[handle].length - 1 + offset);
                break;
        }
        dbgAssertOrIgnore(newLocation < filesOpen[handle].length);
    }
    else  // filesystem
    {
        newLocation = fseek(filesOpen[handle].fileP, offset, whence);
        dbgAssertOrIgnore(newLocation == 0);
        newLocation = ftell(filesOpen[handle].fileP);
    }

#if FILE_VERBOSE_LEVEL >= 3
    dbgMessagef("fileSeek: handle 0x%x seeked %d bytes (mode %d) to %d", handle, offset, whence, newLocation);
#endif

    return newLocation;
}

/*-----------------------------------------------------------------------------
    Name        : fileBlockRead
    Description : Reads a block from the specified file.
    Inputs      : handle - handle to open file
                  dest - buffer to read the data to
                  nBytes - number of bytes to read, in bytes
    Outputs     : fills the dest buffer with nBytes read from handle
    Return      : number of bytes read.  If debugging enabled, we compare this
        to the amount requested and generate a fatal error if there is a difference.
----------------------------------------------------------------------------*/
sdword fileBlockRead(filehandle handle, void *dest, sdword nBytes)
{
    sdword lengthRead;

    dbgAssertOrIgnore(handle);
    dbgAssertOrIgnore(filesOpen[handle].inUse);
    dbgAssertOrIgnore(!filesOpen[handle].textMode);

    if (filesOpen[handle].usingBigfile)
    {
        dbgAssertOrIgnore(filesOpen[handle].offsetVirtual + nBytes <= filesOpen[handle].length);
        if (filesOpen[handle].decompBuf)
        {
            // from memory buffer
            memcpy(dest, filesOpen[handle].decompBuf + filesOpen[handle].offsetVirtual, nBytes);
            lengthRead = nBytes;
        }
        else
        {
            // from bigfile
            fseek(filesOpen[handle].bigFP, filesOpen[handle].offsetVirtual + filesOpen[handle].offsetStart, SEEK_SET);
            lengthRead = fread(dest, 1, nBytes, filesOpen[handle].bigFP);    //read in the data
        }
        filesOpen[handle].offsetVirtual += lengthRead;
    }
    else
    {
        lengthRead = fread(dest, 1, nBytes, filesOpen[handle].fileP);    //read in the data
    }

#ifndef _MACOSX_FIX_MISC  // zero-length .wxd since you can't play the original
#if FILE_ERROR_CHECKING
    if (lengthRead != nBytes)
    {                                                       //make sure it was all read in
        dbgFatalf(DBG_Loc, "fileBlockRead: expected %d bytes, read %d", nBytes, lengthRead);
    }
#endif
#endif

#if FILE_VERBOSE_LEVEL >= 3
    dbgMessagef("fileBlockRead: handle 0x%x read %d bytes to 0x%x", handle, lengthRead, dest);
#endif

    return lengthRead;
}

/*-----------------------------------------------------------------------------
    Name        : fileBlockReadNoError
    Description : Reads a block from the specified file.
    Inputs      : handle - handle to open file
                  dest - buffer to read the data to
                  nBytes - number of bytes to read, in bytes
    Outputs     : fills the dest buffer with nBytes read from handle
    Return      : number of bytes read.  This function will NOT generate an error
                  if fewer than expected bytes are read
----------------------------------------------------------------------------*/
sdword fileBlockReadNoError(filehandle handle, void *dest, sdword nBytes)
{
    sdword lengthRead, ReadAmt = nBytes;

    dbgAssertOrIgnore(handle);
    dbgAssertOrIgnore(filesOpen[handle].inUse);
    dbgAssertOrIgnore(!filesOpen[handle].textMode);

    if (filesOpen[handle].usingBigfile)
    {
        if(filesOpen[handle].offsetVirtual + nBytes > filesOpen[handle].length)
            ReadAmt = filesOpen[handle].length - filesOpen[handle].offsetVirtual;

        if (filesOpen[handle].decompBuf)
        {
            // from memory buffer
            memcpy(dest, filesOpen[handle].decompBuf + filesOpen[handle].offsetVirtual, ReadAmt);
            lengthRead = ReadAmt;
        }
        else
        {
            // from bigfile
            fseek(filesOpen[handle].bigFP, filesOpen[handle].offsetVirtual + filesOpen[handle].offsetStart, SEEK_SET);
            lengthRead = fread(dest, 1, ReadAmt, filesOpen[handle].bigFP);    //read in the data
        }
        filesOpen[handle].offsetVirtual += lengthRead;
    }
    else
    {
        lengthRead = fread(dest, 1, nBytes, filesOpen[handle].fileP);    //read in the data
    }

    return lengthRead;
}

/*-----------------------------------------------------------------------------
    Name        : fileLineRead
    Description : Reads a line from the specified file.
    Inputs      : handle - handle of open file
                  dest - destination string
                  nChars - length of string (dest)
    Outputs     : Reads up to nChars bytes from file converting CF/LF pairs to
        just a CR.  Removed LF characters are not counted in nChars
    Return      : Number of characters read or FR_EndOfFile if the end of file
                    reached.  Note that a zero return value reflects empty lines.
----------------------------------------------------------------------------*/
sdword fileLineRead(filehandle handle, char *dest, sdword nChars)
{
    sdword length;
    int ch;
    char *loc;
#ifdef _WIN32
    char *retVal;
#endif

    dbgAssertOrIgnore(nChars > 0);                                  //validate the params
    dbgAssertOrIgnore(handle);
    dbgAssertOrIgnore(dest != NULL);
    dbgAssertOrIgnore(filesOpen[handle].inUse);
    dbgAssertOrIgnore(filesOpen[handle].textMode);

    if (filesOpen[handle].usingBigfile)
    {
        if (filesOpen[handle].offsetVirtual >= filesOpen[handle].length)
            return FR_EndOfFile;

        if (filesOpen[handle].decompBuf)
        {
            // fake the filesystem fgets stuff on a mem buffer
            length = 0;
            loc = (char *)filesOpen[handle].decompBuf + filesOpen[handle].offsetVirtual;
            while (length < nChars-1)
            {
                if (filesOpen[handle].offsetVirtual >= filesOpen[handle].length)
                    break;

                // getc
                ch = *(loc++);
                ++filesOpen[handle].offsetVirtual;

                /* Check if we have reached the end of the line. */
                if (ch == '\n')
                    break;

                if (ch == '\r')
                {
                    /* Remove the "\n" character if this is a DOS
                       end-of-line. */
                    // getc
                    ch = *(loc++);
                    ++filesOpen[handle].offsetVirtual;
                    if (ch != '\n')
                    {
                        // ungetc
                        --loc;
                        --filesOpen[handle].offsetVirtual;
                    }

                    break;
                }

                /* Add the character to the end of the string. */
                *(dest+length) = ch;
                ++length;
            }

            *(dest+length) = 0;
        }
        else
        {
            fseek(filesOpen[handle].bigFP, filesOpen[handle].offsetVirtual + filesOpen[handle].offsetStart, SEEK_SET);
            // fake the filesystem fgets stuff (like below) here in binary mode
            length = 0;
            while (length < nChars-1)
            {
                if (feof(filesOpen[handle].bigFP) || filesOpen[handle].offsetVirtual >= filesOpen[handle].length)
                    break;

                /* Get the next character from the file. */
                ch = getc(filesOpen[handle].bigFP);
                if (ch == EOF)
                    break;

                ++filesOpen[handle].offsetVirtual;

                /* Check if we have reached the end of the line. */
                if (ch == '\n')
                    break;

                if (ch == '\r')
                {
                    /* Remove the "\n" character if this is a DOS
                       end-of-line. */
                    ch = getc(filesOpen[handle].bigFP);
                    if (ch != '\n')
                        ungetc(ch, filesOpen[handle].bigFP);
                    else
                        ++filesOpen[handle].offsetVirtual;

                    break;
                }

                /* Add the character to the end of the string. */
                *(dest+length) = ch;
                ++length;
            }
            *(dest+length) = 0;
        }
    }
    else
    {
        if (feof(filesOpen[handle].fileP))                      //check if end of file
        {
            return FR_EndOfFile;
        }

#ifdef _WIN32
        /* Read a line from the file. */
        retVal = fgets(dest, nChars, filesOpen[handle].fileP);
        if (retVal != dest)
        {
            return(FR_EndOfFile);
        }

        length = strlen(dest);                                  //get length of string

        if (length)                                             //if not a blank line
        {
            if (dest[length - 1] == '\n')                       //if newline at end of line
            {
                dest[length - 1] = 0;                           //kill newline character
                length--;                                       //one less character
            }
        }
#else
        /* Read a line from the file, manually checking for an end-of-line
           marker. */
        length = 0;
        while (length < nChars - 1)
        {
            /* Get the next character from the file. */
            ch = getc(filesOpen[handle].fileP);
            if (ch == EOF)
                break;

            /* Check if we have reached the end of the line. */
            if (ch == '\n')
                break;

            if (ch == '\r')
            {
                /* Remove the "\n" character if this is a DOS end-of-line. */
                ch = getc(filesOpen[handle].fileP);
                if (ch != '\n')
                    ungetc(ch, filesOpen[handle].fileP);

                break;
            }

            /* Add the character to the end of the string. */
            dest[length] = ch;
            length++;
        }

        dest[length] = '\0';
#endif
    }

#if FILE_VERBOSE_LEVEL >= 3
    dbgMessagef("fileLineRead: handle 0x%x read %d chars to 0x%x ('%s')", handle, length, dest, dest);
#endif

    return length;
}

/*-----------------------------------------------------------------------------
    Name        : fileCharRead
    Description : Read a character from a file.
    Inputs      : handle - handle of open file.
    Outputs     : ..
    Return      : Character read from file.
----------------------------------------------------------------------------*/
sdword fileCharRead(filehandle handle)
{
    sdword ch;

    dbgAssertOrIgnore(handle);
    dbgAssertOrIgnore(filesOpen[handle].inUse);

    if (filesOpen[handle].usingBigfile)
    {
        if (filesOpen[handle].offsetVirtual >= filesOpen[handle].length)
            return EOF;

        if (filesOpen[handle].decompBuf)
        {
            ch = filesOpen[handle].decompBuf[filesOpen[handle].offsetVirtual++];

            /* If reading a text file, check for a new line character. */
            if (filesOpen[handle].textMode && ch == '\r')
            {
                /* Remove the "\n" character if this is a DOS end-of-line. */
                ch = filesOpen[handle].decompBuf[filesOpen[handle].offsetVirtual++];
                if (ch != '\n')
                    filesOpen[handle].offsetVirtual--;

                /* Force the character to be a newline character in case this
                   is a Mac end-of-line. */
                ch = '\n';
            }
        }
        else
        {
            fseek(filesOpen[handle].bigFP, filesOpen[handle].offsetVirtual + filesOpen[handle].offsetStart, SEEK_SET);
            ch = fgetc(filesOpen[handle].bigFP);
            if (ch != EOF)
            {
                ++filesOpen[handle].offsetVirtual;

                /* If reading a text file, check for a new line character. */
                if (filesOpen[handle].textMode && ch == '\r')
                {
                    /* Remove the "\n" character if this is a DOS
                       end-of-line. */
                    ch = fgetc(filesOpen[handle].bigFP);
                    if (ch != '\n')
                        ungetc(ch, filesOpen[handle].bigFP);
                    else
                        filesOpen[handle].offsetVirtual++;

                    /* Force the character to be a newline character in case
                       this is a Mac end-of-line. */
                    ch = '\n';
                }
            }
        }

        return ch;
    }

    /* Read the next character from the local filesystem stream. */
#ifdef _WIN32
    return fgetc(filesOpen[handle].fileP);
#else
    ch = fgetc(filesOpen[handle].fileP);
    if (!filesOpen[handle].textMode || ch != '\r')
        return ch;

    /* Handle possible DOS or Mac line ends. */
    ch = fgetc(filesOpen[handle].fileP);
    if (ch != '\n')
        ungetc(ch, filesOpen[handle].fileP);

    /* Force the character to be a newline character in case this is a Mac
       end-of-line. */
    return '\n';
#endif
}


//
// for using regular underlying i/o stream (filesystem, not bigfile)
//
sdword fileUsingBigfile(filehandle handle)
{
    dbgAssertOrIgnore(handle);
    dbgAssertOrIgnore(filesOpen[handle].inUse);
    return filesOpen[handle].usingBigfile;
}


//
// for using regular underlying i/o stream (for files of the filesystem, not files of a bigfile)
//
FILE *fileStream(filehandle handle)
{
    dbgAssertOrIgnore(handle);
    dbgAssertOrIgnore(filesOpen[handle].inUse);
    return filesOpen[handle].fileP;
}


/*-----------------------------------------------------------------------------
    Name        : filePathBufferSet
    Description : populates preallocated buffer with given path and ensures
                  it is properly formatted/terminated. Buffer should ideally
                  be PATH_MAX bytes long.
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void filePathMaxBufferSet(char *buffer, char *path)
{
    unsigned int path_len = 0;
    
    dbgAssertOrIgnore(buffer != NULL);
    dbgAssertOrIgnore(path   != NULL);
    
    path_len = strlen(path);

    dbgAssertOrIgnore(path_len < PATH_MAX);

    strncpy(buffer, path, PATH_MAX);
    
    // make sure path is delimited
    strcat(buffer, "/");
    
    fileNameReplaceSlashesInPlace(buffer);
}


/*-----------------------------------------------------------------------------
    Name        : filePathPrepend
    Description : Prepend the default path for opening files.
    Inputs      : fileName - file name/relative path to add to end of default path
                  flags    - flags to set the root directory we want
    Outputs     : Copies prepend path to global variable and concatenates fileName
    Return      : pointer to new full path
----------------------------------------------------------------------------*/
char *filePathPrepend(char *fileName, udword flags)
{
    dbgAssertOrIgnore(fileName != NULL);

    if (bitTest(flags, FF_IgnorePrepend))
    {
        strcpy(filePathTempBuffer, "");
    }
    else if (bitTest(flags, FF_HomeworldDataPath))
    {
        strcpy(filePathTempBuffer, fileHomeworldDataPath);
    }
    else if (bitTest(flags, FF_UserSettingsPath))
    {
        strcpy(filePathTempBuffer, fileUserSettingsPath);
    }
    else if (bitTest(flags, FF_CDROM))
    {
        strcpy(filePathTempBuffer, fileCDROMPath);
    }
    else
    {
        strcpy(filePathTempBuffer, fileOverrideBigPath);
    }

    // Fixup double backslash paths
    int length = strlen(filePathTempBuffer);
    if (length >= 2)
    {
        if((filePathTempBuffer[length - 1] == '\\') && (filePathTempBuffer[length - 2] == '\\'))
        {
            filePathTempBuffer[length - 1] = 0;
        }
    }

    strcat(filePathTempBuffer, fileName);

    return filePathTempBuffer;
}

bool32 fileCDROMPathSet(char *path) {
#ifdef _WIN32
    char message[80];

    if (GetDriveType(path) != DRIVE_CDROM)
    {
        sprintf(message, "'%s' Is not a valid CD-ROM; path ignored.", path);
        MessageBox(NULL, message, "Invalid CD-ROM path", MB_OK | MB_APPLMODAL);
        return FALSE;
    }
#endif
    filePathMaxBufferSet(fileCDROMPath, path);
    return TRUE;
}

void fileHomeworldDataPathSet(char *path)
{
    filePathMaxBufferSet(fileHomeworldDataPath, path);
}

bool32 fileOverrideBigPathSet(char *path) {
    filePathMaxBufferSet(fileOverrideBigPath, path);
    return TRUE;
}

bool32 fileUserSettingsPathSet(char *path) {
    filePathMaxBufferSet(fileUserSettingsPath, path);
    return TRUE;
}

//
//  How to interpret the LOGFILELOADS output log:
//
//  3 columns:  main_bigfile  update_bigfile  filesystem
//  M = main bigfile
//  U = update bigfile
//  F = filesystem
//  Each column may be blank (indicating the file was not present there),
//  or have a lowercase letter there (m/u/f) (indicating that it had an
//  old or unused version of the file there), or have an uppercase letter
//  there ([M]/[U]/[F]) (indicating that the file there was used for the
//  load).
//
//  WARNING: If you run with comparebigfiles OFF, it could invalidate some
//  of the conclusions that this output draws.
//

/*-----------------------------------------------------------------------------
    Name        : logfileClear
    Description : clears the logfile
    Inputs      : logfile
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void logfileClear(char *logfile)
{
    FILE *file = NULL;

    if ((file = fopen(logfile, "wt")) != NULL)      // open and close to make it 0 size
    {
        fclose(file);
    }
}

/*-----------------------------------------------------------------------------
    Name        : logFileLog
    Description : logs a log into the logfile
    Inputs      : logfile, str
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void logfileLog(char *logfile, char *str)
{
    FILE *file = NULL;

    if ((file = fopen(logfile, "at")) != NULL)
    {
        fprintf(file, "%s", str);
        fclose(file);
    }
}

/*-----------------------------------------------------------------------------
    Name        : logfileLogf
    Description : logs a log into the logfile, variable parameters
    Inputs      : logfile, str, ...
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void logfileLogf(char *logfile, char *format, ...)
{
    char buffer[200];
    va_list argList;
    va_start(argList, format);                  //get first arg
    vsprintf(buffer, format, argList);          //prepare output string
    va_end(argList);

    logfileLog(logfile,buffer);
}
