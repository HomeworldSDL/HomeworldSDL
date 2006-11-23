// =============================================================================
//  LinkedList.c
// =============================================================================
//  Copyright Relic Entertainment, Inc. All rights reserved.
//  Created June 1997 by Gary Shaw
// =============================================================================

#include "LinkedList.h"

#include "Debug.h"
#include "Memory.h"
#include "SpaceObj.h"

/*=============================================================================
    Private Macros:
=============================================================================*/

#define COMPARE_COLLISION_DISTANCE(obj1,obj2)  \
    ( ((SpaceObj *)obj1)->collOptimizeDist > ((SpaceObj *)obj2)->collOptimizeDist )

#define COMPARE_CAMERA_DISTANCE(obj1,obj2)     \
    ( ((SpaceObj *)obj1)->cameraDistanceSquared < ((SpaceObj *)obj2)->cameraDistanceSquared )

/*=============================================================================
    Private Functions:
=============================================================================*/


/*-----------------------------------------------------------------------------
    Name        : listDetachNode
    Description : detaches a node from a linked list, without deleting it or
                  modifying its node members.
    Inputs      : node to detach
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void listDetachNode(Node *node)
{
    LinkedList *owner = node->belongto;
    Node *nextnode;
    Node *prevnode;

    dbgAssertOrIgnore(owner != NULL);
    dbgAssertOrIgnore(owner->head != NULL);
    dbgAssertOrIgnore(owner->tail != NULL);

    nextnode = node->next;
    prevnode = node->prev;

    if (owner->head == node)
    {
        owner->head = nextnode;
    }

    if (owner->tail == node)
    {
        owner->tail = prevnode;
    }

    if (nextnode != NULL)
    {
        nextnode->prev = prevnode;
    }

    if (prevnode != NULL)
    {
        prevnode->next = nextnode;
    }

    owner->num--;
}

/*=============================================================================
    Functions:
=============================================================================*/


