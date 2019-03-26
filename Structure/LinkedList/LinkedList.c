/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-20 11:28:25
 * @LastEditTime: 2019-03-22 01:19:04
 */
#include "LinkedList.h"

int linked_list_create(SLinkedList **ppLinkedList)
{
    *ppLinkedList = (SLinkedList *)malloc(sizeof(SLinkedList));
    SLinkedList *pLinkedList = *ppLinkedList;
    if (pLinkedList == NULL)
    {
        DBG("link list alloc failed\n");
        return LINKED_LIST_ALLOCATE_FAILED;
    }

    pLinkedList->dwLength = 0;
    pLinkedList->pHeadNode = NULL;
    pLinkedList->pTailNode = NULL;
    return LINKED_LIST_SUCCESS;
}

int linked_list_insert(SLinkedList *pLinkedList, void *c_pData, const long c_lIndex)
{
    if (pLinkedList == NULL)
    {
        return LINKED_LIST_NULL;
    }
    if (c_lIndex < -1 || (c_lIndex > pLinkedList->dwLength && c_lIndex != -1))
    {
        return LINKED_LIST_INDEX_OVERFLOW;
    }

    //initialize new node
    SLinkNode *pNewNode = (SLinkNode *)malloc(sizeof(SLinkNode));
    pNewNode->pData = c_pData;
    pNewNode->pPreNode = NULL;
    pNewNode->pNextNode = NULL;
    if (pNewNode == NULL)
    {
        return LINKED_LIST_ALLOCATE_FAILED;
    }

    if (pLinkedList->dwLength == 0)
    {
        pLinkedList->pHeadNode = pNewNode;
        pLinkedList->pTailNode = pNewNode;
        pLinkedList->dwLength = 1;
    }
    else if (c_lIndex == -1) //insert append to tail
    {
        SLinkNode *pTailNode = pLinkedList->pTailNode;
        pNewNode->pPreNode = pTailNode;
        pTailNode->pNextNode = pNewNode;
        pLinkedList->pTailNode = pNewNode;
        pLinkedList->dwLength++;
    }
    else if (c_lIndex == 0) //insert before head
    {
        SLinkNode *pHeadNode = pLinkedList->pHeadNode;
        pHeadNode->pPreNode = pNewNode;
        pNewNode->pNextNode = pHeadNode;
        pLinkedList->pHeadNode = pNewNode;
        pLinkedList->dwLength++;
    }
    else
    {
        SLinkNode *pTmpNode = pLinkedList->pHeadNode;
        long lTmpPos = 0;

        while (lTmpPos < c_lIndex)
        {
            DBG("pData  %d\n (index %ld of length %ld)\n",
                (*(int *)pTmpNode->pData), c_lIndex, pLinkedList->dwLength);

            pTmpNode = pTmpNode->pNextNode;
            lTmpPos++;
        }

        //this pTmpNode must have a preNode
        SLinkNode *pPreTmpNode = pTmpNode->pPreNode;
        pNewNode->pNextNode = pTmpNode;
        pNewNode->pPreNode = pPreTmpNode;
        pPreTmpNode->pNextNode = pNewNode;
        pTmpNode->pPreNode = pNewNode;
        pLinkedList->dwLength++;
    }

    return LINKED_LIST_SUCCESS;
}

int linked_list_get_at(SLinkedList *pLinkedList, void **ppDataRet, const long c_lIndex)
{
    if (pLinkedList == NULL)
    {
        return LINKED_LIST_NULL;
    }

    //need to confirm nagetive index number
    if (c_lIndex < -1 || (c_lIndex > pLinkedList->dwLength && c_lIndex != -1))
    {
        return LINKED_LIST_INDEX_OVERFLOW;
    }

    if (c_lIndex == -1)
    {
        *ppDataRet = (void *)pLinkedList->pTailNode->pData;
        return LINKED_LIST_SUCCESS;
    }

    long lTmpPos = 0;
    SLinkNode *pTmpNode = pLinkedList->pHeadNode;
    while (lTmpPos < c_lIndex)
    {
        pTmpNode = pTmpNode->pNextNode;
        lTmpPos++;
    }
    *ppDataRet = (void *)pTmpNode->pData;

    return LINKED_LIST_SUCCESS;
}

