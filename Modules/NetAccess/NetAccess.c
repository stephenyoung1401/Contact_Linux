/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-20 19:29:57
 * @LastEditTime: 2019-03-25 20:21:49
 */

#include "NetAccess.h"

extern int gs_port;
extern char gs_server_ip[NET_IP_LENGTH];

extern int gs_sockfd;
extern int bConnFlg;

int net_dboptn2json(SNetDbOptn *pNetDbOptn, cJSON **ppDbJson)
{
    if (pNetDbOptn == NULL)
    {
        return NET_NULL_INPUT;
    }

    *ppDbJson = cJSON_CreateObject();
    cJSON *pRoot = *ppDbJson;

    cJSON_AddNumberToObject(pRoot, "iFieldCnt", pNetDbOptn->iFieldCnt);
    cJSON_AddNumberToObject(pRoot, "iTupleCnt", pNetDbOptn->iTupleCnt);
    cJSON_AddStringToObject(pRoot, "head", pNetDbOptn->szHead);

    cJSON *pField = cJSON_CreateObject();
    for (int i = 0; i < pNetDbOptn->iFieldCnt; i++)
    {
        char lszFld[10];
        memset(lszFld, 0, sizeof(lszFld));
        sprintf(lszFld, "field%d", i + 1);
        cJSON_AddStringToObject(pField, lszFld, pNetDbOptn->pszField[i]);
    }

    cJSON *pParam = cJSON_CreateObject();
    for (int i = 0; i < pNetDbOptn->iFieldCnt; i++)
    {
        char lszPara[10];
        memset(lszPara, 0, sizeof(lszPara));
        sprintf(lszPara, "para%d", i + 1);
        cJSON_AddStringToObject(pParam, lszPara, pNetDbOptn->pszPara[i]);
    }

    cJSON_AddItemToObject(pRoot, "field", pField);
    cJSON_AddItemToObject(pRoot, "para", pParam);

    return NET_JSON_SUCCESS;
}
int net_json2dboptn(SNetDbOptn **ppNetDbOptn, cJSON *pDbJson)
{
    if (pDbJson == NULL)
    {
        return NET_NULL_INPUT;
    }

    *ppNetDbOptn = (SNetDbOptn *)malloc(sizeof(SNetDbOptn));
    SNetDbOptn *pNetDbOptn = *ppNetDbOptn;
    if (*ppNetDbOptn == NULL)
    {
        return NET_ALLOC_FAILED;
    }

    cJSON *pjHead = cJSON_GetObjectItem(pDbJson, "head");
    memset(pNetDbOptn->szHead, 0, sizeof(pNetDbOptn->szHead));
    strcpy(pNetDbOptn->szHead, pjHead->valuestring);

    cJSON *pjFieldCnt = cJSON_GetObjectItem(pDbJson, "iFieldCnt");
    cJSON *pjTupleCnt = cJSON_GetObjectItem(pDbJson, "iTupleCnt");
    pNetDbOptn->iFieldCnt = pjFieldCnt->valueint;
    pNetDbOptn->iTupleCnt = pjTupleCnt->valueint;
    // DBG("FieldCnt = %d TupleCnt = %d\n", pNetDbOptn->iFieldCnt, pNetDbOptn->iTupleCnt);

    cJSON *pjField = cJSON_GetObjectItem(pDbJson, "field");
    cJSON *pjPara = cJSON_GetObjectItem(pDbJson, "para");
    for (int i = 0; i < pNetDbOptn->iFieldCnt; i++)
    {
        char lszField[DB_SQL_FIELD_LENGTH];
        char lszPara[DB_SQL_PARA_LENGTH];

        memset(lszField, 0, sizeof(lszField));
        memset(lszPara, 0, sizeof(lszPara));

        sprintf(lszField, "field%d", i + 1);
        sprintf(lszPara, "para%d", i + 1);

        cJSON *pjFieldChild = cJSON_GetObjectItem(pjField, lszField);
        cJSON *pjParaChild = cJSON_GetObjectItem(pjPara, lszPara);

        DBG("%s %s\n", pjFieldChild->string, pjParaChild->string);

        // DBG("pjFieldChild->valuestring : %s\n", pjFieldChild->valuestring);
        // DBG("pjFieldChild->string : %s\n", pjFieldChild->string);

        pNetDbOptn->pszField[i] = (char *)malloc(sizeof(char) * DB_SQL_FIELD_LENGTH);
        pNetDbOptn->pszPara[i] = (char *)malloc(sizeof(char) * DB_SQL_PARA_LENGTH);

        // DBG("%lu\n", sizeof(pNetDbOptn->pszPara[i]));

        memset(pNetDbOptn->pszField[i], 0, sizeof(char) * DB_SQL_FIELD_LENGTH);
        memset(pNetDbOptn->pszPara[i], 0, sizeof(char) * DB_SQL_PARA_LENGTH);

        // DBG("memset done\n");

        strcpy(pNetDbOptn->pszField[i], pjFieldChild->valuestring);
        strcpy(pNetDbOptn->pszPara[i], pjParaChild->valuestring);

        // DBG("strcpy done\n");
    }

    return NET_JSON_SUCCESS;
}