/*-----------------------------------------------------------------------------
    Name        : listInit
    Description : initializes a linked list
    Inputs      : list
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void listInit(LinkedList *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->num = 0;
}

/*-----------------------------------------------------------------------------
    Name        : listRemoveAll
    Description : removes all nodes from a list, without deleting the nodes
    Inputs      : list
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void listRemoveAll(LinkedList *list)
{
    Node *curnode = list->head;
    Node *nextnode;

    while (curnode != NULL)
    {
        nextnode = curnode->next;

        ClearNodePtr(curnode);

        curnode = nextnode;
    }

    list->head = list->tail = NULL;
    list->num = 0;
}

/*-----------------------------------------------------------------------------
    Name        : listDeleteAll
    Description : deletes (frees memory) of all nodes in a list, and removes
                  these nodes from the list.
    Inputs      : list
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void listDeleteAll(LinkedList *list)
{
    Node *curnode = list->head;
    Node *nextnode;

    while (curnode != NULL)
    {
        nextnode = curnode->next;

        memFree(listGetStructOfNode(curnode));

        curnode = nextnode;
    }

    list->head = list->tail = NULL;
    list->num = 0;
}

/*-----------------------------------------------------------------------------
    Name        : listAddNode
    Description : adds a node to the end of a list
    Inputs      : list, node toadd, structure which contains node toadd
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void listAddNode(LinkedList *list,Node *toadd,void *structure)
{
    if (list->head == NULL)
    {
        dbgAssertOrIgnore(list->tail == NULL);

        list->head = toadd;
        list->tail = toadd;
        toadd->next = NULL;
        toadd->prev = NULL;
        toadd->belongto = list;
        toadd->structptr = structure;
    }
    else
    {
        Node *oldtailnode = list->tail;

        dbgAssertOrIgnore(oldtailnode != NULL);

        dbgAssertOrIgnore(oldtailnode->next == NULL);
        oldtailnode->next = toadd;

        toadd->next = NULL;
        toadd->prev = oldtailnode;
        toadd->belongto = list;
        toadd->structptr = structure;

        list->tail = toadd;
    }
    list->num++;
}

/*-----------------------------------------------------------------------------
    Name        : listAddNodeBeginning
    Description : adds a node to the beginning of a list
    Inputs      : list, node toadd, structure which contains node toadd
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void listAddNodeBeginning(LinkedList *list,Node *toadd,void *structure)
{
    if (list->head == NULL)
    {
        dbgAssertOrIgnore(list->tail == NULL);

        list->head = toadd;
        list->tail = toadd;
        toadd->next = NULL;
        toadd->prev = NULL;
        toadd->belongto = list;
        toadd->structptr = structure;
    }
    else
    {
        Node *oldheadnode = list->head;

        dbgAssertOrIgnore(oldheadnode != NULL);

        dbgAssertOrIgnore(oldheadnode->prev == NULL);
        oldheadnode->prev = toadd;

        toadd->next = oldheadnode;
        toadd->prev = NULL;
        toadd->belongto = list;
        toadd->structptr = structure;

        list->head = toadd;
    }
    list->num++;
}

/*-----------------------------------------------------------------------------
    Name        : listAddNodeAfter
    Description : inserts a node after another node
    Inputs      : insertion point node, toadd node, structure which contains toadd
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void listAddNodeAfter(Node *node,Node *toadd,void *structure)
{
    Node *nextnode;

    dbgAssertOrIgnore(node != NULL);
    nextnode = node->next;
    dbgAssertOrIgnore(node != nextnode);
    dbgAssertOrIgnore(node != toadd);
    dbgAssertOrIgnore(nextnode != toadd);

    node->next = toadd;

    toadd->prev = node;
    toadd->next = nextnode;
    toadd->belongto = node->belongto;
    toadd->structptr = structure;

    if (nextnode != NULL)
    {
        nextnode->prev = toadd;
    }
    else
    {
        node->belongto->tail = toadd;
    }
    node->belongto->num++;
}

/*-----------------------------------------------------------------------------
    Name        : listAddNodeBefore
    Description : inserts a node before another node
    Inputs      : insertion point node, toadd node, structure which contains toadd
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void listAddNodeBefore(Node *node,Node *toadd,void *structure)
{
    Node *prevnode;

    dbgAssertOrIgnore(node != NULL);
    prevnode = node->prev;

    if (prevnode != NULL)
    {
        prevnode->next = toadd;
    }
    else
    {
        node->belongto->head = toadd;
    }

    toadd->prev = prevnode;
    toadd->next = node;
    toadd->belongto = node->belongto;
    toadd->structptr = structure;

    node->prev = toadd;

    node->belongto->num++;
}

/*-----------------------------------------------------------------------------
    Name        : listMoveNodeToHead
    Description : Moves a linked list node to the beginning of the list it's in.
    Inputs      : nodeToMove - node to move to the start of the list.
    Outputs     :
    Return      : void
----------------------------------------------------------------------------*/
void listMoveNodeToHead(Node *nodeToMove)
{
    LinkedList *list;
    void *structure;

    dbgAssertOrIgnore(nodeToMove != NULL);
    list = nodeToMove->belongto;
    structure = listGetStructOfNode(nodeToMove);

    if (list->head == nodeToMove)
    {                                                       //if already at head of list
        return;                                             //nothing to do
    }
    listDetachNode(nodeToMove);
    if (list->head == NULL)
    {
        listAddNode(list, nodeToMove, structure);
    }
    else
    {
        listAddNodeBefore(list->head, nodeToMove, structure);
    }
}

/*-----------------------------------------------------------------------------
    Name        : listRemoveNode
    Description : removes a node from the list it belongs to, and clears the
                  next,prev,etc pointers of that node
    Inputs      : node
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void listRemoveNode(Node *node)
{
    listDetachNode(node);
    ClearNodePtr(node);
}

/*-----------------------------------------------------------------------------
    Name        : listDeleteNode
    Description : removes a node from the list, and frees its memory
    Inputs      : node
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void listDeleteNode(Node *node)
{
    listDetachNode(node);
    memFree(listGetStructOfNode(node));
}

/*-----------------------------------------------------------------------------
    Name        : listMoveContentsOfList
    Description : This function moves the entrie contents of the source list to the dest list.
    Inputs      : destination, source
    Outputs     : none
    Return      : void
----------------------------------------------------------------------------*/
void listMoveContentsOfList(LinkedList *dest, LinkedList *source)
{
    Node *walk;

    // copy list members to the new list
    dest->head  = source->head;
    dest->tail  = source->tail;
    dest->num   = source->num;

    // walk through the list and fix up all of the belongto pointers
    walk = dest->head;
    while (walk != NULL)
    {
        walk->belongto = dest;

        walk = walk->next;
    }

    listInit(source);
}

