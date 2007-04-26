/*=============================================================================
    Name    : task.h
    Purpose : Definitions for a preemptive task manager.

    Created 6/17/1997 by lmoloney
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#ifndef ___TASK_H
#define ___TASK_H

#include <stdlib.h>

#include "LinkedList.h"
#include "Debug.h"

/*=============================================================================
    Switches:
=============================================================================*/

#ifdef HW_BUILD_FOR_DEBUGGING

#define TASK_ERROR_CHECKING     1               //general error checking
#define TASK_VERBOSE_LEVEL      1               //print extra info
#define TASK_TEST               0               //test the task module
#define TASK_MAX_TICKS          1               //certain maximum number of task ticks per frame

#else

#define TASK_ERROR_CHECKING     0               //general error checking
#define TASK_VERBOSE_LEVEL      0               //print extra info
#define TASK_TEST               0               //test the task module
#define TASK_MAX_TICKS          1               //certain maximum number of task ticks per frame

#endif

/*=============================================================================
    Definitions:
=============================================================================*/
#define TSK_NumberTasks         32              //maximum number of tasks

//task structure flags
#define TF_Allocated            1               //task in operation
#define TF_Paused               2               //task is paused
#define TF_OncePerFrame         4               //call once per frame as opposed to <N> Hz
//#define TF_OneOverFreq          8               //actual frequency is 1/frequency specified
#define TF_PauseSave            16              //saved pause bit for pausing all tasks

//maximum number of task ticks per frame
#define TSK_MaxTicks            8

/*=============================================================================
    Type definitions:
=============================================================================*/
typedef sdword taskhandle;
typedef void *taskContext;
typedef void (*taskfunction)(taskContext *);

//structure for each task
typedef struct
{
    udword flags;               //control flags for this task
    udword ticks;               //ticks accumulated since last call
    udword ticksPerCall;        //period in ticks
    taskfunction function;      //task handler entry point
    taskContext context;        //current line & other task-specific data
    char *name;                 //for debugging
}
taskdata;

struct BabyCallBack;
typedef bool (*babyFuncCB)(udword num, void *data, struct BabyCallBack *baby);    //callback function

typedef struct BabyCallBack
{
    Node babylink;
    real32 timeStarted;
    real32 timeToExecute;
    udword numparm;
    void *dataparm;
    babyFuncCB babyCallBackFunc;
}
BabyCallBack;

typedef struct
{
    LinkedList babies;
}CallBacks;

#define BABY_CallBackPeriod   1.0f / 16.0f

extern CallBacks callbacks;

void taskCallBackInit();
void taskCallBackShutDown();
BabyCallBack *taskCallBackRegister(babyFuncCB callback, udword num, void *data, real32 callintime);
void taskCallBackRemove(BabyCallBack *babytogobyebye);
// void taskCallBackProcess();

/*=============================================================================
    Data:
=============================================================================*/
//task structure pointers
extern taskdata *taskData[TSK_NumberTasks];     //pointers to active tasks

extern real32 taskTimeDelta;
extern real32 taskTimeElapsed;                  //time elapsed, in seconds, since program started

extern udword taskProcessIndex;                 //number of calls being executed on this task
extern sdword taskNumberCalls;                  //number of times to call the task

//speed of task timer, in Hz
extern real32 taskFrequency;

/*=============================================================================
    Macros:
=============================================================================*/
//if module not started properly, generate an error
#if TASK_ERROR_CHECKING
#define taskInitCheck()\
    if (taskModuleInit == FALSE)\
    {\
        dbgFatal(DBG_Loc, "Called before task module started or after closed.");\
    }
#else
#define taskInitCheck()
#endif

/* Macros for defining task functions.
 * A declaration is simply DECLARE_TASK(name);
 * A definition looks like
 * DEFINE_TASK(name)
 * {
 *     // Static variable definitions.
 *     taskVar;
 *     // Task-specific variable definitions.  No initializers or funny stuff.
 *     // In reality you are declaring struct members.
 *     taskProg(cvar);
 *     // Executable code.  The task-specific variables are accessible as
 *     // cvar->task_var.  They are malloced, hence not automatically
 *     // initialized.
 *     taskEnd;
 * }
 * "taskVar; taskProg(cvar);" may be replaced by "taskBegin;" if there are no
 * task-speicific variables.  All names beginning with "task" are reserved
 * for the task system implementation.  See also documentation/tasks.txt.
 */
#define DECLARE_TASK(name) void name(taskContext *)
#define DEFINE_TASK(name) void name(taskContext *taskContextPtr)
#define taskVar                                 \
    struct taskContextClass {                   \
        unsigned taskLine
#define taskProg(cont_var)                                              \
    } *cont_var = *taskContextPtr;                                      \
    if (!cont_var) {                                                    \
        cont_var = *taskContextPtr = malloc(sizeof(*cont_var));         \
        cont_var->taskLine = 0;                                         \
    }                                                                   \
    switch (cont_var->taskLine) {                                       \
    default:                                                            \
        dbgWarningf(__FILE__, __LINE__, "Corrupt task state: taskLine = \n", \
                    cont_var->taskLine);                                \
        break;                                                          \
    case 0:
#define taskBegin taskVar; taskProg(taskCurrentContext)
#define taskEnd                                                 \
    }                                                           \
    taskExit()

/* Macros for task continuation and exiting.  Used in the executable code
 * parts of task definitions, see above.
 */
// A yielded task resumes at the following statement on next invocation.
// n is not used currently.
#define taskYield(n)                                                    \
        ((struct taskContextClass *)*taskContextPtr)->taskLine = __LINE__; \
        return;                                                  \
    case __LINE__:
// An exited task gets removed from the task list.
#define taskExit()                              \
        free(*taskContextPtr);                  \
        *taskContextPtr = 0;                    \
        return

//rename the memory block associated with a task
#define taskRename(t, n)    memRename((void *)taskData[t], (n))

/*=============================================================================
    Functions:
=============================================================================*/
//start/close the task manager
sdword taskStartup(udword frequency);
void taskShutdown(void);

#define taskStart(function, period, flags) \
    taskStartName(function, #function, (period), (flags))
taskhandle taskStartName(taskfunction function, char *name,
			 real32 period, udword flags);
void taskPause(taskhandle handle);
void taskResume(taskhandle handle);
// Kill the task, removing it from the task list.
void taskStop(taskhandle handle);

// Pause/resume all tasks.  Previously paused tasks will not be resumed.
void taskFreezeAll(void);
void taskResumeAll(void);
void taskSavePauseStatus(void);

//execute all pending tasks
sdword taskExecuteAllPending(sdword ticks);

//adjust attributes of tasks
udword taskFrequencySet(taskhandle handle, udword frequency);

#endif
