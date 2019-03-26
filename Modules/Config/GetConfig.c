/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-20 09:28:52
 * @LastEditTime: 2019-03-20 18:55:20
 */
#include "GetConfig.h"

//获取文件的完整路径
int config_get_complete_path(const char *pszConfigFileName, char *pszWholeFileName)
{
    char *pszPwdPath = NULL;
    char szWholePath[CONFIG_MAX_FILE_NAME_LENGTH] = {0};

    if (pszConfigFileName == NULL || pszWholeFileName == NULL)
    {
        DBG("GetCompletePath: NULL input\n");
        return CONFIG_NO_FILE;
    }

    //Allocate memery in the function called
    //every user has his own file but executable file is in the /usr/bin/
    pszPwdPath = (char *)getenv("PWD");
    if (pszPwdPath == NULL)
    {
        DBG("pszPwdPath : cannot find var name PWD\n");
        return CONFIG_NO_ENV_COMMAND;
    }

    snprintf(szWholePath, sizeof(szWholePath) - 1, "%s/%s", pszPwdPath, pszConfigFileName);
    strncpy(pszWholeFileName, szWholePath, strlen(szWholePath));
    return CONFIG_SUCCESS;
}

//读取指定的段属性内容
int config_string_content(FILE *fp, const char *pszSectionName,
                          const char *pszKeyName, char *pszOutput,
                          const unsigned int iOutputLen)
{
    char szSectionName[CONFIG_MAX_SECTION_LENGTH] = {0};
    char szKeyName[CONFIG_MAX_KEY_LENGTH] = {0};
    char szContentLine[CONFIG_MAX_CONTENT_LENGTH] = {0};
    char szContentLineBak[CONFIG_MAX_CONTENT_LENGTH] = {0};

    unsigned int iContentLineLen = 0;
    unsigned int iPositionFlag = 0;

    if (fp == NULL || pszSectionName == NULL || pszKeyName == NULL || pszOutput == NULL)
    {
        printf("GetStringContentValue : NULL input\n");
        return CONFIG_NO_FILE;
    }

    sprintf(szSectionName, "[%s]", pszSectionName);
    strcpy(szKeyName, pszKeyName);

    while (feof(fp) == 0)
    {

        memset(szContentLine, 0, sizeof(szContentLine));
        fgets(szContentLine, sizeof(szContentLine), fp);

        // printf("trying... %s \n", szContentLine);

        if (szContentLine[0] == ';' || szContentLine[0] == '\r' ||
            szContentLine[0] == '\n' || szContentLine[0] == '\0')
        {
            continue;
        }

        //find the correct section
        //need to
        if (strncmp(szSectionName, szContentLine, strlen(szSectionName)) == 0)
        {
            // printf("find %s\n", szSectionName);
            while (feof(fp) == 0)
            {
                memset(szContentLine, 0, sizeof(szContentLine));
                memset(szContentLineBak, 0, sizeof(szContentLineBak));
                fgets(szContentLine, sizeof(szContentLine), fp);

                //fgets auto ignore blank
                if (szContentLine[0] == ';')
                {
                    continue;
                }

                //store the pharse
                memcpy(szContentLineBak, szContentLine, strlen(szContentLine));

                //arrtibute compare
                if (strncmp(szContentLineBak, szKeyName, strlen(szKeyName)) == 0)
                {
                    iContentLineLen = strlen(szContentLine);
                    for (iPositionFlag = strlen(szKeyName); iPositionFlag <= iContentLineLen; iPositionFlag++)
                    {
                        if (szContentLine[iPositionFlag] == ' ')
                        {
                            continue;
                        }
                        if (szContentLine[iPositionFlag] == '=')
                        {
                            break;
                        }

                        iPositionFlag = iContentLineLen + 1;
                        break;
                    }

                    iPositionFlag = iPositionFlag + 1;

                    //find attribute but no value
                    if (iPositionFlag > iContentLineLen)
                    {
                        continue;
                    }

                    memset(szContentLine, 0, sizeof(szContentLine));
                    strcpy(szContentLine, szContentLineBak + iPositionFlag);

                    //find value
                    for (iPositionFlag = 0; iPositionFlag < iContentLineLen; iPositionFlag++)
                    {
                        if (szContentLine[iPositionFlag] == '\r' ||
                            szContentLine[iPositionFlag] == '\n' ||
                            szContentLine[iPositionFlag] == '\0')
                        {
                            szContentLine[iPositionFlag] = '\0';
                            break;
                        }
                    }

                    strncpy(pszOutput, szContentLine, iOutputLen - 1);
                    // printf("find key %s = %s\n", pszKeyName, pszOutput);
                    break;
                }
                else if (szContentLine[0] == '[')
                {
                    break;
                }
            }
            break;
        }
    }
}
//读取字符串形式段属性内容
int config_get_string_value(const char *pszSectionName, const char *pszKeyName,
                            const char *pDefaultValue, char *pszOutput,
                            const unsigned int iOutputLen, const char *pszConfigFileName)
{

    char szWholePath[CONFIG_MAX_FILE_NAME_LENGTH] = {0};

    if (pszSectionName == NULL || pszKeyName == NULL ||
        pszOutput == NULL || pszConfigFileName == NULL)
    {
        printf("GetConfigFileStringValue : NULL input\n");
        return CONFIG_NULL_INPUT;
    }

    if (pDefaultValue == NULL)
    {
        strcpy(pszOutput, "");
    }
    else
    {
        strcpy(pszOutput, pDefaultValue);
    }

    config_get_complete_path(pszConfigFileName, szWholePath);
    FILE *fp = NULL;
    fp = fopen(szWholePath, "r");
    if (fp == NULL)
    {
        printf("\nGetConfigFileStringValue : cannot open file %s\n", szWholePath);
        return CONFIG_NO_FILE;
    }

    config_string_content(fp, pszSectionName, pszKeyName, pszOutput, iOutputLen);

    fclose(fp);
    fp = NULL;
    return CONFIG_SUCCESS;
}
//读取数值型段属性内容
int config_get_int_value(const char *pszSectionName, const char *pszKeyName,
                         const unsigned int iDefaultValue, const char *pszConfigFileName,
                         int *piOutput)
{
    char szGetValue[CONFIG_MAX_CONTENT_LENGTH * 2] = {0};
    int iGetValue = 0;

    if (pszSectionName == NULL || pszKeyName == NULL || pszConfigFileName == NULL)
    {
        printf("GetConfigFileIntValue : NULL file\n");
        return CONFIG_NO_FILE;
    }

    config_get_string_value(pszSectionName, pszKeyName,
                            NULL, szGetValue,
                            CONFIG_MAX_CONTENT_LENGTH * 2 - 1,
                            pszConfigFileName);

    if (szGetValue[0] == '\0' || szGetValue[0] == ';')
    {
        iGetValue = iDefaultValue;
    }

    else
    {
        iGetValue = atoi(szGetValue);
    }

    *piOutput = iGetValue;

    return CONFIG_SUCCESS;
}

