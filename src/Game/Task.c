/*=============================================================================
    Name    : task.c
    Purpose : Functions for implementing a non-preemptive task manager.

    Created 6/18/1997 by lmoloney
    Copyright Relic Entertainment, Inc.  All rights reserved.
=============================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Debug.h"
#include "Memory.h"
#include "Demo.h"
#include "TitanInterfaceC.h"
#include "Task.h"

/*=============================================================================
    Data:
=============================================================================*/
//task structures
taskdata *taskData[TSK_NumberTasks];            //pointers to active tasks
sdword taskMaxTask;                             //highest indexed task enabled
sdword taskModuleInit = FALSE;

CallBacks callbacks;

//speed of task timer, in Hz
real32 taskFrequency;

//global handle of task currently being executed
taskhandle taskCurrentTask = -1;


real32 taskTimeDelta   = 0.0f;
real32 taskTimeElapsed = 0.0f;                  //time elapsed, in seconds, since program started

udword taskProcessIndex = 0;                    //number of calls being executed on this task
sdword taskNumberCalls;                         //number of times to call the task

/*=============================================================================
    Functions:
=============================================================================*/
/*-----------------------------------------------------------------------------
    Test task function
-----------------------------------------------------------------------------*/
#if TASK_TEST
DEFINE_TASK(taskTestFunction)
{
    taskVar;

    sdword index;
    sdword aba;

    taskProg(state);

    state->index = 300;
    state->aba = 24;

    taskYield(0);

    for (state->index = 0; state->index < 4; state->index++)
    {
        dbgMessagef("%s: Task test function called %d times.", __func__, index);
        aba++;
        taskYield(0);
    }
    taskEnd;
}
#endif //TASK_TEST

/*-----------------------------------------------------------------------------
    Name        : taskStartup
    Description : Initialize the task module.
    Inputs      : frequency - frequency, in Hz of the timer to be used.
    Outputs     : All taskData structures cleared
    Return      : OKAY on success, ERROR if failure
----------------------------------------------------------------------------*/
sdword taskStartup(udword frequency)
{
    sdword index;

#if TASK_VERBOSE_LEVEL >= 1
    dbgMessagef("%s: Task module started using a frequency of %dHz", __func__, frequency);
#endif

    dbgAssertOrIgnore(taskModuleInit == FALSE);

    for (index = 0; index < TSK_NumberTasks; index++)
    {
        taskData[index] = NULL;
    }

    taskMaxTask = 0;                                        //no tasks
    taskFrequency = (real32)frequency;                      //save the frequency
    taskModuleInit = TRUE;                                  //and say module init

    //call the task handler dispatcher once so that it will set return pointers OK
    taskExecuteAllPending(0);

#if TASK_TEST                                               //test this crazy module
    {
        taskhandle handle;
        handle = taskStart(taskTestFunction, 1, TF_OncePerFrame);
        taskRename(handle, "Task this you commie bastards!");
        taskExecuteAllPending(1);
        taskExecuteAllPending(1);
        taskExecuteAllPending(1);
        taskExecuteAllPending(1);
        taskExecuteAllPending(1);
        taskExecuteAllPending(1);
    }
#endif //TASK_TEST

    return(OKAY);
}

/*-----------------------------------------------------------------------------
    Name        : taskShutdown
    Description : Close the task manager by clearing all tasks.
    Inputs      : void
    Outputs     : Clears all task structures to unused and frees stack memory (if any)
    Return      : void
----------------------------------------------------------------------------*/
void taskShutdown(void)
{
    sdword index;

#if TASK_VERBOSE_LEVEL >= 1
    dbgMessage("taskClose: closing task module");
#endif

    if (!taskModuleInit)
    {
        return;
    }
    for (index = 0; index < taskMaxTask; index++)
    {
        if (taskData[index] != NULL)
        {
            if (taskData[index]->context != NULL)
                free(taskData[index]->context);
            //free structure and stack associated with this task
            memFree(taskData[index]);
            taskData[index] = NULL;
        }
    }
    taskModuleInit = FALSE;
}

/*-----------------------------------------------------------------------------
    Name        : taskPointerAlloc
    Description : Allocate a task pointer out of global task data list
    Inputs      : void
    Outputs     : sets allocated task pointer to -1
    Return      : index of newly allocated task
    Note        : generates a fatal error if no task found
----------------------------------------------------------------------------*/
sdword taskPointerAlloc(void)
{
    sdword index;

    for (index = 0; index < TSK_NumberTasks; index++)       //look for a free task
    {
        if (taskData[index] == NULL)                        //if free
        {
            taskData[index] = (void *)0xffffffff;
            taskMaxTask = max(taskMaxTask, index + 1);      //update max task if needed
            return(index);
        }
    }

#if TASK_ERROR_CHECKING
    dbgFatalf(DBG_Loc, "All %d task pointers in use.", taskMaxTask);
#endif
    return(ERROR);
}

