/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-20 16:56:44
 * @LastEditTime: 2019-03-25 16:24:33
 */
#include "Database_v2.h"
extern sqlite3 *gs_pdb_connected;
extern char gs_pc_fileName[DB_FILE_NAME_LENGTH];
extern unsigned char gs_byConnFlg;


int multi_get_callback(void *para, int columnCount,
                       char **columnValues, char **columnNames);

int databae_open(const char *c_pszFileName)
{
    if (c_pszFileName == NULL)
    {
        return DB_FILE_NAME_NULL;
    }
    else
    {
        strcpy(gs_pc_fileName, c_pszFileName);
    }

    int iRetCode = sqlite3_open(c_pszFileName, &gs_pdb_connected);
    gs_byConnFlg = iRetCode == SQLITE_OK ? DB_CONNECTED : DB_DISCONNECTED;
    return iRetCode == SQLITE_OK ? DB_OPERATION_SUCCESS : DB_OPEN_FAILED;
}

//关闭连接
int database_close()
{
    if (gs_pdb_connected == NULL)
    {
        return DB_OPEN_FAILED;
    }

    int iRetCode = sqlite3_close(gs_pdb_connected);

    if (iRetCode != SQLITE_OK)
    {
        return DB_CLOSE_FAILED;
    }

    if (gs_pc_fileName != NULL)
    {
        gs_pdb_connected = NULL;
    }

    return DB_OPERATION_SUCCESS;
}

int database_multi_get(const SDbOperation *c_pDbOptn, SLinkedList **ppLinkedListRet)
{
    if (gs_byConnFlg != DB_CONNECTED)
    {
        DBG("database disconnected or no inited\n");
        return DB_OPEN_FAILED;
    }

    if (strcmp(c_pDbOptn->szHead, "select") != 0)
    {
        return DB_OPTN_WRONG_CMD;
    }

    int iRetCode = 0;
    iRetCode = linked_list_create(ppLinkedListRet);
    if (iRetCode != LINKED_LIST_SUCCESS)
    {
        return iRetCode;
    }

    SLinkedList *pLinkedList = *ppLinkedListRet;
    char lszSqlCommand[DB_SQL_COMMAND_LENGTH];
    char *pszErrMsg;

    sprintf(lszSqlCommand, "Select * From person;");
    DBG("%s\n", lszSqlCommand);

    iRetCode = sqlite3_exec(gs_pdb_connected, lszSqlCommand, multi_get_callback, pLinkedList, &pszErrMsg);
    if (iRetCode == SQLITE_OK)
    {

        return DB_OPERATION_SUCCESS;
    }
    else
    {
        DBG("%s\n", pszErrMsg);
        return DB_SELECT_FAILED;
    }
}

//按序号，批量读取
int database_multi_get_range(const int c_iBeginIndex, const int c_iEndIndex,
                             SLinkedList **ppLinkedListRet)
{
    if (gs_byConnFlg != DB_CONNECTED)
    {
        DBG("database disconnected or no inited\n");
        return DB_OPEN_FAILED;
    }

    int iRetCode = 0;
    iRetCode = linked_list_create(ppLinkedListRet);
    if (iRetCode != LINKED_LIST_SUCCESS)
    {
        return iRetCode;
    }

    SLinkedList *pLinkedList = *ppLinkedListRet;
    char lszSqlCommand[DB_SQL_COMMAND_LENGTH];
    char *pszErrMsg;

    sprintf(lszSqlCommand, "Select * From person Order by name limit %d , %d;", c_iBeginIndex, c_iEndIndex);
    DBG("%s\n", lszSqlCommand);

    iRetCode = sqlite3_exec(gs_pdb_connected, lszSqlCommand, multi_get_callback, pLinkedList, &pszErrMsg);
    if (iRetCode == SQLITE_OK)
    {

        return DB_OPERATION_SUCCESS;
    }
    else
    {
        DBG("%s\n", pszErrMsg);
        return DB_SELECT_FAILED;
    }
}

int multi_get_callback(void *para, int columnCount,
                       char **columnValues, char **columnNames)
{
    SLinkedList *pLinkedList = (SLinkedList *)para;
    SPersonInfo *pNewPerson = NULL;

    int iRetCode = create_person(&pNewPerson, NULL, NULL, NULL);
    if (iRetCode != PERSON_OPERATION_SUCCESS)
    {
        return iRetCode;
    }
    else
    {
        for (int index = 0; index < columnCount; index++)
        {
            // DBG("%s\n", columnValues[index]);
            switch (columnNames[index][0])
            {
            case 'n':
                strcpy(pNewPerson->szName, columnValues[index]);
                break;
            case 'p':
                strcpy(pNewPerson->szPhone, columnValues[index]);
                break;
            case 'd':
                strcpy(pNewPerson->szDescription, columnValues[index]);
                break;

            default:
                break;
            }
        }
        iRetCode = linked_list_insert(pLinkedList, (void *)pNewPerson, -1);

        return SQLITE_OK;
    }
}

