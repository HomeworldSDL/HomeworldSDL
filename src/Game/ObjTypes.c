/*=============================================================================
    Name    : ObjTypes.c
    Purpose : Provides conversions for datatypes found in objtypes.h

    Created 6/24/1997 by gshaw
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include "ObjTypes.h"

#include <string.h>

#include "ClassDefs.h"
#include "StatScript.h"
#include "StringSupport.h"

#ifdef _MSC_VER
    #define strcasecmp _stricmp
#else
    #include <strings.h>
#endif

/*=============================================================================
    Public Data:
=============================================================================*/

ShipType FirstShipTypeOfRace[NUM_RACES] =
{
    STD_FIRST_SHIP,
    STD_FIRST_SHIP,
    P1_FIRST_SHIP,
    P2_FIRST_SHIP,
    P3_FIRST_SHIP,
    TRADERS_FIRST_SHIP
};

ShipType LastShipTypeOfRace[NUM_RACES] =
{
    STD_LAST_SHIP,
    STD_LAST_SHIP,
    P1_LAST_SHIP,
    P2_LAST_SHIP,
    P3_LAST_SHIP,
    TRADERS_LAST_SHIP
};

uword NumShipTypesInRace[NUM_RACES] =
{
    TOTAL_STD_SHIPS,
    TOTAL_STD_SHIPS,
    TOTAL_P1_SHIPS,
    TOTAL_P2_SHIPS,
    TOTAL_P3_SHIPS,
    TOTAL_TRADERS_SHIPS
};

/*=============================================================================
    Private Data:
=============================================================================*/

static NumStrXlate shiptypeinfo[] =
{
    { (uword)AdvanceSupportFrigate, strdollar(AdvanceSupportFrigate) },
    { (uword)AttackBomber, strdollar(AttackBomber) },
    { (uword)Carrier, strdollar(Carrier) },
    { (uword)CloakedFighter, strdollar(CloakedFighter) },
    { (uword)CloakGenerator, strdollar(CloakGenerator) },
    { (uword)DDDFrigate, strdollar(DDDFrigate) },
    { (uword)DefenseFighter, strdollar(DefenseFighter) },
    { (uword)DFGFrigate, strdollar(DFGFrigate) },
    { (uword)GravWellGenerator, strdollar(GravWellGenerator) },
    { (uword)HeavyCorvette, strdollar(HeavyCorvette) },
    { (uword)HeavyCruiser, strdollar(HeavyCruiser) },
    { (uword)HeavyDefender, strdollar(HeavyDefender) },
    { (uword)HeavyInterceptor, strdollar(HeavyInterceptor) },
    { (uword)IonCannonFrigate, strdollar(IonCannonFrigate) },
    { (uword)LightCorvette, strdollar(LightCorvette) },
    { (uword)LightDefender, strdollar(LightDefender) },
    { (uword)LightInterceptor, strdollar(LightInterceptor) },
    { (uword)MinelayerCorvette, strdollar(MinelayerCorvette) },
    { (uword)MissileDestroyer, strdollar(MissileDestroyer) },
    { (uword)Mothership, strdollar(Mothership) },
    { (uword)MultiGunCorvette, strdollar(MultiGunCorvette) },
    { (uword)Probe, strdollar(Probe) },
    { (uword)ProximitySensor, strdollar(ProximitySensor) },
    { (uword)RepairCorvette, strdollar(RepairCorvette) },
    { (uword)ResearchShip, strdollar(ResearchShip) },
    { (uword)ResourceCollector, strdollar(ResourceCollector) },
    { (uword)ResourceController, strdollar(ResourceController) },
    { (uword)SalCapCorvette, strdollar(SalCapCorvette) },
    { (uword)SensorArray, strdollar(SensorArray) },
    { (uword)StandardDestroyer, strdollar(StandardDestroyer) },
    { (uword)StandardFrigate, strdollar(StandardFrigate) },
    { (uword)Drone, strdollar(Drone) },
    { (uword)TargetDrone, strdollar(TargetDrone) },
    { (uword)HeadShotAsteroid, strdollar(HeadShotAsteroid) },
    { (uword)CryoTray, strdollar(CryoTray) },
    { (uword)P1Fighter, strdollar(P1Fighter) },
    { (uword)P1IonArrayFrigate, strdollar(P1IonArrayFrigate) },
    { (uword)P1MissileCorvette, strdollar(P1MissileCorvette) },
    { (uword)P1Mothership, strdollar(P1Mothership) },
    { (uword)P1StandardCorvette, strdollar(P1StandardCorvette) },
    { (uword)P2AdvanceSwarmer, strdollar(P2AdvanceSwarmer) },
    { (uword)P2FuelPod, strdollar(P2FuelPod) },
    { (uword)P2Mothership, strdollar(P2Mothership) },
    { (uword)P2MultiBeamFrigate, strdollar(P2MultiBeamFrigate) },
    { (uword)P2Swarmer, strdollar(P2Swarmer) },
    { (uword)P3Destroyer, strdollar(P3Destroyer) },
    { (uword)P3Frigate, strdollar(P3Frigate) },
    { (uword)P3Megaship, strdollar(P3Megaship) },
    { (uword)FloatingCity, strdollar(FloatingCity) },
    { (uword)CargoBarge, strdollar(CargoBarge) },
    { (uword)MiningBase, strdollar(MiningBase) },
    { (uword)ResearchStation, strdollar(ResearchStation) },
    { (uword)JunkYardDawg, strdollar(JunkYardDawg) },
    { (uword)JunkYardHQ, strdollar(JunkYardHQ) },
    { (uword)Ghostship,  strdollar(Ghostship)},
    { (uword)Junk_LGun,  strdollar(Junk_LGun)},
    { (uword)Junk_SGun,  strdollar(Junk_SGun)},
    { (uword)ResearchStationBridge,  strdollar(ResearchStationBridge)},
    { (uword)ResearchStationTower,  strdollar(ResearchStationTower)},
    { 0,NULL }
};