/*-----------------------------------------------------------------------------
    Name        : taskStartName
    Description : Start a specific task
    Inputs      : function - entry point of task
                  name - a string describing the task for debugging
                  period - period between consecutive calls
                  stacksize - size of task's local stack
                  flags - control execution of task
    Outputs     : taskdata structure allocated and initialized
    Return      : handle to task for later manipulation of task
    Note        : if a taskdata structure or stack RAM cannot be allocated, the
                    function will generate a fatal error.
                  name must be caller-allocated and outlive the task.
                  This is usually invoked with the taskStart macro.
                  The task is executed once immediately and must yield
                  rather than exit at this first call.
----------------------------------------------------------------------------*/
taskhandle taskStartName(taskfunction function, char *name,
                         real32 period, udword flags)
{
    static taskhandle handle = ERROR;
    taskdata *newTask;
    
    taskInitCheck();
    dbgAssertOrIgnore(function != NULL);
    dbgAssertOrIgnore(period > 0.0f);

    newTask = memAlloc(sizeof(taskdata), "taskData", NonVolatile);
    handle = taskPointerAlloc();
    taskData[handle] = newTask;

#if TASK_VERBOSE_LEVEL >= 2
    dbgMessagef("%s: starting task at 0x%x at %d Hz, flags 0x%x using handle %d at 0x%x",
                __func__, function, 1.0f/period, flags, handle, taskData[handle]);
#endif

    //make task in use and running
    newTask->flags = flags | TF_Allocated;
    newTask->function = function;
    newTask->context = NULL;
    newTask->name = name;
    newTask->ticks = 0;         //no residual ticks
    dbgAssertOrIgnore(period * taskFrequency < (real32)SDWORD_Max);
    newTask->ticksPerCall = (udword)(period * taskFrequency);

    function(&newTask->context);

    if (newTask->context == NULL)
    {
        // Exited already?
#if TASK_ERROR_CHECKING
        dbgFatalf(DBG_Loc, "taskStart: stillborn task %s", name);
#endif
        taskStop(handle);
        return ERROR;
    }

    return handle;
}

/*-----------------------------------------------------------------------------
    Name        : taskStop
    Description : Destroy the specified task (stop and deallocate)
    Inputs      : handle - handle of task returned from taskStart()
    Outputs     : ..
    Return      : void
----------------------------------------------------------------------------*/
void taskStop(taskhandle handle)
{
    taskInitCheck();
    dbgAssertOrIgnore(handle >= 0);
    dbgAssertOrIgnore(handle < taskMaxTask);
    dbgAssertOrIgnore(taskData[handle] != NULL);

#if TASK_VERBOSE_LEVEL >= 2
    dbgMessagef("%s: destroying task %d", __func__, handle);
#endif

    if (taskData[handle]->context != NULL)
    {
        free(taskData[handle]->context);
    }
    memFree(taskData[handle]);
    taskData[handle] = NULL;
    if (handle == taskMaxTask - 1)
    {
        taskMaxTask--;
    }
}