int net_person2json(SNetPersonInfo *pNetPersonInfo, cJSON **ppPersonJson)
{
    if (pNetPersonInfo == NULL)
    {
        return NET_NULL_INPUT;
    }

    *ppPersonJson = cJSON_CreateObject();

    cJSON *pRoot = *ppPersonJson;
    cJSON_AddNumberToObject(pRoot, "iSum", pNetPersonInfo->iSum);
    cJSON_AddNumberToObject(pRoot, "iCur", pNetPersonInfo->iCur);
    cJSON_AddStringToObject(pRoot, "name", pNetPersonInfo->szName);
    cJSON_AddStringToObject(pRoot, "phone", pNetPersonInfo->szPhone);
    cJSON_AddStringToObject(pRoot, "description", pNetPersonInfo->szDescription);

    return NET_JSON_SUCCESS;
}
int net_json2person(SNetPersonInfo **ppNetPersonInfo, cJSON *pPersonJson)
{
    if (pPersonJson == NULL)
    {
        return NET_NULL_INPUT;
    }

    *ppNetPersonInfo = (SNetPersonInfo *)malloc(sizeof(SNetPersonInfo));
    SNetPersonInfo *pNetPersonInfo = *ppNetPersonInfo;
    if (*ppNetPersonInfo == NULL)
    {
        return NET_ALLOC_FAILED;
    }

    cJSON *pjSum = cJSON_GetObjectItem(pPersonJson, "iSum");
    cJSON *pjCur = cJSON_GetObjectItem(pPersonJson, "iCur");
    cJSON *pjName = cJSON_GetObjectItem(pPersonJson, "name");
    cJSON *pjPhone = cJSON_GetObjectItem(pPersonJson, "phone");
    cJSON *pjDes = cJSON_GetObjectItem(pPersonJson, "description");

    pNetPersonInfo->iSum = pjSum->valueint;
    pNetPersonInfo->iCur = pjCur->valueint;

    memset(pNetPersonInfo->szName, 0, sizeof(pNetPersonInfo));
    strcpy(pNetPersonInfo->szName, pjName->valuestring);

    memset(pNetPersonInfo->szPhone, 0, sizeof(pNetPersonInfo->szPhone));
    strcpy(pNetPersonInfo->szPhone, pjPhone->valuestring);

    memset(pNetPersonInfo->szDescription, 0, sizeof(pNetPersonInfo->szDescription));
    strcpy(pNetPersonInfo->szDescription, pjDes->valuestring);

    return NET_JSON_SUCCESS;
}