int linked_list_get(SLinkedList *pLinkedList, SLinkNode **ppNodeRet, const long c_lIndex)
{
    if (pLinkedList == NULL)
    {
        return LINKED_LIST_NULL;
    }

    //need to confirm nagetive index number
    if (c_lIndex < -1 || (c_lIndex > pLinkedList->dwLength && c_lIndex != -1))
    {
        return LINKED_LIST_INDEX_OVERFLOW;
    }

    if (c_lIndex == -1)
    {
        *ppNodeRet = pLinkedList->pTailNode;
        return LINKED_LIST_SUCCESS;
    }

    long lTmpPos = 0;
    SLinkNode *pTmpNode = pLinkedList->pHeadNode;
    while (lTmpPos < c_lIndex)
    {
        pTmpNode = pTmpNode->pNextNode;
        lTmpPos++;
    }
    *ppNodeRet = pTmpNode;

    return LINKED_LIST_SUCCESS;
}

int linked_list_index_of(SLinkedList *pLinkedList, const void *c_pData, long *plIndex)
{
    if (pLinkedList == NULL)
    {
        return LINKED_LIST_NULL;
    }
    if (c_pData == NULL)
    {
        return LINKED_LIST_DATA_NULL;
    }

    long lTmpPos = 0;
    SLinkNode *pTmpNode = pLinkedList->pHeadNode;
    while (pTmpNode->pData != c_pData)
    {
        pTmpNode = pTmpNode->pNextNode;
        lTmpPos++;
    }
    *plIndex = lTmpPos;

    return LINKED_LIST_SUCCESS;
}

int linked_list_remove_at(SLinkedList *pLinkedList, void **ppDataRet, const long c_lIndex)
{
    if (pLinkedList == NULL)
    {
        return LINKED_LIST_NULL;
    }

    //need to confirm nagetive index number
    if (c_lIndex < -1 || (c_lIndex > pLinkedList->dwLength && c_lIndex != -1))
    {
        return LINKED_LIST_INDEX_OVERFLOW;
    }

    if (c_lIndex == -1)
    {
        *ppDataRet = (void *)pLinkedList->pTailNode->pData;

        SLinkNode *pTailNode = pLinkedList->pTailNode;
        pLinkedList->pTailNode = pTailNode->pPreNode;
        pLinkedList->pTailNode->pNextNode = NULL;
        pTailNode->pPreNode = NULL;
        free(pTailNode);
        pLinkedList->dwLength--;
        return LINKED_LIST_SUCCESS;
    }

    long lTmpPos = 0;
    SLinkNode *pTmpNode = pLinkedList->pHeadNode;
    while (lTmpPos < c_lIndex)
    {
        pTmpNode = pTmpNode->pNextNode;
        lTmpPos++;
    }
    *ppDataRet = (void *)pTmpNode->pData;

    //delete the node choosed
    //the only node in the list
    if (pTmpNode->pPreNode == NULL && pTmpNode->pNextNode == NULL)
    {
        free(pTmpNode);
        pLinkedList->dwLength--;
    }
    //pTmpNode is head
    else if (pTmpNode->pPreNode == NULL)
    {
        SLinkNode *pHeadNode = pLinkedList->pHeadNode;
        pLinkedList->pHeadNode = pHeadNode->pNextNode;
        pLinkedList->pHeadNode->pPreNode = NULL;
        pLinkedList->dwLength--;

        pHeadNode->pPreNode = NULL;
        pHeadNode->pNextNode = NULL;
        pHeadNode->pData = NULL;
        free(pHeadNode);
    }
    // pTmpNode is tail
    else if (pTmpNode->pNextNode == NULL)
    {
        SLinkNode *pTailNode = pLinkedList->pTailNode;
        pLinkedList->pTailNode = pTailNode->pPreNode;
        pLinkedList->pTailNode->pNextNode = NULL;
        pLinkedList->dwLength--;

        pTailNode->pPreNode = NULL;
        pTailNode->pNextNode = NULL;
        pTailNode->pData = NULL;
        free(pTailNode);
    }
    else
    {
        SLinkNode *pPreTmpNode = pTmpNode->pPreNode;
        SLinkNode *pSufTmpNode = pTmpNode->pNextNode;
        pPreTmpNode->pNextNode = pSufTmpNode;
        pSufTmpNode->pPreNode = pPreTmpNode;

        pTmpNode->pPreNode = NULL;
        pTmpNode->pNextNode = NULL;
        pTmpNode->pData = NULL;
        free(pTmpNode);

        pLinkedList->dwLength--;
    }

    return LINKED_LIST_SUCCESS;
}

