/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-19 16:18:40
 * @LastEditTime: 2019-03-25 10:34:02
 */
#include "PersonInfo.h"

int print_person_info(const SPersonInfo *c_pPerson)
{
    printf("{ ");
    printf("\n\tName = %s ", c_pPerson->szName);
    printf("\n\tPhoneNumber = %s ", c_pPerson->szPhone);
    printf("\n\tDescription = %s", c_pPerson->szDescription);
    printf("\n}\n");

    return PERSON_OPERATION_SUCCESS;
}

int get_person_info(const SPersonInfo *c_pPerson, char *pszInfo, const int c_iOutput)
{
    char szInfo[PERSON_MAX_INFO_LENGTH];
    memset(szInfo, 0, sizeof(szInfo));

    strcpy(szInfo, "{");
    if (c_pPerson->szName != NULL)
    {
        strcat(szInfo, c_pPerson->szName);
        strcat(szInfo, ",");
    }
    if (c_pPerson->szPhone != NULL)
    {
        strcat(szInfo, c_pPerson->szPhone);
        strcat(szInfo, ",");
    }
    if (c_pPerson->szDescription != NULL)
    {
        strcat(szInfo, c_pPerson->szDescription);
    }
    strcat(szInfo, "}");
    strncpy(pszInfo, szInfo, c_iOutput);

    return PERSON_OPERATION_SUCCESS;
}

int create_person(SPersonInfo **ppPerson, const char *c_pszName, const char *c_pszPhone, const char *c_pszDescription)
{
    *ppPerson = (SPersonInfo *)malloc(sizeof(SPersonInfo));
    SPersonInfo *pPerson = *ppPerson;
    if (pPerson == NULL)
    {
        DBG("struct allocate failed\n");
        return PERSON_ALLOCATE_FAILED;
    }

    if (c_pszName != NULL)
    {
        strcpy(pPerson->szName, c_pszName);
    }
    if (c_pszPhone != NULL)
    {
        strcpy(pPerson->szPhone, c_pszPhone);
    }
    if (c_pszDescription != NULL)
    {
        strcpy(pPerson->szDescription, c_pszDescription);
    }

    return PERSON_OPERATION_SUCCESS;
}

int set_person(SPersonInfo *pPerson, const char *c_pszName, const char *c_pszPhone, const char *c_pszDescription)
{
    if (pPerson == NULL)
    {
        DBG("person input NULL\n");
        return PERSON_INPUT_INVALID;
    }

    if (c_pszName != NULL)
    {
        memset(pPerson->szName, 0, sizeof(pPerson->szName));
        strcpy(pPerson->szName, c_pszName);
    }

    if (c_pszPhone != NULL)
    {
        memset(pPerson->szPhone, 0, sizeof(pPerson->szPhone));
        strcpy(pPerson->szPhone, c_pszPhone);
    }

    if (c_pszDescription != NULL)
    {
        memset(pPerson->szDescription, 0, sizeof(pPerson->szDescription));
        strcpy(pPerson->szDescription, c_pszDescription);
    }

    return PERSON_OPERATION_SUCCESS;
}

#ifdef DEBUG_PERSON_INFO
int main()
{
    SPersonInfo *ps1, *ps2;
    create_person(&ps1, "stephen", "12123123", "des");
    create_person(&ps2, NULL, NULL, NULL);

    print_person_info(ps1);
    print_person_info(ps2);

    set_person(ps2, "young", "222333", "cri");
    char szPs2Info[PERSON_MAX_INFO_LENGTH];
    int iOutputLen = PERSON_MAX_INFO_LENGTH;
    get_person_info(ps2, szPs2Info, iOutputLen);
    printf("%s\n", szPs2Info);
}

#endif // DEBUG_PERSON_INFO