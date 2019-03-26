#include "NetAccess.h"

extern int gs_port;
extern char gs_server_ip[NET_IP_LENGTH];

extern int gs_sockfd;
extern int bConnFlg;

int main()
{
    char lszServerIP[NET_IP_LENGTH];
    int iPortNum = 9090;
    config_get_string_value("Server", "IP", "127.0.0.1", lszServerIP, NET_IP_LENGTH, "config.ini");
    config_get_int_value("Server", "Port", 9090, "config.ini", &iPortNum);
    gs_port = iPortNum;
    strcpy(gs_server_ip, lszServerIP);
    DBG("client config : %s:%d\n", lszServerIP, iPortNum);

    // set test
    SNetDbOptn *pNetDbOptn;
    char lszFields[2][DB_SQL_FIELD_LENGTH] = {"name", "phone"};
    char lszParas[2][DB_SQL_PARA_LENGTH] = {"ste", "123123"};

    net_create_dboptn(&pNetDbOptn, "replace", lszFields, 2, lszParas);

    // DBG("try to connect %s:%d\n", gs_server_ip, gs_port);
    SNetCmdReq *pNetCmdReq;
    net_client_send_db_set_command(pNetDbOptn, &pNetCmdReq);

    //get test
    SNetDbOptn *pNetDbOptn_get;
    char lszFields_get[2][DB_SQL_FIELD_LENGTH] = {"name"};
    char lszParas_get[2][DB_SQL_PARA_LENGTH] = {"ste"};

    net_create_dboptn(&pNetDbOptn_get, "select", lszFields_get, 2, lszParas_get);

    cJSON *pjNetDbGet;
    net_dboptn2json(pNetDbOptn_get, &pjNetDbGet);
    char *pszNetDbGet = cJSON_Print(pjNetDbGet);
    DBG("%s\n", pszNetDbGet);

    SLinkedList *pLinkedList;
    net_client_send_db_get_command(pNetDbOptn_get, &pLinkedList);

    if (pLinkedList != NULL)
    {
        DBG("list length = %lu\n", pLinkedList->dwLength);
        net_dboptn_free(pNetDbOptn_get);
        linked_list_free(pLinkedList);
    }

    if (pjNetDbGet != NULL)
    {
        cJSON_Delete(pjNetDbGet);
        free(pszNetDbGet);
    }

    return 0;
}
