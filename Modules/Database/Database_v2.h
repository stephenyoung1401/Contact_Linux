/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-20 16:54:20
 * @LastEditTime: 2019-03-25 16:24:03
 */
// #define DEBUG_DATABASE_V2

#ifndef DATABASE_V2_H
#define DATABASE_V2_H
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sqlite3.h>

#include "../../Debug/Debug.h"

#include "../../Structure/PersonInfo/PersonInfo.h"
#include "../../Structure/LinkedList/LinkedList.h"

#define DB_FILE_NAME_LENGTH 256
#define DB_SQL_COMMAND_LENGTH 256
#define DB_SQL_HEAD_LENGTH 16
#define DB_SQL_FIELD_LENGTH 32
#define DB_SQL_PARA_LENGTH 128
#define DB_OPTN_LENGTH_JSON 512

#define DB_CONNECTED 1
#define DB_DISCONNECTED -1

#define DB_OPERATION_SUCCESS 0

#define DB_FILE_NAME_NULL -1
#define DB_OPEN_FAILED -2
#define DB_CLOSE_FAILED -3
#define DB_SELECT_FAILED -4
#define DB_INSERT_FAILED -5
#define DB_DELETE_FAILED -6
#define DB_MODIFY_FAILED -7

#define DB_OPTN_NULL -8
#define DB_OPTN_WRONG_CMD -9
#define DB_OPTN_ALLC_FAILED -10
/* SDbOperation jSON
{
    "head":"Replace",
    "Fields":{
        "name",
        "phone",
        "description"
    },
    "Paras":{
        "stephen",
        "123123123",
        "ahahahaha"
    },
    "FieldsCnt":3,
    "TupleCnt":1
}
    "{                          \
        \"head\":\"%s\",        \
        \"Fields\":             \
        {                       \
            \"%s\",             \
            \"%s\",             \
            \"%s\"              \
        },                      \
        \"Paras\":              \
        {                       \
            \"%s\",             \
            \"%s\",             \
            \"%s\",             \
        },                      \
        \"FieldCnt\":%d ,       \
        \"TupleCnt\":%d         \
    }");
*/

typedef struct _DB_OPERATION
{
    char szHead[DB_SQL_HEAD_LENGTH];
    char *pszFields[DB_SQL_FIELD_LENGTH];
    char *pszParas[DB_SQL_PARA_LENGTH];
    int iFieldsCnt;
    int iTupleCnt;
} SDbOperation;

sqlite3 *gs_pdb_connected;
char gs_pc_fileName[DB_FILE_NAME_LENGTH];
unsigned char gs_byConnFlg;

int database_create_dboptn(SDbOperation **ppDbOptn, const char *c_pszHead,
                           const char (*ppFields)[DB_SQL_FIELD_LENGTH], const int c_iFieldCnt,
                           const char (*ppParas)[DB_SQL_PARA_LENGTH]);

// int database_create_dboptn_replace(SDbOperation **ppDbOptn,
//                                    const char **ppFields, const int c_iFieldCnt,
//                                    const char **ppParas, const int c_iTupleCnt);

// //field refer to where para
// int database_create_dboptn_delete(SDbOperation **ppDbOptn,
//                                   const char **ppFields, const int c_iFieldCnt,
//                                   const char **ppParas, const int c_iTupleCnt);

int database_dboptn_free(SDbOperation *pDbOptn);

int database_dboptn_strjson(SDbOperation *pDbOptn, char *pszJsonStr, const int c_iOutput);

int databae_open(const char *c_pszFileName);

//关闭连接
int database_close();

//按序号，批量读取
int database_multi_get_range(const int c_iBeginIndex, const int c_iEndIndex,
                             SLinkedList **ppLinkedListRet);

//按序号，以模糊查找方式读取
int database_multi_get_like(const int c_iBeginIndex, const int c_iEndIndex,
                            const char *pszPharse, const char *c_pszInfo,
                            SLinkedList **ppLinkedListRet);

int database_multi_get(const SDbOperation *c_pDbOptn, SLinkedList **ppLinkedListRet);

//批量修改
int database_multi_modify(const SDbOperation *c_pDbOptn);

//批量删除
int database_multi_delete(const SDbOperation *c_pDbOptn);

//批量添加
int database_multi_insert(const SDbOperation *c_pDbOptn);

#endif