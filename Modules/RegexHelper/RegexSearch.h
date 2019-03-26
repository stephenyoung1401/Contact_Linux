/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-20 16:57:49
 * @LastEditTime: 2019-03-21 23:47:28
 */
// #define DEBUG_REGEX_HELPER

#ifndef REGEX_HELPER_H
#define REGEX_HELPER_H
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>      //need to compile with arm-linux

#include "../../Debug/Debug.h"

#include "../../Structure/PersonInfo/PersonInfo.h"
#include "../../Structure/LinkedList/LinkedList.h"

#define REGEX_MAX_STRING_LENGTH 256
#define REGEX_MAX_PATTERN_LENGTH 64
#define REGEX_MAX_ERROR_LENGTH 256
#define REGEX_DEFAULT_SEARCH_CNT 2

#define REGEX_COMPILE_SUCCESS 0
#define REGEX_COMPILE_FAILED -1

#define REGEX_EXEC_SUCCESS 0
#define REGEX_EXEC_FAILED -2

#define REGEX_ALLOCATE_FAILED -3
#define REGEX_MATCH_FAILED -4
#define REGEX_MATCH_SUCCESS 0

#define REGEX_NULL_INPUT -5

//只返回目标人员信息中是否包含了模式串
int regex_match_no_ret(const SPersonInfo *pszDst, const char *pszPat);

//返回目标人员信息中与模式串匹配的字段的始末位置
//order by name , phone , description
int regex_match_with_ret(const SPersonInfo *pszDst, const char *pszPat, char *pszAttr, int *piStart, int *piEnd);

#endif // !REGEX_HELPER_H
