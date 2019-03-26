/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-19 21:36:13
 * @LastEditTime: 2019-03-20 16:59:31
 */
#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#define DEBUG //the main debug switch

//直接输出
#ifdef DEBUG
#define DBG(...)                                                               \
    fprintf(stderr, " DBG(%s, %s(), %d): ", __FILE__, __FUNCTION__, __LINE__); \
    fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif

//断言调试
#ifdef DEBUG
// #undef EXAM_ASSERT_TEST 0   //禁用
#define EXAM_ASSERT_TEST 1 //启用

#ifdef EXAM_ASSERT_TEST
void assert_report(const char *c_pFileName, const char *c_pFuncName,
                   const unsigned long c_dwLineNo);
//可以在后面加 ;
#define ASSERT_REPORT(condition)                             \
    do                                                       \
    {                                                        \
        if (condition)                                       \
            NULL;                                            \
        else                                                 \
            assert_report(__FILE__, __FUNCTION__, __LINE__); \
    } while (0)

#else // 禁用断言测试
#define ASSERT_REPORT(condition) NULL
#endif

#endif // end of ASSERT

#endif // DEBUG_H