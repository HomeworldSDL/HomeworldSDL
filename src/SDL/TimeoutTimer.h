/*=============================================================================
    Name    : TimeoutTimer.h
    Purpose : Definitions for TimeoutTimers

    Created 98/11/13 by gshaw
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___TIMEOUTTIMER_H
#define ___TIMEOUTTIMER_H

#include "Types.h"

typedef struct TTimer
{
    bool32 enabled;
    bool32 timedOut;
    sqword timerLast;
    udword timeoutTicks;
} TTimer;

void TTimerInit(TTimer *timer);
void TTimerClose(TTimer *timer);
void TTimerDisable(TTimer *timer);
bool32 TTimerUpdate(TTimer *timer);
void TTimerReset(TTimer *timer);
void TTimerStart(TTimer *timer,real32 timeout);
bool32 TTimerIsTimedOut(TTimer *timer);

void GetRawTime(sqword *time);

#endif

