/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-17 21:07:12
 * @LastEditTime: 2019-03-25 20:23:59
 */
#include "Functions.h"

extern int gs_port;
extern char gs_server_ip[NET_IP_LENGTH];

int subOptn_delete_person(SLinkedList *pLinkedList, SLinkNode *pLinkedNode)
{
    int iRetCode = 0;
    char lcChoice;
    SPersonInfo *pPerson = (SPersonInfo *)pLinkedNode->pData;
    iRetCode = showUI_delete_person(pPerson, &lcChoice);
    if (iRetCode != UI_SUCCESS)
    {
        return iRetCode;
    }
    else if (lcChoice != 'y' && lcChoice != 'n')
    {
        return FUNCTIONS_INVALID_INPUT;
    }
    else
    {
        // DBG("removeing ...\n");
        ASSERT_REPORT(pLinkedList != NULL);
        ASSERT_REPORT(pLinkedNode != NULL);

        linked_list_remove(pLinkedList, pLinkedNode);
        // DBG("removeing done ...\n");

#ifdef NET_ENABLED
        SNetDbOptn *pNetDbOptn;
        char lszFields[1][DB_SQL_FIELD_LENGTH] = {"name"};
        char lszParas[1][DB_SQL_PARA_LENGTH];

        strcpy(lszParas[0], pPerson->szName);

        net_create_dboptn(&pNetDbOptn, "delete", lszFields, 1, lszParas);
        DBG("try to connect %s:%d\n", gs_server_ip, gs_port);
        SNetCmdReq *pNetCmdReq;
        net_client_send_db_set_command(pNetDbOptn, &pNetCmdReq);
#endif // NET_ENABLED

#ifdef LOCAL_ENABLED
        SDbOperation *pDbOptn;

        char ppszDeleteFld[1][DB_SQL_FIELD_LENGTH] = {"name"};
        char ppszDeletePara[1][DB_SQL_PARA_LENGTH];
        strcpy(ppszDeletePara[0], pPerson->szName);
        database_create_dboptn(&pDbOptn, "delete", ppszDeleteFld, 1, ppszDeletePara);
        database_multi_delete(pDbOptn);
#endif // LOCAL_ENABLED
    }
    return FUNCTIONS_SUCCESS;
}

//调用showUI_modify_person，对节点的数据域做修改
int subOptn_modify_person(SLinkNode *pLinkedNode)
{
    int iRetCode = 0;
    char lszName[PERSON_MAX_NAME_LENGTH];
    char lszPhone[PERSON_MAX_PHONE_LENGTH];
    char lszDes[PERSON_MAX_DESCRIPTION_LENGTH];
    SPersonInfo *pPersonInfo = (SPersonInfo *)pLinkedNode->pData;
    iRetCode = showUI_modify_person(pPersonInfo, lszName, lszPhone, lszDes);

    strcpy(pPersonInfo->szName, lszName);
    strcpy(pPersonInfo->szPhone, lszPhone);
    strcpy(pPersonInfo->szDescription, lszDes);

#ifdef NET_ENABLED
    SNetDbOptn *pNetDbOptn;
    char lszFields[3][DB_SQL_FIELD_LENGTH] = {"name", "phone", "description"};
    char lszParas[3][DB_SQL_PARA_LENGTH];
    strcpy(lszParas[0], pPersonInfo->szName);
    strcpy(lszParas[1], pPersonInfo->szPhone);
    strcpy(lszParas[2], pPersonInfo->szDescription);

    net_create_dboptn(&pNetDbOptn, "replace", lszFields, 3, lszParas);
    DBG("try to connect %s:%d\n", gs_server_ip, gs_port);
    SNetCmdReq *pNetCmdReq;
    net_client_send_db_set_command(pNetDbOptn, &pNetCmdReq);
#endif // NET_ENABLED

#ifdef LOCAL_ENABLED
    SDbOperation *pDbOptn;
    char ppszInsertFld[3][DB_SQL_FIELD_LENGTH] = {"name", "phone", "description"};
    char ppszInsertPara[3][DB_SQL_PARA_LENGTH];
    strcpy(ppszInsertPara[0], pPersonInfo->szName);
    strcpy(ppszInsertPara[1], pPersonInfo->szPhone);
    strcpy(ppszInsertPara[2], pPersonInfo->szDescription);
    database_create_dboptn(&pDbOptn, "replace", ppszInsertFld, 3, ppszInsertPara);
    database_multi_insert(pDbOptn);

#endif // LOCAL_ENABLED
    return iRetCode == UI_SUCCESS ? FUNCTIONS_SUCCESS : iRetCode;
}