int net_cmd2json(SNetCmdReq *pNetCmdReq, cJSON **ppCmdJson)
{
    if (pNetCmdReq == NULL)
    {
        return NET_NULL_INPUT;
    }

    *ppCmdJson = cJSON_CreateObject();
    cJSON *pRoot = *ppCmdJson;
    cJSON_AddNumberToObject(pRoot, "iRecvFlg", 0);
    cJSON_AddStringToObject(pRoot, "head", pNetCmdReq->szHead);
    // cJSON_AddNumberToObject(pRoot, "iParaCnt", pNetCmdReq->iParaCnt);
    cJSON_AddStringToObject(pRoot, "para", pNetCmdReq->szPara);

    return NET_JSON_SUCCESS;
}

int net_json2cmd(SNetCmdReq **ppNetCmdReq, cJSON *pCmdJson)
{
    if (pCmdJson == NULL)
    {
        return NET_NULL_INPUT;
    }

    *ppNetCmdReq = (SNetCmdReq *)malloc(sizeof(SNetCmdReq));
    SNetCmdReq *pNetCmdReq = *ppNetCmdReq;
    if (*ppNetCmdReq == NULL)
    {
        return NET_ALLOC_FAILED;
    }

    cJSON *pjRecv = cJSON_GetObjectItem(pCmdJson, "iRecvFlg");
    cJSON *pjHead = cJSON_GetObjectItem(pCmdJson, "head");
    // cJSON *pjParaCnt = cJSON_GetObjectItem(pCmdJson, "iParaCnt");
    cJSON *pjPara = cJSON_GetObjectItem(pCmdJson, "para");

    pNetCmdReq->iRecvFlg = pjRecv->valueint;
    // pNetCmdReq->iParaCnt = pjParaCnt->valueint;

    memset(pNetCmdReq->szHead, 0, sizeof(pNetCmdReq->szHead));
    strcpy(pNetCmdReq->szHead, pjHead->valuestring);

    memset(pNetCmdReq->szPara, 0, sizeof(pNetCmdReq->szPara));
    strcpy(pNetCmdReq->szPara, pjPara->valuestring);

    return NET_JSON_SUCCESS;
}

int net_create_dboptn(SNetDbOptn **ppDbOptn, const char *c_pszHead,
                      const char (*ppFields)[DB_SQL_FIELD_LENGTH], const int c_iFieldCnt,
                      const char (*ppParas)[DB_SQL_PARA_LENGTH])
{
    *ppDbOptn = (SNetDbOptn *)malloc(sizeof(SNetDbOptn));
    if (*ppDbOptn == NULL)
    {
        return NET_ALLOC_FAILED;
    }
    SNetDbOptn *pDbOptn = *ppDbOptn;

    memset(pDbOptn->szHead, 0, sizeof(pDbOptn->szHead));
    strcpy(pDbOptn->szHead, c_pszHead);

    pDbOptn->iFieldCnt = c_iFieldCnt;
    pDbOptn->iTupleCnt = 0;

    for (int i = 0; i < c_iFieldCnt; i++)
    {
        pDbOptn->pszField[i] = (char *)malloc(sizeof(char) * DB_SQL_FIELD_LENGTH);
        memset(pDbOptn->pszField[i], 0, sizeof(char) * DB_SQL_FIELD_LENGTH);
        strcpy(pDbOptn->pszField[i], ppFields[i]);

        pDbOptn->pszPara[i] = (char *)malloc(sizeof(char) * DB_SQL_PARA_LENGTH);
        memset(pDbOptn->pszPara[i], 0, sizeof(char) * DB_SQL_PARA_LENGTH);
        strcpy(pDbOptn->pszPara[i], ppParas[i]);
        // DBG("%s  %s\n", pDbOptn->pszField[i], pDbOptn->pszPara[i]);
    }

    return NET_CREATE_SUCCESS;
}

