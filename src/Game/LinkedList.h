// =============================================================================
//  LinkedList.h
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created June 1997 by Gary Shaw
// =============================================================================

#ifndef ___LINKEDLIST_H
#define ___LINKEDLIST_H

#include "Types.h"

// INTERFACE -------------------------------------------------------------------


#ifndef HW_Release
    #define LL_ERROR_CHECKING       1           // general error checking
    #define LL_ANAL_CHECKING        0           // extra-anal error checking
    #define LL_VERBOSE_LEVEL        2           // control specific output code
#else
    #define LL_ERROR_CHECKING       0           // general error checking
    #define LL_ANAL_CHECKING        0           // extra-anal error checking
    #define LL_VERBOSE_LEVEL        0           // control specific output code
#endif


typedef struct Node
{
    struct Node *next;
    struct Node *prev;
    struct LinkedList *belongto;
    void *structptr;     //  pointer to beginning of structure containing this node
} Node;

typedef struct LinkedList
{
    Node *head;
    Node *tail;
    udword num;
} LinkedList;

//  this function should return true if firststruct > secondstruct for ascending order
typedef bool (*MergeSortCompareCb)(void *firststruct,void *secondstruct);


#define listGetStructOfNode(node)       (void *)((node)->structptr)
#define listGetStructOfNonPtrNode(node) (void *)((node).structptr)

#define ClearNode(node)                     \
        (node).next = NULL;                 \
        (node).prev = NULL;                 \
        (node).belongto = NULL;             \
        (node).structptr = NULL;

#define ClearNodePtr(node)                  \
        (node)->next = NULL;                \
        (node)->prev = NULL;                \
        (node)->belongto = NULL;            \
        (node)->structptr = NULL;


#if LL_ANAL_CHECKING
    #define listVerify(list)    listVerifyAnal(list)
#else
    #define listVerify(list)
#endif


#ifndef HW_Release
void listCheckSort(LinkedList *list);
void listCheckSortGeneral(LinkedList *list, MergeSortCompareCb compare);
#endif

#if LL_ERROR_CHECKING
void listVerifyAnal(LinkedList *list);
#endif


void listInit(LinkedList *list);
void listRemoveAll(LinkedList *list);
void listDeleteAll(LinkedList *list);
void listAddNode(LinkedList *list,Node *toadd,void *structure);             //  add node to end of list by default
void listAddNodeBeginning(LinkedList *list,Node *toadd,void *structure);    //  add node to beginning of list
void listAddNodeAfter(Node *node,Node *toadd,void *structure);
void listAddNodeBefore(Node *node,Node *toadd,void *structure);
void listMoveNodeToHead(Node *nodeToMove);
void listRemoveNode(Node *node);
void listDeleteNode(Node *node);
void listMoveContentsOfList(LinkedList *dest, LinkedList *source);
void listMergeSort(LinkedList *list);
void listInsertSort(LinkedList *list);
void listMergeSort2(LinkedList *list);
void listMergeSortGeneral(LinkedList *list, MergeSortCompareCb compare);


#endif