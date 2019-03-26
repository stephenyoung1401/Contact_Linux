/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-20 09:28:37
 * @LastEditTime: 2019-03-21 23:53:48
 */
// #define DEBUG_GET_CONFIG
#ifndef GET_CONFIG_H
#define GET_CONFIG_H

#include "../../Debug/Debug.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>



#define CONFIG_MAX_FILE_NAME_LENGTH 128
#define CONFIG_MAX_FILE_ATTR_LENGTH 20
#define CONFIG_MAX_SECTION_LENGTH 128
#define CONFIG_MAX_KEY_LENGTH 128
#define CONFIG_MAX_CONTENT_LENGTH 256

#define CONFIG_SUCCESS 0
#define CONFIG_NO_ENV_COMMAND -1
#define CONFIG_NO_FILE -2
#define CONFIG_NULL_INPUT -3


//获取文件的完整路径
int config_get_complete_path(const char *pszConfigFileName, char *pszWholeFileName);

//读取指定的段属性内容
int config_string_content(FILE *c_fp,const char *pszSectionName,
                                const char *pszKeyName, char *pszOutput,
                                const unsigned int iOutputLen);

//读取字符串形式段属性内容
int config_get_string_value(const char *pszSectionName,const char *pszKeyName, 
                            const char *pDefaultValue, char *pszOutput, 
                            const unsigned int iOutputLen,const char *pszConfigFileName);

//读取数值型段属性内容
int config_get_int_value(const char *pszSectionName,const char *pszKeyName,
                        const unsigned int iDefaultValue,const char *pszConfigFileName,
                        int *piOutput);

#endif