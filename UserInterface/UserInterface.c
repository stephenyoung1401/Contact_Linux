/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-17 21:00:13
 * @LastEditTime: 2019-03-25 16:34:10
 */
#include "UserInterface.h"
void clear_screen()
{
#ifndef DEBUG
    system("clear");
#endif // !DEBUG
}
//获取新的人员信息
int showUI_add_person(char *pszName, char *pszPhone, char *pszDescription)
{
    clear_screen();
    printf("enter:");
    printf("\nname: ");
    scanf("%s", pszName);

    printf("\nphone: ");
    scanf("%s", pszPhone);

    printf("\ndescription: ");
    scanf("%s", pszDescription);
    return UI_SUCCESS;
}

//对需要删除的人员信息做进一步确认
int showUI_delete_person(const SPersonInfo *c_pPerson, char *pcChoice)
{
    char szChoice[3];
    printf("person info : \n");
    print_person_info(c_pPerson);
    printf("\ny (delete) or n : ");
    scanf("%s", szChoice);
    *pcChoice = szChoice[0];
    return UI_SUCCESS;
}

//获取需要修改的人员信息
int showUI_modify_person(const SPersonInfo *c_pPerson, char *pszName, char *pszPhone, char *pszDescription)
{
    printf("person info : \n");
    print_person_info(c_pPerson);

    printf("enter:");
    printf("\nname: ");
    scanf("%s", pszName);

    printf("\nphone: ");
    scanf("%s", pszPhone);

    printf("\ndescription: ");
    scanf("%s", pszDescription);
    return UI_SUCCESS;
}

//模糊查询人员信息
int showUI_search_person(char *pszInfo)
{
    printf("\nenter info : ");
    scanf("%s", pszInfo);
    char readEnter;
    scanf("%c", &readEnter);
    printf("readEnter : %c", readEnter);
    return UI_SUCCESS;
}

int showUI_search_person_next(char *pszFunc)
{
    printf("Delete (d)\n");
    printf("Modify (m)\n");
    printf("Back to Main (b)\n");
    printf("enter a func : ");
    scanf("%c", pszFunc);
    return UI_SUCCESS;
}

//确定某指定人员，在局部链表中选定人员
// /pLinkedList为一个局部链表
int showUI_confirm_person(const SLinkedList *pLinkedList, int *piChoice)
{
    SLinkNode *pTmpNode = pLinkedList->pHeadNode;
    int l_iCnt = 0;
    while (pTmpNode != NULL)
    {
        printf("\n%d :", l_iCnt);
        if (pTmpNode->pData != NULL)
        {
            print_person_info((SPersonInfo *)pTmpNode->pData);
        }
        pTmpNode = pTmpNode->pNextNode;
        l_iCnt++;
    }

    printf("enter choice : ");
    scanf("%d", piChoice);
    DBG("%d\n", *piChoice);
    return UI_SUCCESS;
}

//显示指定的部分联系人，可以尝试写一个带连续翻页优化的函数，即使用静态变量实现
int showUI_part_person_list(const SLinkedList *c_pLinkedList, long lBeginIndex, long lEndIndex)
{
    long lTmpPos = 0;
    //need to depart data and data structure!!!!!!
    SLinkNode *pTmpNode = c_pLinkedList->pHeadNode;
    while (pTmpNode != NULL && lTmpPos < lBeginIndex)
    {
        pTmpNode = pTmpNode->pNextNode;
        lTmpPos++;
    }
    if (pTmpNode == NULL || lTmpPos != lBeginIndex)
    {
        return UI_INVALID_INPUT;
    }

    while (pTmpNode != NULL && lTmpPos < lEndIndex)
    {
        if (pTmpNode->pData != NULL)
        {
            printf("\n %ld : ", lTmpPos);
            print_person_info((SPersonInfo *)pTmpNode->pData);
        }
        pTmpNode = pTmpNode->pNextNode;
        lTmpPos++;
    }
    return UI_SUCCESS;
}

int showUI_main_command(char *pcFunCalled)
{
    printf("Up page (u) or Down page (d)\n");
    printf("Add person info (a)\n");
    printf("Search person (s)\n");
    printf("Quit (q)\n");
    printf("Delete or Modify should Search first\n");
    scanf("%c", pcFunCalled);
    return UI_SUCCESS;
}

int showUI_quit()
{
    clear_screen();
    printf("free space...\n");
    printf("returning...\n");
    return UI_SUCCESS;
}

#ifdef DEBUG_USER_INTERFACE
int main()
{
}

#endif // DEBUG_USER_INTERFACE