//模糊查询后，希望进行修改删除时，调用该函数，若模糊查询结果为 1 则也可以调用
int subOptn_confirm__person(SLinkedList *pLinkedList, SLinkNode **ppLinkedNode)
{
    if (pLinkedList == NULL)
    {
        DBG("confirm list null\n");
    }

    DBG("confirm list len: %lu\n", pLinkedList->dwLength);

    int iRetCode = 0;
    int liIndexChoosed = -1;
    iRetCode = showUI_confirm_person(pLinkedList, &liIndexChoosed);

    if (iRetCode != UI_SUCCESS || (liIndexChoosed < 0))
    {
        DBG("wrong confirm\n");
        return FUNCTIONS_INVALID_INPUT;
    }
    else if (ppLinkedNode != NULL)
    {
        linked_list_get(pLinkedList, ppLinkedNode, liIndexChoosed);
        return FUNCTIONS_SUCCESS;
    }
    else
    {
        DBG("ppLinkedNode null");
        return FUNCTIONS_INVALID_INPUT;
    }
}

//主界面下只有查询操作和添加操作
int optn_search_person(SLinkedList *pSrcLinkedList, SLinkedList **ppResLinkedList)
{
    int iRetCode = -1;
    char lszInfo[PERSON_MAX_INFO_LENGTH] = {0};
    iRetCode = showUI_search_person(lszInfo);
    DBG("lszInfo: %s\n", lszInfo);
    if (iRetCode != UI_SUCCESS || lszInfo[0] == '\0')
    {
        return FUNCTIONS_INVALID_INPUT;
    }
    else
    {
        // god ! make my own iterator !
        int iRetCode = 0;
        iRetCode = linked_list_create(ppResLinkedList);
        if (iRetCode == LINKED_LIST_SUCCESS)
        {
            SLinkedList *pResLinkedList = *ppResLinkedList;
            SListIterator *pListIter = NULL;
            linked_list_iterator(pSrcLinkedList, &pListIter);

            int cnt = 0;
            SLinkNode *pTmpNode;
            SPersonInfo *pPersonInfo;
            while (linked_list_has_next(pListIter) == LINKED_LIST_ITERATOR_HAS_NEXT)
            {

                linked_list_next(pListIter, &pTmpNode);
                pPersonInfo = (SPersonInfo *)pTmpNode->pData;
                iRetCode = regex_match_no_ret(pPersonInfo, lszInfo);
                if (iRetCode == REGEX_MATCH_SUCCESS)
                {
                    DBG("search success\n");
                    linked_list_insert(pResLinkedList, (void *)pPersonInfo, -1);
                }

                DBG("%d of %lu searching...\n", cnt, pSrcLinkedList->dwLength);
                cnt++;
            }

            if (pListIter != NULL)
            {
                free(pListIter);
            }

            DBG("search done \n");
            char lcFunc;
            showUI_search_person_next(&lcFunc);
            SLinkNode *pTmpNodeChoosed = NULL;
            switch (lcFunc)
            {
            case 'd':
                subOptn_confirm__person(pResLinkedList, &pTmpNodeChoosed);
                subOptn_delete_person(pSrcLinkedList, pTmpNodeChoosed);
                break;
            case 'm':
                subOptn_confirm__person(pResLinkedList, &pTmpNodeChoosed);
                subOptn_modify_person(pTmpNodeChoosed);
                break;
            case 'b':
                return FUNCTIONS_SEARCH_DONE;
                break;

            default:
                return FUNCTIONS_SEARCH_DONE;
                break;
            }
        }
        else
        {
            return iRetCode;
        }
    }
    return FUNCTIONS_SUCCESS;
}

