#include "Types.h"
//#include "NIS.h"
//

#define NIS_HeaderExtra             64          //extra amount of RAM at the end of a header

typedef struct
{
    real32 time;                                //time of the event
    udword code;  //nisopcode                           //what event it is
    sdword shipID;                              //index of the object this event works on, not always a ship
    sdword initialID;                           //index of event in the .script file for correctly time-sorting events
    sdword param[2];                            //parameters, event-specific
}
nisevent;



typedef memsize ShipType;

typedef struct
{
    char identifier[12];                        //NIS_Identifier
    udword version;                             //NIS_VersionNumber
    udword flags;                               //general flags (dynamic)
    udword stringBlock;   //char*                       //pointer to string block
    sdword stringBlockLength;                   //length of the string block
    real32 length;                              //length (seconds) of NIS
    real32 loop;                                //loop point for NIS (or negative for no loop)
    sdword nObjectPaths;                        //number of motion paths in the file
    udword objectPath; //spaceobjpath*    //pointer to list of motion paths
    sdword nCameraPaths;                        //number of camera motion paths
    udword cameraPath; //camerapath*
    sdword nLightPaths;                         //number of light motion paths
    udword lightPath; //lightpath*
    sdword nEvents;                             //number of events, NULL for now
    udword events;  // nisevent*                         //void * because not initially used
    //after here there are NIS_HeaderExtra bytes of data
    //to be used by loaded data.  It is not stored in the file.
    sdword nGenericObjects;                     //number of different generic objects loaded
    udword genericObject; //nisgenericobject**          //registry of genric objects loaded for this NIS
    sdword iLookyObject;                        //the official "mid point of 2-phase focus zoom look at point" object
    ubyte extra[NIS_HeaderExtra - 12];          //data for future expansion
}
nisheader_disk;

typedef struct spaceobjpath_disk
{
    sdword instance;                            //instance of this type object
    sdword type;                              //type of ship
    sdword parentIndex;                         //parent ship (NULL if none)
    udword race;                                //0..5 (see racedefs.h).  For determining what exact ship to use.
    sdword nSamples;                            //number of samples, including 2 extra at start and end
    real32 timeOffset;                          //if positive, path will wait this long before starting
    udword times;  //real32*                            //absolute times, from start of NIS (timeOffset will be added in)
    udword parameters;   //tcb*                         //tension, continuity, bias
    udword curve[6];    //real32*                       //x,y,z,h,p,b
}
spaceobjpath_disk;

typedef struct tagcamerapath_disk
{
    udword oLength;                             //length (seconds) of motion path
    sdword nSamples;                            //number of samples, including 2 extra at start and end
    real32 timeOffset;                          //if positive, path will wait this long before starting
    udword times; //real32*                             //absolute times, from start of NIS (timeOffset will be added in)
    udword parameters;  //tcb*                          //tension, continuity, bias
    udword curve[6];  //real32*                         //same parameters as the spaceobj's
} camerapath_disk;



typedef spaceobjpath_disk lightpath_disk;



typedef struct spaceobjpath
{
    sdword instance;                            //instance of this type object
    ShipType type;                              //type of ship
    ShipType parentIndex;                         //parent ship (NULL if none)
    udword race;                                //0..5 (see racedefs.h).  For determining what exact ship to use.
    sdword nSamples;                            //number of samples, including 2 extra at start and end
    real32 timeOffset;                          //if positive, path will wait this long before starting
    real32 *times;                              //absolute times, from start of NIS (timeOffset will be added in)
    tcb *parameters;                            //tension, continuity, bias
    real32 *curve[6];                           //x,y,z,h,p,b
}
spaceobjpath;


typedef struct tagcamerapath
{
    udword oLength;                             //length (seconds) of motion path
    sdword nSamples;                            //number of samples, including 2 extra at start and end
    real32 timeOffset;                          //if positive, path will wait this long before starting
    real32 *times;                              //absolute times, from start of NIS (timeOffset will be added in)
    tcb *parameters;                            //tension, continuity, bias
    real32 *curve[6];                           //same parameters as the spaceobj's
} camerapath;

typedef spaceobjpath lightpath;



typedef struct
{
    char identifier[12];                        //NIS_Identifier
    udword version;                             //NIS_VersionNumber
    udword flags;                               //general flags (dynamic)
    char *stringBlock;                          //pointer to string block
    sdword stringBlockLength;                   //length of the string block
    real32 length;                              //length (seconds) of NIS
    real32 loop;                                //loop point for NIS (or negative for no loop)
    sdword nObjectPaths;                        //number of motion paths in the file
    spaceobjpath *objectPath;                   //pointer to list of motion paths
    sdword nCameraPaths;                        //number of camera motion paths
    camerapath *cameraPath;
    sdword nLightPaths;                         //number of light motion paths
    lightpath *lightPath;
    sdword nEvents;                             //number of events, NULL for now
    nisevent *events;                           //void * because not initially used
    sdword nGenericObjects;                     //number of different generic objects loaded
    udword **genericObject; //nisgenericobject**        //registry of genric objects loaded for this NIS
    sdword iLookyObject;                        //the official "mid point of 2-phase focus zoom look at point" object
    ubyte extra[NIS_HeaderExtra - 12];          //data for future expansion
}
nisheader;