int net_create_personinfo(SNetPersonInfo **ppPersoninfo,
                          const char *c_pszName, const char *c_pszPhone,
                          const char *c_pszDescription)
{
    *ppPersoninfo = (SNetPersonInfo *)malloc(sizeof(SNetPersonInfo));
    if (*ppPersoninfo == NULL)
    {
        return NET_ALLOC_FAILED;
    }

    SNetPersonInfo *pPersoninfo = *ppPersoninfo;
    memset(pPersoninfo->szName, 0, sizeof(pPersoninfo->szName));
    strcpy(pPersoninfo->szName, c_pszName);

    memset(pPersoninfo->szPhone, 0, sizeof(pPersoninfo->szPhone));
    strcpy(pPersoninfo->szPhone, c_pszPhone);

    memset(pPersoninfo->szDescription, 0, sizeof(pPersoninfo->szDescription));
    strcpy(pPersoninfo->szDescription, c_pszDescription);

    return NET_CREATE_SUCCESS;
}

int net_create_cmdreq(SNetCmdReq **ppCmdReq, const int c_iRecvFlg,
                      const char *c_pszHead, const char *c_pszPara)
{
    *ppCmdReq = (SNetCmdReq *)malloc(sizeof(SNetCmdReq));
    if (*ppCmdReq == NULL)
    {
        return NET_ALLOC_FAILED;
    }

    SNetCmdReq *pCmdReq = *ppCmdReq;
    pCmdReq->iRecvFlg = c_iRecvFlg;
    // pCmdReq->iParaCnt = c_iParaCnt;

    memset(pCmdReq->szHead, 0, sizeof(pCmdReq->szHead));
    strcpy(pCmdReq->szHead, c_pszHead);

    memset(pCmdReq->szPara, 0, sizeof(pCmdReq->szPara));
    strcpy(pCmdReq->szPara, c_pszPara);
    return NET_CREATE_SUCCESS;
}

int net_dboptn_free(SNetDbOptn *pNetDbOptn)
{
    if (pNetDbOptn == NULL)
    {
        return NET_NULL_INPUT;
    }

    for (int i = 0; i < pNetDbOptn->iFieldCnt; i++)
    {
        if (pNetDbOptn->pszField[i] != NULL)
        {
            free(pNetDbOptn->pszField[i]);
        }
        if (pNetDbOptn->pszPara[i] != NULL)
        {
            free(pNetDbOptn->pszPara[i]);
        }
    }

    free(pNetDbOptn);
    return NET_FREE_SUCCESS;
}

int net_personinfo_free(SNetPersonInfo *pPseroninfo)
{
    if (pPseroninfo == NULL)
    {
        return NET_NULL_INPUT;
    }
    free(pPseroninfo);
    return NET_FREE_SUCCESS;
}

int net_cmdreq_free(SNetCmdReq *pNetCmdReq)
{
    if (pNetCmdReq == NULL)
    {
        return NET_NULL_INPUT;
    }
    free(pNetCmdReq);
    return NET_FREE_SUCCESS;
}

int net_check_Json_full(char *pszRecv, int iLenRecv)
{

    int iLeftBracket = 0;
    int iRightBracket = 0;
    for (int i = 0; i < iLenRecv; i++)
    {
        if (pszRecv[i] == '{')
        {
            iLeftBracket++;
        }
        if (pszRecv[i] == '}')
        {
            iRightBracket++;
        }
    }

    if (iLeftBracket != iRightBracket)
    {
        return NET_JSON_NOT_FULL;
    }
    else
    {
        return NET_JSON_FULL;
    }
}

#ifdef NET_CLIENT
int net_init_client()
{
    int li_sockfd;
    li_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    gs_sockfd = li_sockfd;
    DBG("li_sockfd = %d\n", li_sockfd);

    bzero(&gs_server_addr, sizeof(gs_server_addr));
    gs_server_addr.sin_family = AF_INET;
    gs_server_addr.sin_port = htons(gs_port);
    gs_server_addr.sin_addr.s_addr = inet_addr(gs_server_ip);

    DBG("Client connect to %s:%hu\n", inet_ntoa(gs_server_addr.sin_addr), gs_server_addr.sin_port);

    int iRetCode = connect(li_sockfd, (struct sockaddr *)&gs_server_addr, sizeof(gs_server_addr));
    //need to check is disconnected...
    if (iRetCode != 0)
    {
        bConnFlg = 0;
        DBG("disconnected\n");
        return NET_CLIENT_CONNECT_FAILED;
    }
    else
    {
        bConnFlg = 1;
    }

    // *sockfd = li_sockfd;
    return 0;
}

