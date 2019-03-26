/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-17 21:05:54
 * @LastEditTime: 2019-03-25 20:07:10
 */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
// #include <regex.h> //need to compile with arm-linux
#include "../Debug/Debug.h"

#include "../Structure/PersonInfo/PersonInfo.h"
#include "../Structure/LinkedList/LinkedList.h"
#include "../UserInterface/UserInterface.h"

#include "../Modules/RegexHelper/RegexSearch.h"
#include "../Modules/Config/GetConfig.h"
#include "../Modules/Database/Database_v2.h"
#include "../Modules/NetAccess/NetAccess.h"

// #define NET_ENABLED
// #define LOCAL_ENABLED

#ifdef NET_ENABLED
extern int gs_port;
extern char gs_server_ip[NET_IP_LENGTH];
#endif // NET_ENABLED

#ifdef LOCAL_ENABLED
extern sqlite3 *gs_pdb_connected;
extern char gs_pc_fileName[DB_FILE_NAME_LENGTH];
extern unsigned char gs_byConnFlg;
#endif // LOCAL_ENABLED

#define DEBUG_FUNCTIONS

// #define FUNCTIONS_STATUS_MAIN 1  //MAIN
// #define FUNCTIONS_STATUS_ADD 2
// #define FUNCTIONS_STATUS_DELETE 3
// #define FUNCTIONS_STATUS_MODIFY 4
// #define FUNCTIONS_STATUS_CONFIRM 5
// #define FUNCTIONS_STATUS_SEARCH 6
// #define FUNCTIONS_STATUS_QUIT 7

#define FUNCTIONS_SUCCESS 0
#define FUNCTIONS_INVALID_INPUT -1
#define FUNCTIONS_SEARCH_DONE 0

//subLink list should be a part of main list ,not a copy!!!!
int subOptn_delete_person(SLinkedList *pLinkedList, SLinkNode *pLinkedNode);

//调用showUI_modify_person，对节点的数据域做修改
int subOptn_modify_person(SLinkNode *pLinkedNode);

//模糊查询后，希望进行修改删除时，调用该函数，若模糊查询结果为 1 则也可以调用
int subOptn_confirm__person(SLinkedList *pLinkedList, SLinkNode **ppLinkedNode);

//主界面下只有查询操作和添加操作
int optn_search_person(SLinkedList *pSrcLinkedList, SLinkedList **ppResLinkedList);

//调用showUI_add_person并自动添加到链表中
int optn_add_person(SLinkedList *pLinkedList);

//主进程
int optn_main();

//退出时需要进行内存回收
int optn_quit();

#endif