//按序号，以模糊查找方式读取
int database_multi_get_like(const int c_iBeginIndex, const int c_iEndIndex,
                            const char *pszPharse, const char *c_pszInfo,
                            SLinkedList **ppLinkedListRet)

{
    if (gs_byConnFlg != DB_CONNECTED)
    {
        DBG("database disconnected or no inited\n");
        return DB_OPEN_FAILED;
    }

    int iRetCode = 0;
    iRetCode = linked_list_create(ppLinkedListRet);
    if (iRetCode != LINKED_LIST_SUCCESS)
    {
        return iRetCode;
    }

    SLinkedList *pLinkedList = *ppLinkedListRet;
    char lszSqlCommand[DB_SQL_COMMAND_LENGTH];
    char *pszErrMsg;

    sprintf(lszSqlCommand,
            "Select * From person Where %s like '%%%s%%' Order by name limit %d , %d;",
            pszPharse, c_pszInfo, c_iBeginIndex, c_iEndIndex);
    DBG("%s\n", lszSqlCommand);

    iRetCode = sqlite3_exec(gs_pdb_connected, lszSqlCommand, multi_get_callback, pLinkedList, &pszErrMsg);
    if (iRetCode == SQLITE_OK)
    {

        return DB_OPERATION_SUCCESS;
    }
    else
    {
        DBG("%s\n", pszErrMsg);
        return DB_SELECT_FAILED;
    }
}

//批量修改
//only support one para or none
int database_multi_modify(const SDbOperation *c_pDbOptn)
{
    if (c_pDbOptn == NULL)
    {
        return DB_OPTN_NULL;
    }
    if (strcmp(c_pDbOptn->szHead, "modify"))
    {
        return DB_OPTN_WRONG_CMD;
    }

    if (gs_byConnFlg != DB_CONNECTED)
    {
        DBG("database disconnected or no inited\n");
        return DB_OPEN_FAILED;
    }

    char lszSqlCommand[DB_SQL_COMMAND_LENGTH];
    char *pszErrMsg;
    if (c_pDbOptn->iFieldsCnt == 1)
    {
        sprintf(lszSqlCommand,
                "Update person Set %s = '%s';",
                c_pDbOptn->pszFields[0], c_pDbOptn->pszParas[0]);
    }
    else if (c_pDbOptn->iFieldsCnt == 2)
    {
        sprintf(lszSqlCommand,
                "Update person Set %s = '%s' Where %s = '%s' ;",
                c_pDbOptn->pszFields[0], c_pDbOptn->pszParas[0],
                c_pDbOptn->pszFields[1], c_pDbOptn->pszParas[1]);
    }
    else
    {
        DBG("pDbOpt->iFieldsCnt invalid : %d\n", c_pDbOptn->iFieldsCnt);
        return DB_MODIFY_FAILED;
    }

    DBG("%s\n", lszSqlCommand);
    int iRetCode = sqlite3_exec(gs_pdb_connected, lszSqlCommand, NULL, NULL, &pszErrMsg);
    if (iRetCode != SQLITE_OK)
    {
        DBG("%s\n", pszErrMsg);
        return iRetCode;
    }
    if (pszErrMsg != NULL)
    {
        free(pszErrMsg);
    }
    return DB_OPERATION_SUCCESS;
}

//批量删除
int database_multi_delete(const SDbOperation *c_pDbOptn)
{
    if (c_pDbOptn == NULL)
    {
        return DB_OPTN_NULL;
    }
    if (strcmp(c_pDbOptn->szHead, "delete") != 0)
    {
        return DB_OPTN_WRONG_CMD;
    }

    if (gs_byConnFlg != DB_CONNECTED)
    {
        DBG("database disconnected or no inited\n");
        return DB_OPEN_FAILED;
    }

    char lszSqlCommand[DB_SQL_COMMAND_LENGTH];
    char *pszErrMsg;

    if (c_pDbOptn->iFieldsCnt == 1)
    {
        sprintf(lszSqlCommand,
                "Delete From person Where %s = '%s';",
                c_pDbOptn->pszFields[0], c_pDbOptn->pszParas[0]);
    }
    else
    {
        DBG("c_pDbOptn->iFieldsCnt invalid : %d\n", c_pDbOptn->iFieldsCnt);
        return DB_DELETE_FAILED;
    }

    DBG("%s\n", lszSqlCommand);
    int iRetCode = sqlite3_exec(gs_pdb_connected, lszSqlCommand, NULL, NULL, &pszErrMsg);
    if (iRetCode != SQLITE_OK)
    {
        DBG("%s\n", pszErrMsg);
        return iRetCode;
    }
    if (pszErrMsg != NULL)
    {
        free(pszErrMsg);
    }
    return DB_OPERATION_SUCCESS;
}

