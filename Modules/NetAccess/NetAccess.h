/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-20 16:38:13
 * @LastEditTime: 2019-03-25 20:21:25
 */
#define DEBUG_NET_ACCESS

#ifndef NET_ACCESS_H
#define NET_ACCESS_H
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>          //need to compile with arm-linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

// #include "../../Debug/Debug.h"

#include "../cJSON/cJSON.h"
#include "../Database/Database_v2.h"

#include "../../Structure/PersonInfo/PersonInfo.h"
#include "../../Structure/LinkedList/LinkedList.h"


#define NET_COMMAND_LENGTH 128
#define NET_JSON_SUCCESS 0
#define NET_CREATE_SUCCESS 0
#define NET_FREE_SUCCESS 0
#define NET_SERVER_OPERATION_SUCCESS 0

#define NET_ALLOC_FAILED -1
#define NET_NULL_INPUT -2

#define NET_CLIENT_CONNECT_FAILED -3
#define NET_JSON_FULL 1
#define NET_JSON_NOT_FULL -1

#define NET_MAX_RECV_LENGTH 1024
#define NET_IP_LENGTH 20
#define NET_MAX_CONN_CNT 1024                 //need to set to a proper value


#define GS_INT_PORT 9068
typedef struct sockaddr_in SSockAddr;
int gs_port;
char gs_server_ip[NET_IP_LENGTH];
SSockAddr gs_server_addr;

int gs_sockfd;
int bConnFlg;

// #define NET_CLIENT
// #define NET_SERVER

//default for client
// #ifdef NET_CLIENT
// #undef NET_SERVER
// #endif // NET_CLIENT

// #ifndef NET_SERVER

// #ifndef NET_CLIENT
#define NET_CLIENT
// #endif // !NET_CLIENT

// #endif // NET_SERVER

typedef struct _Net_Db_SQL_Optn
{
    int iFieldCnt;
    int iTupleCnt;
    char szHead[DB_SQL_HEAD_LENGTH];
    char *pszField[DB_SQL_FIELD_LENGTH];
    char *pszPara[DB_SQL_PARA_LENGTH];
} SNetDbOptn;

typedef struct _Net_Person_Info
{
    int iSum;
    int iCur;
    char szName[PERSON_MAX_NAME_LENGTH];
    char szPhone[PERSON_MAX_PHONE_LENGTH];
    char szDescription[PERSON_MAX_DESCRIPTION_LENGTH];
} SNetPersonInfo;

typedef struct _Net_Command_Req
{
    int iRecvFlg;
    char szHead[DB_SQL_HEAD_LENGTH];
    // int iParaCnt;
    char szPara[NET_COMMAND_LENGTH];
} SNetCmdReq;

int net_dboptn2json(SNetDbOptn *pNetDbOptn, cJSON **ppDbJson);
int net_json2dboptn(SNetDbOptn **ppNetDbOptn, cJSON *pDbJson);

int net_person2json(SNetPersonInfo *pNetPersonInfo, cJSON **ppPersonJson);
int net_json2person(SNetPersonInfo **ppNetPersonInfo, cJSON *pPersonJson);

int net_cmd2json(SNetCmdReq *pNetCmdReq, cJSON **ppCmdJson);
int net_json2cmd(SNetCmdReq **ppNetCmdReq, cJSON *pCmdJson);

int net_create_dboptn(SNetDbOptn **ppDbOptn, const char *c_pszHead,
                      const char (*ppFields)[DB_SQL_FIELD_LENGTH], const int c_iFieldCnt,
                      const char (*ppParas)[DB_SQL_PARA_LENGTH]);

int net_create_personinfo(SNetPersonInfo **ppPersoninfo,
                          const char *c_pszName, const char *c_pszPhone,
                          const char *c_pszDescription);

int net_create_cmdreq(SNetCmdReq **ppCmdReq, const int c_iRecvFlg,
                      const char *c_pszHead, const char *c_pszPara);

int net_dboptn_free(SNetDbOptn *pNetDbOptn);

int net_personinfo_free(SNetPersonInfo *pPseroninfo);

int net_cmdreq_free(SNetCmdReq *pNetCmdReq);

#ifdef NET_CLIENT
//client
int net_init_client();

int net_close_client();

//修改删除操作，返回操作的结果，并显示错误的信息
int net_client_send_db_set_command(SNetDbOptn *pNetDbOptn, SNetCmdReq **ppNetCmdReq);

//查询表，函数体内，依据iSum与iCur的值，构建一个链表，
int net_client_send_db_get_command(SNetDbOptn *pNetDbOptn, SLinkedList **ppLinkedList);
#endif

#ifdef NET_SERVER
//server
int net_init_server(int *pListenfd, char *pszServerAllowed);

int net_server_receive_cmd(char *pszRecvStr, int iSockfd, SSockAddr sClinetAddr);

int net_server_send_db_select_result(SNetDbOptn *pNetOptn, int iSockfd, SSockAddr sSockAddr);

int net_server_send_db_replace_result(SNetDbOptn *pNetOptn, int iSockfd, SSockAddr sSockAddr);

int net_server_send_db_delete_result(SNetDbOptn *pNetOptn, int iSockfd, SSockAddr sSockAddr);
#endif


#endif