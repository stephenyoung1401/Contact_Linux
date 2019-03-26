/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-19 21:37:00
 * @LastEditTime: 2019-03-20 17:00:42
 */
// #define DEBUG_LINKED_LIST

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "../../Debug/Debug.h"


typedef struct _Link_Node
{
    void *pData;
    struct _Link_Node *pPreNode; // 使用双向链表存储
    struct _Link_Node *pNextNode;
} SLinkNode;

typedef struct _Linked_List
{
    unsigned long dwLength;
    struct _Link_Node *pHeadNode;
    struct _Link_Node *pTailNode;
} SLinkedList;

typedef struct _List_Iterator
{
    struct _Link_Node *pCurNode;
} SListIterator;

#define LINKED_LIST_SUCCESS 0
#define LINKED_LIST_FAILED -1

#define LINKED_LIST_ALLOCATE_FAILED -2
#define LINKED_LIST_INDEX_OVERFLOW -3
#define LINKED_LIST_DATA_NULL -4
#define LINKED_LIST_NULL -5
#define LINKED_LIST_NODE_NULL -6

#define LINKED_LIST_ITERATOR_NO_NEXT -7
#define LINKED_LIST_ITERATOR_HAS_NEXT 0

#define LINKED_LIST_ITERATOR_NULL -8

int linked_list_iterator(SLinkedList *pLinkedList, SListIterator **ppListIter);

int linked_list_next(SListIterator *pListIter, SLinkNode **ppNextNode);

int linked_list_has_next(SListIterator *pListIter);

int linked_list_create(SLinkedList **ppLinkedList);

int linked_list_insert(SLinkedList *pLinkedList, void *c_pData, const long c_lIndex);

int linked_list_remove_at(SLinkedList *pLinkedList, void **ppDataRet, const long c_lIndex);

int linked_list_remove(SLinkedList *pLinkedList, SLinkNode *pNodeRemoved);

int linked_list_get_at(SLinkedList *pLinkedList, void **ppDataRet, const long c_lIndex);

int linked_list_get(SLinkedList *pLinkedList, SLinkNode **pNodeRet, const long c_lIndex);

int linked_list_index_of(SLinkedList *pLinkedList, const void *c_pData, long *plIndex);

int linked_list_clear(SLinkedList *pLinkedList);

int linked_list_free(SLinkedList *pLinkedList);

int linked_list_merge(SLinkedList *pDst_LinkedList, SLinkedList *pSrc_LinkedList);

int linked_list_display(SLinkedList *pLinkedList);
#endif