#include "NetAccess.h"


int main()
{
    //db
    cJSON *pDbJson;
    SNetDbOptn *pNetDbOptn;
    char lszFields[2][DB_SQL_FIELD_LENGTH] = {"name", "phone"};
    char lszParas[2][DB_SQL_PARA_LENGTH] = {"ste", "123123"};

    net_create_dboptn(&pNetDbOptn, "replace", lszFields, 2, lszParas);
    net_dboptn2json(pNetDbOptn, &pDbJson);
    char *pszDbJsonOut = cJSON_Print(pDbJson);
    DBG("%s\n", pszDbJsonOut);

    SNetDbOptn *pNetDbOptn_Ret;
    net_json2dboptn(&pNetDbOptn_Ret, pDbJson);
    DBG("%s\n", pNetDbOptn_Ret->szHead);

    // cJSON *pHead = cJSON_GetObjectItem(pDbJson, "head");
    // printf("pHead->string : %s\n", pHead->string);

    net_dboptn_free(pNetDbOptn);
    cJSON_Delete(pDbJson);
    free(pszDbJsonOut);

    //person
    cJSON *pPersonJson;
    SNetPersonInfo *pNetPerson;
    char lszName[PERSON_MAX_NAME_LENGTH] = "stephen";
    char lszPhone[PERSON_MAX_PHONE_LENGTH] = "123123123";
    char lszDes[PERSON_MAX_DESCRIPTION_LENGTH] = "ajlkjlaksjd";
    net_create_personinfo(&pNetPerson, lszName, lszPhone, lszDes);
    net_person2json(pNetPerson, &pPersonJson);
    char *pszPersonJsonOut = cJSON_Print(pPersonJson);

    DBG("%s\n", pszPersonJsonOut);

    SNetPersonInfo *pNetPerson_Ret;
    net_json2person(&pNetPerson_Ret, pPersonJson);

    DBG("%s\n", pNetPerson_Ret->szName);

    net_personinfo_free(pNetPerson);
    cJSON_Delete(pPersonJson);
    free(pszPersonJsonOut);

    //cmd
    cJSON *pCmdJson;
    SNetCmdReq *pCmdReq;
    char lszCmdHead[DB_SQL_HEAD_LENGTH] = "replace";
    char lszCmdPara[DB_SQL_COMMAND_LENGTH] = "success";
    net_create_cmdreq(&pCmdReq, 0, lszCmdHead, lszCmdPara);
    net_cmd2json(pCmdReq, &pCmdJson);
    char *pszCmdOut = cJSON_Print(pCmdJson);

    DBG("%s\n", pszCmdOut);

    SNetCmdReq *pNetCmd_Ret;
    net_json2cmd(&pNetCmd_Ret, pCmdJson);

    DBG("%s\n", pNetCmd_Ret->szPara);

    net_cmdreq_free(pCmdReq);
    cJSON_Delete(pCmdJson);
    free(pszCmdOut);

    return 0;
}