//批量添加
int database_multi_insert(const SDbOperation *c_pDbOptn)
{
    if (c_pDbOptn == NULL)
    {
        return DB_OPTN_NULL;
    }
    if (strcmp(c_pDbOptn->szHead, "replace"))
    {
        return DB_OPTN_WRONG_CMD;
    }

    if (gs_byConnFlg != DB_CONNECTED)
    {
        DBG("database disconnected or no inited\n");
        return DB_OPEN_FAILED;
    }

    char lszSqlCommand[DB_SQL_COMMAND_LENGTH];
    char *pszErrMsg;

    if (c_pDbOptn->iFieldsCnt == 1)
    {
        sprintf(lszSqlCommand,
                "Replace Into person ('%s') Values ('%s') ;",
                c_pDbOptn->pszFields[0], c_pDbOptn->pszParas[0]);
    }
    else if (c_pDbOptn->iFieldsCnt == 2)
    {
        sprintf(lszSqlCommand,
                "Replace Into person ('%s' , '%s') Values ('%s' , '%s') ;",
                c_pDbOptn->pszFields[0], c_pDbOptn->pszFields[1],
                c_pDbOptn->pszParas[0], c_pDbOptn->pszParas[1]);
    }
    else if (c_pDbOptn->iFieldsCnt == 3)
    {
        sprintf(lszSqlCommand,
                "Replace Into person ('%s', '%s', '%s') Values ('%s', '%s', '%s') ;",
                c_pDbOptn->pszFields[0], c_pDbOptn->pszFields[1], c_pDbOptn->pszFields[2],
                c_pDbOptn->pszParas[0], c_pDbOptn->pszParas[1], c_pDbOptn->pszParas[2]);
    }
    else
    {
        DBG("c_pDbOptn->iFieldsCnt invalid : %d\n", c_pDbOptn->iFieldsCnt);
        return DB_DELETE_FAILED;
    }

    DBG("%s\n", lszSqlCommand);
    int iRetCode = sqlite3_exec(gs_pdb_connected, lszSqlCommand, NULL, NULL, &pszErrMsg);
    if (iRetCode != SQLITE_OK)
    {
        DBG("%s\n", pszErrMsg);
        return iRetCode;
    }
    if (pszErrMsg != NULL)
    {
        free(pszErrMsg);
    }

    return DB_OPERATION_SUCCESS;
}

//2 diamond will become to 1 diamond
int database_create_dboptn(SDbOperation **ppDbOptn, const char *c_pszHead,
                           const char (*ppFields)[DB_SQL_FIELD_LENGTH], const int c_iFieldCnt,
                           const char (*ppParas)[DB_SQL_PARA_LENGTH])
{
    if (ppDbOptn == NULL)
    {
        return DB_OPTN_NULL;
    }
    else
    {
        *ppDbOptn = (SDbOperation *)malloc(sizeof(SDbOperation));
        if (*ppDbOptn == NULL)
        {
            return DB_OPTN_ALLC_FAILED;
        }

        SDbOperation *pDbOptn = *ppDbOptn;
        pDbOptn->iFieldsCnt = c_iFieldCnt;
        pDbOptn->iTupleCnt;
        strcpy(pDbOptn->szHead, c_pszHead);
        for (int i = 0; i < c_iFieldCnt; i++)
        {
            pDbOptn->pszFields[i] = (char *)malloc(sizeof(char) * DB_SQL_FIELD_LENGTH);
            // DBG("pDbOptn->pszFields[i] == NULL %d\n", pDbOptn->pszFields[i] == NULL);
            memset(pDbOptn->pszFields[i], 0, sizeof(sizeof(char) * DB_SQL_FIELD_LENGTH));
            strcpy(pDbOptn->pszFields[i], ppFields[i]);

            pDbOptn->pszParas[i] = (char *)malloc(sizeof(char) * DB_SQL_PARA_LENGTH);
            memset(pDbOptn->pszParas[i], 0, sizeof(sizeof(char) * DB_SQL_PARA_LENGTH));
            strcpy(pDbOptn->pszParas[i], ppParas[i]);
        }

        return DB_OPERATION_SUCCESS;
    }
}

int database_dboptn_free(SDbOperation *pDbOptn)
{
    if (pDbOptn == NULL)
    {
        return DB_OPTN_NULL;
    }
    else
    {
        int lcntFields = pDbOptn->iFieldsCnt;
        for (int i = 0; i < lcntFields; i++)
        {
            if (pDbOptn->pszFields[i] != NULL)
            {
                free(pDbOptn->pszFields[i]);
            }
            if (pDbOptn->pszParas[i] != NULL)
            {
                free(pDbOptn->pszParas[i]);
            }
        }
        if (pDbOptn != NULL)
        {
            free(pDbOptn);
        }

        return DB_OPERATION_SUCCESS;
    }
}

