# 通讯录

 * @Description: Linux 下 C 开发的通讯录程序的一些注意事项
 * @Version: 1.1
 * @Company: NJIT
 * @Author: Stephen Young
 * @LastEditors: Stephen Young
 * @Date: 2019-03-16 15:44:00
 


框架、规范、描述 ———— 为了一个实现源代码的稳定性、维护性、拓展性，在这里写下一些关于整个程序的逻辑的划分，其中涵括了各个模块之间的调用层次关系，函数的接口的详细定义，代码的命名规范，DEBUG版本的内嵌调试语句等。本文档主要目的是为了让代码能够更加像一个实际的工程项目，尽管所实现的功能以及数据结构与算法都是极其简单的。

文档在注释部分仅仅说明了注意点，在代码部分仅仅声明一些必要的函数。


# __规范__ 
## __关于调试__
每个模块及数据结构部分，都有一个受总控开关控制的属于自己的编译开关，即在是一种串联的结构，对部分关键点使用
```C
#define DEBUG_xxx 1

#ifndef DEBUG
#undef DEBUG_xxx
#endif

//直接输出
#ifdef DEBUG_xxx
#define DBG(...) fprintf(stderr, " DBG(%s, %s(), %d): ", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, __VA_ARGS__)
#else
#define DBG(...)
#endif

//断言调试
#ifdef DEBUG_xxx
// #undef EXAM_ASSERT_TEST 1   //禁用
#define EXAM_ASSERT_TEST 0  //启用

    #ifdef EXAM_ASSERT_TEST
    void  assert_report(const char *c_pFileName,const char *c_pFuncName,const unsigned long c_dwLineNo)
    {
        fprintf(stderr,"\n\"%s\" , %s() , Line %u\n",c_pFileName,c_pFuncName,c_dwLineNo);
        abort();
    }
    //可以在后面加 ; 
    #define  ASSERT_REPORT( condition ) \   
    do{                                \
        if ( condition )                   \
            NULL;                             \
        else                               \
            assert_report( __FILE__, __FUNCTION__, __LINE__ );    \
    }while(0)

    #else // 禁用断言测试
    #define ASSERT_REPORT( condition )  NULL
    // #endif

#endif /* end of ASSERT */
```
## __变量命名__
在本程序中，应谨遵以下的命名规则，并即时补全表格中未提及的情况

类型    |   缩写    
:-----: | :-----:  
`bool`    |   bFlg    
`int  `   |   iCnt
`short int` | nStep
`long int` | lSum
`char `    |    cNo
`unsigned char` |byHead
`float `    | fAvg
`double `   | dDelta
`unsigned int`(WORD)       |wCnt
`unsigned long int`(DWORD) |dwCnt 
字符串        |sFileName
用0结尾的字符串|szFileName
一级指针       | pData
二级指针        | ppData
全局变量    |   g_
静态变量    |   s_
枚举        |   EM
结构体      |   S
联合体      |   U
常量const   |   c_

## __函数命名__
``` C
/**
 * @brief  简要说明函数的功能
 * @note   说明调用者应该注意的问题
 * @param  [入口/出口]参数，参数含义
 *          对于入口参数 声明为 `const` 类型 防止出错便于检查
 * @retval 错误编码的数值以及含义，即为宏定义的部分内容
 */
 //函数命名使用小写，以下划线的方式隔开
int get_person_info(const SPersonInfo *c_pPerson, char *pszInfo, const int c_iOutput);
```



# __框架__ 

## __总体划分__
总体上划分为 __人机交互界面(View)__ - __总的调度模块(Control)__ - __各个子模块(Module)__ 这三大部分。__（这里后期最后补上图）__

其中调度模块为实际上的主函数，负责在合适的时候调度交互界面，并选择合适的功能模块实现需求。

人机交互界面只负责将用户的输入信息传递给调度者，必要时输出显示的信息（后期考虑实现更美观的界面时，只需要注意返回到调度者的缓冲信息即可）。

各个子模块之间应不存在任何的相互依赖关系，当然，为了实现一个更具拓展性与维护性的功能模块，应当在在每个模块下再进行一些合理合规范的划分，但必须仍然在低耦合的情况下进行。

此外，数据结构部分是由整个工程共同接入的，避免为了分离而分离的情况，减少不必要的强制类型转换。