int net_close_client()
{
    if (gs_sockfd != 0)
    {
        close(gs_sockfd);
    }
    return NET_FREE_SUCCESS;
}

//client
//修改删除操作，返回操作的结果，并显示错误的信息
int net_client_send_db_set_command(SNetDbOptn *pNetDbOptn, SNetCmdReq **ppNetCmdReq)
{
    int sockfd;
    int iRetCode;
    if (bConnFlg != 1)
    {
        iRetCode = net_init_client();

        // int iRetCode = connect(sockfd, (struct sockaddr *)&gs_server_addr, sizeof(gs_server_addr));

        if (iRetCode != 0)
        {
            DBG("disconnected\n");
            return NET_CLIENT_CONNECT_FAILED;
        }
    }
    else
    {
        DBG("has connected before\n");
        sockfd = gs_sockfd;
    }

    cJSON *pjDbJson;
    net_dboptn2json(pNetDbOptn, &pjDbJson);
    char *pszDbJsonOut = cJSON_Print(pjDbJson);
    DBG("%s\n", pszDbJsonOut);

    char lszRecvLine[NET_MAX_RECV_LENGTH];
    int liRecvCnt = 0;

    sendto(sockfd, pszDbJsonOut, strlen(pszDbJsonOut), 0,
           (struct sockaddr *)&gs_server_addr, sizeof(gs_server_addr));
    liRecvCnt = recv(sockfd, lszRecvLine, NET_MAX_RECV_LENGTH, 0);

    lszRecvLine[liRecvCnt] = '\0';
    DBG("%s\n", lszRecvLine);

    while (1)
    {
        //check : is this a full json string

        if (net_check_Json_full(lszRecvLine, liRecvCnt) != NET_JSON_FULL)
        {
            char lszRecvLine_Rep[NET_MAX_RECV_LENGTH];
            int liRecvCnt_Rep = recv(sockfd, lszRecvLine_Rep, NET_MAX_RECV_LENGTH, 0);
            strncat(lszRecvLine, lszRecvLine_Rep, liRecvCnt_Rep);
            lszRecvLine[liRecvCnt + liRecvCnt_Rep] = '\0';
            liRecvCnt += liRecvCnt_Rep;

            DBG("lens(%d) : %s\n", liRecvCnt, lszRecvLine);
        }
        else
        {
            break;
        }
    }

    //build cmd req
    DBG("recv:  %s", lszRecvLine);
    cJSON *pjCmdJson = cJSON_Parse(lszRecvLine);
    net_json2cmd(ppNetCmdReq, pjCmdJson);

    cJSON_Delete(pjCmdJson);
    free(pszDbJsonOut);
    return NET_SERVER_OPERATION_SUCCESS;
}