/*-----------------------------------------------------------------------------
    Name        : taskExecuteAllPending
    Description : Execute all pending tasks
    Inputs      : ticks - number of system timer ticks since the last time called
    Outputs     : all pending tasks executed
    Return      : ?
    Note        : Because the stack is corrupted in this function, we can use
        no local variables or passed parameters.
----------------------------------------------------------------------------*/
static sdword tTicks, tLocalTicks;
sdword taskExecuteAllPending(sdword ticks)
{
#if TASK_VERBOSE_LEVEL >= 2
    if (ticks > 0)
    {
        dbgMessagef("%s: executing tasks for %d ticks", __func__, ticks);
    }
#endif

    /* Don't do a taskInitCheck because this sometimes gets called on
       exiting, after all the tasks are shut down. */
    if (taskModuleInit == FALSE)
    {
        return(OKAY);
    }
    //taskInitCheck();

    if (demDemoRecording)
    {
        demNumberTicksSave(ticks);
    }
    else if (demDemoPlaying)
    {
        ticks = demNumberTicksLoad(ticks);
    }

    taskTimeDelta = ticks / taskFrequency;
    taskTimeElapsed += taskTimeDelta;
    tTicks = ticks;             //save the ticks parameter

    // Verify we're not in any task currently.
    dbgAssertOrIgnore(taskCurrentTask == -1);

    for (taskCurrentTask = 0; taskCurrentTask < taskMaxTask; taskCurrentTask++)
    {
        if (taskData[taskCurrentTask] == NULL
            || bitTest(taskData[taskCurrentTask]->flags, TF_Paused))
            continue;

        if (TitanActive)
            titanPumpEngine();

        //calculate number of calls
        if (bitTest(taskData[taskCurrentTask]->flags, TF_OncePerFrame))
        {
/* Don't do this if check, because if you do it is possible that
   hundreds of other tasks will get executed and this task will starve
   when the game AI gets heavy.  Luke, later check if there's a better
   way to do this. */
/*
            if (taskData[taskCurrentTask]->ticks == 0)  //see if time to execute
            {
*/
            taskNumberCalls = 1; //flag to call once
            taskData[taskCurrentTask]->ticks =
                taskData[taskCurrentTask]->ticksPerCall;
/*
            }
*/
            taskData[taskCurrentTask]->ticks--;
        }
        else
        {                       //else it's a real-time task
            //add leftover ticks from last frame
            tLocalTicks = tTicks + taskData[taskCurrentTask]->ticks;
            //save leftover ticks for next frame
            taskData[taskCurrentTask]->ticks =
                tLocalTicks % taskData[taskCurrentTask]->ticksPerCall;
            taskNumberCalls =
                tLocalTicks / taskData[taskCurrentTask]->ticksPerCall;
#if TASK_MAX_TICKS
            taskNumberCalls = min(taskNumberCalls, TSK_MaxTicks);
#endif //TASK_MAX_TICKS
            }
#if TASK_VERBOSE_LEVEL >= 3
        if (taskNumberCalls)
        {
            dbgMessagef(
                "%s: task %d (%s) to run %d times, "
                "%d ticks elapsed / %d per call = %d",
                __func__,
                taskCurrentTask, taskData[taskCurrentTask]->name,
                taskNumberCalls,
                tLocalTicks, taskData[taskCurrentTask]->ticksPerCall,
                tLocalTicks / taskData[taskCurrentTask]->ticksPerCall);
        }
#endif
        taskProcessIndex++;
        for (; taskNumberCalls > 0; taskNumberCalls--)
        {
            taskData[taskCurrentTask]->function(
                &taskData[taskCurrentTask]->context);
            if (taskData[taskCurrentTask]->context == NULL) {
                // Task has exited.
                taskStop(taskCurrentTask);
                break;
            }
        }
    }
    taskCurrentTask = -1;       //not currently executing any tasks
    return(OKAY);
}

/*-----------------------------------------------------------------------------
    Name        : taskPause
    Description : Pauses selected task.
    Inputs      : handle - handle of task
    Outputs     : Sets the paused bit of specified taskData structure
    Return      : void
----------------------------------------------------------------------------*/
void taskPause(taskhandle handle)
{
    taskInitCheck();
    dbgAssertOrIgnore(handle >= 0);
    dbgAssertOrIgnore(handle < taskMaxTask);
    dbgAssertOrIgnore(taskData[handle] != NULL);

    bitSet(taskData[handle]->flags, TF_Paused);
}

/*-----------------------------------------------------------------------------
    Name        : taskResume
    Description : Resumes selected task.
    Inputs      : handle - handle of task
    Outputs     : Clears the paused bit of specified taskData structure
    Return      : void
----------------------------------------------------------------------------*/
void taskResume(taskhandle handle)
{
    taskInitCheck();
    dbgAssertOrIgnore(handle >= 0);
    dbgAssertOrIgnore(handle < taskMaxTask);
    dbgAssertOrIgnore(taskData[handle] != NULL);

    bitClear(taskData[handle]->flags, TF_Paused);
}