//调用showUI_add_person并自动添加到链表中
int optn_add_person(SLinkedList *pLinkedList)
{
    char lszName[PERSON_MAX_NAME_LENGTH] = {0};
    char lszPhone[PERSON_MAX_PHONE_LENGTH] = {0};
    char lszDes[PERSON_MAX_DESCRIPTION_LENGTH] = {0};
    int iRetCode = 0;
    iRetCode = showUI_add_person(lszName, lszPhone, lszDes);
    if (iRetCode == UI_SUCCESS)
    {
        SPersonInfo *pNewPerson = NULL;
        iRetCode = create_person(&pNewPerson, lszName, lszPhone, lszDes);
        if (iRetCode == PERSON_OPERATION_SUCCESS)
        {
            iRetCode = linked_list_insert(pLinkedList, (void *)pNewPerson, -1);
            iRetCode = FUNCTIONS_SUCCESS;
        }

#ifdef NET_ENABLED
        SNetDbOptn *pNetDbOptn;
        char lszFields[3][DB_SQL_FIELD_LENGTH] = {"name", "phone", "description"};
        char lszParas[3][DB_SQL_PARA_LENGTH];
        strcpy(lszParas[0], pNewPerson->szName);
        strcpy(lszParas[1], pNewPerson->szPhone);
        strcpy(lszParas[2], pNewPerson->szDescription);

        net_create_dboptn(&pNetDbOptn, "replace", lszFields, 3, lszParas);
        DBG("try to connect %s:%d\n", gs_server_ip, gs_port);
        SNetCmdReq *pNetCmdReq;
        net_client_send_db_set_command(pNetDbOptn, &pNetCmdReq);

#endif // NET_ENABLED
#ifdef LOCAL_ENABLED
        SDbOperation *pDbOptn;
        char ppszInsertFld[3][DB_SQL_FIELD_LENGTH] = {"name", "phone", "description"};
        char ppszInsertPara[3][DB_SQL_PARA_LENGTH];
        strcpy(ppszInsertPara[0], pNewPerson->szName);
        strcpy(ppszInsertPara[1], pNewPerson->szPhone);
        strcpy(ppszInsertPara[2], pNewPerson->szDescription);
        database_create_dboptn(&pDbOptn, "replace", ppszInsertFld, 3, ppszInsertPara);
        database_multi_insert(pDbOptn);

#endif // LOCAL_ENABLED
        return iRetCode;
    }
    return FUNCTIONS_SUCCESS;
}

//主进程
int optn_main(SLinkedList *pLinkedList)
{
    int iRetCode = 0;

    char lcFunc;
    long llBeginIndex = 0;
    int liWindowSize = 5; //number of list will show of
    SLinkedList *pResLinkedList = NULL;
    while (1)
    {

        showUI_part_person_list(pLinkedList, llBeginIndex, llBeginIndex + liWindowSize);
        showUI_main_command(&lcFunc);
        switch (lcFunc)
        {
        case 'a':
            optn_add_person(pLinkedList);
            break;

        case 's':
            optn_search_person(pLinkedList, &pResLinkedList);
            break;

        case 'q':
            optn_quit(pLinkedList);
            break;

        case 'u':
            llBeginIndex += liWindowSize;
            if (llBeginIndex >= pLinkedList->dwLength)
            {
                llBeginIndex = 0;
            }

            break;

        case 'd':
            llBeginIndex -= liWindowSize;
            if (llBeginIndex < 0)
            {
                llBeginIndex = pLinkedList->dwLength - 5;
            }

            break;

        default:
            break;
        }
    }
    return iRetCode;
}

//退出时需要进行内存回收
int optn_quit(SLinkedList *pLinkedList)
{
    showUI_quit();
    if (pLinkedList != NULL)
    {
        linked_list_free(pLinkedList);
    }
#ifdef NET_ENABLED
    net_close_client();
#endif
    exit(0);
}