static NumStrXlate shipraceinfo[] =
{
    { (uword)R1, strdollar(R1) },
    { (uword)R2, strdollar(R2) },
    { (uword)P1, strdollar(P1) },
    { (uword)P2, strdollar(P2) },
    { (uword)P3, strdollar(P3) },
    { (uword)Traders, strdollar(Traders) },
    { 0,NULL }
};

static NumStrXlate nisraceinfo[] =
{
    { (uword)R1, strdollar(R1) },
    { (uword)R2, strdollar(R2) },
    { (uword)P1, strdollar(P1) },
    { (uword)P2, strdollar(P2) },
    { (uword)P3, strdollar(P3) },
    { (uword)Traders, strdollar(Traders) },
    { (uword)NSR_Asteroid , "Asteroids" },
    { (uword)NSR_DustCloud, "DustClouds" },
    { (uword)NSR_GasCloud , "GasClouds" },
    { (uword)NSR_Nebula   , "Nebulae" },
    { (uword)NSR_Derelict , "Derelicts" },
    { (uword)NSR_Effect   , "ETG" },
    { (uword)NSR_Generic  , "Misc" },
    { (uword)NSR_Missile  , "Missile" },
    { 0,NULL }
};
static NumStrXlate shipclassinfo[] =
{
    { (uword)CLASS_Mothership,     strdollar(CLASS_Mothership) },
    { (uword)CLASS_HeavyCruiser,   strdollar(CLASS_HeavyCruiser) },
    { (uword)CLASS_Carrier,        strdollar(CLASS_Carrier) },
    { (uword)CLASS_Destroyer,      strdollar(CLASS_Destroyer) },
    { (uword)CLASS_Frigate,        strdollar(CLASS_Frigate) },
    { (uword)CLASS_Corvette,       strdollar(CLASS_Corvette) },
    { (uword)CLASS_Fighter,        strdollar(CLASS_Fighter) },
    { (uword)CLASS_Resource,       strdollar(CLASS_Resource) },
    { (uword)CLASS_NonCombat,      strdollar(CLASS_NonCombat) },
    { 0,NULL }
};

/*static NumStrXlate shipclassniceinfo[] =
{
    { (uword)CLASS_Mothership,     "MOTHERSHIP"},
    { (uword)CLASS_HeavyCruiser,   "CRUISER"},
    { (uword)CLASS_Carrier,        "CARRIER"},
    { (uword)CLASS_Destroyer,      "DESTROYER"},
    { (uword)CLASS_Frigate,        "FRIGATE"},
    { (uword)CLASS_Corvette,       "CORVETTE"},
    { (uword)CLASS_Fighter,        "FIGHTER"},
    { (uword)CLASS_Resource,       "RESOURCE"},
    { (uword)CLASS_NonCombat,      "NON-COMBAT"},
    { 0,NULL }
};*/

