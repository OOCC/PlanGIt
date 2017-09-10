/*
单链表的操作C语言实现
Slyar 2009.3.23
//www.slyar.com
*/
 
#include <stdio.h>
#include <stdlib.h>
 
/* 给 char 类型定义别名 datatype */
typedef char datatype;
 
/* 定义链表节点类型 */
typedef struct node
{
        datatype data;
        struct node *next;
}linklist;
 
/* 函数声明部分 */
linklist* CreatList();
linklist* Get(linklist*, int);
linklist* Locate(linklist*, datatype);
void PrintList(linklist*);
void InsertAfter(linklist*, datatype);
void InsertBefore(linklist*, int, datatype);
void DeleteAfter(linklist*);
void Deleter(linklist*, int);
void FreeList(linklist*);
 
int main()
{
        int pos;
        datatype value;
       
        /* 测试创建链表函数 */
        printf("请输入一串字符，以 $ 结束\n");
        linklist *head, *p;
        head = CreatList();
       
        /* 测试打印链表函数 */
        printf("\n打印链表\n");
        PrintList(head);
 
        /* 测试按序号查找函数 */
        printf("\n请输入要查找的节点序号:\n");
        scanf("%d", &pos);
        getchar();
        p = Get(head, pos);
        if(p != NULL)
        {
                printf("%c\n", p -> data);
        }
        else
        {
                printf("Can't Get This Key!\n");
        }
       
        /* 测试按值查找函数 */
        printf("\n请输入要查找的值:\n");
        scanf("%c", &value);
        p = Locate(head, value);
        if(p != NULL)
        {
                printf("%c\n", p -> data);
        }
        else
        {
                printf("Can't Get This Key!\n");
        }
       
        /* 测试插入节点函数 */
        printf("\n你想在第几个节点前插入？\n");
        scanf("%d", &pos);
        getchar();
        printf("请输入要插入的值\n");
        scanf("%c", &value);
        InsertBefore(head, pos, value);
        PrintList(head);        
 
        /* 测试删除节点函数 */
        printf("\n你想删除第几个节点？\n");
        scanf("%d", &pos);
        Deleter(head, pos);
        PrintList(head);
       
        /* 销毁链表 */
        FreeList(head);
        
        //system("pause");
        return 0;
}
 
/* 带头结点后插法创建链表函数 */
linklist* CreatList()
{
        datatype key;
        /* head 为头指针, s 为新节点, r 为尾指针 */
        linklist *head, *s, *r;
        head = (linklist*) malloc(sizeof(linklist));
        r = head;
        key = getchar();
        /* 遇到 $ 就停止创建 */
        while(key != '$')
        {
                s = (linklist*) malloc(sizeof(linklist));
                s -> data = key;
                /* 新节点插入表尾 */
                r -> next = s;
                /* 尾指针指向新的表尾 */
                r = s;
                key = getchar();
        }
        r -> next = NULL;
        /* 返回头指针 */
        return head;
}
 
/* 打印链表函数 */
void PrintList(linklist* head)
{
        linklist *p;
        p = head -> next;
        while(p != NULL)
        {
                printf("%c", p -> data);
                p = p -> next;
        }
        printf("\n");
}
 
/* 查找链表中第 i 个节点 */
linklist* Get(linklist* head, int i)
{
        /* j 为扫描计数器 */
        int j = 0;
        linklist *p;
        /* p 指向头节点 */
        p = head;
        /* 到达表尾或序号不合法就退出循环 */
        while((p -> next != NULL) && (j < i))
        {
                p = p -> next;
                j++;
        }
        if (i == j)
        {
                return p;
        }
        else
        {
                return NULL;
        }
}
 
/* 在链表中查找值 key 并返回所在节点 */
linklist* Locate(linklist* head, datatype key)
{
        linklist *p;
        /* p 指向开始结点 */
        p = head -> next;
        while(p != NULL)
        {
                if(p -> data != key)
                {
                        p = p -> next;
                }
                else
                {
                        break;
                }
        }
        return p;
}
 
/* 在节点 p 后插入 key */
void InsertAfter(linklist* p, datatype key)
{
        linklist *s;
        s = (linklist*) malloc(sizeof(linklist));
        s -> data = key;
        /* 先将 s 指向后一个节点，再将前一个节点指向 s */
        s -> next = p -> next;
        p -> next = s;
}
 
/* 将 key 插入链表第 i 个节点之前 */
void InsertBefore(linklist* head, int i, datatype key)
{
        linklist *p;
        int j = i - 1;
        /* 找到第 i-1 个节点 p */
        p = Get(head, j);
        if (p == NULL)
        {
                printf("Insert Error!\n");
        }
        else
        {
                /* 将 key 插入节点 p 之后 */
                InsertAfter(p, key);
        }
}
 
/* 删除 p 节点的后继节点 */
void DeleteAfter(linklist* p)
{
        linklist *r;
        /* r 指向要删除的节点 */
        r = p -> next;
        /* 将 p 直接与 r 下一个节点链接 */
        p -> next = r -> next;
        /* 释放节点 r */
        free(r);
}
 
/* 删除链表的第 i 个节点 */
void Deleter(linklist* head, int i)
{
        linklist *p;
        int j = i - 1;
        /* 找到第 i-1 个节点 p */
        p = Get(head, j);
        if ((p != NULL) && (p -> next != NULL))
        {
                /* 删除 p 的后继节点 */
                DeleteAfter(p);
        }
        else
        {
                printf("Delete Error!\n");
        }
}
 
/* 递归释放单链表 */
void FreeList(linklist* p)
{
        if (p -> next != NULL)
        {
                FreeList(p -> next);
        }
        free(p);
}