#ifdef DEBUG_GET_CONFIG
int main()
{
    char lszWholeFileName[CONFIG_MAX_FILE_NAME_LENGTH];
    config_get_complete_path("config", lszWholeFileName);
    printf("%s\n", lszWholeFileName);

    char lszKeyName[CONFIG_MAX_KEY_LENGTH] = "IP";
    char lszSectionName[CONFIG_MAX_SECTION_LENGTH] = "Database";
    char lszDefaultVal[CONFIG_MAX_CONTENT_LENGTH] = "192.168.1.1";
    char lszOutput[CONFIG_MAX_CONTENT_LENGTH];
    unsigned int liOutput = CONFIG_MAX_CONTENT_LENGTH;

    // config_get_string_value("Database","IP","192.168.1.1",lszOutput,liOutput,"config");
    config_get_string_value(lszSectionName, lszKeyName, lszDefaultVal, lszOutput, liOutput, "config");

    printf("[%s]\n", lszSectionName);
    printf("%s=%s\n", lszKeyName, lszOutput);

    memset(lszKeyName, 0, sizeof(lszKeyName));
    memset(lszSectionName, 0, sizeof(lszSectionName));
    memset(lszDefaultVal, 0, sizeof(lszDefaultVal));
    memset(lszOutput, 0, sizeof(lszOutput));

    strcpy(lszKeyName,"port");
    strcpy(lszSectionName,"Database");
    
    int liDefault = 8080;
    int iOutput = 0;
    config_get_int_value(lszSectionName, lszKeyName, liDefault, "config", &iOutput);
    printf("%s=%d\n", lszKeyName, iOutput);

    return 0;
}

#endif // DEBUG_GET_CONFIG