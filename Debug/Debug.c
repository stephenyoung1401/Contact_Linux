/*
 * @Description: 
 * @Version: 
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-19 21:36:07
 * @LastEditTime: 2019-03-20 16:59:28
 */
#include "Debug.h"

void assert_report(const char *c_pFileName, const char *c_pFuncName,
                   const unsigned long c_dwLineNo)
{
    fprintf(stderr, "\n\"%s\" , %s() , Line %lu\n", c_pFileName, c_pFuncName, c_dwLineNo);
    abort();
}