## __数据结构__
### 1. 联系人信息结构体
``` C
#define PERSON_MAX_NAME_LENGTH 24     // 这里需要考虑机器的对齐问题 

#define PERSON_MAX_PHONE_LENGTH 16

#define PERSON_MAX_DESCRIPTION_LENGTH 100

typedef Struct _Person_Info
{
    char szName[PERSON_MAX_NAME_LENGTH];
    char szPhone[PERSON_MAX_PHONE_LENGTH];
    char szDescription[PERSON_MAX_DESCRIPTION_LENGTH];
} SPersonInfo;

#define PERSON_SUCCESS 0    //所有成功的操作的统一返回值
//打印人员信息
//pPerson:输入参数
int print_person_info(const SPersonInfo *c_pPerson);

//将人员信息打包成一个字符串的形式
//pszInfo:输出参数，由iOutput指定输出的长度，需要在外定义一个合适的字符数组
int get_person_info(const SPersonInfo *c_pPerson, char *pszInfo, const int c_iOutput);


#define PERSON_ALLOCATE_FAILED -1   //申请内存空间失败

#define PERSON_INPUT_OVERFLOW -2    //输入参数超出允许长度

#define PERSON_INPUT_INVALID -3     //输入参数不合法
//人员内存分配在函数内完成，调用时，先声明指针，再取该指针的地址传入，等待被malloc到的地址赋值
//输入const参数 __可以为空__ ，即创建一个未初始化的人员
int create_person(SPersonInfo **ppPerson,const char *c_pszName,const char *c_pszPhone,const char *c_pszDescription);

//带错误校验的设置人员信息
int set_person(SPersonInfo *pPerson,const char *c_pszName,const char *c_pszPhone,const char *c_pszDescription);
```
### 2. 基于链表的内存存储结构
``` C
typedef struct _Link_Node
{
    void *pData;
    struct _Link_Node *pPreNode; // 使用双向链表存储
    struct _Link_Node *pNextNode;
} SLinkNode;

typedef struct _Linked_List
{
    unsigned long dwLength;
    struct _Link_Node *pHeadNode;
    struct _Link_Node *pTailNode;
} SLinkedList;

#define LINKED_LIST_ALLOCATE_FAILED -1
//函数内分配内存
int linked_list_create(SLinkedList **ppLinkedList);

#define LINKED_LIST_INDEX_OVERFLOW -2   //-1为尾插，不能小于-1或者大于等于dwLength
#define LINKED_LIST_DATA_NULL -3        //插入NULL是无意义的
#define LINKED_LIST_LINKED_LIST_NULL -4 //链表不能为空
//插入分为头插尾插中间插入，均由一个函数确定
int linked_list_insert(SLinkedList *pLinkedList, const void *c_pData, const long c_lIndex);

//从链表中移除节点，并释放，返回节点内的数据域
int linked_list_remove_at(SLinkedList *pLinkedList, void *pDataRet, const long c_lIndex);

//与removeat相同的操作方式，但是不释放节点
int linked_list_get_at(SLinkedList *pLinkedList, void *pDataRet, const long c_lIndex);

//释放整个链表的节点，除了链表本身
int linked_list_clear(SLinkedList *pLinkedList);

//释放整个链表包括链表本身
int linked_list_free(SLinkedList *pLinkedList);

//合并两个链表，头尾相接，不释放
int linked_list_merge(SLinkedList *pDst_LinkedList,SLinkedList *pSrc_LinkedList);

//显示整个链表（旨在debug时使用）
int linked_list_display(SLinkedList *pLinkedList);

```
### 3. 基于二叉树实现的存储结构（选做）
有一个矛盾的地方，若是采用模糊查询的方式，那么AVL树的结构优势就体现不出来，因为字符串过长，每次比较都会耗费较大的开销
```C
typedef struct _Tree_Node
{
    void *data;
    unsigned long height;
    struct _Tree_Node *left;
    struct _Tree_Node *right;
} STreeNode;
```


## __模块__
### 1. 人机交互
所有的ui模块都是显示信息，获取输入，对数据本身不做修改
``` C
//获取新的人员信息    
int showUI_add_person(char *pszName,char *pszPhone,char *pszDescription);

//对需要删除的人员信息做进一步确认
int showUI_delete_person(const SPersonInfo *c_pPerson, char *pcChoice);

//获取需要修改的人员信息
int showUI_modify_person(const SPersonInfo *pPerson, char *pszName,char *pszPhone,char *pszDescription);

//模糊查询人员信息
int showUI_search_person(char *pszInfo);

//确定某指定人员，在局部链表中选定人员
// /pLinkedList为一个局部链表
int showUI_confirm_person(SLinkedList *pLinkedList,int *piChoice);

//显示指定的部分联系人，可以尝试写一个带连续翻页优化的函数，即使用静态变量实现
int showUI_part_person_list(const SLinkedList *c_pLinkedList,long lBeginIndex,long lEndIndex);

```

### 2. 调度模块
调度模块含有每个操作所对应的流程，可以修改数据，和调用UI
``` C
//下面的两个函数是针对选定的人员的操作
//调用showUI_delete_person,在主界面下需要跳转到查询界面，该函数需要在查询界面进入
int subOptn_delete_person(SLinkNode *pLinkedList);

//调用showUI_modify_person，对节点的数据域做修改
int subOptn_modify_person(SLinkNode *pLinkedList);

//模糊查询后，希望进行修改删除时，调用该函数，若模糊查询结果为 1 则也可以调用
int subOptn_confirm__person(SLinkedList *pLinkedList);

//主界面下只有查询操作和添加操作
int optn_search_person(SLinkedList *pLinkedList);

//调用showUI_add_person并自动添加到链表中
int optn_add_person(SLinkedList *pLinkedList);

//主进程
int optn_main();

//退出时需要进行内存回收
int optn_quit();
```

