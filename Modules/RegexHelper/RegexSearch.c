/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-19 21:48:18
 * @LastEditTime: 2019-03-22 10:58:45
 */
#include "RegexSearch.h"

static char *sub_str(const char *c_pszSrc, unsigned int wStart, unsigned int wEnd)
{
    unsigned int wLength = wEnd - wStart;

    static char s_tmpBuf[REGEX_MAX_STRING_LENGTH];

    strncpy(s_tmpBuf, c_pszSrc + wStart, wLength);

    s_tmpBuf[wLength] = '\0';

    return s_tmpBuf;
}

//只返回目标人员信息中是否包含了模式串
int regex_match_no_ret(const SPersonInfo *pszDstPerson, const char *pszPat)
{
    if (pszDstPerson == NULL || pszPat == NULL)
    {
        return REGEX_NULL_INPUT;
    }
    else
    {
        char lszWholePat[PERSON_MAX_INFO_LENGTH] = {0};
        strcpy(lszWholePat, ".*");
        strcat(lszWholePat, pszPat);
        strcat(lszWholePat, ".*");

        int l_iRetCode = -1;
        char l_szErrorInfo[REGEX_MAX_ERROR_LENGTH] = {0};
        regex_t *pRegex = (regex_t *)malloc(sizeof(regex_t));
        if (pRegex == NULL)
        {
            DBG("allocate failed \n");
            return REGEX_ALLOCATE_FAILED;
        }
        else
        {
            l_iRetCode = regcomp(pRegex, lszWholePat, REG_NEWLINE);
            if (l_iRetCode != REGEX_COMPILE_SUCCESS)
            {
                regerror(l_iRetCode, pRegex, l_szErrorInfo, REGEX_MAX_ERROR_LENGTH);
                DBG("%s\n", l_szErrorInfo);
                return REGEX_COMPILE_FAILED;
            }
            else
            {
                int l_iRetCode_name = 0;
                int l_iRetCode_phone = 0;
                int l_iRetCode_des = 0;
                if ((l_iRetCode_name = regexec(pRegex, pszDstPerson->szName, (size_t)0, NULL, 0)) != REGEX_EXEC_SUCCESS)
                {
                    regerror(l_iRetCode_name, pRegex, l_szErrorInfo, REGEX_MAX_ERROR_LENGTH);
                    // DBG("name : %s\n", l_szErrorInfo);
                    memset(l_szErrorInfo, 0, REGEX_MAX_ERROR_LENGTH);
                }
                if ((l_iRetCode_phone = regexec(pRegex, pszDstPerson->szPhone, (size_t)0, NULL, 0)) != REGEX_EXEC_SUCCESS)
                {
                    regerror(l_iRetCode_phone, pRegex, l_szErrorInfo, REGEX_MAX_ERROR_LENGTH);
                    // DBG("phone : %s\n", l_szErrorInfo);
                    memset(l_szErrorInfo, 0, REGEX_MAX_ERROR_LENGTH);
                }
                if ((l_iRetCode_des = regexec(pRegex, pszDstPerson->szDescription, (size_t)0, NULL, 0)) != REGEX_EXEC_SUCCESS)
                {
                    regerror(l_iRetCode_des, pRegex, l_szErrorInfo, REGEX_MAX_ERROR_LENGTH);
                    // DBG("description : %s\n", l_szErrorInfo);
                    memset(l_szErrorInfo, 0, REGEX_MAX_ERROR_LENGTH);
                }

                regfree(pRegex);

                l_iRetCode = l_iRetCode_name == 0 || l_iRetCode_phone == 0 || l_iRetCode_des == 0;
                if (l_iRetCode == 1)
                {
                    // DBG("matched : %s %s %s \n",
                    //     pszDstPerson->szName, pszDstPerson->szName, pszDstPerson->szDescription);
                    return REGEX_MATCH_SUCCESS;
                }
                else
                {
                    // DBG("failed : %s %s %s \n",
                        // pszDstPerson->szName, pszDstPerson->szName, pszDstPerson->szDescription);
                    return REGEX_MATCH_FAILED;
                }
            }
        }
    }
}

