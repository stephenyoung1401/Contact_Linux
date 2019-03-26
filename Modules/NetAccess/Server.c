#include "NetAccess.h"
#include "../Config/GetConfig.h"

extern int gs_port;
extern char gs_server_ip[NET_IP_LENGTH];
// extern SSockAddr gs_server_addr;

int main()
{
    //network
    char lszServerAllowed[NET_IP_LENGTH];
    int iPortNum = 9090;
    config_get_string_value("Server", "Allow", "0.0.0.0", lszServerAllowed, NET_IP_LENGTH, "config.ini");
    config_get_int_value("Server", "Port", 9090, "config.ini", &iPortNum);
    gs_port = iPortNum;
    strcpy(gs_server_ip, lszServerAllowed);
    DBG("server config %s:%d\n", lszServerAllowed, iPortNum);

    //database 
    char lszDbFileName[DB_FILE_NAME_LENGTH];
    config_get_string_value("Database", "File", "contact.db", lszDbFileName, DB_FILE_NAME_LENGTH, "config.ini");
    int iRetCode = databae_open(lszDbFileName);

    if (iRetCode != DB_OPERATION_SUCCESS)
    {
        DBG("open failed\n");
        return 1;
    }

    int listenfd;
    int pid;
    int connfd;

    int iLenClientAddr;

    SSockAddr lClientAddr;

    int iLenRecv;
    char pszRecvStr[NET_MAX_CONN_CNT];
    net_init_server(&listenfd, lszServerAllowed);

    DBG("listening...\n");
    while (1)
    {
        iLenClientAddr = sizeof(lClientAddr);
        connfd = accept(listenfd, (struct sockaddr *)&lClientAddr, &iLenClientAddr);

        if ((pid == fork()) == 0)
        {
            close(listenfd);
            while ((1))
            {
                iLenRecv = recvfrom(connfd, pszRecvStr, NET_MAX_CONN_CNT, 0,
                                    (struct sockaddr *)&lClientAddr, &iLenClientAddr);
                if (iLenRecv > 0)
                {
                    pszRecvStr[iLenRecv] = '\0';

                    DBG("%s\n", pszRecvStr);

                    // send(connfd, pszRecvStr, iLenRecv, 0);
                    while (net_check_Json_full(pszRecvStr, iLenRecv) != NET_JSON_FULL)
                    {
                        char lszRecvLine_Rep[NET_MAX_RECV_LENGTH];
                        int liRecvCnt_Rep = recv(connfd, lszRecvLine_Rep, NET_MAX_RECV_LENGTH, 0);
                        strncat(pszRecvStr, lszRecvLine_Rep, liRecvCnt_Rep);
                        pszRecvStr[iLenRecv + liRecvCnt_Rep] = '\0';
                        iLenRecv += liRecvCnt_Rep;

                        DBG("lens(%d) : %s\n", iLenRecv, pszRecvStr);
                    }

                    net_server_receive_cmd(pszRecvStr, connfd, lClientAddr);

                }
            }
            close(connfd);
        }
    }

    return 0;
}