//查询表，函数体内，依据iSum与iCur的值，构建一个链表，
int net_client_send_db_get_command(SNetDbOptn *pNetDbOptn, SLinkedList **ppLinkedList)
{
    int sockfd;
    int iRetCode;
    if (bConnFlg != 1)
    {
        iRetCode = net_init_client();
        
        // iRetCode = connect(sockfd, (struct sockaddr *)&gs_server_addr, sizeof(gs_server_addr));
        
        if (iRetCode != 0)
        {
            DBG("disconnected\n");
            return NET_CLIENT_CONNECT_FAILED;
        }
    }
    else
    {
        DBG("has connected before\n");
        sockfd = gs_sockfd;
    }

    cJSON *pjDbJson;
    net_dboptn2json(pNetDbOptn, &pjDbJson);
    char *pszDbJsonOut = cJSON_Print(pjDbJson);
    DBG("%s\n", pszDbJsonOut);

    sendto(sockfd, pszDbJsonOut, strlen(pszDbJsonOut), 0,
           (struct sockaddr *)&gs_server_addr, sizeof(gs_server_addr));

    DBG("create list\n");

    iRetCode = linked_list_create(ppLinkedList);
    SLinkedList *pLinkedList = *ppLinkedList;
    if (iRetCode != LINKED_LIST_SUCCESS)
    {
        DBG("alloc failed\n");
    }

    //make a tick to receive
    long tick = 0;

    while (1)
    {
        DBG("start receiving a full person\n");
        //receive a full person
        char lszRecvLine[NET_MAX_RECV_LENGTH];
        int liRecvCnt = 0;
        liRecvCnt = recv(sockfd, lszRecvLine, NET_MAX_RECV_LENGTH, 0);
        lszRecvLine[liRecvCnt] = '\0';
        while (1)
        {
            if (net_check_Json_full(lszRecvLine, liRecvCnt) != NET_JSON_FULL)
            {
                char lszRecvLine_Rep[NET_MAX_RECV_LENGTH];
                int liRecvCnt_Rep = recv(sockfd, lszRecvLine_Rep, NET_MAX_RECV_LENGTH, 0);
                strncat(lszRecvLine, lszRecvLine_Rep, liRecvCnt_Rep);
                lszRecvLine[liRecvCnt + liRecvCnt_Rep] = '\0';
                liRecvCnt += liRecvCnt_Rep;

                DBG("lens(%d) : %s\n", liRecvCnt, lszRecvLine);
            }
            else
            {
                break;
            }
        }
        DBG("end received a full person\n");

        //check is a full list
        cJSON *pjNetPerson = cJSON_Parse(lszRecvLine);
        SNetPersonInfo *pNetperson;
        SPersonInfo *pPerson;
        char *pjNetPersonOut = cJSON_Print(pjNetPerson);
        DBG("%s\n", pjNetPersonOut);
        free(pjNetPersonOut);

        net_json2person(&pNetperson, pjNetPerson);
        create_person(&pPerson, pNetperson->szName, pNetperson->szPhone, pNetperson->szDescription);

        DBG("curpos : %d\n", pNetperson->iCur);

        linked_list_insert(pLinkedList, (void *)pPerson, -1);

        cJSON_Delete(pjNetPerson);

        if (pNetperson->iCur >= pNetperson->iSum)
        {
            DBG("all received\n");
            net_personinfo_free(pNetperson);
            break;
        }
        else
        {
            net_personinfo_free(pNetperson);
            continue;
        }
    }
}
#endif // NET_CLIENT
// #define NET_SERVER
#ifdef NET_SERVER
int net_init_server(int *pListenfd, char *pszServerAllowed)
{

    int li_listenfd;
    li_listenfd = socket(AF_INET, SOCK_STREAM, 0);

    in_addr_t allowedIP = inet_addr(pszServerAllowed);

    bzero(&gs_server_addr, sizeof(gs_server_addr));
    gs_server_addr.sin_family = AF_INET;
    gs_server_addr.sin_addr.s_addr = htonl(allowedIP);
    gs_server_addr.sin_port = htons(gs_port);

    bind(li_listenfd, (const struct sockaddr *)&gs_server_addr, sizeof(gs_server_addr));
    int iRetCode = listen(li_listenfd, NET_MAX_CONN_CNT);
    if (iRetCode != 0)
    {
        DBG("server init failed\n");
    }
    DBG("Server : %s:%hu\n", inet_ntoa(gs_server_addr.sin_addr), gs_server_addr.sin_port);

    *pListenfd = li_listenfd;
    return iRetCode == 0 ? iRetCode : NET_SERVER_OPERATION_SUCCESS;
}