static NumStrXlate asteroidtypeinfo[] =
{
    { (uword)Asteroid0, strdollar(Asteroid0) },
    { (uword)Asteroid1, strdollar(Asteroid1) },
    { (uword)Asteroid2, strdollar(Asteroid2) },
    { (uword)Asteroid3, strdollar(Asteroid3) },
    { (uword)Asteroid4, strdollar(Asteroid4) },
    { 0,NULL }
};

static NumStrXlate dustcloudtypeinfo[] =
{
    { (uword)DustCloud0, strdollar(DustCloud0) },
    { (uword)DustCloud1, strdollar(DustCloud1) },
    { (uword)DustCloud2, strdollar(DustCloud2) },
    { (uword)DustCloud3, strdollar(DustCloud3) },
    { 0,NULL }
};

static NumStrXlate gascloudtypeinfo[] =
{
    { (uword)GasCloud0, strdollar(GasCloud0) },
    { (uword)GasCloud1, strdollar(GasCloud1) },
    { 0,NULL }
};

static NumStrXlate nebulatypeinfo[] =
{
    { (uword)Nebula0, strdollar(Nebula0) },
    { 0, NULL }
};

static NumStrXlate derelicttypeinfo[] =
{
    { (uword)AngelMoon,           strdollar(AngelMoon)},
    { (uword)AngelMoon_clean,     strdollar(AngelMoon_clean)},
    { (uword)Crate,               strdollar(Crate)},
    { (uword)FragmentPanel0a,     strdollar(FragmentPanel0a)},
    { (uword)FragmentPanel0b,     strdollar(FragmentPanel0b)},
    { (uword)FragmentPanel0c,     strdollar(FragmentPanel0c)},
    { (uword)FragmentPanel1,      strdollar(FragmentPanel1)},
    { (uword)FragmentPanel2,      strdollar(FragmentPanel2)},
    { (uword)FragmentPanel3,      strdollar(FragmentPanel3)},
    { (uword)FragmentStrut,       strdollar(FragmentStrut)},
    { (uword)Homeworld,           strdollar(Homeworld)},
    { (uword)LifeBoat,            strdollar(LifeBoat)},
    { (uword)PlanetOfOrigin,      strdollar(PlanetOfOrigin)},
    { (uword)PlanetOfOrigin_scarred, strdollar(PlanetOfOrigin_scarred)},
    { (uword)PrisonShipOld,       strdollar(PrisonShipOld)},
    { (uword)PrisonShipNew,       strdollar(PrisonShipNew)},
    { (uword)Scaffold,            strdollar(Scaffold)},
    { (uword)Scaffold_scarred,    strdollar(Scaffold_scarred)},
    { (uword)ScaffoldFingerA_scarred, strdollar(ScaffoldFingerA_scarred)},
    { (uword)ScaffoldFingerB_scarred, strdollar(ScaffoldFingerB_scarred)},
    { (uword)Shipwreck,           strdollar(Shipwreck)},
    //pre-revision ships as junkyard derelicts:
    { (uword)JunkAdvanceSupportFrigate,     "AdvanceSupportFrigate"},
    { (uword)JunkCarrier,                   "Carrier"},
    { (uword)JunkDDDFrigate,                "DDDFrigate"},
    { (uword)JunkHeavyCorvette,             "HeavyCorvette"},
    { (uword)JunkHeavyCruiser,              "HeavyCruiser"},
    { (uword)JunkIonCannonFrigate,          "IonCannonFrigate"},
    { (uword)JunkLightCorvette,             "LightCorvette"},
    { (uword)JunkMinelayerCorvette,         "MinelayerCorvette"},
    { (uword)JunkMultiGunCorvette,          "MultiGunCorvette"},
    { (uword)JunkRepairCorvette,            "RepairCorvette"},
    { (uword)JunkResourceController,        "ResourceController"},
    { (uword)JunkSalCapCorvette,            "SalCapCorvette"},
    { (uword)JunkStandardFrigate,           "StandardFrigate"},
    //junk derelicts
    { (uword)Junk0_antenna,                 "Junk0_antenna"},
    { (uword)Junk0_fin1,                    "Junk0_fin1"},
    { (uword)Junk0_fin2,                    "Junk0_fin2"},
    { (uword)Junk0_GunAmmo,                 "Junk0_GunAmmo"},
    { (uword)Junk0_panel,                   "Junk0_panel"},
    { (uword)Junk0_sensors,                 "Junk0_sensors"},
    { (uword)Junk1_partA,                   "Junk1_partA"},
    { (uword)Junk1_partB,                   "Junk1_partB"},
    { (uword)Junk1_shell,                   "Junk1_shell"},
    { (uword)Junk1_strut,                   "Junk1_strut"},
    { (uword)Junk2_panelA,                  "Junk2_panelA"},
    { (uword)Junk2_panelB,                  "Junk2_panelB"},
    { (uword)Junk2_panelC,                  "Junk2_panelC"},
    { (uword)Junk2_panelD,                  "Junk2_panelD"},
    { (uword)Junk2_shipwreck,               "Junk2_shipwreck"},
    { (uword)Junk3_Boiler,                  "Junk3_Boiler"},
    { (uword)Junk3_BoilerCasing,            "Junk3_BoilerCasing"},
    { (uword)M13PanelA,                     "M13PanelA"},
    { (uword)M13PanelB,                     "M13PanelB"},
    { (uword)M13PanelC,                     "M13PanelC"},
    { (uword)HyperspaceGate,                "HyperspaceGate"},
    { 0,NULL }
};

