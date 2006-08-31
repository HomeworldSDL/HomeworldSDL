/*=============================================================================
    Name    : CloakGenerator.h
    Purpose : Definitions for the Cloak Generator

    Created 01/06/1998 by bpasechnik
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___CLOAKGENERATOR_H
#define ___CLOAKGENERATOR_H

#include "Types.h"
#include "SpaceObj.h"
#include "LinkedList.h"


/*=============================================================================
    Types:
=============================================================================*/
typedef struct
{
    Node cloaknode;
    SpaceObj *spaceobj;
    real32  CloakStatus;
} CloakStruct;

typedef struct
{
    sdword CloakOn;
    real32 CloakStatus;
    LinkedList  CloakList;
    bool CloakLowWarning;
} CloakGeneratorSpec;

typedef struct
{
    real32 CloakingRadius;
    real32 CloakingTime;
    real32 DeCloakingTime;
    real32 MaxCloakingTime;
    real32 MinCharge;
    real32 ReChargeRate;
    real32 CloakingRadiusSqr;
} CloakGeneratorStatics;



/*=============================================================================
    Public data:
=============================================================================*/

extern CustShipHeader CloakGeneratorHeader;

/*=============================================================================
    Public Functions:
=============================================================================*/

void CloakGeneratorAddObj(Ship *ship,  SpaceObj *objtoadd);
//void CloakGeneratorDied(Ship *ship);
//void CloakGeneratorRemoveShipReferences(Ship *ship,SpaceObj *objtoremove);
void CloakGeneratorJustDisabled(Ship *ship);

#endif