#ifdef DEBUG_FUNCTIONS
int main()
{
    SLinkedList *pLinkedList;
    char lszDbName[CONFIG_MAX_FILE_NAME_LENGTH];
// #define NET_ENABLED
#ifdef NET_ENABLED
    DBG("NET_ENABLED\n");
    char lszServerIP[NET_IP_LENGTH];
    int iPortNum = 9090;
    config_get_string_value("Server", "IP", "127.0.0.1", lszServerIP, NET_IP_LENGTH, "config.ini");
    config_get_int_value("Server", "Port", 9090, "config.ini", &iPortNum);
    gs_port = iPortNum;
    strcpy(gs_server_ip, lszServerIP);
    DBG("config : %s:%d\n", lszServerIP, iPortNum);

    // set test
    SNetDbOptn *pNetDbOptn;
    char lszFields[2][DB_SQL_FIELD_LENGTH] = {"", ""};
    char lszParas[2][DB_SQL_PARA_LENGTH] = {"", ""};

    net_create_dboptn(&pNetDbOptn, "replace", lszFields, 2, lszParas);

    // DBG("try to connect %s:%d\n", gs_server_ip, gs_port);
    SNetCmdReq *pNetCmdReq;
    net_client_send_db_set_command(pNetDbOptn, &pNetCmdReq);

    //get
    SNetDbOptn *pNetDbOptn_get;
    char lszFields_get[2][DB_SQL_FIELD_LENGTH] = {"name"};
    char lszParas_get[2][DB_SQL_PARA_LENGTH] = {"ste"};

    net_create_dboptn(&pNetDbOptn_get, "select", lszFields_get, 2, lszParas_get);

    cJSON *pjNetDbGet;
    net_dboptn2json(pNetDbOptn_get, &pjNetDbGet);
    char *pszNetDbGet = cJSON_Print(pjNetDbGet);
    DBG("%s\n", pszNetDbGet);

    net_client_send_db_get_command(pNetDbOptn_get, &pLinkedList);
    if (pNetDbOptn_get != NULL)
    {
        net_dboptn_free(pNetDbOptn_get);
        cJSON_Delete(pjNetDbGet);
        free(pszNetDbGet);
    }
#endif // NET_ENABLED

#ifdef LOCAL_ENABLED
    DBG("LOCAL_ENABLED\n");
    char lszDbFileName[DB_FILE_NAME_LENGTH];
    config_get_string_value("Database", "File", "contact.db", lszDbFileName, DB_FILE_NAME_LENGTH, "config.ini");
    int iRetCode = databae_open(lszDbFileName);

    if (iRetCode != DB_OPERATION_SUCCESS)
    {
        printf("open failed\n");
        return 1;
    }
    else
    {
        DBG("opened %s\n", lszDbFileName);
    }

    SDbOperation *pDbOptnGet;
    database_create_dboptn(&pDbOptnGet, "select", NULL, 0, NULL);
    iRetCode = database_multi_get(pDbOptnGet, &pLinkedList);

    //show
    if (iRetCode == DB_OPERATION_SUCCESS)
    {
        SListIterator *pListItrerator = NULL;
        SLinkNode *pNode;
        SPersonInfo *pPerson;
        int index = 0;
        linked_list_iterator(pLinkedList, &pListItrerator);

        while (linked_list_has_next(pListItrerator) == LINKED_LIST_ITERATOR_HAS_NEXT)
        {
            linked_list_next(pListItrerator, &pNode);
            pPerson = (SPersonInfo *)(pNode->pData);
            printf("%d : %s   %s   %s\n", index++,
                   pPerson->szName, pPerson->szPhone, pPerson->szDescription);
        }
        // DBG("display done\n");
        if (pListItrerator != NULL)
        {
            free(pListItrerator);
        }

        if (pPerson != NULL)
        {
            pPerson = NULL;
        }
    }
    if (pDbOptnGet != NULL)
    {
        database_dboptn_free(pDbOptnGet);
    }
#endif // LOCAL_ENABLED
    DBG("\n\nentering main");
    if (pLinkedList == NULL)
    {
        printf("--------------------disconnected--------------\n");
    }
    else
    {
        optn_main(pLinkedList);
    }
}
#endif // DEBUG_FUNCTIONS