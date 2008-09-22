#include "Types.h"

typedef struct tagGeoFileHeader_disk
{
    char    identifier[MESH_FileIDLength];  // File identifier.
    udword  version;                        // File version.
    udword   pName;   //    *               // Offset to a file name.
    udword  fileSize;                       // File size (in bytes), not counting this header.
    udword  localSize;                      // Object size (in bytes).
    udword  nPublicMaterials;               // Number of public materials.
    udword  nLocalMaterials;                // Number of local materials.
    udword  oPublicMaterial;                //list of public materials
    udword  oLocalMaterial;                 //list of local materials
    udword  nPolygonObjects;                // Number of polygon objects.
    ubyte   reserved[24];                   // Reserved for future use.
}
GeoFileHeader_disk;


typedef struct polygonobject_disk
{
    udword  pName;    // *                        // Name for animation.
    ubyte   flags;                          // General flags (see above)
    ubyte   iObject;                        // fixed up at load time so we know what object index we have when recursively processing
    uword   nameCRC;                        // 16-bit CRC of name (!!!!no room for 32 right now - make room next version)
    sdword  nVertices;                      // Number of vertices in vertex list for this object.
    sdword  nFaceNormals;                   // Number of face normals for this object.
    sdword  nVertexNormals;                 // Number of vertex normals for this object.
    sdword  nPolygons;                      // Number of polygons in this object.
    udword pVertexList; //vertexentry *pVertexList;     // Offset to the vertex list in this object.
    udword pNormalList;//normalentry *pNormalList;     // Offset to the normal list in this object.
    udword pPolygonList; //polyentry   *pPolygonList;   // Offset to the polygon list in this object.
    udword pMother; // struct polygonobject *pMother;          // link to parent object
    udword pDaughter; //struct polygonobject *pDaughter;        // link to child object
    udword pSister; //struct polygonobject *pSister;          // link to sibling object
    hmatrix localMatrix;
}
polygonobject_disk;


typedef struct
{
    char *name;
    udword localSize;                       //size required for each instance of this mesh
    sdword nPublicMaterials;                //number of shared materials
    sdword nLocalMaterials;                 //number of local materials
    sdword nPolygonObjects;                 //number of polygon objects in mesh file
    struct tagmaterialentry *localMaterial; //list of local materials
    struct tagmaterialentry *publicMaterial;//list of public materials
#if MESH_RETAIN_FILENAMES
    char *fileName;                         //for debugging
#endif
    polygonobject object[1];                //array of polygon object files
}
meshdata_disk;

/*
typedef struct tagvertex
{
    real32 x;                               // X component of this vertex.
    real32 y;                               // Y component of this vertex.
    real32 z;                               // Z component of this vertex.
    sdword iVertexNormal;                   // Index into the point normal list.
}
vertexentry;

typedef struct tagnormal
{
    real32 x;                               // X component of this normal.
    real32 y;                               // Y component of this normal.
    real32 z;                               // Z component of this normal.
    ubyte  pad[4];                          // Reserved for later use.
}
normalentry;

typedef struct tagpolygon
{
    sdword  iFaceNormal;                    // Index into the face normal list.
    uword  iV0;                             // Index to V0 of the polygon.
    uword  iV1;                             // Index to V1 of the polygon.
    uword  iV2;                             // Index to V2 of the polygon.
    uword  iMaterial;                       // Index into material list.
    real32 s0;
    real32 t0;
    real32 s1;
    real32 t1;
    real32 s2;
    real32 t2;
    uword  flags;                           // Flags for this polygon.
    ubyte reserved[2];
}
polyentry;

*/

typedef struct tagmaterialentry_disk
{
    udword  pName;   // *                        // Offset to name of material (may be a CRC32).
    color  ambient;                         // Ambient color information.
    color  diffuse;                         // Diffuse color information.
    color  specular;                        // Specular color information.
    real32 kAlpha;                          // Alpha blending information.
    udword texture;  // *                       // Pointer to texture information (or CRC32).
    uword  flags;                           // Flags for this material.
    ubyte  nFullAmbient;                    // Number of self-illuminating colors in texture.
    bool8  bTexturesRegistered;             // Set to TRUE when some textures have been registered.
    udword   textureNameSave; // *               // the name of the texture, after the texture has been registered
}
materialentry_disk;

/*

typedef struct polygonobject
{
    char *pName;                            // Name for animation.
    ubyte   flags;                          // General flags (see above)
    ubyte   iObject;                        // fixed up at load time so we know what object index we have when recursively processing
    uword   nameCRC;                        // 16-bit CRC of name (!!!!no room for 32 right now - make room next version)
    sdword  nVertices;                      // Number of vertices in vertex list for this object.
    sdword  nFaceNormals;                   // Number of face normals for this object.
    sdword  nVertexNormals;                 // Number of vertex normals for this object.
    sdword  nPolygons;                      // Number of polygons in this object.
    vertexentry *pVertexList;               // Offset to the vertex list in this object.
    normalentry *pNormalList;               // Offset to the normal list in this object.
    polyentry   *pPolygonList;              // Offset to the polygon list in this object.
    struct polygonobject *pMother;          // link to parent object
    struct polygonobject *pDaughter;        // link to child object
    struct polygonobject *pSister;          // link to sibling object
    hmatrix localMatrix;
}
polygonobject;

typedef struct hmatrix
{
    real32 m11,m21,m31,m41, m12,m22,m32,m42, m13,m23,m33,m43, m14,m24,m34,m44;

} hmatrix;


*/