### 3. 联系人模糊查询（正则表达式实现）

```C
//只返回目标人员信息中是否包含了模式串
int regex_match_no_ret(const SPersonInfo *pszDst,const char *pszPat);

//返回目标人员信息中与模式串匹配的字段的始末位置
int regex_match_with_ret(const SPersonInfo *pszDst,cosnt char *pszPat,char * pszAttr, int *piStart,int *piEnd);
```

### 4. 数据库操作
考虑到IO操作的延迟，应该成批地对通讯录进行增删改，
这里提供了（单项信息的增删改和）批量信息的增删改，
但是这是单用户环境下的操作，因此不会涉及锁的问题。

同时，为了防止出现退出时忘了保存的情况，
（主进程将会对每次操作写入日志文件），在每隔一段时间时，将会自动保存内存中的内容

此外，为了防止占用内存过多的情况出现，这里将只读取有限的人员信息到内存，
当内存中信息不满足要求时，才再次访问数据库，以此达到时间与空间的平衡点。

创建存放在内存中的文件，记录修改、增加、删除的记录，使得成批写入成为可能。

```C

//封装了一个数据库的操作，便于在内存中存储，或者网络流中传输
typedef struct _DB_SQL_OPTN
{
    char szHead[DB_SQL_HEAD_LENGTH];
    char *pszField[DB_SQL_FIELD_LENGTH];
    char *pszPara[DB_SQL_PARA_LEGNTH];
    int iFieldCnt;
    int iTupleCnt;
} SDbOptn;


//打开文件形式的数据库
int databae_open(const char *c_pszFileName,sqlite3 **ppDbConn);

//关闭连接
int database_close(sqlite3 *pDbConn);

//按序号，批量读取
int database_multi_get_range(const sqlite3 *c_pDbConn, 
                            const long c_lBeginIndex,const long c_lEndIndex, 
                            SLinkedList **pLinkedListRet)

//按序号，以模糊查找方式读取
int database_multi_get_like(const sqlite3 *c_pDbConn, 
                            const long c_lBeginIndex,const long c_lEndIndex,
                            const char *pszPharse, const char *c_pszInfo , 
                            SLinkedList **pLinkedListRet)

//批量修改
int database_multi_modify(const char *c_pszFileName,const sqlite3 *c_pDbConn);

//批量删除
int database_multi_delete(const char *c_pszFileName,const sqlite3 *c_pDbConn);

//批量添加
int database_multi_insert(const char *c_pszFileName,const sqlite3 *c_pDbConn);



```

### 5. 配置文件读取

```C
//获取文件的完整路径
int config_get_complete_path(const char *pszConfigFileName, char *pszWholeFileName);

//读取指定的段属性内容
int config_string_content(const FILE *c_fp,const char *pszSectionName,
                                const char *pszKeyName, char *pszOutput,
                                const unsigned int iOutputLen);

//读取字符串形式段属性内容
int config_get_string_value(const char *pszSectionName,const char *pszKeyName, 
                            const char *pDefaultValue, char *pszOutput, 
                            const unsigned int iOutputLen,const char *pszConfigFileName);

//读取数值型段属性内容
int config_get_int_value(const char *pszSectionName,const char *pszKeyName,
                        const unsigned int iDefaultValue,const char *pszConfigFileName
                        int *piOutput);
```

### 6. 网络传输
使用JSON包装一个操作和人员信息，
```C
typedef struct _Net_Db_SQL_Optn
{
    int iFieldCnt;
    int iTupleCnt;
    char szHead[DB_SQL_HEAD_LENGTH];
    char *pszField[DB_SQL_FIELD_LENGTH];
    char *pszPara[DB_SQL_PARA_LEGNTH];
} SNetDbOptn;

typedef struct _Net_Person_Info
{
    int iSum;
    int iCur;
    char szName[PERSON_MAX_NAME_LENGTH];
    char szPhone[PERSON_MAX_PHONE_LENGTH];
    char szDescription[PERSON_MAX_DESCRIPTION_LENGTH];
} SNetPersonInfo;

typedef struct _Net_Command_Req
{
    char bRecvFlg;
    char szCommand;
    int iParaCnt;
    char *pszPara[NET_COMMAND_LENGTH];
} SNetCmdReq;

//修改删除操作，返回操作的结果，并显示错误的信息
int send_db_set_command(SNetDbOptn *pNetDbOptn,SNetCmdReq **ppNetCmdReq);

//查询表，函数体内，依据iSum与iCur的值，构建一个链表，
int send_db_get_command(SNetDbOptn *pNetDbOptn,SLinkedList **ppLinkedList);

```
