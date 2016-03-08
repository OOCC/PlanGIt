/****************************************************************************
   �ļ���     : main.c
   
   �޸���ʷ   :    
   1.  �޸�ʱ�� :        2016-02-28 created by luoyefeisong
       �޸����� :        ����������Ժ���
   2.  �޸�ʱ�� :        
       �޸����� :
****************************************************************************/
#include <stdio.h>
#include "linklist_pub.h"
#include "public_def.h"
#include "type_def.h"

int main()
{
	SLL stList={0};
	SLL_NODE stNode={0};
    ULONG ulCount=0;
    CHAR cChar=0;
    CHAR acTemp[5] = {'x','m','s','b',0};
    CHAR *pcDataGet = NULL;
    
    SLL_INIT(&stList);
    printf("init stList successfully\n");

    SLL_NODE_INIT(&stNode);
    printf("init stNode successfully\n");
        
    SLL_ADD(&stList, &stNode);
    printf("add stNode successfully\n");

    ulCount = SLL_COUNT(&stList);
    printf("node number is %lu\n", ulCount);

    SLL_NODE_SET_DATA(&stNode, acTemp);
    printf("set data successfully,data is %s\n", (CHAR *)stNode.ulData);

    pcDataGet = (CHAR *)SLL_NODE_GET_DATA(&stNode);
    printf("get data successfully,data is %s\n", pcDataGet);

    return VOS_OK;
}
