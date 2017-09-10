#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

//    定义一个节点的结构
typedef struct node
{
    int member;            //数据域
    struct node * pNext;//指针域
}Node,*pNode;

//    定义一个栈结构
typedef struct stack
{
    pNode Top;            //栈顶
    pNode Bottom;        //栈底
}Stack,* pStack;

void InitStack(pStack );        //    初始化栈的函数
bool Push(pStack ,int);            //    进行压栈操作的函数
void TraverseStack(pStack );    //    遍历栈函数
bool Empty(pStack );            //    判断栈是否为空的函数
int Pop(pStack );                //    进行出栈操作的函数
void Clear(pStack );            //    清空栈的函数

int main(void)
{
    Stack s;                        //    定义一个栈
    int i;
    int num;
    int data;                        //    临时保存用户输入的数据
    int re_num;                        //    保存Pop函数的返回值
    InitStack(&s);
    printf("你想输入几个数据啊：");
    scanf("%d",&num);
    for (i = 0;i < num;i++)
    {
        printf("第 %d 个数：",i+1);
        scanf("%d",&data);
        if (Push(&s,data))                //    调用Push函数
        {
            continue;
        }
        else
        {
            printf("进行进栈操作失败！\n");
            exit(-1);
        }
    }
    TraverseStack(&s);                //    调用遍历函数
    printf("你想去掉几个数啊: ");
    scanf("%d",&data);
    printf("你去掉的数字是：");
    for (i = 0; i < data;i++)
    {
        re_num = Pop(&s);            //    调用Pop函数，并把返回值赋给re_num;
        printf("%d ",re_num);
    }
    printf("看看删除后还有啥：");
    TraverseStack(&s);
    printf("\n");
    Clear(&s);                        //    调用清空栈函数
    printf("遍历下看看栈清空没····\n");
    TraverseStack(&s);
    printf("\n");
    return 0;
}

//    进行栈的初始化的函数
void InitStack(pStack ps)
{
    ps->Top = (pNode)malloc(sizeof(Node));        //    分配内存空间给栈顶
    if (NULL == ps->Top)
    {
        printf("动态分配内存失败\n");
        exit(-1);
    }
    else
    {
        ps->Bottom = ps->Top;                    //    使栈底也指向栈顶空间
        ps->Top->pNext = NULL;                    //    栈顶指针置为NULL；
    }
    return ;
}

//    进行进栈操作的函数
bool Push(pStack ps,int data)
{
    pNode pNew = (pNode)malloc(sizeof(Node));    //    定义一个新节点，并分配内存空间
    if (NULL == pNew)
    {
        return false;
    }
    pNew->member = data;                        //    把要进栈的数据赋给新节点的member成员
    pNew->pNext = ps->Top;                        //    使新节点的指针指向栈顶
    ps->Top = pNew;                                //    把新节点作为新栈顶

    return true;
}

//    遍历栈的函数
void TraverseStack(pStack ps)
{
    pNode pNew = ps->Top;
    while(pNew!= ps->Bottom)                //    只要栈顶不等于栈底，循环
    {
        printf("%d ",pNew->member);            //    打印栈顶的成员member
        pNew = pNew->pNext;                //    栈顶指针向下移动一次
    }
    return ;
}

//    判断栈是否为空
bool Empty(pStack ps)
{
    if(ps->Top == ps->Bottom)    //    栈顶等于栈底，不就是栈中没数据么
    {
        return true;
    }
    else
    {
        return false;
    }
}

//    进行出栈操作函数
int Pop(pStack ps)
{
    pNode pSwap = NULL;            
    int return_val;
    if (Empty(ps))                //判断栈是否为空，为空就不能进行出栈操作
    {
        exit(-1);
    }
    else
    {
        return_val = ps->Top->member;    //    把栈顶的成员member的值赋给return_val做为函数返回值
        pSwap = ps->Top;                //    使pSwap指向栈顶
        ps->Top = ps->Top->pNext;        //    使栈顶指向栈顶下一个节点
        free(pSwap);                    //    释放以前的栈顶空间
        return return_val;
    }
}

//    清空栈的函数
void Clear(pStack ps)
{
    pNode pNew = NULL;
    
    while (ps->Top != ps->Bottom)        //    栈顶和栈底不等，循环
    {
        pNew = ps->Top;                    //    使一个新节点和栈顶指向同一空间
        ps->Top = ps->Top->pNext;        //    使栈顶指向栈顶的下一个节点
        free(pNew);                        //    释放掉以前的栈顶空间
    }
    return ;
}