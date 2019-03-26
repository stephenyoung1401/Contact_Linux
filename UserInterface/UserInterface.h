/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-17 21:00:21
 * @LastEditTime: 2019-03-24 21:32:47
 */
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>      //need to compile with arm-linux

#include "../Structure/PersonInfo/PersonInfo.h"
#include "../Structure/LinkedList/LinkedList.h"


#define UI_SUCCESS 0
#define UI_INVALID_INPUT -1


//获取新的人员信息    
int showUI_add_person(char *pszName,char *pszPhone,char *pszDescription);

//对需要删除的人员信息做进一步确认
int showUI_delete_person(const SPersonInfo *c_pPerson, char *pcChoice);

//获取需要修改的人员信息
int showUI_modify_person(const SPersonInfo *c_pPerson, char *pszName,char *pszPhone,char *pszDescription);

//模糊查询人员信息
int showUI_search_person(char *pszInfo);

int showUI_search_person_next(char *pszFunc);

//确定某指定人员，在局部链表中选定人员
// /pLinkedList为一个局部链表
int showUI_confirm_person(const SLinkedList *pLinkedList,int *piChoice);

//显示指定的部分联系人，可以尝试写一个带连续翻页优化的函数，即使用静态变量实现
int showUI_part_person_list(const SLinkedList *c_pLinkedList,long lBeginIndex,long lEndIndex);

int showUI_main_command(char *pcFunCalled);

int showUI_quit();

#endif