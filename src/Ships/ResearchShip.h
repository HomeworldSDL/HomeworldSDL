// =============================================================================
//  ResearchShip.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 01/06/98 Bpasechnik
// =============================================================================

#ifndef ___RESEARCHSHIP_H
#define ___RESEARCHSHIP_H

#include "SpaceObj.h"

typedef struct
{
    sdword seed;
    sdword done;
    udword dockers;
    sdword busy_docking;
    udword pie_plate_num;
    sdword prepshipforanother;
    sdword master;
    sdword rotate_state;
    real32 theta;
    Ship *masterptr;
    Ship *dockwith;
    sdword have_removed_from_parade;
    sdword dockordernumber;
    vector rotate_point;
    real32 rotate_distance;
} ResearchShipSpec;

typedef struct
{
    real32 R1final_dock_distance;
    real32 R1parallel_dock_distance;
    real32 R1VerticalDockDistance;
    real32 max_rotate;
    real32 rotate_acelleration;
    real32 rotate_slow;
    real32 RotationAngle;
    real32 R2DockFinalDistance;
} ResearchShipStatics;

extern CustShipHeader ResearchShipHeader;

void addMonkeyResearchShip(Ship *ship);
void addMonkeyResearchShipChangePosition(Ship *dockwith, Ship *ship,sdword dockindex);
void ResearchShipMakeReadyForHyperspace(Ship *ship);
void toFakeOneShip(Ship *ship, vector *oldpos, real32 *oldradius);
void toUnFakeOneShip(Ship *ship, vector *oldpos,real32 *oldradius);

#endif