//server
int net_server_receive_cmd(char *pszRecvStr, int iSockfd, SSockAddr sClinetAddr)
{
    cJSON *pjRecvCmd = cJSON_Parse(pszRecvStr);
    SNetDbOptn *pNetDbOptn;
    net_json2dboptn(&pNetDbOptn, pjRecvCmd);

    DBG("%s\n", pNetDbOptn->szHead);

    if (strcmp(pNetDbOptn->szHead, "delete") == 0)
    {
        net_server_send_db_delete_result(pNetDbOptn, iSockfd, sClinetAddr);
    }
    else if (strcmp(pNetDbOptn->szHead, "replace") == 0)
    {
        net_server_send_db_replace_result(pNetDbOptn, iSockfd, sClinetAddr);
    }
    else if (strcmp(pNetDbOptn->szHead, "select") == 0)
    {
        net_server_send_db_select_result(pNetDbOptn, iSockfd, sClinetAddr);
    }
    else
    {
        DBG("%s\n", cJSON_Print(pjRecvCmd));
    }

    cJSON_Delete(pjRecvCmd);
    net_dboptn_free(pNetDbOptn);
    return NET_SERVER_OPERATION_SUCCESS;
}

int net_server_send_db_select_result(SNetDbOptn *pNetOptn, int iSockfd, SSockAddr sSockAddr)
{
    char lppszFields[3][DB_SQL_FIELD_LENGTH] = {0};
    char lppszParas[3][DB_SQL_PARA_LENGTH] = {0};
    for (int i = 0; i < pNetOptn->iFieldCnt; i++)
    {
        strcpy(lppszFields[i], pNetOptn->pszField[i]);
        strcpy(lppszParas[i], pNetOptn->pszPara[i]);
    }

    SDbOperation *pDbOptnGet;
    SLinkedList *pLinkList;
    //may be crushed here
    DBG("%s\n", pNetOptn->szHead);
    // database_create_dboptn(&pDbOptn, pNetOptn->szHead, lppszFields,
    //    pNetOptn->iFieldCnt, lppszParas);
    // database_create_dboptn(&pDbOptn, "select", NULL, 0, NULL);
    database_create_dboptn(&pDbOptnGet, "select", NULL, 0, NULL);

    int iRetCode = database_multi_get(pDbOptnGet, &pLinkList);

    if (iRetCode != DB_OPERATION_SUCCESS)
    {
        DBG("get failed\n");
    }
    else
    {
        DBG("get success\n");
    }

    // ASSERT_REPORT(pLinkList == NULL);
    // DBG("&pLinkedList  %lu\n", &pLinkedList);
    // DBG("perosn count : %lu\n", pLinkedList->dwLength);

    int iSumPersonCnt = pLinkList->dwLength;
    int iPersonCnt = 0;
    SListIterator *pListIterator;
    SNetPersonInfo *pNetPerson;
    SPersonInfo *pDbPerson;
    SLinkNode *pLinkedNode;
    linked_list_iterator(pLinkList, &pListIterator);
    while (linked_list_has_next(pListIterator) == LINKED_LIST_ITERATOR_HAS_NEXT)
    {
        linked_list_next(pListIterator, &pLinkedNode);
        pDbPerson = (SPersonInfo *)pLinkedNode->pData;

        net_create_personinfo(&pNetPerson, pDbPerson->szName,
                              pDbPerson->szPhone, pDbPerson->szDescription);
        iPersonCnt++;
        pNetPerson->iSum = iSumPersonCnt;
        pNetPerson->iCur = iPersonCnt;
        DBG("person %d of %d\n", pNetPerson->iCur, pNetPerson->iSum);

        cJSON *pjNetPerson;
        net_person2json(pNetPerson, &pjNetPerson);

        char *pszNetPersonOut = cJSON_Print(pjNetPerson);
        int iLenNetPersonOut = strlen(pszNetPersonOut);

        // struct msghdr !!!!!!!!!!!!!!!!!!!
        // sendmsg()
        DBG("\n(%d) : %s\n", iLenNetPersonOut, pszNetPersonOut);

        //if no sleep ,it always loss some info!!!!!
        sleep(1);
        send(iSockfd, pszNetPersonOut, iLenNetPersonOut, 0);

        cJSON_Delete(pjNetPerson);
        free(pszNetPersonOut);
    }

    database_dboptn_free(pDbOptnGet);
    // net_dboptn_free(pNetOptn);
    return DB_OPERATION_SUCCESS;
}