static NumStrXlate explosiontypeinfo[] =
{
    { ET_Explosion,           "ET_Explosion"           },

    { ET_LightLaserHit,       "ET_LightLaserHit"       },
    { ET_MedLaserHit,         "ET_MedLaserHit"         },
    { ET_HeavyLaserHit,       "ET_HeavyLaserHit"       },
    { ET_PlasmaBombHit,       "ET_PlasmaBombHit"       },

    { ET_InterceptorExplosion,"ET_InterceptorExplosion"},
    { ET_CorvetteExplosion,   "ET_CorvetteExplosion"   },
    { ET_FrigateExplosion,    "ET_FrigateExplosion"    },
    { ET_CarrierExplosion,    "ET_CarrierExplosion"    },
    { ET_ResourcerExplosion,  "ET_ResourcerExplosion"  },

    { 0,NULL }
};

static NumStrXlate guntypeinfo[] =
{
    { (uword)GUN_Fixed,  strdollar(GUN_Fixed) },
    { (uword)GUN_Gimble, strdollar(GUN_Gimble) },
    { (uword)GUN_NewGimble, strdollar(GUN_NewGimble) },
    { (uword)GUN_MissileLauncher, strdollar(GUN_MissileLauncher) },
    { (uword)GUN_MineLauncher, strdollar(GUN_MineLauncher) },
    { 0,NULL }
};

static NumStrXlate dockpointtypeinfo[] =
{
    { (uword)DockingCone, strdollar(DockingCone) },
    { (uword)LatchPoint, strdollar(LatchPoint) },
    { (uword)LaunchPoint, strdollar(LaunchPoint) },

    { 0,NULL }
};

static NumStrXlate salvagepointtypeinfo[] =
{
    { (uword)Heading, strdollar(Heading) },
    { (uword)Up, strdollar(Up) },
    { (uword)AttachPoint, strdollar(AttachPoint) },

    { 0,NULL }
};

static NumStrXlate navlighttypeinfo[] =
{
    { (uword)NAVLIGHT_Default, strdollar(NAVLIGHT_Default) },
    { 0,NULL }
};