int linked_list_remove(SLinkedList *pLinkedList, SLinkNode *pNodeRemoved)
{
    if (pLinkedList == NULL)
    {
        return LINKED_LIST_NULL;
    }
    if (pNodeRemoved == NULL)
    {
        return LINKED_LIST_NODE_NULL;
    }

    long lIndex;
    linked_list_index_of(pLinkedList, (void *)pNodeRemoved->pData, &lIndex);
    void *pDataRet;
    linked_list_remove_at(pLinkedList, &pDataRet, lIndex);

    if (pDataRet != NULL)
    {
        free(pDataRet);
    }

    if (pNodeRemoved->pData != NULL)
    {
        free(pNodeRemoved->pData);
        pNodeRemoved->pData = NULL;
    }
    if (pNodeRemoved != NULL)
    {
        free(pNodeRemoved);
    }

    return LINKED_LIST_SUCCESS;
}

int linked_list_clear(SLinkedList *pLinkedList)
{
    if (pLinkedList == NULL)
    {
        return LINKED_LIST_NULL;
    }
    SLinkNode *pTmpNode = pLinkedList->pTailNode;
    while (pTmpNode != NULL)
    {
        pLinkedList->pTailNode = pTmpNode->pPreNode;
        free(pTmpNode->pData);
        free(pTmpNode);
        pTmpNode = pLinkedList->pTailNode;
    }

    pLinkedList->pTailNode = NULL;
    pLinkedList->pHeadNode = NULL;
    pLinkedList->dwLength = 0;

    return LINKED_LIST_SUCCESS;
}

int linked_list_free(SLinkedList *pLinkedList)
{
    int iRetValue = linked_list_clear(pLinkedList);
    if (iRetValue == LINKED_LIST_SUCCESS)
    {
        free(pLinkedList);
        return LINKED_LIST_SUCCESS;
    }
    else if (iRetValue == LINKED_LIST_NULL)
    {
        return LINKED_LIST_NULL;
    }
    else
    {
        return LINKED_LIST_FAILED;
    }
}

int linked_list_merge(SLinkedList *pDst_LinkedList, SLinkedList *pSrc_LinkedList)
{
    pDst_LinkedList->pTailNode->pNextNode = pSrc_LinkedList->pHeadNode;
    pSrc_LinkedList->pHeadNode->pPreNode = pDst_LinkedList->pTailNode;
    pDst_LinkedList->dwLength += pSrc_LinkedList->dwLength;
    // free(pSrc_LinkedList);
    return LINKED_LIST_SUCCESS;
}

//ppListIter will point to a null node before the headNode
int linked_list_iterator(SLinkedList *pLinkedList, SListIterator **ppListIter)
{
    if (pLinkedList == NULL)
    {
        return LINKED_LIST_NULL;
    }
    *ppListIter = (SListIterator *)malloc(sizeof(SListIterator));
    if (*ppListIter == NULL)
    {
        return LINKED_LIST_ALLOCATE_FAILED;
    }
    else
    {
        SListIterator *pListIter = *ppListIter;
        // (*ppListIter)->pCurNode = pLinkedList->pHeadNode;
        SLinkNode *pIterHeadNode = (SLinkNode *)malloc(sizeof(SLinkNode));
        pIterHeadNode->pData = NULL;
        pIterHeadNode->pNextNode = pLinkedList->pHeadNode;
        pIterHeadNode->pPreNode = NULL;
        (*ppListIter)->pCurNode = pIterHeadNode;

        return LINKED_LIST_SUCCESS;
    }
}