int net_server_send_db_replace_result(SNetDbOptn *pNetOptn, int iSockfd, SSockAddr sSockAddr)
{

    DBG("pNetOptn->pszFields[0] : %s\n", pNetOptn->pszField[0]);
    char lppszFields[3][DB_SQL_FIELD_LENGTH];
    char lppszParas[3][DB_SQL_PARA_LENGTH];
    for (int i = 0; i < pNetOptn->iFieldCnt; i++)
    {
        strcpy(lppszFields[i], pNetOptn->pszField[i]);
        strcpy(lppszParas[i], pNetOptn->pszPara[i]);
    }

    SDbOperation *pDbOptn;
    database_create_dboptn(&pDbOptn, pNetOptn->szHead, lppszFields,
                           pNetOptn->iFieldCnt, lppszParas);

    int iRetCode = database_multi_insert(pDbOptn);
    DBG("replace code : %d\n", iRetCode);

    SNetCmdReq *pNetCmdReq;
    net_create_cmdreq(&pNetCmdReq, iRetCode, pDbOptn->szHead, " ");

    cJSON *pjCmdReq;
    net_cmd2json(pNetCmdReq, &pjCmdReq);
    char *pszCmdJsonOut = cJSON_Print(pjCmdReq);
    int iLenCmdOut = strlen(pszCmdJsonOut);

    send(iSockfd, pszCmdJsonOut, iLenCmdOut, 0);

    cJSON_Delete(pjCmdReq);
    database_dboptn_free(pDbOptn);
    net_cmdreq_free(pNetCmdReq);

    return DB_OPERATION_SUCCESS;
}

int net_server_send_db_delete_result(SNetDbOptn *pNetOptn, int iSockfd, SSockAddr sSockAddr)
{

    char lppszFields[3][DB_SQL_FIELD_LENGTH] = {0};
    char lppszParas[3][DB_SQL_PARA_LENGTH] = {0};
    for (int i = 0; i < pNetOptn->iFieldCnt; i++)
    {
        strcpy(lppszFields[i], pNetOptn->pszField[i]);
        strcpy(lppszParas[i], pNetOptn->pszPara[i]);
    }
    SDbOperation *pDbOptn;
    database_create_dboptn(&pDbOptn, pNetOptn->szHead, lppszFields,
                           pNetOptn->iFieldCnt, lppszParas);

    int iRetCode = database_multi_delete(pDbOptn);
    DBG("delete : %d\n", iRetCode);

    SNetCmdReq *pNetCmdReq;
    net_create_cmdreq(&pNetCmdReq, iRetCode, pDbOptn->szHead, " ");

    cJSON *pjCmdReq;
    net_cmd2json(pNetCmdReq, &pjCmdReq);
    char *pszCmdJsonOut = cJSON_Print(pjCmdReq);
    int iLenCmdOut = strlen(pszCmdJsonOut);

    send(iSockfd, pszCmdJsonOut, iLenCmdOut, 0);
    // sendto(iSockfd,pszCmdJsonOut,iLenCmdOut,0,(SSockAddr *)&sSockAddr,sizeof(sSockAddr));

    cJSON_Delete(pjCmdReq);
    database_dboptn_free(pDbOptn);
    net_cmdreq_free(pNetCmdReq);

    return DB_OPERATION_SUCCESS;
}

#endif // NET_SERVER