//返回目标人员信息中与模式串匹配的字段的始末位置
int regex_match_with_ret(const SPersonInfo *pszDstPerson, const char *pszPat,
                         char *pszAttr, int *piStart, int *piEnd)
{
    if (pszDstPerson == NULL || pszPat == NULL)
    {
        return REGEX_NULL_INPUT;
    }
    else
    {
        char lszWholePat[PERSON_MAX_INFO_LENGTH] = {0};
        strcpy(lszWholePat, ".*");
        strcat(lszWholePat, pszPat);
        strcat(lszWholePat, ".*");

        int l_iRetCode = -1;
        char l_szErrorInfo[REGEX_MAX_ERROR_LENGTH] = {0};
        regmatch_t pRegMatch[REGEX_DEFAULT_SEARCH_CNT];
        const size_t c_nRegMatch = REGEX_DEFAULT_SEARCH_CNT;
        regex_t *pRegex = (regex_t *)malloc(sizeof(regex_t));
        if (pRegex == NULL)
        {
            DBG("allocate failed \n");
            return REGEX_ALLOCATE_FAILED;
        }
        else
        {
            l_iRetCode = regcomp(pRegex, lszWholePat, 0);
            if (l_iRetCode != REGEX_COMPILE_SUCCESS)
            {
                regerror(l_iRetCode, pRegex, l_szErrorInfo, REGEX_MAX_ERROR_LENGTH);
                DBG("%s\n", l_szErrorInfo);
                return REGEX_COMPILE_FAILED;
            }
            else
            {
                if (l_iRetCode =
                        regexec(pRegex, pszDstPerson->szName, c_nRegMatch, pRegMatch, 0) != REG_NOMATCH)
                {
                    memset(pszAttr, 0, sizeof(pszAttr));
                    strcpy(pszAttr, "name");
                    *piStart = pRegMatch[0].rm_so;
                    *piEnd = pRegMatch[0].rm_eo;
                    DBG("%s : %d to %d\n", pszAttr, *piStart, *piEnd);
                    return REGEX_MATCH_SUCCESS;
                }
                else if (l_iRetCode =
                             regexec(pRegex, pszDstPerson->szPhone, c_nRegMatch, pRegMatch, 0) != REG_NOMATCH)
                {
                    memset(pszAttr, 0, sizeof(pszAttr));
                    strcpy(pszAttr, "phone");
                    *piStart = pRegMatch[0].rm_so;
                    *piEnd = pRegMatch[0].rm_eo;
                    DBG("%s : %d to %d\n", pszAttr, *piStart, *piEnd);
                    return REGEX_MATCH_SUCCESS;
                }
                else if (l_iRetCode =
                             regexec(pRegex, pszDstPerson->szDescription, c_nRegMatch, pRegMatch, 0) != REG_NOMATCH)
                {
                    memset(pszAttr, 0, sizeof(pszAttr));
                    strcpy(pszAttr, "description");
                    *piStart = pRegMatch[0].rm_so;
                    *piEnd = pRegMatch[0].rm_eo;
                    DBG("%s : %d to %d\n", pszAttr, *piStart, *piEnd);
                    return REGEX_MATCH_SUCCESS;
                }
                else
                {
                    return REGEX_MATCH_FAILED;
                }
            }
        }
    }
}

#ifdef DEBUG_REGEX_HELPER
int main()
{
    SPersonInfo *pPer1;
    create_person(&pPer1, "old young", "15151822322", "asdasd");

    if (regex_match_no_ret(pPer1, "you") == REGEX_MATCH_SUCCESS)
    {
        printf("\t(you) matched pPer1\n");
    }

    if (regex_match_no_ret(pPer1, "1518") == REGEX_MATCH_SUCCESS)
    {
        printf("\t(1518) matched pPer1\n");
    }

    if (regex_match_no_ret(pPer1, "asd") == REGEX_MATCH_SUCCESS)
    {
        printf("\t(asd) matched pPer1\n");
    }

    char szAttr[20];
    int liStart;
    int liEnd;
    if (regex_match_with_ret(pPer1, "old", szAttr, &liStart, &liEnd) == REGEX_MATCH_SUCCESS)
    {
        printf("\told %s (%d, %d)\n", szAttr, liStart, liEnd);
        memset(szAttr, 0, sizeof(szAttr));
        liStart = 0;
        liEnd = 0;
    }

    if (regex_match_with_ret(pPer1, "223", szAttr, &liStart, &liEnd) == REGEX_MATCH_SUCCESS)
    {
        printf("\t223 %s (%d, %d)\n", szAttr, liStart, liEnd);
        memset(szAttr, 0, sizeof(szAttr));
        liStart = 0;
        liEnd = 0;
    }

    if (regex_match_with_ret(pPer1, "3213132131", szAttr, &liStart, &liEnd) == REGEX_MATCH_SUCCESS)
    {
        printf("\t3213132131 %s (%d, %d)\n", szAttr, liStart, liEnd);
        memset(szAttr, 0, sizeof(szAttr));
        liStart = 0;
        liEnd = 0;
    }

    free(pPer1);
}
#endif // DEBUG_REGEX_HELPER