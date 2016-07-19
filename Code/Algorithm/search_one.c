/*************************************************************************
	> File Name: search_one.c
	> Author: marvin
	> Mail: marvinkoala@google.com
	> Created Time: Tue 19 Jul 2016 06:53:36 PM CST
 ************************************************************************/
#include <stdio.h>
#include "log.h"


#define MAXNUM      (10000001)

int aiNumList[MAXNUM]={0};
int Get1Count(int *pNumList, int iLstLen, int iMaxNum);

int main()
{
    int iMax=MAXNUM,iIndex,iNum;
    //int aiNumList[MAXNUM];
    int iCntOf1=0;


    //Generate 1~MAX num list
    for (iIndex=0, iNum=1; iNum <= iMax; iIndex++, iNum++)
    {
        aiNumList[iIndex] = iNum;
    }


    //get 1 count from NumList
    LogInfo("Start Search\n");
    iCntOf1 = Get1Count(aiNumList, sizeof(aiNumList), iMax);
    LogInfo("Stop Search. 1~%d total has: %d  num(1)", iMax, iCntOf1);
    

    return 0;
}

//func: count nums of 1 in pNumList
//in: pNumList
//out: void
//ret: nums of 1
//note: 
int Get1Count(int *pNumList, int iLstLen, int iMaxNum)
{
    float fltmp=0,tmp2=0, tmp3=0;
    int iCnt=0,i=0,key=1,tmp4=0,tmp5;
    int iTmpCnt[2]={0};
    int iIndex=0, iMaxIndex=0, i1Num,i10Num;
	int tmp0[2]={0};
	int tmp1[2]={0};
    
   
    if (iMaxNum < 10 && iMaxNum > 0) return 1;
    iMaxIndex = iLstLen/sizeof(pNumList[0])-1;
       

    fltmp = iMaxNum/100.0;
    tmp0[1] = (int)fltmp; //how many 100 in iMaxNum
    i1Num = iMaxNum % 10;
    i10Num = (iMaxNum - i1Num)%100;
    tmp0[0] = i10Num*10+i1Num;


	key = (fltmp >= 1? 2 : 1);
	tmp1[1] = (key > 1? 100 : 0);
	tmp1[0] = tmp0[0];
    iTmpCnt[1] = (key > 1? 1: 0);


    //LogInfo("key:%d tmp1[0]:%d tmp1[1]:%d", key, tmp1[0], tmp1[1]);
    for (i=0; i < key; i++)
    {
		for (iIndex=0; iIndex < tmp1[i]; iIndex++)
		{
            //even index
			tmp2 = pNumList[iIndex]/10.0;
			tmp3 = tmp2 - (int)tmp2 + 0.9;
            tmp4 = (int)(tmp3 * 10);
            if (tmp4 == 10)
		    {
		        iTmpCnt[i]++;
                continue;
            }
            
            //odd index
		    if ((pNumList[iIndex]/10) == 1)
		    {
		        iTmpCnt[i]++;
		    }
		}
    }


   
    //LogInfo("c0:%d c1:%d t0:%d index: %d", iTmpCnt[0], iTmpCnt[1], tmp0[1], iIndex);
    return (iTmpCnt[0]+iTmpCnt[1]*tmp0[1]);
}
