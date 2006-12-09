// =============================================================================
//  CommandDefs.h
//  - List of commands and command attributes
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created 4/26/1999 by fpoiker
// =============================================================================

#ifndef ___COMMANDDEFS_H
#define ___COMMANDDEFS_H

#define COMMAND_NULL                             0
#define COMMAND_MOVE                             1
#define COMMAND_ATTACK                           2
#define COMMAND_DOCK                             3
#define COMMAND_LAUNCH_SHIP                      4
#define COMMAND_COLLECT_RESOURCES                5
#define COMMAND_BUILDING_SHIP                    6
#define COMMAND_SPECIAL                          7
#define COMMAND_HALT                             8
#define COMMAND_MILITARY_PARADE                  9
#define COMMAND_MP_HYPERSPACING                 10

#define COMMAND_MASK_FORMATION              0x0001
#define COMMAND_MASK_PROTECTING             0x0002
#define COMMAND_MASK_PASSIVE_ATTACKING      0x0004
#define COMMAND_MASK_HOLDING_PATTERN        0x0008
#define COMMAND_MASK_ATTACKING_AND_MOVING   0x0010

#endif