int linked_list_next(SListIterator *pListIter, SLinkNode **ppNextNode)
{
    if (pListIter == NULL)
    {
        return LINKED_LIST_ITERATOR_NULL;
    }
    if (pListIter->pCurNode->pNextNode != NULL)
    {
        *ppNextNode = pListIter->pCurNode->pNextNode;
        pListIter->pCurNode = pListIter->pCurNode->pNextNode;
        return LINKED_LIST_SUCCESS;
    }
    else
    {
        *ppNextNode = NULL;
        return LINKED_LIST_ITERATOR_NO_NEXT;
    }
}

int linked_list_has_next(SListIterator *pListIter)
{
    if (pListIter == NULL)
    {
        return LINKED_LIST_ITERATOR_NULL;
    }
    // if (pListIter->pCurNode == NULL)
    // {
    //     return LINKED_LIST_ITERATOR_NO_NEXT;
    // }
    if (pListIter->pCurNode->pNextNode != NULL)
    {
        return LINKED_LIST_ITERATOR_HAS_NEXT;
    }
    else
    {
        return LINKED_LIST_ITERATOR_NO_NEXT;
    }
}

#ifdef DEBUG_LINKED_LIST
typedef struct _test_data
{
    int data;
} STestData;

int linked_list_display(SLinkedList *pLinkedList);

int main()
{
    SLinkedList *pLinkedList;
    linked_list_create(&pLinkedList);
    STestData *pTData;
    int index = 0;

    DBG("for loop\n");
    for (index = 0; index < 100; index++)
    {
        pTData = (STestData *)malloc(sizeof(STestData));
        pTData->data = index;
        linked_list_insert(pLinkedList, pTData, -1);
    }
    DBG("for loop end\n");

    pTData = (STestData *)malloc(sizeof(STestData));
    pTData->data = 10;
    linked_list_insert(pLinkedList, pTData, 2);

    pTData = (STestData *)malloc(sizeof(STestData));
    pTData->data = 45;
    linked_list_insert(pLinkedList, pTData, 5);
    DBG("random insert end\n");

    STestData *pDataRmv;
    linked_list_remove_at(pLinkedList, (void *)&pDataRmv, -1);
    printf("pDataRmv %d\n", pDataRmv->data);
    free(pDataRmv);

    long pos;
    linked_list_index_of(pLinkedList, (void *)pTData, &pos);
    printf("pos %ld\n", pos);

    SLinkNode *pNodeGet;
    linked_list_get(pLinkedList, &pNodeGet, 10);
    printf("linked_list_get %d\n", ((STestData *)pNodeGet->pData)->data);

    linked_list_remove(pLinkedList, pNodeGet);
    // DBG("");

    linked_list_display(pLinkedList);

    // DBG("display done\n");

    STestData *pDataRet;
    linked_list_get_at(pLinkedList, (void *)&pDataRet, 10);
    printf("pDataRet %d\n", pDataRet->data);

    STestData *pDataRet1;
    linked_list_get_at(pLinkedList, (void *)&pDataRet1, pLinkedList->dwLength - 1);
    printf("pDataRet1 %d\n", pDataRet1->data);

    SLinkNode *pNode;
    linked_list_get(pLinkedList, &pNode, 22);
    STestData *pDataRet2 = pNode->pData;
    printf("pNode %d\n", pDataRet2->data);

    free(pDataRet);
    free(pDataRet1);
    free(pNode);
    free(pNodeGet);

    linked_list_free(pLinkedList);
    return 0;
}
int linked_list_display(SLinkedList *pLinkedList)
{
    // SLinkNode *pTmpNode = pLinkedList->pHeadNode;
    // int index = 0;
    // while (pTmpNode != NULL)
    // {
    //     // DBG("");
    //     printf("%d  %d\n", index++, *((int *)pTmpNode->pData));
    //     pTmpNode = pTmpNode->pNextNode;
    // }

    SListIterator *pListItrerator;
    int index = 0;
    linked_list_iterator(pLinkedList, &pListItrerator);
    while (linked_list_has_next(pListItrerator) == LINKED_LIST_ITERATOR_HAS_NEXT)
    {
        SLinkNode *pNode;
        linked_list_next(pListItrerator, &pNode);
        printf("%d : %d\n", index++, ((STestData *)pNode->pData)->data);
    }

    return LINKED_LIST_SUCCESS;
}
#endif // DEBUG_LINKED_LIST