/*-----------------------------------------------------------------------------
    Name        : listMergeSort
    Description : sorts list from highest to lowest, using the mergesort
                  algorithm (order n*log(n))
    Inputs      : list to sort, compare callback function which should return
                  true if its second argument is > its first argument.
    Outputs     : list is sorted
    Return      :
----------------------------------------------------------------------------*/
void listMergeSort(LinkedList *list)
{
    LinkedList leftlist;
    LinkedList rightlist;

    Node *node1;
    Node *node2;

    udword i,half;
    Node *firstrightnode;
    Node *lastleftnode;

    Node *leftnodeadd;
    Node *rightnodeadd;
    Node *newleftnodeadd;
    Node *newrightnodeadd;

    if (list->num <= 1)
    {
        return;
    }
    else if (list->num == 2)
    {
        node1 = list->head;
        node2 = list->tail;

        if ( COMPARE_COLLISION_DISTANCE(listGetStructOfNode(node1),listGetStructOfNode(node2)) )
        {
            node1->prev = node2;
            node1->next = NULL;
            node2->next = node1;
            node2->prev = NULL;
            list->head = node2;
            list->tail = node1;
        }
        return;
    }
    else
    {
        half = list->num >> 1;

        firstrightnode = list->head;
        for (i=0;i<half;i++)
        {
            firstrightnode = firstrightnode->next;
        }

        lastleftnode = firstrightnode->prev;

        // break the list in half
        lastleftnode->next = NULL;
        firstrightnode->prev = NULL;

        leftlist.head = list->head;
        leftlist.tail = lastleftnode;
        leftlist.num = half;

        rightlist.head = firstrightnode;
        rightlist.tail = list->tail;
        rightlist.num = list->num - half;

        listMergeSort(&leftlist);
        listMergeSort(&rightlist);

        // Now merge the leftlist and rightlist into the list
        listInit(list);

        leftnodeadd = leftlist.head;
        rightnodeadd = rightlist.head;

        while ((leftnodeadd != NULL) || (rightnodeadd != NULL))
        {
            if (leftnodeadd == NULL)
            {
                goto addrightnode;
            }
            if (rightnodeadd == NULL)
            {
                goto addleftnode;
            }
            if (COMPARE_COLLISION_DISTANCE(listGetStructOfNode(leftnodeadd),listGetStructOfNode(rightnodeadd)))
            {
addrightnode:
                newrightnodeadd = rightnodeadd->next;
                listAddNode(list,rightnodeadd,listGetStructOfNode(rightnodeadd));
                rightnodeadd = newrightnodeadd;
            }
            else
            {
addleftnode:
                newleftnodeadd = leftnodeadd->next;
                listAddNode(list,leftnodeadd,listGetStructOfNode(leftnodeadd));
                leftnodeadd = newleftnodeadd;
            }
        }
    }
}