int database_dboptn_strjson(SDbOperation *pDbOptn, char *pszJsonStr, const int c_iOutput)
{
    char lszDbOptnStr[DB_OPTN_LENGTH_JSON];
    memset(lszDbOptnStr, 0, sizeof(lszDbOptnStr));
    sprintf(lszDbOptnStr,
            "{\
        \"head\":\"%s\",\
        \"Fields\":\
        {\
            \"%s\",\
            \"%s\",\
            \"%s\"\
        },\
        \"Paras\":\
        {\
            \"%s\",\
            \"%s\",\
            \"%s\",\
        },\
        \"FieldCnt\":%d ,\
        \"TupleCnt\":%d\
    }",
            pDbOptn->szHead,
            pDbOptn->iFieldsCnt > 0 ? pDbOptn->pszFields[0] : "NULL",
            pDbOptn->iFieldsCnt > 1 ? pDbOptn->pszFields[1] : "NULL",
            pDbOptn->iFieldsCnt > 2 ? pDbOptn->pszFields[2] : "NULL",
            pDbOptn->iFieldsCnt > 0 ? pDbOptn->pszParas[0] : "NULL",
            pDbOptn->iFieldsCnt > 1 ? pDbOptn->pszParas[1] : "NULL",
            pDbOptn->iFieldsCnt > 2 ? pDbOptn->pszParas[2] : "NULL",
            pDbOptn->iFieldsCnt,
            pDbOptn->iTupleCnt);

    strncpy(pszJsonStr, lszDbOptnStr, c_iOutput);
    return DB_OPERATION_SUCCESS;
}

#ifdef DEBUG_DATABASE_V2

int print_dboptn(SDbOperation *pDboptn)
{
    printf("%s  --(%d)\n", pDboptn->szHead, pDboptn->iFieldsCnt);
    for (int i = 0; i < pDboptn->iFieldsCnt; i++)
    {
        printf("%s  %s\n", pDboptn->pszFields[i], pDboptn->pszParas[i]);
    }
    DBG("print done\n");
    return 0;
}
int main()
{
    int iRetCode = databae_open("contact.db");
    if (iRetCode != DB_OPERATION_SUCCESS)
    {
        printf("open failed\n");
        return 1;
    }

    //insert test
    SDbOperation *pDbOptn;

    // char ppszInsertFld[3][DB_SQL_FIELD_LENGTH] = {"name", "phone", "description"};
    // char ppszInsertPara[3][DB_SQL_PARA_LENGTH] = {"stephen la", "9999999", "lalalala"};
    // database_create_dboptn(&pDbOptn, "delete", ppszDeleteFld, 3, ppszDeletePara);
    // database_create_dboptn(&pDbOptn, "replace", ppszDeleteFld, 3, ppszDeletePara);
    // database_multi_delete(pDbOptn);

    char ppszDeleteFld[3][DB_SQL_FIELD_LENGTH] = {"name"};
    char ppszDeletePara[3][DB_SQL_PARA_LENGTH] = {"stephen la"};
    database_create_dboptn(&pDbOptn, "delete", ppszDeleteFld, 1, ppszDeletePara);
    database_multi_delete(pDbOptn);

    char lszJsonStr[512];
    database_dboptn_strjson(pDbOptn, lszJsonStr, 512);
    printf("\n%s\n", lszJsonStr);

    print_dboptn(pDbOptn);

    database_dboptn_free(pDbOptn);

    //get test
    SLinkedList *pLinkList;
    // iRetCode = database_multi_get_range(0, 9, &pLinkList);
    // iRetCode = database_multi_get_like(0, 9, "name", "n", &pLinkList);
    SDbOperation *pDbOptnGet;
    database_create_dboptn(&pDbOptnGet, "select", NULL, 0, NULL);

    iRetCode = database_multi_get(pDbOptnGet, &pLinkList);

    DBG("pLinkList->dwLength = %ld\n", pLinkList->dwLength);

    if (iRetCode == DB_OPERATION_SUCCESS)
    {
        SListIterator *pListItrerator = NULL;
        SLinkNode *pNode;
        SPersonInfo *pPerson;
        int index = 0;
        linked_list_iterator(pLinkList, &pListItrerator);

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
        if (pNode != NULL)
        {
            free(pNode);
        }
        if (pPerson != NULL)
        {
            pPerson = NULL;
        }

        linked_list_free(pLinkList);
    }

    database_close();
}
#endif // DEBUG_DATABASE_V2