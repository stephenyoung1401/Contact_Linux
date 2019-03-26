/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-19 21:36:34
 * @LastEditTime: 2019-03-20 17:01:00
 */
// #define DEBUG_PERSON_INFO

#ifndef PERSON_INFO_H
#define PERSON_INFO_H

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "../../Debug/Debug.h"

#define PERSON_MAX_NAME_LENGTH 24
#define PERSON_MAX_PHONE_LENGTH 16
#define PERSON_MAX_DESCRIPTION_LENGTH 100
#define PERSON_MAX_INFO_LENGTH (PERSON_MAX_NAME_LENGTH + PERSON_MAX_PHONE_LENGTH + PERSON_MAX_DESCRIPTION_LENGTH)

#define PERSON_OPERATION_SUCCESS 0

#define PERSON_ALLOCATE_FAILED -1
#define PERSON_INPUT_OVERFLOW -2
#define PERSON_INPUT_INVALID -3

typedef struct _Person_Info
{
    char szName[PERSON_MAX_NAME_LENGTH];
    char szPhone[PERSON_MAX_PHONE_LENGTH];
    char szDescription[PERSON_MAX_DESCRIPTION_LENGTH];
} SPersonInfo;

/**
 * @brief  
 * @note   
 * @param  *c_pPerson: 
 * @retval 
 */
int print_person_info(const SPersonInfo *c_pPerson);

/**
 * @brief  
 * @note   
 * @param  *c_pPerson: 
 * @param  *pszInfo: 
 * @param  c_iOutput: 
 * @retval 
 */
int get_person_info(const SPersonInfo *c_pPerson, char *pszInfo, const int c_iOutput);

/**
 * @brief  
 * @note   person struct allocate inside the function
 * @param  **ppPerson: 
 * @param  *c_pszName: 
 * @param  *c_pszPhone: 
 * @param  *c_pszDescription: 
 * @retval 
 */
int create_person(SPersonInfo **ppPerson, const char *c_pszName, const char *c_pszPhone, const char *c_pszDescription);

/**
 * @brief  
 * @note   
 * @param  *pPerson: 
 * @param  *c_pszName: 
 * @param  *c_pszPhone: 
 * @param  *c_pszDescription: 
 * @retval 
 */
int set_person(SPersonInfo *pPerson, const char *c_pszName, const char *c_pszPhone, const char *c_pszDescription);

#endif // !PERSON_INFO_H