void listMergeSort2(LinkedList *list)
{
    LinkedList leftlist;
    LinkedList rightlist;

    Node *node1;
    Node *node2;

    udword i,half;
    Node *firstrightnode;
    Node *lastleftnode;

    Node *leftnodeadd;
    Node *rightnodeadd;
    Node *newleftnodeadd;
    Node *newrightnodeadd;

    if (list->num <= 1)
    {
        return;
    }
    else if (list->num == 2)
    {
        node1 = list->head;
        node2 = list->tail;

        if ( COMPARE_CAMERA_DISTANCE(listGetStructOfNode(node1),listGetStructOfNode(node2)) )
        {
            node1->prev = node2;
            node1->next = NULL;
            node2->next = node1;
            node2->prev = NULL;
            list->head = node2;
            list->tail = node1;
        }
        return;
    }
    else
    {
        half = list->num >> 1;

        firstrightnode = list->head;
        for (i=0;i<half;i++)
        {
            firstrightnode = firstrightnode->next;
        }

        lastleftnode = firstrightnode->prev;

        // break the list in half
        lastleftnode->next = NULL;
        firstrightnode->prev = NULL;

        leftlist.head = list->head;
        leftlist.tail = lastleftnode;
        leftlist.num = half;

        rightlist.head = firstrightnode;
        rightlist.tail = list->tail;
        rightlist.num = list->num - half;

        listMergeSort2(&leftlist);
        listMergeSort2(&rightlist);

        // Now merge the leftlist and rightlist into the list
        listInit(list);

        leftnodeadd = leftlist.head;
        rightnodeadd = rightlist.head;

        while ((leftnodeadd != NULL) || (rightnodeadd != NULL))
        {
            if (leftnodeadd == NULL)
            {
                goto addrightnode;
            }
            if (rightnodeadd == NULL)
            {
                goto addleftnode;
            }
            if (COMPARE_CAMERA_DISTANCE(listGetStructOfNode(leftnodeadd),listGetStructOfNode(rightnodeadd)))
            {
addrightnode:
                newrightnodeadd = rightnodeadd->next;
                listAddNode(list,rightnodeadd,listGetStructOfNode(rightnodeadd));
                rightnodeadd = newrightnodeadd;
            }
            else
            {
addleftnode:
                newleftnodeadd = leftnodeadd->next;
                listAddNode(list,leftnodeadd,listGetStructOfNode(leftnodeadd));
                leftnodeadd = newleftnodeadd;
            }
        }
    }
}
void listMergeSortGeneral(LinkedList *list, MergeSortCompareCb compare)
{
    LinkedList leftlist;
    LinkedList rightlist;

    Node *node1;
    Node *node2;

    udword i,half;
    Node *firstrightnode;
    Node *lastleftnode;

    Node *leftnodeadd;
    Node *rightnodeadd;
    Node *newleftnodeadd;
    Node *newrightnodeadd;

    if (list->num <= 1)
    {
        return;
    }
    else if (list->num == 2)
    {
        node1 = list->head;
        node2 = list->tail;

        if ( compare(listGetStructOfNode(node1),listGetStructOfNode(node2)) )
        {
            node1->prev = node2;
            node1->next = NULL;
            node2->next = node1;
            node2->prev = NULL;
            list->head = node2;
            list->tail = node1;
        }
        return;
    }
    else
    {
        half = list->num >> 1;

        firstrightnode = list->head;
        for (i=0;i<half;i++)
        {
            firstrightnode = firstrightnode->next;
        }

        lastleftnode = firstrightnode->prev;

        // break the list in half
        lastleftnode->next = NULL;
        firstrightnode->prev = NULL;

        leftlist.head = list->head;
        leftlist.tail = lastleftnode;
        leftlist.num = half;

        rightlist.head = firstrightnode;
        rightlist.tail = list->tail;
        rightlist.num = list->num - half;

        listMergeSortGeneral(&leftlist, compare);
        listMergeSortGeneral(&rightlist, compare);

        // Now merge the leftlist and rightlist into the list
        listInit(list);

        leftnodeadd = leftlist.head;
        rightnodeadd = rightlist.head;

        while ((leftnodeadd != NULL) || (rightnodeadd != NULL))
        {
            if (leftnodeadd == NULL)
            {
                goto addrightnode;
            }
            if (rightnodeadd == NULL)
            {
                goto addleftnode;
            }
            if (compare(listGetStructOfNode(leftnodeadd),listGetStructOfNode(rightnodeadd)))
            {
addrightnode:
                newrightnodeadd = rightnodeadd->next;
                listAddNode(list,rightnodeadd,listGetStructOfNode(rightnodeadd));
                rightnodeadd = newrightnodeadd;
            }
            else
            {
addleftnode:
                newleftnodeadd = leftnodeadd->next;
                listAddNode(list,leftnodeadd,listGetStructOfNode(leftnodeadd));
                leftnodeadd = newleftnodeadd;
            }
        }
    }
}