/*-----------------------------------------------------------------------------
    Name        : taskSavePauseStatus
    Description :
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void taskSavePauseStatus(void)
{
    sdword index;

    taskInitCheck();
    for (index = 0; index < taskMaxTask; index++)           //for all tasks
    {
        if (taskData[index] != NULL)                        //if task active
        {
            if (bitTest(taskData[index]->flags, TF_Paused)) //save the pause bit
            {
                bitSet(taskData[index]->flags, TF_PauseSave);
            }
            else
            {
                bitClear(taskData[index]->flags, TF_PauseSave);
            }
            // deliberately do not pause task, just make sure TF_PauseSave gets set correctly
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : taskFreezeAll
    Description : Freeze all tasks.
    Inputs      : void
    Outputs     : Copies paused bit into pause save bit, then sets pause bit
                    of all active tasks.
    Return      : void
----------------------------------------------------------------------------*/
void taskFreezeAll(void)
{
    sdword index;

    taskInitCheck();
    for (index = 0; index < taskMaxTask; index++)           //for all tasks
    {
        if (taskData[index] != NULL)                        //if task active
        {
            if (bitTest(taskData[index]->flags, TF_Paused)) //save the pause bit
            {
                bitSet(taskData[index]->flags, TF_PauseSave);
            }
            else
            {
                bitClear(taskData[index]->flags, TF_PauseSave);
            }
            bitSet(taskData[index]->flags, TF_Paused);      //pause this task
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : taskResumesAll
    Description : Resumes all previously unpaused tasks.
    Inputs      : void
    Outputs     : Copies pause save bit into paused bit.
    Return      : void
    Note        : It is up to the calling party to ensure that the next call to
                    taskExecuteAllPending() does not try to execute all the task
                    ticks which would have happended while the tasks were paused.
----------------------------------------------------------------------------*/
void taskResumeAll(void)
{
    sdword index;

    taskInitCheck();
    for (index = 0; index < taskMaxTask; index++)           //for all tasks
    {
        if (taskData[index] != NULL)                        //if task active
        {
            if (bitTest(taskData[index]->flags, TF_PauseSave)) //save the pause bit
            {
                bitSet(taskData[index]->flags, TF_Paused);
            }
            else
            {
                bitClear(taskData[index]->flags, TF_Paused);
            }
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : taskPeriodSet
    Description : Set frequency for task
    Inputs      : handle - handle of task to adjust
                  period - period of task
    Outputs     :
    Return      : old period
----------------------------------------------------------------------------*/
real32 taskPeriodSet(taskhandle handle, real32 period)
{
    real32 old;

    taskInitCheck();
    dbgAssertOrIgnore(handle >= 0);
    dbgAssertOrIgnore(handle < taskMaxTask);
    dbgAssertOrIgnore(taskData[handle] != NULL);
    old = (real32)taskData[handle]->ticksPerCall / taskFrequency;
    taskData[handle]->ticksPerCall = (udword)(period * taskFrequency);
    return(old);
}

////////////////
///  CALLBACK TASK HANDLING
////////////////

taskhandle babyTaskHandle = -1;

DECLARE_TASK(taskCallBackProcess);

void taskCallBackInit()
{
    listInit(&callbacks.babies);        //initialize linked list of babies
    babyTaskHandle = taskStart(taskCallBackProcess, BABY_CallBackPeriod, 0);
}

void taskCallBackShutDown()
{
    if (babyTaskHandle != -1)
    {
        taskStop(babyTaskHandle);
    }

    listDeleteAll(&callbacks.babies);
}

BabyCallBack *taskCallBackRegister(babyFuncCB callback, udword num,
                                   void *data, real32 callintime)
{
    BabyCallBack *baby;
    //(maybe do check of time to see if it is volatile?)
    baby = memAlloc(sizeof(BabyCallBack), "BabyCallBack", Volatile);
    baby->babyCallBackFunc = callback;
    baby->numparm = num;
    baby->dataparm = data;
    baby->timeToExecute = callintime;
    baby->timeStarted = taskTimeElapsed;
    listAddNode(&callbacks.babies, &baby->babylink,baby);
    return(baby);
}

void taskCallBackRemove(BabyCallBack *babytogobyebye)
{
    BabyCallBack *baby;
    Node *babynode;

    babynode = callbacks.babies.head;
    while(babynode != NULL)
    {
        baby = (BabyCallBack *) listGetStructOfNode(babynode);
        if(baby == babytogobyebye)
        {
            //returned TRUE, so delete
            listDeleteNode(babynode);
            return;
        }
        babynode = babynode->next;
    }
}

DEFINE_TASK(taskCallBackProcess)
{
    static BabyCallBack *baby;
    static Node *babynode,*tempnode;

    taskBegin;

    taskYield(0);

    while(1)
    {
        babynode = callbacks.babies.head;
        while(babynode != NULL)
        {
            baby = (BabyCallBack *) listGetStructOfNode(babynode);
            if(taskTimeElapsed - baby->timeStarted >= baby->timeToExecute)
            {
                //time elapsed so execute callback
                if((baby->babyCallBackFunc)(baby->numparm, baby->dataparm, baby))
                {
                    //returned TRUE, so delete
                    tempnode = babynode->next;
                    listDeleteNode(babynode);
                    babynode = tempnode;
                    continue;
                }
                //reset task time
                baby->timeStarted = taskTimeElapsed;
            }
            babynode = babynode->next;
        }
        //break;
        taskYield(0);
    }
    taskEnd;
}