static NumStrXlate gunsoundtypeinfo[] =
{
    { (uword)GS_LargeEnergyCannon, strdollar(GS_LargeEnergyCannon) },
    { (uword)GS_LargeIonCannon, strdollar(GS_LargeIonCannon) },
    { (uword)GS_LargePlasmaBomb, strdollar(GS_LargePlasmaBomb) },
    { (uword)GS_LargeProjectile, strdollar(GS_LargeProjectile) },
    { (uword)GS_MediumEnergyCannon, strdollar(GS_MediumEnergyCannon) },
    { (uword)GS_MediumIonCannon, strdollar(GS_MediumIonCannon) },
    { (uword)GS_MediumPlasmaBomb, strdollar(GS_MediumPlasmaBomb) },
    { (uword)GS_MediumProjectile, strdollar(GS_MediumProjectile) },
    { (uword)GS_MineLauncher, strdollar(GS_MineLauncher) },
    { (uword)GS_MissleLauncher, strdollar(GS_MissleLauncher) },
    { (uword)GS_SmallEnergyCannon, strdollar(GS_SmallEnergyCannon) },
    { (uword)GS_SmallIonCannon, strdollar(GS_SmallIonCannon) },
    { (uword)GS_SmallPlasmaBomb, strdollar(GS_SmallPlasmaBomb) },
    { (uword)GS_SmallProjectile, strdollar(GS_SmallProjectile) },
    { (uword)GS_VeryLargeEnergyCannon, strdollar(GS_VeryLargeEnergyCannon) },
    { (uword)GS_VeryLargeIonCannon, strdollar(GS_VeryLargeIonCannon) },
    { (uword)GS_VeryLargePlasmaBomb, strdollar(GS_VeryLargePlasmaBomb) },
    { (uword)GS_Laser, strdollar(GS_Laser) },
    { 0,NULL }
};

static NumStrXlate bullettypeinfo[] =
{
    { (uword)BULLET_Projectile, strdollar(BULLET_Projectile) },
    { (uword)BULLET_PlasmaBomb, strdollar(BULLET_PlasmaBomb) },
    { (uword)BULLET_Beam, strdollar(BULLET_Beam) },
    { 0,NULL }
};

static NumStrXlate missiletypeinfo[] =
{
    { (uword)MISSILE_Regular, strdollar(MISSILE_Regular) },
    { (uword)MISSILE_Mine, strdollar(MISSILE_Mine) },
    { 0,NULL }
};

static NumStrXlate objtypeinfo[] =
{
    { (uword)OBJ_ShipType, "Ship" },
    { (uword)OBJ_BulletType, "Bullet" },
    { (uword)OBJ_AsteroidType, "Asteroid" },
    { (uword)OBJ_NebulaType, "Nebula" },
    { (uword)OBJ_GasType, "GasCloud" },
    { (uword)OBJ_DustType, "DustCloud" },
    { (uword)OBJ_DerelictType, "Derelict" },
    { (uword)OBJ_EffectType, "Effect" },
    { (uword)OBJ_MissileType, "Missile" },
    { 0,NULL }
};

static NumStrXlate SpecialEffectinfo[] =
{
    { (uword)EGT_MINE_WALL_EFFECT, "EGT_MINE_WALL_EFFECT"},
    { (uword)EGT_BURST_CHARING_EFFECT, "EGT_BURST_CHARING_EFFECT"},
    { (uword)EGT_BURST_MUZZLE_EFFECT, "EGT_BURST_MUZZLE_EFFECT"},
    { (uword)EGT_BURST_BULLET_EFFECT, "EGT_BURST_BULLET_EFFECT"},
    { (uword)EGT_BURST_HIT_EFFECT, "EGT_BURST_HIT_EFFECT"},
    { (uword)EGT_BURST_EXPLOSION_EFFECT, "EGT_BURST_EXPLOSION_EFFECT"},
    { (uword)EGT_CLOAK_ON      , "EGT_CLOAK_ON"},
    { (uword)EGT_CLOAK_OFF     , "EGT_CLOAK_OFF"},
    { (uword)EGT_GRAVWELL_ON   , "EGT_GRAVWELL_ON"},
    { (uword)EGT_GRAVWELL_OFF  , "EGT_GRAVWELL_OFF"},
    { (uword)EGT_LIGHTNING_GLOW, "EGT_LIGHTNING_GLOW"},
    { (uword)EGT_BULLET_DEFLECT, "EGT_BULLET_DEFLECT"},
    { (uword)EGT_CRATE_GENERATED, "EGT_CRATE_GENERATED"},
    { (uword)EGT_CRATE_IS_FOUND_SHIP, "EGT_CRATE_IS_FOUND_SHIP"},
    { (uword)EGT_CRATE_IS_FOUND_MONEY, "EGT_CRATE_IS_FOUND_MONEY"},
    { (uword)EGT_CRATE_IS_FOUND_RESEARCH, "EGT_CRATE_IS_FOUND_RESEARCH"},
    { (uword)EGT_CRATE_TIME_OUT, "EGT_CRATE_TIME_OUT"},
    { (uword)EGT_REPAIR_BEAM, "EGT_REPAIR_BEAM"},
    { (uword)EGT_CAUGHT_GRAVWELL, "EGT_CAUGHT_GRAVWELL"},

    { 0,NULL }
};