/*-----------------------------------------------------------------------------
    Name        : listInsertSort
    Description : does a modified insert sort (finds out of place objects, sorts them,
                  and then merges them into a final sorted list).
    Inputs      : list to sort
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
void listInsertSort(LinkedList *list)
{
    Node *prevprev = NULL;
    Node *prev = NULL;
    Node *cur = NULL;

    void *structprevprev;
    void *structprev;
    void *structcur;

    LinkedList outOfOrderList;

    Node *curNode;
    Node *outOfOrderNode;
    Node *outOfOrderNodeNext;
    void *structoutOfOrderNode;

    Node *node1;
    Node *node2;

    Node *curnext;

    if (list->num <= 1)
    {
        return;
    }
    else if (list->num == 2)
    {
        node1 = list->head;
        node2 = list->tail;

        if ( COMPARE_COLLISION_DISTANCE(listGetStructOfNode(node1),listGetStructOfNode(node2)) )
        {
            node1->prev = node2;
            node1->next = NULL;
            node2->next = node1;
            node2->prev = NULL;
            list->head = node2;
            list->tail = node1;
        }
        return;
    }
    else
    {
        listInit(&outOfOrderList);

        prevprev = list->head;
        prev = prevprev->next;
        cur = prev->next;

        structprevprev = listGetStructOfNode(prevprev);
        structprev = listGetStructOfNode(prev);

        // find out of order items in list, and put in outOfOrderList

        do
        {
            structcur = listGetStructOfNode(cur);

            if (COMPARE_COLLISION_DISTANCE(structprev,structprevprev))    // prev > prevprev
            {
                if (COMPARE_COLLISION_DISTANCE(structcur,structprev))     // cur > prev
                {
                    // everything in order, so continue
                    prevprev = prev;
                    structprevprev = structprev;

                    prev = cur;
                    structprev = structcur;

                    cur = cur->next;
                }
                else
                {
                    if (COMPARE_COLLISION_DISTANCE(structcur,structprevprev)) // cur > prevprev
                    {
                        // prev must be out of order, so remove it and put in outOfOrderList
                        listRemoveNode(prev);
                        listAddNode(&outOfOrderList,prev,structprev);

                        prev = cur;
                        structprev = structcur;

                        cur = cur->next;
                    }
                    else
                    {
                        // cur must be out of order, so remove it and put in outOfOrderList
                        curnext = cur->next;

                        listRemoveNode(cur);
                        listAddNode(&outOfOrderList,cur,structcur);

                        cur = curnext;
                    }
                }
            }
            else
            {
                if (COMPARE_COLLISION_DISTANCE(structcur,structprev))     // cur > prev
                {
                    // just prevprev is out of order
                    listRemoveNode(prevprev);
                    listAddNode(&outOfOrderList,prevprev,structprevprev);

                    prevprev = prev;
                    structprevprev = structprev;

                    prev = cur;
                    structprev = structcur;

                    cur = cur->next;
                }
                else
                {
                    // both prevprev and prev are out of order

                    listRemoveNode(prev);
                    listAddNode(&outOfOrderList,prev,structprev);

                    listRemoveNode(prevprev);
                    listAddNode(&outOfOrderList,prevprev,structprevprev);

                    prevprev = cur;
                    structprevprev = structcur;

                    prev = prevprev->next;
                    if (prev != NULL)
                    {
                        structprev = listGetStructOfNode(prev);

                        cur = prev->next;
                    }
                    else
                    {
                        break;
                    }
                }
            }

        } while (cur != NULL);

        // list is now in order, with out of order items in outOfOrderList

        // sort outOfOrderList:
        listMergeSort(&outOfOrderList);

        // now merge outOfOrderList into list (outOfOrderList is now in order)

        outOfOrderNode = outOfOrderList.head;
        curNode = list->head;

        while (outOfOrderNode != NULL)
        {
            // put outOfOrderNode into list
            structoutOfOrderNode = listGetStructOfNode(outOfOrderNode);
            outOfOrderNodeNext = outOfOrderNode->next;

            for (;;)
            {
                if (curNode == NULL)
                {
                    listAddNode(list,outOfOrderNode,structoutOfOrderNode);
                    break;
                }

                if (COMPARE_COLLISION_DISTANCE(listGetStructOfNode(curNode),structoutOfOrderNode))
                {
                    listAddNodeBefore(curNode,outOfOrderNode,structoutOfOrderNode);
                    break;
                }

                curNode = curNode->next;
            }

            outOfOrderNode = outOfOrderNodeNext;
        }

        // list is now sorted

        // don't bother doing a listRemoveAll on outOfOrderList because we are exiting

        return;
    }
}

#ifndef HW_Release
void listCheckSort(LinkedList *list)
{
    Node *node1;
    Node *node2;

    if (list->num <= 1)
    {
        return;
    }
    else if (list->num == 2)
    {
        node1 = list->head;
        node2 = list->tail;

        if ( COMPARE_COLLISION_DISTANCE(listGetStructOfNode(node1),listGetStructOfNode(node2)) )
        {
            dbgAssertOrIgnore(FALSE);
        }
    }
    else
    {
        node1 = list->head;
        node2 = node1->next;

        while (node2 != NULL)
        {
            if ( COMPARE_COLLISION_DISTANCE(listGetStructOfNode(node1),listGetStructOfNode(node2)) )
            {
                dbgAssertOrIgnore(FALSE);
            }

            node1 = node2;
            node2 = node2->next;
        }
    }
}

void listCheckSortGeneral(LinkedList *list, MergeSortCompareCb compare)
{
    Node *node1;
    Node *node2;

    if (list->num <= 1)
    {
        return;
    }
    else if (list->num == 2)
    {
        node1 = list->head;
        node2 = list->tail;

        if ( compare(listGetStructOfNode(node1),listGetStructOfNode(node2)) )
        {
            dbgAssertOrIgnore(FALSE);
        }
    }
    else
    {
        node1 = list->head;
        node2 = node1->next;

        while (node2 != NULL)
        {
            if ( compare(listGetStructOfNode(node1),listGetStructOfNode(node2)) )
            {
                dbgAssertOrIgnore(FALSE);
            }

            node1 = node2;
            node2 = node2->next;
        }
    }
}
#endif

/*-----------------------------------------------------------------------------
    Name        : listVerifyAnal
    Description : Verify that a linked list is valid.
    Inputs      :
    Outputs     :
    Return      :
----------------------------------------------------------------------------*/
#if LL_ERROR_CHECKING
void listVerifyAnal(LinkedList *list)
{
    sdword count, diff, firstDiff;
    Node *node;

    for (count = 0, node = list->head; node != NULL; node = node->next, count++)
    {
        //verify prev/next pointers
        dbgAssertOrIgnore(node->prev != node);
        dbgAssertOrIgnore(node->next != node);
        if (node->prev != NULL || node->next != NULL)
        {
            dbgAssertOrIgnore(node->prev != node->next);
        }
        if (count > 0)
        {
            dbgAssertOrIgnore(node->prev->next == node);
        }
        if (count < list->num - 1)
        {
            dbgAssertOrIgnore(node->next->prev == node);
        }
        //check the belongto pointer
        dbgAssertOrIgnore(node->belongto == list);
        //check the structptrs
        if (count == 0)
        {
            firstDiff = (ubyte *)node - (ubyte *)node->structptr;
            dbgAssertOrIgnore(firstDiff >= 0);
        }
        else
        {
            diff = (ubyte *)node - (ubyte *)node->structptr;
            dbgAssertOrIgnore(diff >= 0);
            dbgAssertOrIgnore(diff == firstDiff);
        }
        //check if the head/tail pointers are valid
        if (count == 0)
        {
            dbgAssertOrIgnore(node == list->head);
        }
        if (count == list->num - 1)
        {
            dbgAssertOrIgnore(node == list->tail);
        }
    }
    dbgAssertOrIgnore(count == list->num);
}
#endif
