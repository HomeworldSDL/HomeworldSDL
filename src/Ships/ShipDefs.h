#ifndef ___SHIPDEFS_H
#define ___SHIPDEFS_H

#include "Types.h"

enum
{
    ShipType_Uninitialized = -1,

    AdvanceSupportFrigate = 0,
    AttackBomber,
    Carrier,
    CloakedFighter,
    CloakGenerator,
    DDDFrigate,
    DefenseFighter,
    DFGFrigate,
    GravWellGenerator,
    HeavyCorvette,
    HeavyCruiser,
    HeavyDefender,
    HeavyInterceptor,
    IonCannonFrigate,
    LightCorvette,
    LightDefender,
    LightInterceptor,
    MinelayerCorvette,
    MissileDestroyer,
    Mothership,
    MultiGunCorvette,
    Probe,
    ProximitySensor,
    RepairCorvette,
    ResearchShip,
    ResourceCollector,
    ResourceController,
    SalCapCorvette,
    SensorArray,
    StandardDestroyer,
    StandardFrigate,
    Drone,                      // Special ship of R1
    TargetDrone,
    HeadShotAsteroid,           // Headshot asteroid is now a ship!
    CryoTray,                   // CryoTray is now a ship!

    P1Fighter,
    P1IonArrayFrigate,
    P1MissileCorvette,
    P1Mothership,
    P1StandardCorvette,

    P2AdvanceSwarmer,
    P2FuelPod,
    P2Mothership,
    P2MultiBeamFrigate,
    P2Swarmer,

    P3Destroyer,
    P3Frigate,
    P3Megaship,

    FloatingCity,
    CargoBarge,
    MiningBase,
    ResearchStation,
    JunkYardDawg,
    JunkYardHQ,
    Ghostship,
    Junk_LGun,
    Junk_SGun,
    ResearchStationBridge,
    ResearchStationTower
};

/* It seems that this needs to be unsigned, else something breaks in
   NIS.c. */
typedef udword ShipType;

#define STD_FIRST_SHIP        AdvanceSupportFrigate
#define STD_LAST_SHIP         CryoTray

#define P1_FIRST_SHIP         P1Fighter
#define P1_LAST_SHIP          P1StandardCorvette

#define P2_FIRST_SHIP         P2AdvanceSwarmer
#define P2_LAST_SHIP          P2Swarmer

#define P3_FIRST_SHIP         P3Destroyer
#define P3_LAST_SHIP          P3Megaship

#define TRADERS_FIRST_SHIP    FloatingCity
#define TRADERS_LAST_SHIP     ResearchStationTower

#define DefaultShip           (TRADERS_LAST_SHIP+1)    // not really a ship, just a flag indicating we should use Default ship behaviour
#define TOTAL_NUM_SHIPS       (TRADERS_LAST_SHIP+1)

#define TOTAL_STD_SHIPS       (STD_LAST_SHIP - STD_FIRST_SHIP + 1)
#define TOTAL_P1_SHIPS        (P1_LAST_SHIP - P1_FIRST_SHIP + 1)
#define TOTAL_P2_SHIPS        (P2_LAST_SHIP - P2_FIRST_SHIP + 1)
#define TOTAL_P3_SHIPS        (P3_LAST_SHIP - P3_FIRST_SHIP + 1)
#define TOTAL_TRADERS_SHIPS   (TRADERS_LAST_SHIP - TRADERS_FIRST_SHIP + 1)

#define TOTAL_OTHER_SHIPS     (TOTAL_P1_SHIPS + TOTAL_P2_SHIPS + TOTAL_P3_SHIPS + TOTAL_TRADERS_SHIPS)

#endif