static NumStrXlate tacticsinfo[] =
{
    { (uword)Evasive, strdollar(Evasive) },
    { (uword)Neutral, strdollar(Neutral) },
    { (uword)Aggressive, strdollar(Aggressive) },
    { 0,NULL }
};

/*=============================================================================
    Functions:
=============================================================================*/

/*-----------------------------------------------------------------------------
    Name        : RaceBitsToRace
    Description : Converts bit positions representing race (e.g. RACE1_VALID)
                  to race enumeration (e.g. R1)
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
ShipRace RaceBitsToRace(uword raceBits)
{
    if (raceBits & R1_VALID)
    {
        return R1;
    }
    else if (raceBits & R2_VALID)
    {
        return R2;
    }
    else if (raceBits & P1_VALID)
    {
        return P1;
    }
    else if (raceBits & P2_VALID)
    {
        return P2;
    }
    else if (raceBits & P3_VALID)
    {
        return P3;
    }
    else if (raceBits & Traders_VALID)
    {
        return Traders;
    }

    return 0;
}

/*-----------------------------------------------------------------------------
    Name        : NumToStr
    Description : Converts a DEFINE to a string
    Inputs      : numstrtab - table cross-referencing numbers and strings
                  num - number to convert
    Outputs     :
    Return      : pointer to converted string if found, NULL otherwise
----------------------------------------------------------------------------*/
char *NumToStr(NumStrXlate numstrtab[],uword num)
{
    uword i;
    NumStrXlate *curnumstr;

    for (i=0,curnumstr=numstrtab;curnumstr->str != NULL;i++,curnumstr++)
    {
        if (num == curnumstr->number)
        {
            return curnumstr->str;
        }
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
    Name        : StrToNum
    Description : Converts a string to a DEFINE
    Inputs      : numstrtab - table cross-referencing numbers and strings
                  str - string to convert
    Outputs     :
    Return      : number if converted correctly, -1 otherwise
----------------------------------------------------------------------------*/
sdword StrToNum(NumStrXlate numstrtab[],char *str)
{
    sdword i;
    NumStrXlate *curnumstr;

    for (i=0,curnumstr=numstrtab;curnumstr->str != NULL;i++,curnumstr++)
    {
        if (strcasecmp(str,curnumstr->str) == 0)
        {
            return curnumstr->number;
        }
    }
    return -1;
}

/*=============================================================================
    The following functions convert between ShipClass, ShipRace, ShipType
    and the strings representing them and vice-versa.
=============================================================================*/

char *TacticsTypeToStr(TacticsType tactics)
{
    return NumToStr(tacticsinfo,(uword)tactics);
}

TacticsType StrToTacticsType(char *str)
{
    return StrToNum(tacticsinfo,str);
}

char *ShipClassToStr(ShipClass shipclass)
{
    return NumToStr(shipclassinfo,(uword)shipclass);
}

char *ShipClassToNiceStr(ShipClass shipclass)
{
    return(strGetString((uword) shipclass+strClassOffset));
}

char *ShipRaceToStr(ShipRace shiprace)
{
    return NumToStr(shipraceinfo,(uword)shiprace);
}

char *ShipTypeToStr(ShipType shiptype)
{
    return NumToStr(shiptypeinfo,(uword)shiptype);
}

char *ShipStatToNiceStr(ShipStatsType stattype)
{
    // There are 6 stat types for each ship type in the ship view stat window
    return(strGetString((uword) (stattype + strShipStatNameOffset)));
}

/*
char *ShipTypeStatToNiceStr(ShipType shiptype, ShipStatsType stattype)
{
    // There are 6 stat types for each ship type in the ship view stat window
    return(strGetString((uword) ((shiptype * 6 + stattype) + strShipStatOffset)));
}
*/

ShipRace StrToNisRace(char *string)
{
    return((ShipRace)StrToNum(nisraceinfo, string));
}

char *NisRaceToStr(ShipRace race)
{
    return(NumToStr(nisraceinfo, (uword)race));
}

char *ShipTypeToNiceStr(ShipType shiptype)
{
    return(strGetString((uword) shiptype + strShipOffset));
}

char *GunTypeToStr(GunType guntype)
{
    return NumToStr(guntypeinfo,(uword)guntype);
}

char *GunSoundTypeToStr(GunSoundType gunsoundtype)
{
    return NumToStr(gunsoundtypeinfo,(uword)gunsoundtype);
}

char *DockPointTypeToStr(DockPointType dockpointtype)
{
    return NumToStr(dockpointtypeinfo,(uword)dockpointtype);
}

char *SalvagePointTypeToStr(SalvagePointType salvagepointtype)
{
    return NumToStr(salvagepointtypeinfo,(uword)salvagepointtype);
}

char *ExplosionTypeToStr(EffectType type)
{
    return NumToStr(explosiontypeinfo,(uword)type);
}

char *AsteroidTypeToStr(AsteroidType asteroidtype)
{
    return NumToStr(asteroidtypeinfo,(uword)asteroidtype);
}

char *DustCloudTypeToStr(DustCloudType dustcloudtype)
{
    return NumToStr(dustcloudtypeinfo,(uword)dustcloudtype);
}

char *GasCloudTypeToStr(GasCloudType gascloudtype)
{
    return NumToStr(gascloudtypeinfo,(uword)gascloudtype);
}

char *NebulaTypeToStr(NebulaType nebulatype)
{
    return NumToStr(nebulatypeinfo, (uword)nebulatype);
}

char *DerelictTypeToStr(DerelictType derelicttype)
{
    return NumToStr(derelicttypeinfo,(uword)derelicttype);
}

char *BulletTypeToStr(BulletType bulletType)
{
    return NumToStr(bullettypeinfo,(uword)bulletType);
}

char *MissileTypeToStr(MissileType missileType)
{
    return NumToStr(missiletypeinfo,(uword)missileType);
}

char *NAVLightTypeToStr(NAVLightType navlightType)
{
    return(NumToStr(navlighttypeinfo, (uword)navlightType));
}

char *ObjTypeToStr(ObjType objtype)
{
    return(NumToStr(objtypeinfo,(uword)objtype));
}

ShipClass StrToShipClass(char *str)
{
    return StrToNum(shipclassinfo,str);
}

ShipRace StrToShipRace(char *str)
{
    return StrToNum(shipraceinfo,str);
}

SpecialEffectType StrToEffectNum(char *str)
{
    return StrToNum(SpecialEffectinfo,str);
}

ShipType StrToShipType(char *str)
{
    return StrToNum(shiptypeinfo,str);
}

AsteroidType StrToAsteroidType(char *str)
{
    return StrToNum(asteroidtypeinfo,str);
}

DustCloudType StrToDustCloudType(char *str)
{
    return StrToNum(dustcloudtypeinfo, str);
}

GasCloudType StrToGasCloudType(char *str)
{
    return StrToNum(gascloudtypeinfo, str);
}

NebulaType StrToNebulaType(char *str)
{
    return StrToNum(nebulatypeinfo, str);
}

DerelictType StrToDerelictType(char *str)
{
    return StrToNum(derelicttypeinfo,str);
}

EffectType StrToExplosionType(char *str)
{
    return StrToNum(explosiontypeinfo,str);
}

GunType StrToGunType(char *str)
{
    return StrToNum(guntypeinfo,str);
}

GunSoundType StrToGunSoundType(char *str)
{
    return StrToNum(gunsoundtypeinfo,str);
}

DockPointType StrToDockPointType(char *str)
{
    return StrToNum(dockpointtypeinfo,str);
}

SalvagePointType StrToSalvagePointType(char *str)
{
    return StrToNum(salvagepointtypeinfo,str);
}

BulletType StrToBulletType(char *str)
{
    return StrToNum(bullettypeinfo,str);
}

MissileType StrToMissileType(char *str)
{
    return StrToNum(missiletypeinfo,str);
}

NAVLightType StrToNAVLightType(char *str)
{
    return StrToNum(navlighttypeinfo, str);
}

ObjType StrToObjType(char *str)
{
    return StrToNum(objtypeinfo, str);
}

