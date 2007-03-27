// =============================================================================
//  standard_library.h
// =============================================================================
//  It may be called the "Standard Library" but that's not to say it's in a
//  standard location or file on a particular platform! This header attempts
//  to put all those platform-specific "#include <*>" tweaks in one place.
// =============================================================================

// filesystem directory traversal

#ifdef __MINGW32__
    #include <direct.h>
#elif defined(_MSC_VER)
    #include <direct.h>
    #include <io.h>
#else
    #include <sys/stat.h>
    #include <dirent.h>
#endif

